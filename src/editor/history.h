#ifndef CATA_SRC_EDITOR_HISTORY_H
#define CATA_SRC_EDITOR_HISTORY_H

#include <memory>
#include <vector>
#include <string>

#include "../optional.h"
#include "imgui.h"

namespace editor
{
struct me_project;
struct me_canvas_tools_state;

struct me_file_revision {
    std::unique_ptr<me_project> project;
    int num = 0;

    me_file_revision();
    me_file_revision( const me_file_revision & ) = delete;
    me_file_revision( me_file_revision && );
    ~me_file_revision();

    me_file_revision &operator=( const me_file_revision & ) = delete;
    me_file_revision &operator=( me_file_revision && );

    me_file_revision make_copy() const;
};

struct me_history_state {
    me_history_state() = default;
    ~me_history_state() = default;
    me_history_state( std::unique_ptr<me_project> &&project, bool was_loaded );

    me_history_state( const me_history_state & ) = delete;
    me_history_state( me_history_state && ) = default;
    me_history_state &operator=( const me_history_state & ) = delete;
    me_history_state &operator=( me_history_state && ) = default;

    inline me_project &project() {
        return *current_revision.project;
    }

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
    inline bool is_changed() const {
        return file_has_changes;
    }

    inline bool can_undo() const {
        return current_revision.num != file_history[file_history.size() - 1].num;
    }

    inline void queue_undo() {
        switch_to_revision = current_revision.num - 1;
    }

    inline bool can_redo() const {
        return current_revision.num != file_history[0].num;
    }

    inline void queue_redo() {
        switch_to_revision = current_revision.num + 1;
    }

    bool has_unsaved_changes() const;
    bool has_unexported_changes() const;

    bool file_has_changes = false;
    cata::optional<ImGuiID> current_widget_changed = 0;
    std::string current_widget_changed_str;
    cata::optional<ImGuiID> last_widget_changed = 0;
    cata::optional<int> switch_to_revision;
    me_file_revision current_revision;
    std::vector<me_file_revision> file_history;
    int history_capacity = 200;
    cata::optional<int> last_saved_revision;
    cata::optional<int> last_exported_revision;
    int edit_counter = 0;
};

/**
 * =============== Windows ===============
 */
void show_file_history( me_history_state &state, bool &show );

void handle_revision_change( me_history_state &state, me_canvas_tools_state &tools );

} // namespace editor

#endif // CATA_SRC_EDITOR_HISTORY_H
