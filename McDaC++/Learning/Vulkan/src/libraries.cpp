#define DO_PRAGMA(x) _Pragma (#x)
#define SUPRESS_WARNING(x) DO_PRAGMA(GCC diagnostic ignored #x)

SUPRESS_WARNING(-Wold-style-cast)
SUPRESS_WARNING(-Wsign-conversion)
SUPRESS_WARNING(-Wdouble-promotion)
SUPRESS_WARNING(-Wcast-align)
SUPRESS_WARNING(-Wimplicit-int-conversion)

#define STB_IMAGE_IMPLEMENTATION // Define the stb_image functions
#include "stb_image.h"
