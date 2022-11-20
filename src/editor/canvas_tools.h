#ifndef CATA_SRC_EDITOR_CANVAS_TOOLS_H
#define CATA_SRC_EDITOR_CANVAS_TOOLS_H

#include "uuid.h"
#include "../enum_traits.h"

#include <cassert>


namespace editor
{
struct me_state;

enum class CanvasTool {
    Brush,
    Bucket,
    BucketGlobal,

    _Num,
};

struct me_canvas_tools_state {
    public:
        void serialize( JsonOut &jsout ) const;
        void deserialize( JsonIn &jsin );

        inline const uuid_t &get_brush() const {
            return brush;
        }

        inline void set_brush( const uuid_t &uuid ) {
            assert( !ongoing_tool_operation );
            brush = uuid;
        }

        inline CanvasTool get_tool() const {
            return tool;
        }

        inline void set_tool( CanvasTool t ) {
            assert( !ongoing_tool_operation );
            tool = t;
        }

        inline void start_tool_operation() {
            assert( !ongoing_tool_operation );
            ongoing_tool_operation = true;
        }

        inline bool has_ongoing_tool_operation() {
            return ongoing_tool_operation;
        }

        inline bool end_tool_operation() {
            assert( ongoing_tool_operation );
            ongoing_tool_operation = false;
            bool ret = tool_op_changed_data;
            tool_op_changed_data = false;
            return ret;
        }

        inline void set_tool_operation_changed_data() {
            assert( ongoing_tool_operation );
            tool_op_changed_data = true;
        }

    private:
        bool ongoing_tool_operation = false;
        bool tool_op_changed_data = false;
        CanvasTool tool = CanvasTool::Brush;
        uuid_t brush = UUID_INVALID;
};

/**
 * =============== Windows ===============
 */
void show_toolbar( me_canvas_tools_state &tools, bool &show );

} // namespace editor

template<>
struct enum_traits<editor::CanvasTool> {
    static constexpr editor::CanvasTool last = editor::CanvasTool::_Num;
};

#endif // CATA_SRC_EDITOR_CANVAS_TOOLS_H
