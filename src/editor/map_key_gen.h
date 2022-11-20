#ifndef CATA_SRC_EDITOR_MAP_KEY_GEN_H
#define CATA_SRC_EDITOR_MAP_KEY_GEN_H

#include "../mapgen_map_key.h"

namespace editor
{

struct me_map_key_generator {
    private:
        std::vector<map_key> opts;

    public:
        me_map_key_generator();
        ~me_map_key_generator() = default;

        void blacklist( const map_key &opt );

        inline map_key operator()() {
            if( opts.empty() ) {
                return default_map_key;
            } else {
                return opts[0];
            }
        }
};

} // namespace editor

#endif // CATA_SRC_EDITOR_MAP_KEY_GEN_H
