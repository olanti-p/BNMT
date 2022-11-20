#ifndef CATA_SRC_EDITOR_TITLE_SCREEN_H
#define CATA_SRC_EDITOR_TITLE_SCREEN_H

#include "../options.h"

#include "imgui.h"

namespace editor
{

struct titlescreen_ui_retval {
    bool exit = false;
    bool exit_to_desktop = false;
    bool make_new = false;
    bool load_existing = false;
    std::string load_path;
};

struct me_titlescreen_state {
    me_titlescreen_state() = default;
    me_titlescreen_state( const me_titlescreen_state & ) = delete;
    me_titlescreen_state( me_titlescreen_state && ) = default;
    ~me_titlescreen_state() = default;

    me_titlescreen_state &operator=( const me_titlescreen_state & ) = delete;
    me_titlescreen_state &operator=( me_titlescreen_state && ) = default;

    bool open_file_dialog = false;
    cata::optional<titlescreen_ui_retval> ret;
    cata::optional<std::string> popup_prompt;
};

/**
 * ============= Entry point =============
 */
void show_title_screen( me_titlescreen_state &state );

} // namespace editor

#endif // CATA_SRC_EDITOR_TITLE_SCREEN_H
