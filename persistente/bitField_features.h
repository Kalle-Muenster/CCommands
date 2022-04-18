#define ENVIRONMENTIONED "this is bitField teststring"
#define BITFIELD_ZERO '_'
#define BITFIELD_ONES '|'
///[bitFieldReflector]
#define NUMBER_OF_FEATURES (3)
const static int bitField_NUMBER_OF_MACROS = NUMBER_OF_FEATURES;
const static char* FEATURE_NAMES[NUMBER_OF_FEATURES] = {
    "ENVIRONMENTIONED\0\"this is bitField teststring\"",
    "BITFIELD_ZERO\0'_'",
    "BITFIELD_ONES\0'|'",
};
