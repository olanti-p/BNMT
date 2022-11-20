#include "state_export.h"

#include "../fstream_utils.h"
#include "../game.h"

#include "canvas_tools.h"
#include "project.h"
#include "history.h"
#include "save_and_export.h"
#include "state.h"
#include "uistate.h"
#include "widgets.h"
#include "ImGuiFileDialog.h"

namespace editor
{
void handle_file_saving( me_state &state )
{
    if( state.uistate->tools_state->has_ongoing_tool_operation() ) {
        return;
    }

    me_save_export_state &sestate = *state.sestate;

    if( ImGui::IsKeyDown( ImGuiKey_LeftCtrl ) && ImGui::IsKeyPressed( ImGuiKey_S ) ) {
        if( ImGui::IsKeyDown( ImGuiKey_LeftShift ) || !sestate.file_save_path ) {
            sestate.open_save_as = true;
        } else {
            sestate.do_save = true;
        }
    }

    if( sestate.open_save_as ) {
        sestate.open_save_as = false;
        ImGui::SetNextWindowSize( ImVec2( 580, 380 ), ImGuiCond_FirstUseEver );
        ImGuiFileDialog::Instance()->OpenDialog( "SaveToFile",
                "Save As...", ".json",
                sestate.file_save_path ? *sestate.file_save_path : ".",
                1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite );
    }

    if( ImGuiFileDialog::Instance()->Display( "SaveToFile" ) ) {
        if( ImGuiFileDialog::Instance()->IsOk() ) {
            sestate.file_save_path = ImGuiFileDialog::Instance()->GetFilePathName();
            sestate.do_save = true;
        } else {
            sestate.do_exit_after_save = false;
        }
        ImGuiFileDialog::Instance()->Close();
    }

    if( sestate.do_save ) {
        sestate.do_save = false;
        assert( sestate.file_save_path );
        write_to_file( *sestate.file_save_path, [&]( std::ostream & oss ) {
            oss << serialize( state.project() );
        } );
        state.histate->last_saved_revision = state.histate->current_revision.num;
        if( sestate.do_exit_after_save ) {
            state.uistate->do_loop = false;
        }
    }
}

void handle_file_exporting( me_state &state )
{
    if( state.uistate->tools_state->has_ongoing_tool_operation() ) {
        return;
    }

    me_save_export_state &sestate = *state.sestate;

    if( ImGui::IsKeyDown( ImGuiKey_LeftCtrl ) && ImGui::IsKeyPressed( ImGuiKey_E ) ) {
        if( ImGui::IsKeyDown( ImGuiKey_LeftShift ) || !sestate.file_export_path ) {
            sestate.open_export_as = true;
        } else {
            sestate.do_export = true;
        }
    }

    if( sestate.open_export_as ) {
        sestate.open_export_as = false;
        ImGui::SetNextWindowSize( ImVec2( 580, 380 ), ImGuiCond_FirstUseEver );
        ImGuiFileDialog::Instance()->OpenDialog( "ExportToFile",
                "Export As...", ".json",
                sestate.file_export_path ? *sestate.file_export_path : ".",
                1, nullptr, ImGuiFileDialogFlags_ConfirmOverwrite );
    }

    if( ImGuiFileDialog::Instance()->Display( "ExportToFile" ) ) {
        if( ImGuiFileDialog::Instance()->IsOk() ) {
            sestate.file_export_path = ImGuiFileDialog::Instance()->GetFilePathName();
            sestate.do_export = true;
        }
        ImGuiFileDialog::Instance()->Close();
    }

    if( g->export_editor_project_on_start ) {
        sestate.file_export_path = *g->export_editor_project_on_start;
        sestate.do_export = true;
        g->export_editor_project_on_start.reset();
    }

    if( sestate.do_export ) {
        sestate.do_export = false;
        assert( sestate.file_export_path );
        write_to_file( *sestate.file_export_path, [&]( std::ostream & oss ) {
            std::string s = editor_export::to_string( state.project() );
            oss << editor_export::format_string( s );
        } );
        state.histate->last_exported_revision = state.histate->current_revision.num;
    }
}

void save_on_close_widget_block( me_state &state, bool keep_open )
{
    if( !keep_open ) {
        if( state.histate->has_unsaved_changes() ) {
            ImGui::OpenPopup( "###warn-unsaved-on-close" );
        } else {
            state.uistate->do_loop = false;
        }
    }

    me_save_export_state &sestate = *state.sestate;

    if( ImGui::BeginPopupModal( "###warn-unsaved-on-close", nullptr,
                                ImGuiWindowFlags_AlwaysAutoResize ) ) {
        ImGui::TextCentered( "Do you want to save the changes?" );
        ImGui::Text( " " );
        ImVec2 btn_sz( ImGui::GetFrameHeight() * 5.0f, ImGui::GetFrameHeight() );
        if( ImGui::Button( "Don't Save", btn_sz ) ) {
            ImGui::CloseCurrentPopup();
            state.uistate->do_loop = false;
        }
        ImGui::SameLine();
        if( ImGui::Button( "Cancel", btn_sz ) ) {
            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if( ImGui::Button( "Save", btn_sz ) ) {
            ImGui::CloseCurrentPopup();
            sestate.do_exit_after_save = true;
            if( sestate.file_save_path ) {
                sestate.do_save = true;
            } else {
                sestate.open_save_as = true;
            }
        }
        ImGui::EndPopup();
    }
}

void save_and_export_widget_block( me_state &state )
{
    me_save_export_state &sestate = *state.sestate;

    std::string save_btn = string_format( "%sSave###save-button",
                                          state.histate->has_unsaved_changes() ? "* " : "" );
    if( ImGui::Button( save_btn.c_str() ) ) {
        if( !sestate.file_save_path ) {
            sestate.open_save_as = true;
        } else {
            sestate.do_save = true;
        }
    }
    ImGui::SameLine();
    if( ImGui::Button( "Save As..." ) ) {
        sestate.open_save_as = true;
    }

    handle_file_saving( state );

    std::string export_btn = string_format( "%sExport###export-button",
                                            state.histate->has_unexported_changes() ? "* " : "" );
    if( ImGui::Button( export_btn.c_str() ) ) {
        if( !sestate.file_export_path ) {
            sestate.open_export_as = true;
        } else {
            sestate.do_export = true;
        }
    }
    ImGui::SameLine();
    if( ImGui::Button( "Export As..." ) ) {
        sestate.open_export_as = true;
    }

    handle_file_exporting( state );
}

} // namespace editor
