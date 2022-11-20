#include "project.h"

#include "file.h"
#include "palette.h"
#include "state.h"
#include "uuid.h"
#include "widgets.h"
#include "uistate.h"

#include <chrono>

namespace editor
{

const me_file *me_project::get_file_by_uuid( const uuid_t &fid ) const
{
    for( const me_file &file : files ) {
        if( fid == file.uuid ) {
            return &file;
        }
    }
    return nullptr;
}

const me_palette *me_project::get_palette_by_uuid( const uuid_t &fid ) const
{
    for( const me_palette &palette : palettes ) {
        if( fid == palette.uuid ) {
            return &palette;
        }
    }
    return nullptr;
}

void show_project_ui( me_state &state, me_project &project )
{
    ImGui::SetNextWindowSize( ImVec2( 250.0f, 200.0f ), ImGuiCond_FirstUseEver );
    ImGui::Begin( "Project Overview" );

    ImGui::Text( "Mapgens:" );

    bool changed_mapgens = ImGui::VectorWidget()
    .with_for_each( [&]( size_t idx ) {
        uuid_t this_uuid = project.files[idx].uuid;
        if( ImGui::ImageButton( "toggle_palette", "me_palette" ) ) {
            state.uistate->toggle_show_palette( project.files[idx].base.inline_palette_id );
        }
        ImGui::HelpPopup( "Show/hide inline palette for this mapgen." );
        ImGui::SameLine();
        if( ImGui::ImageButton( "toggle_mapobjects", "me_mapobject" ) ) {
            state.uistate->toggle_show_mapobjects( project.files[idx].uuid );
        }
        ImGui::HelpPopup( "Show/hide map objects for this mapgen." );
        ImGui::SameLine();
        if( ImGui::Selectable(
                string_format( "Mapgen #%d", idx ).c_str(),
                state.uistate->active_file_id && *state.uistate->active_file_id == this_uuid )
          ) {
            state.uistate->active_file_id = this_uuid;
        }
    } )
    .with_add( [&]()->bool {
        bool ret = false;
        if( ImGui::Button( "New mapgen" ) )
        {
            uuid_t new_mapgen = project.uuid_gen();
            project.files.emplace_back();
            project.files.back().uuid = new_mapgen;
            uuid_t new_palette = project.uuid_gen();
            project.files.back().base.inline_palette_id = new_palette;
            project.palettes.emplace_back();
            project.palettes.back().uuid = new_palette;
            ret = true;
        }
        return ret;
    } )
    .with_delete( [&]( size_t idx ) {
        uuid_t pal_uuid = project.files[idx].base.inline_palette_id;
        project.files.erase( std::next( project.files.cbegin(), idx ) );
        for( auto it = project.palettes.cbegin(); it != project.palettes.cend(); it++ ) {
            if( it->uuid == pal_uuid ) {
                project.palettes.erase( it );
                break;
            }
        }
    } )
    .with_duplicate( [&]( size_t idx ) {
        me_file copy = project.files[ idx ];
        me_palette pcopy = *project.get_palette_by_uuid( copy.base.inline_palette_id );
        copy.uuid = project.uuid_gen();
        pcopy.uuid = project.uuid_gen();
        copy.base.inline_palette_id = pcopy.uuid;
        project.files.insert( std::next( project.files.cbegin(), idx + 1 ), std::move( copy ) );
        project.palettes.push_back( std::move( pcopy ) );
    } )
    .run( project.files );

    ImGui::Text( "Inline palettes:" );
    for( const me_palette &pal : project.palettes ) {
        ImGui::Selectable( string_format( "Palette [uuid=%d]", pal.uuid ).c_str(), false );
    }

    if( changed_mapgens ) {
        state.mark_changed();
    }

    ImGui::End();
}

std::string timestamp_string()
{
    using namespace std::chrono;
    uint64_t ms = duration_cast<milliseconds>( system_clock::now().time_since_epoch() ).count();
    return string_format( "%u", ms );
}

std::unique_ptr<me_project> create_empty_project()
{
    auto ret = std::make_unique<me_project>();
    ret->project_uuid = timestamp_string();
    return ret;
}

} // namespace editor
