#include "file.h"

#include "palette.h"
#include "state.h"
#include "project.h"
#include "uistate.h"
#include "widgets.h"

namespace editor
{

static void show_canvas_hint()
{
    ImGui::Text( "Use mouse to paint canvas with palette entries." );
}

void show_file_info( me_state &state, me_file &file, bool &show )
{
    if( !ImGui::Begin( "File Info", &show ) ) {
        ImGui::End();
        return;
    }
    ImGui::PushID( file.uuid );

    ImGui::Text( "Mapgen type:" );
    if( ImGui::RadioButton( "Oter", file.mtype == MapgenType::Oter ) ) {
        file.mtype = MapgenType::Oter;
        file.base.set_size( file.mapgensize().raw() );
        state.mark_changed();
    }
    ImGui::HelpPopup(
        "Overmap terrain mapgen.\n\n"
        "Must be assigned to one (or more) overmap terrain types.\n"
        "When game generates local map for an omt, it randomly selects one of the overmap mapgens associated "
        "with given omt's type and runs it, then applies automatic transformations such as rotation.\n"
        "Each omt type must have at least 1 omt mapgen assigned to it."
    );
    ImGui::SameLine();
    if( ImGui::RadioButton( "Update", file.mtype == MapgenType::Update ) ) {
        file.mtype = MapgenType::Update;
        file.base.set_size( file.mapgensize().raw() );
        state.mark_changed();
    }
    ImGui::HelpPopup(
        "Update mapgen.\n\n"
        "Invoked by basecamp upgrade routines.\n"
        "Can be used for automatic calculation of camp blueprint requirements."
    );
    ImGui::SameLine();
    if( ImGui::RadioButton( "Nested", file.mtype == MapgenType::Nested ) ) {
        file.mtype = MapgenType::Nested;
        file.base.set_size( file.mapgensize().raw() );
        state.mark_changed();
    }
    ImGui::HelpPopup(
        "Nested mapgen.\n\n"
        "Can be invoked by omt and upgrate mapgens.\n"
        "This is essentially a 'chunk' of any size up to 24x24 that can be procedurally placed by the calling mapgen."
    );
    ImGui::Separator();

    if( ImGui::Button( "Show/hide inline palette" ) ) {
        state.uistate->toggle_show_palette( file.base.inline_palette_id );
    }
    ImGui::Separator();

    if( file.mtype == MapgenType::Oter ) {
        if( ImGui::InputId( "om_terrain", file.oter.om_terrain ) ) {
            state.mark_changed();
        }
        ImGui::HelpPopup( "Overmap terrain type to assign this mapgen to." );
        if( ImGui::InputIntClamped( "weight", file.oter.weight, 0, 10000 ) ) {
            state.mark_changed( "file-info-oter-weight-input" );
        }
        ImGui::HelpPopup(
            "Weight of this mapgen, defaults to 100.\n\n"
            "The higher this value is, the more frequently this mapgen will be chosen "
            "to generate the overmap terrain."
        );
        if( ImGui::InputIntRange( "rotation", file.oter.rotation ) ) {
            state.mark_changed();
        }
        ImGui::Text( "Oter mapgen base:" );
        ImGui::HelpPopup( "Defines how to fill in the 'empty' tiles in the canvas." );

        if( ImGui::RadioButton( "Fill terrain", file.oter.mapgen_base == OterMapgenBase::FillTer ) ) {
            file.oter.mapgen_base = OterMapgenBase::FillTer;
            state.mark_changed();
        }
        ImGui::HelpPopup(
            "Fill with terrain type.\n\n"
            "Useful for maps where most of the terrain is monotonic (e.g. solid rock, or open air)."
        );
        ImGui::SameLine();
        if( ImGui::RadioButton( "Predecessor mapgen",
                                file.oter.mapgen_base == OterMapgenBase::PredecessorMapgen ) ) {
            file.oter.mapgen_base = OterMapgenBase::PredecessorMapgen;
            state.mark_changed();
        }
        ImGui::HelpPopup(
            "Run this mapgen on top of a map created for some other overmap terrain type.\n\n"
            "Useful for generating objects that don't occupy the whole 24x24 area, "
            "or maps that are extremely similar to some other maps."
            "For example, a small 8x8 glade in the woods may use 'forest' predecessor mapgen "
            "to generate the greenery, and then place some grass in the center.\n\n"
            "Keep in mind that predecessor mapgen may place items, monsters and vehices!"
        );
        ImGui::SameLine();
        if( ImGui::RadioButton( "Rows", file.oter.mapgen_base == OterMapgenBase::Rows ) ) {
            file.oter.mapgen_base = OterMapgenBase::Rows;
            state.mark_changed();
        }
        ImGui::HelpPopup(
            "Use a 24x24 canvas to place tiles.\n\n"
            "The most straightforward method, just define a bunch of palettes ('symbol: data' pairs) "
            "and then place symbols on the canvas to define positions.\n"
            "Most useful for complex layouts with little variation, such as buildings."
        );

        if( file.oter.mapgen_base == OterMapgenBase::PredecessorMapgen ) {
            if( ImGui::InputId( "predecessor_mapgen", file.oter.predecessor_mapgen ) ) {
                state.mark_changed();
            }
            ImGui::HelpPopup( "Overmap type id to run predecessor mapgen for." );
        } else {
            if( ImGui::InputId( "fill_ter", file.oter.fill_ter ) ) {
                state.mark_changed();
            }
            ImGui::HelpPopup( "Terrain type to fill empty spots with." );
        }
        if( file.oter.mapgen_base == OterMapgenBase::Rows ) {
            show_canvas_hint();
        }
    } else if( file.mtype == MapgenType::Update ) {
        if( ImGui::InputText( "update_mapgen_id", &file.update.update_mapgen_id ) ) {
            state.mark_changed();
        }
        ImGui::HelpPopup( "ID of this update mapgen." );
        if( ImGui::InputId( "fill_ter", file.update.fill_ter ) ) {
            state.mark_changed();
        }
        ImGui::HelpPopup( "Terrain type to fill empty spots with." );
    } else { // MapgenType::Nested
        if( ImGui::InputText( "nested_mapgen_id", &file.nested.nested_mapgen_id ) ) {
            state.mark_changed();
        }
        ImGui::HelpPopup( "ID of this nested mapgen." );
        if( ImGui::InputIntRange( "rotation", file.nested.rotation ) ) {
            state.mark_changed();
        }
        ImGui::HelpPopup( "Allowed rotations." );
        // Only square nested mapgens are possible
        if( ImGui::InputIntClamped( "mapgensize", file.nested.size.x, 1, SEEX * 2 ) ) {
            file.nested.size.y = file.nested.size.x;
            file.base.set_size( file.mapgensize().raw() );
            state.mark_changed();
        }
        ImGui::HelpPopup( "Size of this nested mapgen." );
        show_canvas_hint();
    }

    ImGui::PopID();
    ImGui::End();
}

point_rel_etile me_file::mapgensize() const
{
    if( mtype == MapgenType::Nested ) {
        return point_rel_etile( nested.size );
    } else {
        return point_rel_etile( SEEX * 2, SEEY * 2 );
    }
}

void me_mapgen_base::set_size( const point &s )
{
    if( size == s ) {
        return;
    }
    // TODO: graciously transfer entries from old size
    size = s;
    rows.clear();
    rows.resize( s.x * s.y, UUID_INVALID );
}

me_mapgen_base::~me_mapgen_base() = default;

void me_mapgen_base::remove_usages( const uuid_t &uuid )
{
    for( uuid_t &cell : rows ) {
        if( cell == uuid ) {
            cell = UUID_INVALID;
        }
    }
}

} // namespace editor
