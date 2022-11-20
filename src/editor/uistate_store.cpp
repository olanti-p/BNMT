#include "uistate_store.h"

#include "editor_engine.h"
#include "uistate.h"
#include "../json.h"
#include "../fstream_utils.h"

#include <iostream>
#include <map>
#include <memory>

#ifdef DebugLog
#  undef DebugLog
#endif

#include "imgui_internal.h"

namespace editor
{
// TODO: Simplify this, there can only be a single instance of ui state alive in each project file.
//       It's 4 am, and it works, so I'm leaving it here as is.
static std::map<std::string, std::unique_ptr<me_uistate>> pdata;

me_uistate &get_uistate_for_project( const std::string &project_uuid )
{
    auto it = pdata.find( project_uuid );
    if( it != pdata.end() ) {
        assert( it->second );
        return *it->second;
    }
    me_uistate *ptr = pdata.emplace( project_uuid, std::make_unique<me_uistate>() ).first->second.get();
    assert( ptr );
    return *ptr;
}

void MyUserData_ClearAll( ImGuiContext *ctx, ImGuiSettingsHandler *handler )
{
    pdata.clear();
}

void MyUserData_ApplyAll( ImGuiContext *ctx, ImGuiSettingsHandler *handler )
{
    // TODO: is this needed?
}

void *MyUserData_ReadOpen( ImGuiContext *ctx, ImGuiSettingsHandler *handler, const char *name )
{
    std::string project_uuid = name;
    me_uistate *ptr = &get_uistate_for_project( project_uuid );
    return ptr;
}

void MyUserData_ReadLine( ImGuiContext *ctx, ImGuiSettingsHandler *handler, void *entry,
                          const char *line )
{
    me_uistate &uistate = *static_cast<me_uistate *>( entry );

    std::string_view line_str = line;

    if( line_str.substr( 0, 5 ) != "JSON=" ) {
        return;
    }
    std::string json_data( line_str.substr( 5 ) );
    deserialize( uistate, json_data );
}

void MyUserData_WriteAll( ImGuiContext *ctx, ImGuiSettingsHandler *handler,
                          ImGuiTextBuffer *out_buf )
{
    for( const auto &it : pdata ) {
        out_buf->appendf( "[%s][%s]\n", handler->TypeName, it.first.c_str() );
        std::string data = serialize( *it.second );
        out_buf->appendf( "JSON=%s\n", data.c_str() );
        out_buf->append( "\n" );
    }
}

void initialize_settings_export()
{
    ImGuiSettingsHandler ini_handler;
    ini_handler.TypeName = "ProjectUiState";
    ini_handler.TypeHash = ImHashStr( "ProjectUiState" );
    ini_handler.ClearAllFn = MyUserData_ClearAll;
    ini_handler.ApplyAllFn = MyUserData_ApplyAll;
    ini_handler.ReadOpenFn = MyUserData_ReadOpen;
    ini_handler.ReadLineFn = MyUserData_ReadLine;
    ini_handler.WriteAllFn = MyUserData_WriteAll;
    ImGui::AddSettingsHandler( &ini_handler );
}

} // namespace editor

namespace io
{

template<>
std::string enum_to_string<editor::CanvasTool>( editor::CanvasTool data )
{
    switch( data ) {
        // *INDENT-OFF*
        case editor::CanvasTool::Brush: return "Brush";
        case editor::CanvasTool::Bucket: return "Bucket";
        case editor::CanvasTool::BucketGlobal: return "BucketGlobal";
        // *INDENT-ON*
        case editor::CanvasTool::_Num:
            break;
    }
    debugmsg( "Invalid editor::CanvasTool" );
    abort();
}
} // namespace io

namespace editor
{
namespace detail
{

void open_palette::serialize( JsonOut &jsout ) const
{
    jsout.start_object();
    jsout.member( "uuid", uuid );
    jsout.member( "open", open );
    jsout.end_object();
}

void open_palette::deserialize( JsonIn &jsin )
{
    JsonObject jo = jsin.get_object();

    jo.read( "uuid", uuid );
    jo.read( "open", open );
}

void open_mapping::serialize( JsonOut &jsout ) const
{
    jsout.start_object();
    jsout.member( "uuid", uuid );
    jsout.member( "palette", palette );
    jsout.member( "open", open );
    jsout.end_object();
}

void open_mapping::deserialize( JsonIn &jsin )
{
    JsonObject jo = jsin.get_object();

    jo.read( "uuid", uuid );
    jo.read( "palette", palette );
    jo.read( "open", open );
}

void open_mapgenobject::serialize( JsonOut &jsout ) const
{
    jsout.start_object();
    jsout.member( "uuid", uuid );
    jsout.member( "open", open );
    jsout.end_object();
}

void open_mapgenobject::deserialize( JsonIn &jsin )
{
    JsonObject jo = jsin.get_object();

    jo.read( "uuid", uuid );
    jo.read( "open", open );
}

} // namespace detail

void me_uistate::serialize( JsonOut &jsout ) const
{
    // These are intentionally omitted:
    // - do_loop
    jsout.start_object();
    jsout.member( "show_demo_wnd", show_demo_wnd );
    jsout.member( "show_asset_lib", show_asset_lib );
    jsout.member( "show_file_info", show_file_info );
    jsout.member( "show_file_history", show_file_history );
    jsout.member( "show_toolbar", show_toolbar );
    jsout.member( "active_file_id", active_file_id );
    jsout.member( "open_palettes", open_palettes );
    jsout.member( "open_mappings", open_mappings );
    jsout.member( "open_mapgenobjects", open_mapgenobjects );
    jsout.member( "camera", camera );
    jsout.member( "tools_state", tools_state );
    jsout.member( "expanded_mapping_pieces", expanded_mapping_pieces );
    jsout.member( "expanded_mapobjects", expanded_mapobjects );
    jsout.end_object();
}

void me_uistate::deserialize( JsonIn &jsin )
{
    JsonObject jo = jsin.get_object();

    jo.read( "show_demo_wnd", show_demo_wnd );
    jo.read( "show_asset_lib", show_asset_lib );
    jo.read( "show_file_info", show_file_info );
    jo.read( "show_file_history", show_file_history );
    jo.read( "show_toolbar", show_toolbar );
    jo.read( "active_file_id", active_file_id );
    jo.read( "open_palettes", open_palettes );
    jo.read( "open_mappings", open_mappings );
    jo.read( "open_mapgenobjects", open_mapgenobjects );
    jo.read( "camera", camera );
    jo.read( "tools_state", tools_state );
    jo.read( "expanded_mapping_pieces", expanded_mapping_pieces );
    jo.read( "expanded_mapobjects", expanded_mapobjects );
}

void me_camera::serialize( JsonOut &jsout ) const
{
    // These are intentionally omitted:
    // - drag_delta
    jsout.start_object();
    jsout.member( "pos", pos );
    jsout.member( "scale", scale );
    jsout.end_object();
}

void me_camera::deserialize( JsonIn &jsin )
{
    JsonObject jo = jsin.get_object();

    jo.read( "pos", pos );
    jo.read( "scale", scale );
}

void me_canvas_tools_state::serialize( JsonOut &jsout ) const
{
    // These are intentionally omitted:
    // - ongoing_tool_operation
    // - ongoing_brush_stroke
    // - brush_stroke_changed_data
    jsout.start_object();
    jsout.member_as_string( "tool", tool );
    jsout.member( "brush", brush );
    jsout.end_object();
}

void me_canvas_tools_state::deserialize( JsonIn &jsin )
{
    JsonObject jo = jsin.get_object();

    jo.read( "tool", tool );
    jo.read( "brush", brush );
}

} // namespace editor
