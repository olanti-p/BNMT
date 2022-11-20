#ifndef CATA_SRC_EDITOR_WEIGHTED_LIST_H
#define CATA_SRC_EDITOR_WEIGHTED_LIST_H

class JsonOut;
class JsonIn;

namespace editor
{
namespace detail
{

template<typename T>
struct weighted_list_entry {
    T val;
    int weight = 0;
};

} // namespace detail

template<typename T>
struct me_weighted_list {
    std::vector<detail::weighted_list_entry<T>> entries;

    bool is_uniform() const {
        if( entries.empty() ) {
            return false;
        }
        int weight_zero = entries[0].weight;
        for( auto it = std::next( entries.cbegin() ); it != entries.cend(); it++ ) {
            if( it->weight != weight_zero ) {
                return false;
            }
        }
        return true;
    }
};

} // namespace editor

#endif // CATA_SRC_EDITOR_WEIGHTED_LIST_H
