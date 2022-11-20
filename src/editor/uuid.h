#ifndef CATA_SRC_EDITOR_UUID_H
#define CATA_SRC_EDITOR_UUID_H

#include <cstdint>

class JsonOut;
class JsonIn;

namespace editor
{

using uuid_t = uint64_t;
constexpr uuid_t UUID_INVALID = 0;

struct uuid_generator {
    private:
        uuid_t counter = UUID_INVALID;

    public:
        inline uuid_t operator()() {
            counter++;
            return counter;
        }

        void serialize( JsonOut &jsout ) const;
        void deserialize( JsonIn &jsin );
};

} // namespace editor

#endif // CATA_SRC_EDITOR_UUID_H
