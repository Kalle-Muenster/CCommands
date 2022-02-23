/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      junkYard.h                                 ||
||     Author:    Autogenerated                              ||
||     Generated: 17.11.2020                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef using_junkYard
#define using_junkYard (1)

#ifndef COMMANDLINER_ESCENTIALS_DEFINED
#include "eszentielle/.CommandlinerTypes.h"
#endif

#if defined( EXPORT_COMMANDLINER_LIBRARIES ) && defined( EXPORT_JUNKYARD_API )
#define JUNKYARD_API __declspec(dllexport)
#endif

#if defined(_OnTheFly_) && defined( using_commandLiner ) && (!defined(WITH_JUNKYARD))
#error '.junkYard.h' cannot be _OnTheFly_ together with '.commandLiner.h' - see comment in .junkYard.h
/*
    define 'WITH_JUNKYARD' before including '.commandLiner.h'
    commandLiner then will import the junkYard (.cc) source
    on it's own automatically without causing the error then
*/
#endif


#if defined(__cplusplus) && !defined(_OnTheFly_)
COMMANDLINER_EXTERN_C
#endif

#ifndef JUNKYARD_API
#define JUNKYARD_API
#endif

    typedef JUNKYARD_API  void(*cmDtFunc)(void*);
    typedef JUNKYARD_API  void(*cmDtCall)(void);
    typedef JUNKYARD_API void*(*cmDtPunk)(void*);
    typedef struct JUNKYARD_API Junk Junk;
    typedef struct JUNKYARD_API Junk {
        ptval      hold;
        cmDtFunc   drop;
        ptval      self;
        cmDtFunc   dtor;
        Junk*      next;
    }   Junk,      yard;

    /*
    void mentor(void* chunk) {
        // dummy for ehm. destructionOnJunkyard
    }

    typedef struct JUNKYARD_API Junk {
        ptval      ctor; //<- ehm. hold
        Junk*      next; 
        cmDtFunc   mtor; //<- ehm. dTor
        ptval      dtor; //<- ehm. func
        ptval      chnk; //<- ehm. junk
        
    }   Junk, yard;

    Junk* junktor() {
        Junk*    (*ctor)(void) = &junktor;
        Junk*      next = 0;
        cmDtFunc   mtor = &mentor; //<- ehm. dTor
        ptval      dtor; //<- ehm. func
        ptval      chnk; //<- ehm. junk
    }
    */
#ifdef  EXPORT_COMMANDLINER_LIBRARIES
#include <junkYard.h>
#else
    // retreives a pointer to the junk yard,.. there should be no usecase for calling this
    // (its just used from within junkyard api functions internally)
    JUNKYARD_API yard*   junk_getYard(void);

    // installs a parameterless function which then automatically will be called
    // on dropAllChunks() and/or on dropCycle() calls on the junk yard.
    JUNKYARD_API void    junk_installCleansener( cmDtCall );

    // allocate cbSize byte on raw memory which automatically will be 'cleansened'
    // allocated memory will be freed on recycle() calls if not referenced anymore, 
	// or (even if still referenced somehow) in any case with process shutting down
    JUNKYARD_API void*   junk_allocateJunkChunk( uint  cbSize );

    // same like allocateJunkChunk() but instead of allocating new memory, instead
    // integrate already existing memory that before was allocated some where else.
    JUNKYARD_API void*   junk_registerJunkChunk( void* );

    // allocate an object of objSize bytes, which at shuttdown or at dropAllChunks()
    // calls will be passed as parameter to the given destructor function at least.
    JUNKYARD_API void*   junk_allocateNewObject( cmDtFunc objDtor, uint objSize );

	// same like allocateNewObject() but instanciates an object from passed memory
	// which already has been allocated in a different environment by different api 
    // (e.g. keep aware that in such case passed destructor itself will not only be
    // responsible for object de-initialization but also for freeing object memory) 	
    JUNKYARD_API void*   junk_objectivateMemory( cmDtFunc objDtor, void* alienData );

    // dropYard() cleansen all known memory on a junkyard which has allocated memory
    // (at regular shutdown this also is called by commandLiner installed exit() handler)
    JUNKYARD_API void    junk_dropYard( void );

    // cycle() when called, all memory on the junk yard known to be not in use
    // anymore will be freed. Memory which is known being STILL IN USE will stay intact.
    JUNKYARD_API void    junk_cycle( void );
	
    // mark a piece of junk for being dropped during the next recycle call
    JUNKYARD_API ptval   junk_drop( void* );

    // drop one piece of junk from the junkyard (frees one portion of memory
    // which was previously allocated on the junkyard)
    JUNKYARD_API ptval   junk_free( void* );

#endif

#ifdef _OnTheFly_
#ifndef WITH_JUNKYARD
#include ".junkYard.cc"
#endif
#elif defined(__cplusplus)
COMMANDCLOSER_EXTERN_C
#endif
#endif
