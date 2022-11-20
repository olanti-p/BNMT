#ifndef CATA_SRC_EDITOR_PALETTE_H
#define CATA_SRC_EDITOR_PALETTE_H

#include <vector>
#include <memory>

#include "imgui.h"
#include "../mapgen_map_key.h"

#include "piece.h"
#include "uuid.h"
#include "sprite_ref.h"

struct ImDrawList;
struct ImVec4;
class JsonOut;
class JsonIn;
template<typename T> struct enum_traits;
struct SpriteRef;

namespace editor
{
struct me_file;
struct me_project;

struct me_mapping {
    std::vector<std::unique_ptr<me_piece>> pieces;

    me_mapping() = default;
    me_mapping( const me_mapping &rhs );
    me_mapping( me_mapping && ) = default;
    ~me_mapping() = default;

    me_mapping &operator=( const me_mapping &rhs );
    me_mapping &operator=( me_mapping && ) = default;

    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );

    template<typename T>
    const T *get_first_piece_of_type() const {
        for( auto &piece : pieces ) {
            T *ptr = dynamic_cast<T *>( piece.get() );
            if( ptr ) {
                return ptr;
            }
        }
        return nullptr;
    }

    template<typename T>
    T *get_first_piece_of_type() {
        const me_mapping *this_c = this;
        return const_cast<T *>( this_c->get_first_piece_of_type<T>() );
    }

    bool has_piece_of_type( PieceType pt ) const;
};

struct me_palette_entry {
    uuid_t uuid;
    map_key key;
    ImVec4 color;
    me_mapping mapping;

    mutable bool sprite_cache_valid = false;
    mutable cata::optional<SpriteRef> sprite_cache;

    void build_sprite_cache() const;

    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );
};

struct me_palette {
    static me_palette make_inline() {
        me_palette ret;
        ret.is_inline = true;
        return ret;
    }

    bool is_inline = false;
    uuid_t uuid;
    palette_eid id;
    std::vector<me_palette_entry> entries;

    const map_key &key_from_uuid( const uuid_t &uuid ) const;
    const ImVec4 &color_from_uuid( const uuid_t &uuid ) const;
    const SpriteRef *sprite_from_uuid( const uuid_t &uuid ) const;

    me_palette_entry *find_entry( const uuid_t &uuid );
    const me_palette_entry *find_entry( const uuid_t &uuid ) const;

    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );
};

/**
 * =============== Windows ===============
 */
map_key pick_available_key( const me_palette &pal );

/**
 * =============== Windows ===============
 */
void show_mapping( me_state &state, editor::me_palette &p, editor::me_palette_entry &entry,
                   bool &show );
void show_palette( me_state &state, me_palette &p, bool &show );

} // namespace editor

#endif // CATA_SRC_EDITOR_PALETTE_H
