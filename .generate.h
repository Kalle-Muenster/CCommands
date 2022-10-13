#ifndef IncludesFor_generate
#define IncludesFor_generate (1)

#include <stdint.h>
#include ".environMentor.h"

#if defined(__cplusplus) && !defined(_OnTheFly_)
extern "C" {
#endif

    typedef union generator generator;
    typedef union generator {
        generator(*call)(const char*);
        generator(*func)(const char*, const char*);
    } generator;

    // change persistent configurations
    const char* generate_setBase64Encoding(const char* enabled);
    const char* generate_setTabulatorString(const char* tabString);

    // generator api:
    void        generate_setNamespace(const byte* nameSpace);
    generator   generate_Initialize(const char* options);
    generator   generate_newWrap(const char* target);
    generator   generate_newClass(const char* classname, const char* basename);
    generator   generate_newCommand(const char* name);
    generator   generate_newController(const char* usermode, const char* basemode);
    generator   generate_newResource(const char* filename);
    const char* generate_parseTemplate(const byte* teplt);

#ifdef _OnTheFly_
#include ".generate.cc"
#elif defined(__cplusplus)
}
#endif
#endif

