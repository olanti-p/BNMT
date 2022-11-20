#include "piece.h"
#include "piece_impl.h"

#define REG_PIECE( piece_class ) ret.push_back( std::make_unique<piece_class>() )

namespace editor
{

std::string me_piece::fmt_summary() const
{
    return string_format( "%s: %s", io::enum_to_string<PieceType>( get_type() ), fmt_data_summary() );
}

const std::vector<std::unique_ptr<me_piece>> &get_piece_templates()
{
    static std::vector<std::unique_ptr<me_piece>> ret;
    if( ret.empty() ) {
        ret.reserve( static_cast<int>( PieceType::NumJmTypes ) );
        REG_PIECE( me_piece_field );
        REG_PIECE( me_piece_npc );
        REG_PIECE( me_piece_faction );
        REG_PIECE( me_piece_sign );
        REG_PIECE( me_piece_graffiti );
        REG_PIECE( me_piece_vending_machine );
        REG_PIECE( me_piece_toilet );
        REG_PIECE( me_piece_gaspump );
        REG_PIECE( me_piece_liquid );
        REG_PIECE( me_piece_igroup );
        REG_PIECE( me_piece_loot );
        REG_PIECE( me_piece_mgroup );
        REG_PIECE( me_piece_monster );
        REG_PIECE( me_piece_vehicle );
        REG_PIECE( me_piece_item );
        REG_PIECE( me_piece_trap );
        REG_PIECE( me_piece_furniture );
        REG_PIECE( me_piece_terrain );
        REG_PIECE( me_piece_ter_furn_transform );
        REG_PIECE( me_piece_make_rubble );
        REG_PIECE( me_piece_computer );
        REG_PIECE( me_piece_sealed_item );
        REG_PIECE( me_piece_translate );
        REG_PIECE( me_piece_zone );
        REG_PIECE( me_piece_nested );
        REG_PIECE( me_piece_alt_trap );
        REG_PIECE( me_piece_alt_furniture );
        REG_PIECE( me_piece_alt_terrain );
    }
    return ret;
}

std::unique_ptr<me_piece> make_new_piece( PieceType pt )
{
    for( const std::unique_ptr<me_piece> &it : get_piece_templates() ) {
        if( it->get_type() == pt ) {
            return it->clone();
        }
    }
    std::cerr << "Failed to generate piece with type " << static_cast<int>( pt ) << std::endl;
    std::abort();
}

bool is_alt_piece( PieceType pt )
{
    return pt == editor::PieceType::AltTerrain ||
           pt == editor::PieceType::AltFurniture ||
           pt == editor::PieceType::AltTrap;
}

bool is_piece_exclusive( PieceType pt )
{
    return is_alt_piece( pt );
}

bool is_available_as_mapping( PieceType pt )
{
    return !(
               pt == editor::PieceType::Terrain ||
               pt == editor::PieceType::Furniture ||
               pt == editor::PieceType::Trap ||
               pt == editor::PieceType::Loot
           );
}

bool is_available_as_mapobject( PieceType pt )
{
    return !(
               pt == editor::PieceType::AltTerrain ||
               pt == editor::PieceType::AltFurniture ||
               pt == editor::PieceType::AltTrap ||
               pt == editor::PieceType::SealedItem
           );
}

} // namespace editor
