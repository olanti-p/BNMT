#include "canvas_tools.h"

#include "widgets.h"
#include "imgui.h"

namespace editor
{

void show_toolbar( me_canvas_tools_state &tools, bool &show )
{
    if( !ImGui::Begin( "Toolbar", &show,
                       ImGuiWindowFlags_AlwaysAutoResize |
                       ImGuiWindowFlags_NoCollapse |
                       ImGuiWindowFlags_NoResize
                     ) ) {
        ImGui::End();
        return;
    }

    if( ImGui::RadioButton( "Brush", tools.get_tool() == CanvasTool::Brush ) ) {
        tools.set_tool( CanvasTool::Brush );
    }
    ImGui::HelpPopup( "Hold LMB to draw with selected tile." );
    if( ImGui::RadioButton( "Bucket", tools.get_tool() == CanvasTool::Bucket ) ) {
        tools.set_tool( CanvasTool::Bucket );
    }
    ImGui::HelpPopup( "Click LMB to flood fill with selected tile." );
    if( ImGui::RadioButton( "Bucket (global)", tools.get_tool() == CanvasTool::BucketGlobal ) ) {
        tools.set_tool( CanvasTool::BucketGlobal );
    }
    ImGui::HelpPopup( "Click LMB to replace all such tiles with selected tile." );

    ImGui::End();
}

} // namespace editor
