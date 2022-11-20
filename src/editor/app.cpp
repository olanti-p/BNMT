#include "app.h"

#include "editor_engine.h"
#include "project.h"
#include "state.h"
#include "title_screen.h"
#include "uistate.h"

#include "../fstream_utils.h"

namespace editor
{
me_main_app::me_main_app() = default;
me_main_app::me_main_app( me_main_app && ) = default;
me_main_app::~me_main_app() = default;

me_main_app &me_main_app::operator=( me_main_app && ) = default;

void init_app( me_main_app &app )
{
    app.title_state = std::make_unique<me_titlescreen_state>();
}

void show_app( me_main_app &app )
{
    if( app.editor_state ) {
        show_me_ui( *app.editor_state );
    } else if( app.title_state ) {
        show_title_screen( *app.title_state );
    }
}

void update_app_state( me_main_app &app )
{
    if( app.title_state && app.title_state->ret ) {
        const editor::titlescreen_ui_retval &retval = *app.title_state->ret;
        if( retval.exit ) {
            if( retval.exit_to_desktop ) {
                app.run_state.do_exit_to_dektop = true;
            } else {
                app.run_state.do_exit_to_game = true;
            }
        } else if( retval.make_new ) {
            app.editor_state = std::make_unique<me_state>();
            std::string project_uuid = app.editor_state->project().project_uuid;
            set_project_ini_path( project_uuid );
        } else if( retval.load_existing ) {
            std::unique_ptr<me_project> f = std::make_unique<me_project>();
            auto reader = [&]( JsonIn & jsin ) {
                f->deserialize( jsin );
            };
            if( read_from_file_json( retval.load_path, reader ) ) {
                app.editor_state = std::make_unique<me_state>( std::move( f ), &retval.load_path );
                std::string project_uuid = app.editor_state->project().project_uuid;
                set_project_ini_path( project_uuid );
            } else {
                app.title_state->popup_prompt =
                    string_format( "Failed to load project:\n%s\nSee debug.log for details.", retval.load_path );
            }
        }
        app.title_state->ret.reset();
    } else if( app.editor_state && !app.editor_state->uistate->do_loop ) {
        app.editor_state.reset();
        set_default_ini_path();
    }
}

} // namespace editor
