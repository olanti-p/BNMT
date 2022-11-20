#ifndef CATA_SRC_EDITOR_EDITABLE_ID_H
#define CATA_SRC_EDITOR_EDITABLE_ID_H

#include "../type_id.h"

#include <string>
#include <vector>

class JsonOut;
class JsonIn;
template<typename T> struct enum_traits;

class npc_template;

namespace editor
{

namespace detail
{
void serialize_eid( JsonOut &jsout, const std::string &data );
void deserialize_eid( JsonIn &jsin, std::string &data );
} // namespace detail

template<typename T>
struct editable_id {
    public:
        std::string data;

        editable_id() = default;
        editable_id( const editable_id<T> & ) = default;
        editable_id( editable_id<T> && ) = default;
        editable_id( const std::string &s ) : data( s ) {}
        editable_id( const string_id<T> &id ) : data( id.str() ) {}
        ~editable_id() = default;

        editable_id &operator= ( const editable_id<T> & ) = default;
        editable_id &operator= ( editable_id<T> && ) = default;

        bool is_valid() const {
            return string_id<T>( data ).is_valid();
        }

        bool is_null() const {
            return string_id<T>( data ).is_null();
        }

        const T &obj() const {
            return string_id<T>( data ).obj();
        }

        static const editable_id<T> NULL_ID() {
            return string_id<T>::NULL_ID();
        }

        static const std::vector<std::string> &get_all_opts();

        void serialize( JsonOut &jsout ) const {
            detail::serialize_eid( jsout, data );
        }
        void deserialize( JsonIn &jsin ) {
            detail::deserialize_eid( jsin, data );
        }

    private:
        // TODO: invalidate on data change
        static std::vector<std::string> all_opts;
};

template<typename T>
std::vector<std::string> editable_id<T>::all_opts;

struct snippet_category_tag {};
struct liquid_item_tag {};
struct item_group_tag {};

using field_eid = editable_id<field_type>;
using furn_eid = editable_id<furn_t>;
using igroup_eid = editable_id<item_group_tag>;
using item_eid = editable_id<itype>;
using liquid_eid = editable_id<liquid_item_tag>;
using mgroup_eid = editable_id<MonsterGroup>;
using npc_template_eid = editable_id<npc_template>;
using oter_eid = editable_id<oter_t>;
using oter_type_eid = editable_id<oter_type_t>;
using palette_eid = editable_id<mapgen_palette>;
using snippet_category_eid = editable_id<snippet_category_tag>;
using ter_eid = editable_id<ter_t>;
using trait_eid = editable_id<mutation_branch>;
using trap_eid = editable_id<trap>;
using vgroup_eid = editable_id<VehicleGroup>;

} // namespace editor

#endif // CATA_SRC_EDITOR_EDITABLE_ID_H
