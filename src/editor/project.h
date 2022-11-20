#ifndef CATA_SRC_EDITOR_PROJECT_H
#define CATA_SRC_EDITOR_PROJECT_H

#include "file.h"
#include "palette.h"
#include "uuid.h"

namespace editor
{
struct me_state;

struct me_project {
    std::string project_uuid;
    uuid_generator uuid_gen;
    std::vector<me_file> files;
    std::vector<me_palette> palettes;

    void serialize( JsonOut &jsout ) const;
    void deserialize( JsonIn &jsin );

    const me_file *get_file_by_uuid( const uuid_t &fid ) const;
    inline me_file *get_file_by_uuid( const uuid_t &fid ) {
        const me_project *this_c = this;
        return const_cast<me_file *>( this_c->get_file_by_uuid( fid ) );
    }

    const me_palette *get_palette_by_uuid( const uuid_t &pid ) const;
    inline me_palette *get_palette_by_uuid( const uuid_t &pid ) {
        const me_project *this_c = this;
        return const_cast<me_palette *>( this_c->get_palette_by_uuid( pid ) );
    }
};

void show_project_ui( me_state &state, me_project &project );

std::unique_ptr<me_project> create_empty_project();

} // namespace editor

#endif // CATA_SRC_EDITOR_PROJECT_H
