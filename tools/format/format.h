#ifndef CATA_TOOLS_FORMAT_FORMAT_H
#define CATA_TOOLS_FORMAT_FORMAT_H

class JsonIn;
class JsonOut;

void format( JsonIn &jsin, JsonOut &jsout, int depth = -1, bool force_wrap = false );

#endif // CATA_TOOLS_FORMAT_FORMAT_H
