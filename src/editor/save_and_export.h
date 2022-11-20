#ifndef CATA_SRC_EDITOR_SAVE_AND_EXPORT_H
#define CATA_SRC_EDITOR_SAVE_AND_EXPORT_H

#include "../optional.h"

#include <string>

namespace editor
{
struct me_state;

struct me_save_export_state {
    me_save_export_state() = default;
    ~me_save_export_state() = default;

    me_save_export_state( const me_save_export_state & ) = delete;
    me_save_export_state( me_save_export_state && ) = default;
    me_save_export_state &operator=( const me_save_export_state & ) = delete;
    me_save_export_state &operator=( me_save_export_state && ) = default;

    bool open_save_as = false;
    bool do_save = false;
    bool do_exit_after_save = false;
    cata::optional<std::string> file_save_path;

    bool open_export_as = false;
    bool do_export = false;
    cata::optional<std::string> file_export_path;
};

void handle_file_saving( me_state &state );
void handle_file_exporting( me_state &state );

void save_on_close_widget_block( me_state &state, bool keep_open );
void save_and_export_widget_block( me_state &state );

} // namespace editor

#endif // CATA_SRC_EDITOR_SAVE_AND_EXPORT_H
