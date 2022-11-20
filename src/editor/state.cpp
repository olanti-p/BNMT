#include "state.h"

#include "project.h"
#include "history.h"
#include "save_and_export.h"
#include "uistate.h"

namespace editor
{

void show_me_ui( me_state &state )
{
    run_ui_for_state( state );
}

me_state::me_state() : me_state( create_empty_project() ) { }

me_state::me_state( std::unique_ptr<me_project> &&project ) : me_state( std::move( project ),
            nullptr ) { }

me_state::me_state( std::unique_ptr<me_project> &&project,
                    const std::string *loaded_from_path ) : histate( std::move( project ), !!loaded_from_path )
{
    if( loaded_from_path ) {
        sestate->file_save_path = *loaded_from_path;
    }
}

me_state::~me_state() = default;

me_state::me_state( me_state && ) = default;
me_state &me_state::operator=( me_state && ) = default;

me_project &me_state::project()
{
    return histate->project();
}

void me_state::mark_changed( const char *id )
{
    histate->mark_changed( id );
}

bool me_state::is_changed() const
{
    return histate->is_changed();
}

} // namespace editor
