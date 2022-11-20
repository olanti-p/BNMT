#include "palette.h"

#include "canvas_tools.h"
#include "color.h"
#include "file.h"
#include "map_key_gen.h"
#include "piece_impl.h"
#include "project.h"
#include "state.h"
#include "uistate.h"
#include "widgets.h"

#include "../translations.h"

#include <unordered_set>

namespace editor
{
map_key pick_available_key( const me_palette &pal )
{
    me_map_key_generator gen;
    for( const auto &it : pal.entries ) {
        gen.blacklist( it.key );
    }
    return gen();
}

static bool is_expanded( const me_state &state, const uuid_t &piece_id )
{
    return state.uistate->expanded_mapping_pieces.count( piece_id ) != 0;
}

static void expand_piece( me_state &state, const uuid_t &piece_id )
{
    state.uistate->expanded_mapping_pieces.insert( piece_id );
}

static void collapse_piece( me_state &state, const uuid_t &piece_id )
{
    state.uistate->expanded_mapping_pieces.erase( piece_id );
}

void show_mapping( me_state &state, editor::me_palette &p, editor::me_palette_entry &entry,
                   bool &show )
{
    std::string wnd_id = string_format( "Mappings##wnd-mappings-%d-%d", p.uuid, entry.uuid );
    ImGui::SetNextWindowSize( ImVec2( 450.0f, 300.0f ), ImGuiCond_FirstUseEver );
    ImGui::SetNextWindowPos( ImVec2( 50.0f, 50.0f ), ImGuiCond_FirstUseEver );
    if( !ImGui::Begin( wnd_id.c_str(), &show ) ) {
        ImGui::End();
        return;
    }
    ImGui::PushID( entry.uuid );

    auto &list = entry.mapping.pieces;

    bool changed = ImGui::VectorWidget()
    .with_add( [&]()->bool {
        std::vector<std::pair<std::string, PieceType>> piece_opts;
        for( const auto &it : editor::get_piece_templates() )
        {
            PieceType pt = it->get_type();
            if( !is_available_as_mapping( pt ) ) {
                continue;
            }
            if( is_piece_exclusive( pt ) && entry.mapping.has_piece_of_type( pt ) ) {
                continue;
            }
            piece_opts.emplace_back( io::enum_to_string<PieceType>( pt ), pt );
        }

        std::sort( piece_opts.begin(), piece_opts.end(), []( const auto & a, const auto & b ) -> bool {
            return localized_compare( a, b );
        } );

        std::string new_piece_str;
        new_piece_str += "Add mapping...";
        new_piece_str += '\0';
        for( const auto &it : piece_opts )
        {
            new_piece_str += it.first;
            new_piece_str += '\0';
        }
        bool ret = false;
        int new_piece_type = 0;
        if( ImGui::Combo( "##pick-new-mapping", &new_piece_type, new_piece_str.c_str() ) )
        {
            if( new_piece_type != 0 ) {
                auto ptr = editor::make_new_piece( piece_opts[new_piece_type - 1].second );
                uuid_t uuid = state.project().uuid_gen();
                ptr->uuid = uuid;
                ptr->init_new();
                list.push_back( std::move( ptr ) );
                expand_piece( state, uuid );
                ret = true;
            }
        }
        return ret;
    } )
    .with_for_each( [&]( size_t idx ) {
        const uuid_t &piece_id = list[idx]->uuid;
        if( is_expanded( state, piece_id ) ) {
            if( ImGui::ArrowButton( "##collapse", ImGuiDir_Down ) ) {
                collapse_piece( state, piece_id );
            }
            ImGui::HelpPopup( "Hide details." );
            ImGui::SameLine();
            ImGui::Text( "%d %s", static_cast<int>( idx ), list[idx]->fmt_summary().c_str() );
            list[idx]->show_ui( state );
            ImGui::Separator();
        } else {
            if( ImGui::ArrowButton( "##expand", ImGuiDir_Right ) ) {
                expand_piece( state, piece_id );
            }
            ImGui::HelpPopup( "Show details." );
            ImGui::SameLine();
            ImGui::Text( "%d %s", static_cast<int>( idx ), list[idx]->fmt_summary().c_str() );
        }
    } )
    .with_can_duplicate( [&]( size_t idx ) -> bool {
        return !editor::is_piece_exclusive( list[idx]->get_type() );
    } )
    .with_duplicate( [&]( size_t idx ) {
        list.insert( std::next( list.cbegin(), idx + 1 ), list[idx]->clone() );
    } )
    .run( list );

    if( changed ) {
        state.mark_changed();
    }

    if( state.is_changed() ) {
        entry.sprite_cache_valid = false;
    }

    ImGui::PopID();
    ImGui::End();
}

static void show_palette_entries( me_state &state, me_palette &palette )
{
    std::vector<me_palette_entry> &list = palette.entries;
    std::unordered_set<map_key> checked;
    std::unordered_set<map_key> dupe_symbols;

    for( const editor::me_palette_entry &entry : list ) {
        if( checked.count( entry.key ) > 0 ) {
            dupe_symbols.insert( entry.key );
        } else {
            checked.insert( entry.key );
        }
    }

    me_project &proj = state.project();
    me_canvas_tools_state &tools = *state.uistate->tools_state;

    bool changed = ImGui::VectorWidget()
    .with_add( [&]() -> bool {
        bool ret = false;
        if( ImGui::ImageButton( "add", "me_add" ) )
        {
            list.emplace_back( me_palette_entry{
                proj.uuid_gen(),
                pick_available_key( palette ),
                col_default_piece_color,
                me_mapping(),
                false,
                cata::nullopt
            } );
            ret = true;
        }
        ImGui::HelpPopup( "Add new entry." );
        return ret;
    } )
    .with_duplicate( [&]( size_t idx ) {
        const me_palette_entry &src = list[ idx ];
        list.insert( std::next( list.cbegin(), idx + 1 ), me_palette_entry{
            proj.uuid_gen(),
            pick_available_key( palette ),
            src.color,
            src.mapping,
            false,
            cata::nullopt
        } );
    } )
    .with_delete( [&]( size_t idx ) {
        const uuid_t &uuid = list[ idx ].uuid;
        for( me_file &file : proj.files ) {
            file.base.remove_usages( uuid );
        }
        if( tools.get_brush() == uuid ) {
            tools.set_brush( UUID_INVALID );
        }
        list.erase( std::next( list.cbegin(), idx ) );
    } )
    .with_for_each( [&]( size_t idx ) {
        if( list[idx].uuid == tools.get_brush() ) {
            if( ImGui::ImageButton( "unpick", "me_clear_rows_brush" ) ) {
                tools.set_brush( UUID_INVALID );
            }
            ImGui::HelpPopup( "Unselect (turns brush into eraser)." );
        } else {
            if( ImGui::ImageButton( "pick", "me_set_rows_brush" ) ) {
                tools.set_brush( list[idx].uuid );
            }
            ImGui::HelpPopup( "Select as active for brush." );
        }
        ImGui::SameLine();

        if( ImGui::ColorEdit4( "MyColor##3", ( float * )&list[idx].color,
                               ImGuiColorEditFlags_NoInputs | ImGuiColorEditFlags_NoLabel ) ) {
            state.mark_changed( "palette-entry-color" );
        }
        ImGui::SameLine();

        ImGui::SetNextItemWidth( ImGui::GetFrameHeight() );
        bool is_dupe_symbol = dupe_symbols.count( list[idx].key ) > 0;
        if( is_dupe_symbol ) {
            ImGui::BeginErrorArea();
        }
        if( ImGui::InputSymbol( "##key", list[idx].key.str, default_map_key.str.c_str() ) ) {
            state.mark_changed( "palette-entry-key" );
        }
        ImGui::HelpPopup( "Symbol to use on canvas." );
        if( is_dupe_symbol ) {
            ImGui::EndErrorArea();
        }
        {
            cata::optional<std::string> text;
            me_piece_alt_terrain *ptr = list[idx].mapping.get_first_piece_of_type<me_piece_alt_terrain>();
            if( ptr ) {
                text = ptr->fmt_data_summary();
            }
            ImGui::SameLine();
            ImGui::BeginDisabled();
            ImGui::Button(
                text ? text->c_str() : "-",
                ImVec2( ImGui::GetFrameHeight() * 8.0f, 0.0f )
            );
            ImGui::EndDisabled();
        }
        {
            cata::optional<std::string> text;
            me_piece_alt_furniture *ptr = list[idx].mapping.get_first_piece_of_type<me_piece_alt_furniture>();
            if( ptr ) {
                text = ptr->fmt_data_summary();
            }
            ImGui::SameLine();
            ImGui::BeginDisabled();
            ImGui::Button(
                text ? text->c_str() : "-",
                ImVec2( ImGui::GetFrameHeight() * 8.0f, 0.0f )
            );
            ImGui::EndDisabled();
        }
        ImGui::SameLine();
        if( ImGui::ArrowButton( "##mapping", ImGuiDir_Right ) ) {
            state.uistate->toggle_show_mapping( palette.uuid, list[idx].uuid );
        }
        ImGui::HelpPopup( "Show/hide mappings\nassociated with this symbol." );

        int additional_pieces = 0;
        std::string additional_summary;
        for( const auto &it : list[idx].mapping.pieces ) {
            if( it->get_type() == PieceType::AltTerrain || it->get_type() == PieceType::AltFurniture ) {
                continue;
            }
            additional_pieces += 1;
            additional_summary += it->fmt_summary();
            additional_summary += "\n";
        }
        if( additional_pieces > 0 ) {
            ImGui::SameLine();
            ImGui::Text( "+ %d", additional_pieces );
            ImGui::HelpPopup( additional_summary.c_str() );
        }
    } )
    .run( list );

    if( changed ) {
        state.mark_changed();
    }
}

void show_palette( me_state &state, me_palette &p, bool &show )
{
    ImGui::SetNextWindowSize( ImVec2( 670.0f, 120.0f ), ImGuiCond_FirstUseEver );
    ImGui::SetNextWindowPos( ImVec2( 50.0f, 50.0f ), ImGuiCond_FirstUseEver );

    std::string wnd_id = string_format( "Palette##palette-%d", p.uuid );
    if( !ImGui::Begin( wnd_id.c_str(), &show ) ) {
        ImGui::End();
        return;
    }
    ImGui::PushID( p.uuid );

    if( p.is_inline ) {
        ImGui::Text( "<inline palette>" );
    } else {
        ImGui::Text( "id: %s", p.id.data.c_str() );
    }

    show_palette_entries( state, p );

    ImGui::PopID();
    ImGui::End();
}

me_mapping::me_mapping( const me_mapping &rhs )
{
    *this = rhs;
}

me_mapping &me_mapping::operator=( const me_mapping &rhs )
{
    pieces.reserve( rhs.pieces.size() );
    for( const auto &piece : rhs.pieces ) {
        pieces.emplace_back( piece->clone() );
    }
    return *this;
}

bool me_mapping::has_piece_of_type( PieceType pt ) const
{
    for( const auto &piece : pieces ) {
        if( piece->get_type() == pt ) {
            return true;
        }
    }
    return false;
}

const map_key &me_palette::key_from_uuid( const uuid_t &uuid ) const
{
    if( uuid == UUID_INVALID ) {
        return default_map_key;
    }
    const me_palette_entry *entry = find_entry( uuid );
    if( entry ) {
        return entry->key;
    }

    std::cerr << "Tried to find palette key, but uuid was not found " << uuid << std::endl;
    std::abort();
}

const ImVec4 &me_palette::color_from_uuid( const uuid_t &uuid ) const
{
    if( uuid == UUID_INVALID ) {
        static ImVec4 default_color = ImVec4();
        return default_color;
    }
    const me_palette_entry *entry = find_entry( uuid );
    if( entry ) {
        return entry->color;
    }

    std::cerr << "Tried to find palette color, but uuid was not found " << uuid << std::endl;
    std::abort();
}

const SpriteRef *me_palette::sprite_from_uuid( const uuid_t &uuid ) const
{
    if( uuid == UUID_INVALID ) {
        return nullptr;
    }
    const me_palette_entry *entry = find_entry( uuid );
    if( entry ) {
        if( !entry->sprite_cache_valid ) {
            entry->build_sprite_cache();
        }
        if( !entry->sprite_cache ) {
            return nullptr;
        } else {
            return &*entry->sprite_cache;
        }
    }

    std::cerr << "Tried to find sprite, but uuid was not found " << uuid << std::endl;
    std::abort();
}

me_palette_entry *me_palette::find_entry( const uuid_t &uuid )
{
    if( uuid == UUID_INVALID ) {
        return nullptr;
    }
    for( auto &it : entries ) {
        if( it.uuid == uuid ) {
            return &it;
        }
    }
    return nullptr;
}

const me_palette_entry *me_palette::find_entry( const uuid_t &uuid ) const
{
    if( uuid == UUID_INVALID ) {
        return nullptr;
    }
    for( const auto &it : entries ) {
        if( it.uuid == uuid ) {
            return &it;
        }
    }
    return nullptr;
}

void me_palette_entry::build_sprite_cache() const
{
    sprite_cache.reset();

    // Try furniture tile
    {
        const me_piece_alt_furniture *ptr = mapping.get_first_piece_of_type<me_piece_alt_furniture>();
        if( ptr ) {
            auto list = ptr->list;
            if( !list.entries.empty() ) {
                sprite_cache = SpriteRef( list.entries[0].val.data );
            }
        }
    }

    // Try terrain tile
    if( !sprite_cache ) {
        const me_piece_alt_terrain *ptr = mapping.get_first_piece_of_type<me_piece_alt_terrain>();
        if( ptr ) {
            auto list = ptr->list;
            if( !list.entries.empty() ) {
                sprite_cache = SpriteRef( list.entries[0].val.data );
            }
        }
    }

    sprite_cache_valid = true;
}

} // namespace editor
