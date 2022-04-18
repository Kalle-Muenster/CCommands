#define CLINE_INCLUDES "math.h,stdio.h"
#define NUMBER "%%s\",ulltoa((ulong)%s,&value[0],10)"
#define CLINE_ECHO (0)
#define CLINE_ASYNCHRON (0)
#define CLINE_DEFINES "CRYPTISCHES_WORT="dasAndere""
///[cLineReflector]
#define NUMBER_OF_FEATURES (5)
const static int cLine_NUMBER_OF_MACROS = NUMBER_OF_FEATURES;
const static char* FEATURE_NAMES[NUMBER_OF_FEATURES] = {
    "CLINE_INCLUDES\0\"math.h,stdio.h\"",
    "NUMBER\0\"%%s\\\",ulltoa((ulong)%s,&value[0],10)\"",
    "CLINE_ECHO\0(0)",
    "CLINE_ASYNCHRON\0(0)",
    "CLINE_DEFINES\0\"CRYPTISCHES_WORT=\"dasAndere\"\"",
};
