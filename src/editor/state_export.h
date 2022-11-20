#ifndef CATA_SRC_EDITOR_STATE_EXPORT_H
#define CATA_SRC_EDITOR_STATE_EXPORT_H

#include <string>

#include "state.h"

namespace editor_export
{

std::string to_string( const editor::me_project &project );

std::string format_string( const std::string &js );

} // namespace editor_export

#endif // CATA_SRC_EDITOR_STATE_EXPORT_H
