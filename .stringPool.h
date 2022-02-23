/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      stringPool.h                               ||
||     Author:    Kalle                                      ||
||     Generated: 13.08.2019                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifdef using_stringPool
  #if using_stringPool<1
   #ifdef _OnTheFly_ 
    #include ".stringPool.cc"
    #undef  Pool
    #define Pool INTERNAL_POOLBOTTOM
   #endif
   #undef  using_stringPool
   #define using_stringPool (1)
  #endif
#else  
#define using_stringPool (1)


#if defined(using_commandLiner) && (!defined(WITH_STRGPOOL))
#error '.stringPool.h' must be included before '.commandLiner.h'
/*
    When including a "stringPool" and the "commandLiner", then
	the '.stringPool.h' header always MUST be included at first
	then BEFORE the ".commandLiner.h" header will be included.
	If including sources _OnTheFly_ instead macro WITH_STRGPOOL
    is to be defined before including commandLiner to make it
    also fetching stringPool.cc source automatically within fly
*/
#endif

#if defined(__cplusplus) && !defined(_OnTheFly_)
COMMANDLINER_EXTERN_C
#endif

#ifndef COMMANDLINER_ESCENTIALS_DEFINED
#if defined(__TINYC__)
    typedef uint8_t                byte;
    typedef uint16_t               word;
    typedef uint32_t               uint;
    typedef __SIZE_TYPE__          ptval;
    typedef __PTRDIFF_TYPE__       ptdif;    
    typedef int64_t                slong;
    typedef uint64_t               ulong;
    typedef int                    cmBl;
#elif defined(_MSC_BUILD)
    typedef int                    cmBl;
    typedef unsigned __int8        byte;
    typedef unsigned __int16       word;
    typedef unsigned __int32       uint;
    typedef signed   __int64       slong;
#ifndef QT_VERSION
    typedef unsigned __int64       ulong;
#endif
#if defined(_WIN32) && !defined(_WIN64)
    typedef unsigned __int32       ptval;
#elif !defined(__cplusplus)
#ifndef _WIN64
    typedef uint        ptval;
    typedef int         ptdif;
#else
    typedef ulong       ptval;
    typedef slong       ptdif;
#endif
#else 
#ifdef _WIN64
    typedef ulong       ptval;
    typedef slong       ptdif;
#endif
#endif
#else // defined(__GNUC__)
    typedef int                    cmBl;
    typedef unsigned char          byte;
    typedef unsigned short         word;
    typedef unsigned int           uint;
    typedef signed long long int   slong;
    typedef unsigned long long int ulong;
    typedef uintptr_t              ptval;
    typedef intptr_t               ptdiv;
#endif
#define COMMANDLINER_ESCENTIALS_DEFINED 1
#endif

#ifndef CYCLE_SIZE
#define CYCLE_SIZE (4096)
#endif
#ifndef CYCLE_COUNT
#define CYCLE_COUNT (16)
#endif
#ifndef EMPTY
#define EMPTY (0xFFFFFFFF)
#define EMPTY_(T) ((T)-1)
#endif
#ifndef MAX_NAM_LEN
#define MAX_NAM_LEN (255)
#endif

#define DEFAULT_POOLBOTTOM Bottom

#if defined( EXPORT_COMMANDLINER_LIBRARIES ) && defined( EXPORT_STRINGPOOL_API )
#define STRINGPOOL_API __declspec(dllexport)
#else
#define STRINGPOOL_API
#endif

typedef struct STRINGPOOL_API StringPool StringPool;
typedef struct STRINGPOOL_API WhirlVars {
#if defined(__GNUC__)
  public:
#endif
    uint        Pls;
    uint        Cnt;
    uint        Ovr;
    StringPool* running;
    char*       Cur;
    uint*       Chk;
} WhirlVars;
typedef struct STRINGPOOL_API StringPool {
#if defined(__GNUC__)
  public:
#endif
    byte    Cyc[CYCLE_SIZE+MAX_NAM_LEN];
    byte    Cut[CYCLE_COUNT];
    uint    Pos[CYCLE_COUNT];
struct {
    uint    Pls;
    uint    Cnt;
    uint    Ovr;
    StringPool* running;
    char*   Cur;
    uint*   Chk; };
} StringPool, POOL;
typedef struct STRINGPOOL_API Slice Slice;
typedef char* (*DeSliceFun)(struct Slice*);
typedef struct STRINGPOOL_API Slice {
#ifdef __GNUC__
public:
#endif
    unsigned char cut;
    unsigned int  len;
    ulong         pos;
	DeSliceFun    dsc;
} Slice;

typedef StringPool STRINGPOOL_API POOL;

#ifdef  SET_POOLBOTTOM
#define EXTERNAL_POOLBOTTOM SET_POOLBOTTOM
#endif

 #ifdef        EXTERNAL_POOLBOTTOM
#define   Pool EXTERNAL_POOLBOTTOM
#elif defined( INTERNAL_POOLBOTTOM )
#define   Pool INTERNAL_POOLBOTTOM
#else
#define   Pool  DEFAULT_POOLBOTTOM
#endif

#define pool_(funk) pool_ ## funk ## _ex
#define POOL_VOIDCALL(fnam) pool_(fnam)( StringPool* inst )
#define POOL_FUNCTION(fnam,prms) pool_(fnam)( StringPool* inst, prms )
#define POOL_FUNCTION2P(fnam,arg1,arg2) pool_(fnam)( StringPool* inst, arg1, arg2 )
#define POOL_CREATE_BOTTOM(bottom) static StringPool bottom ## Instance = { \
			{0}, {0}, {0}, EMPTY, EMPTY, 0, &bottom ## Instance, (char*)&bottom ## Instance.Cyc[0], 0 \
	    }; static StringPool* bottom = pool_InitializeCycle_ex( whirlVar( &bottom ## Instance ) )
#define pool_scope  POOL* Pool = pool_getBottom();

#ifdef  EXPORT_COMMANDLINER_LIBRARIES
#include <stringPool.h>
#else

StringPool* whirlVar( StringPool* poolBottom );

// must have called before can use.
StringPool* POOL_VOIDCALL(InitializeCycle);
#if defined( EXTERNAL_POOLBOTTOM ) || defined( INTERNAL_POOLBOTTOM ) 
 #define   pool_InitializeCycle() pool_(InitializeCycle)(Pool)
#else
StringPool* pool_InitializeCycle(void);
#endif

void       pool_setBottom(StringPool*);
StringPool* pool_getBottom(void);

#ifndef NO_CHECKPOINT_MODE
// pushes an additional chunk which can later be popped again,
// by restoring the prior state it was in before.
StringPool* POOL_VOIDCALL(push);
#define    pool_push() pool_(push)(Pool)

// If additional instances may have been pushed before, this
// frees the current active one on the top. - pop .. then it
// returns pointer to the prior instance which lays down below
// If no additional pools have been allocated yet before, it
// does nothing then, returning pointer then just to the bottom, 
// just to the floor.
StringPool* POOL_VOIDCALL(pop);
#define    pool_pop() pool_(pop)(Pool)

// Set an existing instance into the Pool as 'current' whirl
void       POOL_FUNCTION(attach,StringPool*);
#define    pool_attach(newpool) pool_(attach)(Pool,newpool)

// snatch the current active whirl off from the pool
StringPool* POOL_VOIDCALL(detach);
#define    pool_detach() pool_(detach)(Pool)
#endif



// write a string to the pool and return char* to data
char*   POOL_FUNCTION( set, const char* );
#define pool_set(srcstr) pool_(set)(Pool,srcstr)

// write cstring to the pool and return charpointer to
char*   POOL_FUNCTION( sets, const char* );
#define pool_sets(srcstr) pool_(sets)(Pool,srcstr)

// write 'cbSize' bytes binary 'data' into the pool
byte*   POOL_FUNCTION2P( setb, void* data, uint cbSize );
#define pool_setb(ptDat,cbLen) pool_(setb)(Pool,ptDat,cbLen)

// write 4byte (sizeof(uint)) binary 'data' into the pool
uint*   POOL_FUNCTION( seti, uint data );
#define pool_seti(uiDat) pool_(seti)(Pool,uiDat)

// write char 'c' 'count' times into the pool (e.g. like 'fill')
char*   POOL_FUNCTION2P( setc, char c, uint count );
#define pool_setc(bChar,cbLen) pool_(setc)(Pool,bChar,cbLen)

// write formated (by 'fmt') string 'src' into the pool
char*   POOL_FUNCTION2P( setf, const char* fmt, const char* src );
#define pool_setf(strFmt,strSrc) pool_(setf)(Pool,strFmt,strSrc)

// write int 'num' (formated by 'fmt') into the pool
char*   POOL_FUNCTION2P( setfi, const char* fmt, int num );
#define pool_setfi(strFmt,intSrc) pool_(setfi)(Pool,strFmt,intSrc)

// write 64bit of data (8byte), passed by value, into the pool
char*   POOL_FUNCTION( set8, ulong acht );
#define pool_set8(acht) pool_(set8)(Pool,acht)

// Check if a 'sizeplan' could match without
// passing the point of no return at least.
// returns: 'positive value': count on bytes
//          left 'best before' point of no return.
// returns: 'negative value': count on bytes
//          the sizeplan would push beyond
//          the cycle's point of no return.
//          generates commander error: 'overlap'
// returns: 'false': The sizeplan would exceed
//          even a whole CYCLE_SIZE-ed chunk.
//          generates commander error: 'buffer'
int     POOL_FUNCTION(sizePlan,int planedsize);
#define pool_sizePlan(planedsize) pool_(sizePlan)(Pool,planedsize)

// Do a check if requested size on bytes
// could mach (strait through readable)
// without reaching point of no return:
// - If NOT: returns NULL (and generates
// commander error 'buffer').
// - If YES: returns a pointer to the offset
// to the pool buffer's actual write position
// used for startingpoint of the data which
// is planed to be written. (the offset is
// initially 0, but will raise with count on
// bytes written till pointer was obtained)
uint*   POOL_FUNCTION(ensure,uint planedsize);
#define pool_ensure(size) pool_(ensure)(Pool,size)

// check if last operation has caused an overlap
// return true if last operation caused cycling
// over the 'Point of no return' mark. which means
// that the pool currently is in a state where
// actually NO data yet was over written, but where
// the next operation WILL overwrites or takes mem
// which before at some point was in use allready
int     POOL_VOIDCALL(overlap);
#define pool_overlap() pool_(overlap)(Pool)

// Get the string which was just written before
char*   POOL_VOIDCALL(get);
#define pool_get() pool_(get)(Pool)
// Get slice to the string just written before. 
Slice   POOL_VOIDCALL(slic);
#define pool_slic() pool_(slic)(Pool)

// Return the string which was written 'pos' times
// ago since the last call to pool_set() functions
char*   POOL_FUNCTION(last,int);
#define pool_last(number) pool_(last)(Pool,number)

// Get the 'slice' which was written 'num' times
// before lastone. returns same content as pool_last()
// also would, but returned as a Slice encutter instead
Slice   POOL_FUNCTION(slice,int);
#define pool_slice(atpos) pool_(slice)(Pool,atpos)

// Merge last 'count' on strings which where set
// by any of the pool_set..() function calls...
char*   POOL_FUNCTION(merge,int);
#define pool_merge(count) pool_(merge)(Pool,count)

// Get a merged range of Slices from the cycle...
// ...same like pool_merge() does, but merged contents
// are retreived via Slicen cutters instead of char*
// pointers which strictly depend on propper termination
// and may fail when working on strings which not
// strictly are ordered 'strait forward' accessible. 
Slice POOL_FUNCTION(slices,int);
#define pool_slices(merget) pool_(slices)(Pool,merget)
#define slice_get(poolslice) poolslice.dsc(&poolslice)
#define slcPt_get(sliceptr) sliceptr->dsc(sliceptr)
#define slice_len(poolslice) poolslice.len
#define slcPt_len(sliceptr) sliceptr->len

#ifndef NO_CHECKPOINT_MODE
// Set the cycle into 'checkpoint' mode.
// As soon a checkpoint is set, it will
// not 'cycle' anymore, but will push a
// new pool each time it's reaching end.
// when 'collectCheckpoint()' is called
// for collecting any chunks pushed in 
// between, it switches back to recycle 
// mode where overwriting the beginning 
uint*   POOL_VOIDCALL(setCheckpoint);
#define pool_setCheckpoint() pool_(setCheckpoint)(Pool) 
#define pool_deployBark() pool_setCheckpoint()

// Collect all strings which have been stored
// since last call to 'setCheckpoint()' and 
// returns these merged into (maybe several)
// long, huge string chunks (each CYCLE_SIZE
// count on bytes large. if since 'checkpoint'
// additional instances may have been pushed,
// 'collectCheckpoint()' then is callable 
// severeal times after another for returning
// each of the CYCLE_SIZEed chunks which have
// been pushed during larger transfere opperations.
// each call then pops current pool and restores 
// previous one,. when all allocated pools are
// collected and have been poped again it returns
// NULL then and switches back it's behavior to
// recycling the actually stated pool instance
char*   POOL_VOIDCALL(collectCheckpoint);
#define pool_collectCheckpoint() pool_(collectCheckpoint)(Pool)
#endif

// Return count on 'slices' left till point of no return
// will be passed. ...Where 'slices' here should be seen 
// rather predictional value. it assumes avarage string 
// sizes of 'SLICE_SIZE' per pool_set() call. so at least
// much more set calls may be possible to make if many 
// string slices set where even shorter then SLICE_SIZE.
int     POOL_VOIDCALL(slicesTillPointOfNoReturn);
#define pool_slicesTillPointOfNoReturn() pool_(slicesTillPointOfNoReturn)(Pool)

#ifndef NO_CHECKPOINT_MODE
// Count on Slices already in use (better: count
// on Slices pointing to strings which where written 
// since pool_setCheckpoint() or pool_deployBark() where
// called before..
int     POOL_VOIDCALL(slicesSinceCheckpoint);
#define pool_slicesSinceCheckpoint() pool_(slicesSinceCheckpoint)(Pool)

// Exact count on bytes which have been written 
// since setCheckpoint() has been called before
int     POOL_VOIDCALL(byteSinceCheckpoint);
#define pool_byteSinceCheckpoint() pool_(byteSinceCheckpoint)(Pool)

// Free any pool instances which maybe have been pushed since last
// call to 'pool_setCheckpoint()' has been made.
void    POOL_VOIDCALL(cleanupCheckpoint);
#define pool_cleanupCheckpoint() pool_(cleanupCheckpoint)(Pool)

// Free a pool which previously was obtained by push'n'detach
int     POOL_FUNCTION(freeState, StringPool*);
#define pool_freeState(state) pool_(freeState)(Pool,state)
#endif

// How many Pools are currently allocated at all?
uint    POOL_VOIDCALL(cyclesPushed);
#define pool_cyclesPushed() pool_(cyclesPushed)(Pool)

// Get exact size of string content between slice 'number'
// and one slice before slice 'number'.
uint    POOL_FUNCTION(getSliceSize,uint);
#define pool_getSliceSize(number) pool_(getSliceSize)(Pool,number)

#ifndef NO_CHECKPOINT_MODE
// Pop any pool chunks which may have been pushed since programm start 
// and which still are attached to the bottom at least. will be called
// automatically by the process's at_exit() handlers when used within
// scripts or projects which also include commandLiner/environMentor.
// when used 'standalone' within any other projects which may not use
// cmLiner/envMentor one should ensure then to having it called once
// before the process is terminating at least. It shouldn't have any
// negative effects calling it multiple times during application shuttdown.
// (e.g. having this called two or three times is better then letting
// process close without having it called neither never at all)
void    POOL_VOIDCALL(freeAllCycles);
#define pool_freeAllCycles() pool_(freeAllCycles)(Pool)
#endif 

// Print out actual pool state stats for debug purpose, like fillstate,
// pools count, everage slicen sizes, count on overlaps, etc... 
#if DEBUG>0
void    POOL_VOIDCALL(PrintStatistics);
#define pool_PrintStatistics() pool_(PrintStatistics)(Pool)
#endif

#endif

#if defined( EXPERIMENTAL )

    typedef char* (*SimplePoolStream)(StringPool*);
    typedef char* (*PoolStreamGetter)(void*);
    typedef struct PoolStream{
        SimplePoolStream data;
        PoolStreamGetter read;
        char*            last;
		StringPool*       pool;
    } PoolStream;
	#define poolStream(plStrm) plStrm.read(&plStrm)
	#define pntrStream(ptPStr) ptPStr->read(ptPlStr)
    PoolStream POOL_FUNCTION( createStream, SimplePoolStream function );
	#define    pool_createStream(getfu) pool_(createStream)( Pool, getfu )
	
#endif


#ifdef  INTERNAL_POOLBOTTOM
#undef  using_stringPool
#define using_stringPool (0)
#else
 #ifdef _OnTheFly_
  #ifndef WITH_STRGPOOL
   #include ".stringPool.cc"
   #undef  Pool
   #define Pool DEFAULT_POOLBOTTOM
  #endif
 #endif
#endif
#if defined(__cplusplus) && !defined(_OnTheFly_)
COMMANDCLOSER_EXTERN_C
#endif
#endif
