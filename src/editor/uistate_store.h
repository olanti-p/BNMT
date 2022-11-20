#ifndef CATA_SRC_EDITOR_UISTATE_STORE_H
#define CATA_SRC_EDITOR_UISTATE_STORE_H

#include <string>

namespace editor
{
struct me_uistate;

me_uistate &get_uistate_for_project( const std::string &project_uuid );

void initialize_settings_export();

} // namespace editor

#endif // CATA_SRC_EDITOR_UISTATE_STORE_H
