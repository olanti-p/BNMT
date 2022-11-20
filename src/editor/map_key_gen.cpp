#include "map_key_gen.h"

#include "../text_snippets.h"
#include "../catacharset.h"

namespace editor
{

me_map_key_generator::me_map_key_generator()
{
    const translation &trans = SNIPPET.get_snippet_ref_by_id( snippet_id( "me_auto_map_keys" ) );
    std::u32string s_u32 = utf8_to_utf32( trans.raw );
    // TODO: support combining characters
    opts.reserve( s_u32.size() );
    for( const char32_t &ch32 : s_u32 ) {
        opts.emplace_back( utf32_to_utf8( ch32 ) );
    }
}

void me_map_key_generator::blacklist( const map_key &opt )
{
    std::remove( opts.begin(), opts.end(), opt );
}

} // namespace editor
