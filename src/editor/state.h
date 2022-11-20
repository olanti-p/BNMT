#ifndef CATA_SRC_EDITOR_STATE_H
#define CATA_SRC_EDITOR_STATE_H

#include "../pimpl.h"

namespace editor
{
struct asset_library;
struct me_history_state;
struct me_project;
struct me_save_export_state;
struct me_uistate;

struct me_state {
    me_state();
    explicit me_state( std::unique_ptr<me_project> &&project );
    me_state( std::unique_ptr<me_project> &&project, const std::string *loaded_from_path );
    me_state( const me_state & ) = delete;
    me_state( me_state && );
    ~me_state();

    me_state &operator=( const me_state & ) = delete;
    me_state &operator=( me_state && );

    pimpl<me_history_state> histate;
    pimpl<me_save_export_state> sestate;
    me_uistate *uistate = nullptr;

    me_project &project();

    /**
     * Mark project as changed.
     *
     * @param id (optional) If edit operation repeatedly generates change events that should be
     *           collapsed into a single undo/redo operation, pass id of the operation here.
     *           Respects current ImGui id stack.
     */
    void mark_changed( const char *id = nullptr );

    /**
     * Check whether project has been marked as changed.
     */
    bool is_changed() const;
};

/**
 * ============= Entry point =============
 */
void show_me_ui( me_state &state );

} // namespace editor

#endif // CATA_SRC_EDITOR_STATE_H
