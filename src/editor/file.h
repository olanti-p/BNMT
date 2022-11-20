#ifndef CATA_SRC_EDITOR_FILE_H
#define CATA_SRC_EDITOR_FILE_H

#include "../game_constants.h"
#include "../coordinates.h"

#include "uuid.h"
#include "palette.h"
#include "mapobject.h"

struct ImVec4;
class JsonOut;
class JsonIn;
template<typename T> struct enum_traits;

namespace editor
{
struct me_state;

struct me_mapgen_base {
    me_mapgen_base() {
        set_size( point( SEEX * 2, SEEY * 2 ) );
    }
    ~me_mapgen_base();

    point size;
    // TODO: refer to palette entries by their ids
    std::vector<uuid_t> rows;
    uuid_t inline_palette_id = UUID_INVALID;

    void set_size( const point &s );
    inline void set_uuid_at( const point &pos, const uuid_t &uuid ) {
        rows[ pos.y * size.x + pos.x ] = uuid;
    }
    inline const uuid_t &get_uuid_at( const point &pos ) const {
        return rows[ pos.y * size.x + pos.x ];
    }
    void remove_usages( const uuid_t &uuid );

    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );
};

enum class OterMapgenBase {
    FillTer,
    PredecessorMapgen,
    Rows,
    _Num,
};

struct me_mapgen_oter {
    oter_eid om_terrain;
    int weight = 100;
    OterMapgenBase mapgen_base = OterMapgenBase::FillTer;
    ter_eid fill_ter = ter_eid::NULL_ID();
    oter_eid predecessor_mapgen;
    me_int_range rotation;

    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );
};

struct me_mapgen_update {
    std::string update_mapgen_id;
    ter_eid fill_ter = ter_eid::NULL_ID();

    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );
};

struct me_mapgen_nested {
    std::string nested_mapgen_id;
    point size = point( 24, 24 );
    me_int_range rotation;

    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );
};

enum class MapgenType {
    Oter,
    Update,
    Nested,
    _Num,
};

struct me_file {
    uuid_t uuid = UUID_INVALID;

    MapgenType mtype = MapgenType::Oter;
    me_mapgen_base base;
    me_mapgen_oter oter;
    me_mapgen_update update;
    me_mapgen_nested nested;

    std::vector<me_mapobject> objects;

    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );

    inline bool uses_rows() const {
        return mtype == editor::MapgenType::Nested ||
               (
                   mtype == editor::MapgenType::Oter &&
                   oter.mapgen_base == editor::OterMapgenBase::Rows
               );
    }

    point_rel_etile mapgensize() const;
};

/**
 * =============== Windows ===============
 */
void show_file_info( me_state &state, me_file &file, bool &show );

} // namespace editor

template<>
struct enum_traits<editor::OterMapgenBase> {
    static constexpr editor::OterMapgenBase last = editor::OterMapgenBase::_Num;
};

template<>
struct enum_traits<editor::MapgenType> {
    static constexpr editor::MapgenType last = editor::MapgenType::_Num;
};

#endif // CATA_SRC_EDITOR_FILE_H
