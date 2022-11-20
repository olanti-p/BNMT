#ifndef CATA_SRC_EDITOR_UISTATE_H
#define CATA_SRC_EDITOR_UISTATE_H

#include "../optional.h"
#include "../pimpl.h"

#include "uuid.h"
#include "camera.h"
#include "canvas_tools.h"

namespace editor
{
struct me_state;
struct me_camera;
struct me_canvas_tools_state;

namespace detail
{
struct open_palette {
    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );

    uuid_t uuid = UUID_INVALID;
    bool open = true;
};

struct open_mapping {
    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );

    uuid_t palette = UUID_INVALID;
    uuid_t uuid = UUID_INVALID;
    bool open = true;
};

struct open_mapgenobject {
    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );

    uuid_t uuid = UUID_INVALID;
    bool open = true;
};
} // namespace detail

/**
 * Editor UI state.
 *
 * It's saving and loading is managed by imgui settings save/load routines,
 * so it may fall out of sync with the project state (e.g. use nonexistent uuids).
 *
 * As such, it is required to check members for validity when using them,
 * and in case of invalidation - reset to some neutral but valid state.
*/
struct me_uistate {
    me_uistate();
    me_uistate( const me_uistate & ) = delete;
    me_uistate( me_uistate && );
    ~me_uistate();

    me_uistate &operator=( const me_uistate & ) = delete;
    me_uistate &operator=( me_uistate && );

    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );

    bool do_loop = true; // Setting this to false will quit the editor
    bool show_demo_wnd = false; // Whether to show ImGui Demo window
    bool show_asset_lib = false; // Whether to show asset library
    bool show_file_info = true; // Whether to show file info
    bool show_file_history = true; // Whether to show undo/redo history
    bool show_toolbar = true; // Whether to show canvas toolbar
    cata::optional<uuid_t> active_file_id; // UUID of active file

    std::vector<detail::open_palette> open_palettes; // List of open palettes
    std::vector<detail::open_mapping> open_mappings; // List of open mappings
    std::vector<detail::open_mapgenobject> open_mapgenobjects; // List of open mapgenobjects

    pimpl<me_camera> camera;
    pimpl<me_canvas_tools_state> tools_state;

    std::set<uuid_t> expanded_mapping_pieces;
    std::set<uuid_t> expanded_mapobjects;

    void toggle_show_palette( uuid_t uuid );
    void toggle_show_mapping( uuid_t palette, uuid_t uuid );
    void toggle_show_mapobjects( uuid_t uuid );
};

/**
 * =============== Windows ===============
 */
void show_ui_control_window( me_state &state );
void run_ui_for_state( me_state &state );

} // namespace editor

#endif // CATA_SRC_EDITOR_UISTATE_H
