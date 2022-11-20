#ifndef CATA_SRC_EDITOR_INT_RANGE_H
#define CATA_SRC_EDITOR_INT_RANGE_H

class JsonOut;
class JsonIn;

namespace editor
{

struct me_int_range {
    int min = 0;
    int max = 0;

    inline operator bool() const {
        return min != 0 || max != 0;
    }

    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );
};

} // namespace editor

#endif // CATA_SRC_EDITOR_INT_RANGE_H
