#ifndef CATA_SRC_EDITOR_PIECE_IMPL_H
#define CATA_SRC_EDITOR_PIECE_IMPL_H

#include "piece.h"

#include "../calendar.h"
#include "../enum_traits.h"
#include "editable_id.h"
#include "int_range.h"
#include "weighted_list.h"

namespace editor
{

struct me_piece_field : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_field, PieceType::Field );

    field_eid ftype;
    int intensity = 1;
    time_duration age = 0_seconds;
};

struct me_piece_npc : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_npc, PieceType::NPC );

    npc_template_eid npc_class;
    bool target = false;
    std::vector<trait_eid> traits;
};

struct me_piece_faction : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_faction, PieceType::Faction );

    std::string id;
};

struct me_piece_sign : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_sign, PieceType::Sign );

    bool use_snippet = false;
    snippet_category_eid snippet;
    std::string text;
};

struct me_piece_graffiti : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_graffiti, PieceType::Graffiti );

    bool use_snippet = false;
    snippet_category_eid snippet;
    std::string text;
};

struct me_piece_vending_machine : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_vending_machine, PieceType::VendingMachine );

    bool reinforced = false;
    bool use_default_group = true;
    igroup_eid item_group;
};

struct me_piece_toilet : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_toilet, PieceType::Toilet );

    bool use_default_amount = true;
    me_int_range amount;
};

enum class GasPumpFuel {
    Random,
    Gasoline,
    Diesel,

    _Num,
};

struct me_piece_gaspump : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_gaspump, PieceType::GasPump );

    bool use_default_amount = true;
    me_int_range amount;
    GasPumpFuel fuel;
};

struct me_piece_liquid : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_liquid, PieceType::Liquid );

    bool use_default_amount = true;
    me_int_range amount;
    liquid_eid liquid;
    bool spawn_always = true;
    me_int_range chance;
};

struct me_piece_igroup : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_igroup, PieceType::Igroup );

    igroup_eid group_id;
    me_int_range chance;
    bool spawn_once = true;
    me_int_range repeat;
};

struct me_piece_loot : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_loot, PieceType::Loot );

    // TODO
};

struct me_piece_mgroup : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_mgroup, PieceType::Mgroup );

    mgroup_eid group_id;
    bool spawn_always = true;
    me_int_range chance;
    bool use_default_density = true;
    float density = -1.0f;
};

struct me_piece_monster : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_monster, PieceType::Monster );

    // TODO
};

enum class VehicleStatus {
    LightDamage,
    Undamaged,
    Disabled,

    _Num,
};

struct me_piece_vehicle : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_vehicle, PieceType::Vehicle );

    vgroup_eid group_id;
    me_int_range chance;
    VehicleStatus status = VehicleStatus::LightDamage;
    bool random_fuel_amount = true;
    int fuel = 100;
    std::set<int> allowed_rotations;
};

struct me_piece_item : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_item, PieceType::Item );

    item_eid item_id;
    me_int_range amount;
    bool spawn_one = true;
    me_int_range chance;
    bool spawn_once = true;
    me_int_range repeat;
};

struct me_piece_trap : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_trap, PieceType::Trap );

    // TODO
};

struct me_piece_furniture : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_furniture, PieceType::Furniture );

    // TODO
};

struct me_piece_terrain : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_terrain, PieceType::Terrain );

    // TODO
};

struct me_piece_ter_furn_transform : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_ter_furn_transform, PieceType::TerFurnTransform );

    // TODO
};

struct me_piece_make_rubble : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_make_rubble, PieceType::MakeRubble );

    // TODO
};

struct me_piece_computer : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_computer, PieceType::Computer );

    // TODO
};

struct me_piece_sealed_item : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_sealed_item, PieceType::SealedItem );

    // TODO
};

struct me_piece_translate : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_translate, PieceType::Translate );

    // TODO
};

struct me_piece_zone : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_zone, PieceType::Zone );

    // TODO
};

struct me_piece_nested : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_nested, PieceType::Nested );

    // TODO
};

struct me_piece_alt_trap : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_alt_trap, PieceType::AltTrap );

    void init_new() override;

    me_weighted_list<trap_eid> list;
};

struct me_piece_alt_furniture : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_alt_furniture, PieceType::AltFurniture );

    void init_new() override;

    me_weighted_list<furn_eid> list;
};

struct me_piece_alt_terrain : public me_piece {
    IMPLEMENT_ME_PIECE( me_piece_alt_terrain, PieceType::AltTerrain );

    void init_new() override;

    me_weighted_list<ter_eid> list;
};

} // namespace editor

template<>
struct enum_traits<editor::GasPumpFuel> {
    static constexpr editor::GasPumpFuel last = editor::GasPumpFuel::_Num;
};

template<>
struct enum_traits<editor::VehicleStatus> {
    static constexpr editor::VehicleStatus last = editor::VehicleStatus::_Num;
};

#endif // CATA_SRC_EDITOR_PIECE_IMPL_H
