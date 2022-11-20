#ifndef CATA_SRC_EDITOR_PIECE_H
#define CATA_SRC_EDITOR_PIECE_H

#include "../calendar.h"

#include "piece_type.h"
#include "editable_id.h"
#include "int_range.h"
#include "uuid.h"

#include <memory>
#include <string>

class JsonOut;
class JsonObject;

#define IMPLEMENT_ME_PIECE(piece_class, piece_type)                     \
    piece_class() = default;                                            \
    piece_class( const piece_class& ) = default;                        \
    piece_class( piece_class&&) = default;                              \
    ~piece_class() = default;                                           \
    PieceType get_type() const override {                               \
        return piece_type;                                              \
    }                                                                   \
    std::unique_ptr<me_piece> clone() const override {                  \
        return std::make_unique<piece_class>( *this );                  \
    };                                                                  \
    void serialize( JsonOut &jsout ) const override;                    \
    void deserialize( JsonObject &jsin ) override;                      \
    void export_func( JsonOut& jo ) const override;                     \
    void show_ui( me_state& state ) override;                           \
    std::string fmt_data_summary() const override;

namespace editor
{
struct me_state;

struct me_piece {
    me_piece() = default;
    virtual ~me_piece() = default;

    virtual PieceType get_type() const = 0;

    uuid_t uuid = UUID_INVALID;

    virtual std::unique_ptr<me_piece> clone() const = 0;

    virtual void serialize( JsonOut &jsout ) const = 0;
    virtual void deserialize( JsonObject &jsin ) = 0;
    virtual void export_func( JsonOut &jo ) const = 0;

    virtual void show_ui( me_state &state ) = 0;

    virtual void init_new() {};

    /**
     * Returns "Type: data" summary string
    */
    std::string fmt_summary() const;

    /**
     * Returns data summary string;
    */
    virtual std::string fmt_data_summary() const = 0;
};

const std::vector<std::unique_ptr<me_piece>> &get_piece_templates();
std::unique_ptr<me_piece> make_new_piece( PieceType pt );

bool is_alt_piece( PieceType pt );
bool is_piece_exclusive( PieceType pt );
bool is_available_as_mapping( PieceType pt );
bool is_available_as_mapobject( PieceType pt );

} // namespace editor

#endif // CATA_SRC_EDITOR_PIECE_H
