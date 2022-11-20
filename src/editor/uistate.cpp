#include "uistate.h"

#include "camera.h"
#include "canvas_tools.h"
#include "canvas.h"
#include "file.h"
#include "palette.h"
#include "project.h"
#include "uistate_store.h"
#include "history.h"
#include "save_and_export.h"
#include "state.h"
#include "uistate.h"
#include "widgets.h"

namespace editor
{
me_uistate::me_uistate() = default;
me_uistate::me_uistate( me_uistate && ) = default;
me_uistate::~me_uistate() = default;
me_uistate &me_uistate::operator=( me_uistate && ) = default;

void me_uistate::toggle_show_palette( uuid_t uuid )
{
    for( auto &it : open_palettes ) {
        if( it.uuid == uuid ) {
            it.open = false;
            return;
        }
    }
    open_palettes.emplace_back();
    open_palettes.back().uuid = uuid;
}

void me_uistate::toggle_show_mapping( uuid_t palette, uuid_t uuid )
{
    for( auto &it : open_mappings ) {
        if( it.uuid == uuid && it.palette == palette ) {
            it.open = false;
            return;
        }
    }
    open_mappings.emplace_back();
    open_mappings.back().uuid = uuid;
    open_mappings.back().palette = palette;
}

void me_uistate::toggle_show_mapobjects( uuid_t uuid )
{
    for( auto &it : open_mapgenobjects ) {
        if( it.uuid == uuid ) {
            it.open = false;
            return;
        }
    }
    open_mapgenobjects.emplace_back();
    open_mapgenobjects.back().uuid = uuid;
}

void show_ui_control_window( me_state &state )
{
    bool keep_open = true;
    ImGui::Begin( "Advanced Map Editor", &keep_open );
    ImGui::Text( "Close this window to close the project." );

    save_on_close_widget_block( state, keep_open );

    me_uistate &uistate = *state.uistate;

    // Controls
    if( ImGui::Button( "Toggle Demo Window" ) ) {
        uistate.show_demo_wnd = !uistate.show_demo_wnd;
    }
    ImGui::SameLine();
    if( ImGui::Button( "Toggle File Info" ) ) {
        uistate.show_file_info = !uistate.show_file_info;
    }

    if( ImGui::Button( "Toggle History" ) ) {
        uistate.show_file_history = !uistate.show_file_history;
    }
    ImGui::SameLine();
    if( ImGui::Button( "Toggle Toolbar" ) ) {
        uistate.show_toolbar = !uistate.show_toolbar;
    }

    save_and_export_widget_block( state );

    // Camera
    {
        ImGui::TextDisabled( "(?: Camera contols)" );
        ImGui::HelpPopup(
            "Camera controls:\n\n"
            "- Drag the view with RMB to pan.\n"
            "- Scroll over the view to zoom.\n"
            "- Use widgets below to manually control zoom and position.\n"
            "- Hold Ctrl to see summary of highlighted tile.\n"
            "\nIn canvas mode:\n"
            "- Press MMB (mouse wheel) on tile to select it.\n"
            "- Press MMB outside bounds (or on empty tile) to clear selection."
        );
        ImGui::DragInt( "Zoom", &uistate.camera->scale, 0.2f, MIN_SCALE, MAX_SCALE );
        ImGui::DragPoint( "Pos", &uistate.camera->pos, 1.0f, -10000, 10000 );
    }

    // Mouse position
    {
        point_abs_screen screen_pos = get_mouse_pos();
        point_abs_etile etile_pos = get_mouse_tile_pos( *uistate.camera );
        ImGui::Text( "Mouse pos, px: %s", screen_pos.to_string().c_str() );
        ImGui::Text( "Mouse pos, tile: %s", etile_pos.to_string().c_str() );
    }

    ImGui::End();
}

void run_ui_for_state( me_state &state )
{
    if( !state.uistate ) {
        state.uistate = &get_uistate_for_project( state.project().project_uuid );
    }

    me_project &proj = state.project();
    show_project_ui( state, proj );

    me_file *active_file = nullptr;
    if( state.uistate->active_file_id ) {
        active_file = proj.get_file_by_uuid( *state.uistate->active_file_id );
        if( !active_file ) {
            state.uistate->active_file_id.reset();
        }
    }

    // TODO: multiple files on same canvas
    show_canvas( state, active_file );
    show_ui_control_window( state );

    me_uistate &uistate = *state.uistate;

    if( uistate.show_demo_wnd ) {
        ImGui::ShowDemoWindow( &uistate.show_demo_wnd );
    }
    if( uistate.show_file_info && active_file ) {
        show_file_info( state, *active_file, uistate.show_file_info );
    }
    if( uistate.show_file_history ) {
        show_file_history( *state.histate, uistate.show_file_history );
    }
    if( uistate.show_toolbar ) {
        show_toolbar( *uistate.tools_state, uistate.show_toolbar );
    }

    for( auto &it : uistate.open_palettes ) {
        if( !it.open ) {
            continue;
        }
        me_palette *pal = proj.get_palette_by_uuid( it.uuid );
        if( pal ) {
            show_palette( state, *pal, it.open );
        } else {
            it.open = false;
        }
    }
    for( auto &it : uistate.open_mappings ) {
        if( !it.open ) {
            continue;
        }
        me_palette *pal = proj.get_palette_by_uuid( it.palette );
        if( pal ) {
            me_palette_entry *entry = pal->find_entry( it.uuid );
            if( entry ) {
                show_mapping( state, *pal, *entry, it.open );
            } else {
                it.open = false;
            }
        } else {
            it.open = false;
        }
    }
    for( auto &it : uistate.open_mapgenobjects ) {
        if( !it.open ) {
            continue;
        }
        me_file *f = proj.get_file_by_uuid( it.uuid );
        if( f ) {
            show_mapobjects( state, *f, it.open );
        } else {
            it.open = false;
        }
    }
    for( auto it = uistate.open_palettes.cbegin(); it != uistate.open_palettes.cend(); ) {
        if( !it->open ) {
            for( auto &mit : uistate.open_mappings ) {
                if( mit.palette == it->uuid ) {
                    mit.open = false;
                }
            }
            it = uistate.open_palettes.erase( it );
        } else {
            it++;
        }
    }
    for( auto it = uistate.open_mappings.cbegin(); it != uistate.open_mappings.cend(); ) {
        if( !it->open ) {
            it = uistate.open_mappings.erase( it );
        } else {
            it++;
        }
    }
    for( auto it = uistate.open_mapgenobjects.cbegin(); it != uistate.open_mapgenobjects.cend(); ) {
        if( !it->open ) {
            it = uistate.open_mapgenobjects.erase( it );
        } else {
            it++;
        }
    }

    handle_revision_change( *state.histate, *uistate.tools_state );
}

} // namespace editor
