#ifndef CATA_SRC_EDITOR_APP_H
#define CATA_SRC_EDITOR_APP_H

#include <memory>

namespace editor
{
struct me_titlescreen_state;
struct me_state;

struct app_run_state {
    bool do_exit_to_game = false;
    bool do_exit_to_dektop = false;
};

struct me_main_app {
    me_main_app();
    me_main_app( const me_main_app & ) = delete;
    me_main_app( me_main_app && );
    ~me_main_app();

    me_main_app &operator=( const me_main_app & ) = delete;
    me_main_app &operator=( me_main_app && );

    std::unique_ptr<me_titlescreen_state> title_state;
    std::unique_ptr<me_state> editor_state;

    app_run_state run_state;
};

/**
 * =============== Helpers ===============
 */
void init_app( me_main_app &app );
void update_app_state( me_main_app &app );

/**
 * =============== Windows ===============
 */
void show_app( me_main_app &app );


} // namespace editor

#endif // CATA_SRC_EDITOR_APP_H
