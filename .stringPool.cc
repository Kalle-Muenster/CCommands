/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      stringPool.cc                              ||
||     Author:    Kalle                                      ||
||     Generated: 13.08.2019                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
#include <.CheckForProjectSettings.h>
#include ".stringPool.h"
#endif

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef DEBUG
#define DEBUG 0
#endif

static struct WhirlVars pool_protopool_vars = {
     EMPTY, EMPTY, 0, NULL, NULL, NULL,
};
StringPool* whirlVar( StringPool* poolBottom )
{
    pool_protopool_vars.running = poolBottom;
	*((struct WhirlVars*)&poolBottom->Pls) = pool_protopool_vars;
    return poolBottom;
}

#ifndef EXTERNAL_POOLBOTTOM
#ifdef prefix
#undef prefix
#endif
#define prefix(name,plus) name ## plus
static StringPool prefix( Pool, Instance) = { { 0 },{ 0 },{ 0 },
	EMPTY, EMPTY, 0, &prefix( Pool, Instance ),
    (char*)&prefix(Pool,Instance).Cyc[0], 0
};
static StringPool* Pool = &prefix(Pool,Instance);
#ifndef INTERNAL_POOLBOTTOM
StringPool* pool_InitializeCycle(void) {
    return pool_(InitializeCycle)(Pool);
}
#endif
#endif

static StringPool* defaultInstance = NULL;
uint _writeCStringPool( StringPool* inst, const byte* src );
uint _writeDataToCycle( StringPool* inst, const byte* src, uint cbSize );
uint _writeCharToCycle( StringPool* inst, byte c, uint count );

#undef  Pool
#define Pool inst
#define pool inst->running

#if defined(using_commandLiner) || defined(WITH_STRGPOOL) 
void poolDingsBumsWeg(void)
{
	StringPool* dings = (StringPool*)getDingens("pool");
	#if DEBUG
	if( dings == &PoolInstance ) {
		printf("%s(): dingsbums points 'The Bottom' of the pool\n",__FUNCTION__);
	} else {
		printf("%s(): dingsbums (%p) is not 'The Bottom' (%p)\n",__FUNCTION__,dings,&PoolInstance);
	}
	#endif
	pool_freeAllCycles_ex( dings );
}
#endif

void pool_setBottom(StringPool* poolDingsbums)
{
#ifndef CLINE_INTERNAL
#if defined(using_commandLiner) || defined(WITH_STRGPOOL) 
	if(!getDingens( "pool" ) )
		addDingens( "pool", poolDingsbums, &poolDingsBumsWeg );
#endif
#endif
	defaultInstance = poolDingsbums;
	#if DEBUG
	printf("%s(): %p\n",__FUNCTION__,poolDingsbums);
	#endif
}

StringPool* POOL_VOIDCALL( InitializeCycle )
{
	#if DEBUG
	printf("%s(): bottom at the floor is: %p\n",__FUNCTION__,&PoolInstance);
	#endif
    if( inst->Pls == EMPTY ) {
		inst->Cnt = EMPTY;
        inst->Pls = 0;
        inst->running = inst; }
	if( pool->Cnt == EMPTY ) {
        pool->Cnt = 0;
        pool->Ovr = 0;
		pool->Chk = 0;
        const int SLICESIZE = (CYCLE_SIZE / CYCLE_COUNT);
        for( int i=0; i<CYCLE_COUNT; i++ ) {
            pool->Cyc[ pool->Pos[i] = (i*SLICESIZE) ] = 0;
        } pool->running = inst;
    } pool_setBottom( pool ); 
    return pool;
}

StringPool* pool_getBottom( void )
{
#ifdef using_commandLiner
    if( !defaultInstance ) {
		if( !(defaultInstance = (StringPool*)getDingens( "pool" )))
			setErrorText( "initialization" ); } 
#endif
    return defaultInstance;
}

#if DEBUG>0
void POOL_VOIDCALL(PrintStatistics)
{
    printf("\ncurrent Pool string cycle statistics:\n\n");
    printf("    cycle buffer size: %i\n",CYCLE_SIZE);
    printf("    number of slices: %i\n",CYCLE_COUNT);
    int isInitialized = false;
    if(inst->Cur)
        isInitialized = pool->Cnt != EMPTY && pool->Pls != EMPTY;
    printf("    is initialized: %s\n",isInitialized?"true":"false");
    if(!isInitialized)
        return;
    printf("    count on slices in cycle: %i\n\n",pool->Cnt);
    for( uint i = 0; i < pool->Cnt; ++i ) {
        printf( " slice %i position: %i, length: %i\n",
                i, pool->Pos[i], pool_getSliceSize(i) );
    }
    printf("\n");
}
#endif

static char* slice_toString( Slice* chunk )
{
	#if DEBUG > 0
	printf("%s(): slice->len is %i\n",__FUNCTION__,chunk->len);
	#endif
    char* str = (char*)chunk->pos;
    *(str + chunk->len) = '\0';
    *str = chunk->cut;
    #if DEBUG > 0
	printf("%s(): string is %s\n",__FUNCTION__,str);
	#endif
    return str;
}

int POOL_VOIDCALL( slicesTillPointOfNoReturn )
{
    return (CYCLE_COUNT - (pool->Cnt-1));
}

uint POOL_FUNCTION( getSliceSize , uint sliceNumber )
{
    if( (sliceNumber <= CYCLE_COUNT) && sliceNumber ) {
        return ( pool->Pos[sliceNumber-1] > pool->Pos[sliceNumber]
               ? pool->Pos[sliceNumber-1] - pool->Pos[sliceNumber]
               : CYCLE_SIZE - pool->Pos[sliceNumber] );
    } return 0;
}

uint POOL_VOIDCALL( cyclesPushed )
{
    return inst->Pls;
}

#ifndef NO_CHECKPOINT_MODE
int POOL_VOIDCALL( slicesSinceCheckpoint )
{
    return pool->Cnt-1;
}

int POOL_VOIDCALL( byteSinceCheckpoint )
{
    if (!inst->Pls) return *pool->Chk; // + (pool->Cnt-1);
    StringPool* next = pool;
    uint bytes = *pool->Chk;
    for (uint i = 0; i < inst->Pls; ++i) {
        next = next->running;
        bytes += next->Pos[1] + next->Ovr;
    } return (int)bytes;
}

StringPool* POOL_VOIDCALL( push )
{
    StringPool* t = pool_(InitializeCycle)(inst);
    pool = (StringPool*)malloc(sizeof(StringPool));
    pool_protopool_vars.running = inst;
    *((struct WhirlVars*)&pool->Pls) = pool_protopool_vars;
    pool->Cur = (char*)&pool->Cyc[0];
    pool->Cnt = EMPTY;
    pool->Pls = 0;
   #if DEBUG>0
    printf( "%s(): new whirl from the bottom as new pool (adr: %p)\n",
           __FUNCTION__, inst->running );
   #endif
    pool_(InitializeCycle)(inst);
    pool->Chk = &pool->Pos[0];
    pool->running = t;
    inst->Pls++;
    return pool;
}

StringPool* POOL_VOIDCALL( pop )
{
    if ( inst->running != inst ) {
       #if DEBUG>0
        printf( "%s(): cycle at address: %p\n",
                __FUNCTION__, inst->running );
       #endif
        StringPool* t = inst->running->running;
        inst->running->Cur = NULL;
        free( inst->running );
        inst->running = t;
        inst->Pls--;
    } return inst->running;
}

void POOL_FUNCTION( attach, StringPool* pushed )
{
    inst->running = pushed->running;
    pushed->running = inst;
}

StringPool* POOL_VOIDCALL( detach )
{
    if( pool != inst ) {
        StringPool* detach = inst->running;
        inst->running = detach->running;
        detach->running = detach;
    #if DEBUG>0
        printf( "%s(): detached whirl (at adr. %p) from the bottom\n",
              __FUNCTION__, detach );
    #endif
        return detach;
    } return NULL;
}

int POOL_FUNCTION( freeState, StringPool* state )
{
    uint storecount = inst->Pls;
    if( state != inst->running ) {
        StringPool* storestate = inst->running;
        StringPool* check = storestate->running;
        while( check != state && check != inst )
            check = check->running;
        if( check != inst )
            check->running = inst;
        inst->Pls = EMPTY;
        inst->running = state;
        pool_freeAllCycles();
        inst->running = storestate;
        inst->Pls = storecount;
    } else pool_freeAllCycles();
    return storecount - inst->Pls;
}
#endif // NO_CHECPOINT_MODE

int POOL_FUNCTION( sizePlan, int sizeplan )
{
    if( sizeplan <= CYCLE_SIZE ) {
        sizeplan -= CYCLE_SIZE - pool->Pos[0];
        sizeplan = (sizeplan==0)? 1 : -sizeplan;
      #ifdef using_commandLiner
        if( sizeplan < 0 ) setErrorText("overlap");
	  #endif
    } else sizeplan = 0;
    #ifdef using_commandLiner
	    if (sizeplan == 0 ) setErrorText("buffer"); 
    #endif
	return sizeplan;
}

uint* POOL_FUNCTION( ensure, uint straitLength )
{
    uint plan = (uint)pool_sizePlan( straitLength );
    if( !plan ) {
      #if DEBUG
        printf( "%s(): FATAL: requested size exceeds max of %i byte\n",
              __FUNCTION__, CYCLE_SIZE );
      #endif
        return NULL;
    } else if ( plan > straitLength ) {
        pool->Pos[0] = pool->Cut[0] = pool->Ovr = 0;
        pool->Cnt = pool->Cnt ? 1 : 0;
    } return &pool->Pos[0];
}

int POOL_VOIDCALL( overlap )
{
    return pool->Ovr;
}

#ifndef NO_CHECKPOINT_MODE
uint* POOL_VOIDCALL( setCheckpoint )
{
   #if DEBUG>0
    printf("%s():\n", __FUNCTION__);
   #endif
    pool_(InitializeCycle)(inst)->Cnt = 1;
    pool->Chk = pool_ensure( CYCLE_SIZE );
   #ifdef using_commandLiner
   // catch 'overlap' error when happens
   // If in checkpoint mode, auto pushes
   // new instance on point of no return
   // so no overlap will happen at least
    catchError( "overlap" );
   #endif
    return pool->Chk;
}
#endif

#define writeCStringPool( pbSrc ) _writeCStringPool( Pool, pbSrc )
uint   _writeCStringPool( StringPool* inst, const byte* src )
{
    uint overstep=0;
    while (
        pool->Cyc [ ( pool->Pos[0] != CYCLE_SIZE )
            ? ++pool->Pos[0]
            : pool->Pos[0] + ++overstep
        ] = *(src++) );
    return overstep;
}

#define writeDataToCycle( pbSrc, srcLen ) _writeDataToCycle( Pool, pbSrc, srcLen )
uint   _writeDataToCycle( StringPool* inst, const byte* src, uint cbSize )
{
    uint overstep=0;
    for( uint i=0; i<=cbSize; i++ ) {
        pool->Cyc [ ( pool->Pos[0] != CYCLE_SIZE )
            ? ++pool->Pos[0]
            : pool->Pos[0] + ++overstep
        ] = i==cbSize?0:*(src++);
    } return overstep;
}

#define writeCharToCycle( bChar, uiCnt ) _writeCharToCycle( Pool, bChar, uiCnt )
uint   _writeCharToCycle( StringPool* inst, byte c, uint count )
{
    uint overstep=0;
    for( uint i=0; i<=count; i++ ) {
        pool->Cyc [ ( pool->Pos[0] != CYCLE_SIZE )
            ? ++pool->Pos[0]
            : pool->Pos[0] + ++overstep
        ] = i==count?0:c;
    } return overstep;
}


#define writePoolSlice( ptDat, cbLen ) _writePoolSlice( Pool, ptDat, cbLen )
byte*  _writePoolSlice( StringPool* inst, ptval ptData, int cbSize )
{
    if( ptData < 256u ) {
        if( (!ptData) && cbSize==EMPTY ) return 0;//(byte*)ptData;
        pool->Cut[0] = (byte)ptData;
    } else {
        pool->Cut[0] = *(const byte*)ptData;
    } const byte* src = (const byte*)ptData;
    for(uint i=CYCLE_COUNT-1; i>0; i--) {
        pool->Pos[i]=pool->Pos[i-1];
        pool->Cut[i]=pool->Cut[i-1];
    } pool->Pos[0]--;
    pool->Ovr = cbSize == EMPTY
              ? writeCStringPool(src)
              : ptData < 256u
              ? writeCharToCycle(pool->Cut[0],cbSize)
              : writeDataToCycle(src,cbSize);
    if(pool->Pos[0]== CYCLE_SIZE)
       pool->Pos[0] = 0;
#ifndef NO_CHECKPOINT_MODE
    if(pool->Cnt) {
       pool->Cnt++; // when bottom is in 'checkpoint' mode
        if( pool->Ovr ) {
            pool->Cur = (char*)&pool->Cyc[0];
           #if DEBUG>0
            printf("%s(): current count = %i, overstep = %i\n"
                   "  pushing new cycle !\n", __FUNCTION__,
                   pool->Cnt, pool->Ovr );
           #endif
            pool_push()->Cnt = 1;
            return (byte*)(pool->Cur = (char*)&pool->running->Cyc[
                                           pool->running->Pos[1]
                                       ]);
        }
    } else
#endif // end CHECKPOINT_MODE
    { // else bottom in regular 'recycling' mode
       #if DEBUG>0
        if( pool->Ovr ) {
            printf( "%s(): Point of no return is reached!\n"
                 "  now, when adding data, slices from"
                  " the beginning will be overwritten!\n",
                  __FUNCTION__ ); }
       #endif
    } return (byte*)(pool->Cur = (char*)&pool->Cyc[pool->Pos[1]]);
}


char* POOL_FUNCTION( sets, const char* name )
{
    return (char*)writePoolSlice((ptval)(void*)name,EMPTY);
}

byte* POOL_FUNCTION2P( setb, void* data, uint cbSize )
{
    return writePoolSlice( (ptval)data, cbSize );
}

uint* POOL_FUNCTION( seti, uint data )
{
    return (uint*)writePoolSlice( (ptval)(void*)&data, sizeof(uint) );
}

char* POOL_FUNCTION2P( setc, char c, uint count )
{
    return (char*)writePoolSlice( (ptval)*(byte*)&c, count );
}

char* POOL_FUNCTION( set, const char* name )
{
    return (char*)writePoolSlice((ptval)(void*)name,EMPTY);
}

char* POOL_FUNCTION( set8, ulong data )
{
    return (char*)writePoolSlice((ptval)(void*)&data,8);
}

char* POOL_FUNCTION2P( setf, const char* fmt, const char* src )
{
   #if defined(__TINYC__)
    char buffer[strlen(fmt)+strlen(src)+2];
    sprintf(&buffer[0],fmt,src);
    pool_set(&buffer[0]);
   #else
    uint bufLen = (uint)(strlen(fmt) + strlen(src) + 2);
    char* buffer = (char*)malloc( bufLen );
    sprintf( buffer, fmt, src );
    pool_set( buffer );
    free( buffer );
   #endif
    return pool->Cur;
}

char* POOL_FUNCTION2P( setfi, const char*fmt, int src )
{
   #if defined(__TINYC__)
    char buffer[strlen(fmt)+16];
    sprintf(&buffer[0],fmt,src);
    pool_set(&buffer[0]);
   #else
    uint bufLen = (uint)(strlen(fmt) + 32);
    char* buffer = (char*)malloc( bufLen );
    sprintf( buffer, fmt, src );
    pool_set( buffer );
    free( buffer );
   #endif
    return pool->Cur;
}

char* POOL_VOIDCALL( get )
{
    return pool->Cur;
}

Slice POOL_VOIDCALL( slic )
{
    pool->Cut[1] = pool->Cyc[pool->Pos[1]];
    Slice cut = { (byte)pool->Cut[1],
        (uint)(pool->Pos[0]-pool->Pos[1]),
        (ptval)pool->Cur, &slice_toString,
    };
    return *(Slice*)&cut;
}

char* POOL_FUNCTION( last, int pos )
{
    if( CYCLE_COUNT > pos ) {
        for( int i = 1; i <= pos; ++i )
            pool->Cyc[pool->Pos[i]] = '\0';
    } pos %= CYCLE_COUNT;
    pool->Cyc[pool->Pos[pos]] = pool->Cut[pos];
    return (char*)&pool->Cyc[pool->Pos[pos]];
}

Slice POOL_FUNCTION( slice, int at )
{
    if( CYCLE_COUNT > at ) {
        for( int s = 1; s <= at; ++s )
            pool->Cyc[pool->Pos[s]] = '\0';
    } at %= CYCLE_COUNT;
    Slice slce = { (byte)pool->Cut[at],
        (uint)(pool->Pos[at - 1] - pool->Pos[at]),
        (ptval)&pool->Cyc[pool->Pos[at]], &slice_toString,
    };
    return slce;
}

Slice POOL_FUNCTION( slices, int count )
{
    count %= CYCLE_COUNT;
    for( int i = 1; i <= count; ++i )
        pool->Cyc[pool->Pos[i]] = pool->Cut[i];
    pool->Cur = (char*)&pool->Cyc[pool->Pos[count]];
    pool->Cnt = 0;
    Slice slce = { (byte)pool->Cut[count],
        (uint)(pool->Pos[count - 1] - pool->Pos[count]),
        (ptval)pool->Cur, &slice_toString,
    };
    return slce;
}

char* POOL_FUNCTION( merge, int count )
{
    count %= CYCLE_COUNT;
    for( int i=1; i<=count; ++i )
        pool->Cyc[ pool->Pos[i] ] = pool->Cut[i];
    pool->Cnt = 0;
    return pool->Cur
         = (char*)&pool->Cyc[pool->Pos[count]];
}

#ifndef NO_CHECKPOINT_MODE
char* POOL_VOIDCALL( collectCheckpoint )
{
    int pushed, wasLooping;
    StringPool* current = pool;
    wasLooping = false;
    while( pushed = ( (current->running->Cnt >= 1)
                    && current != inst) ) {
        current = current->running;
        wasLooping++;
    } if ( wasLooping ) {
       #if DEBUG>0
        printf( "%s(): cycle was pushed %i times!\n",
              __FUNCTION__, wasLooping );
       #endif
        current->Cnt = 0;
        return current->Cur;
    } else {
        if( current->Cnt ) {
           #if DEBUG>0
            printf( "%s(): fished %i strings out from the pool\n",
                  __FUNCTION__, current->Cnt-1 );
           #endif
            if( (--current->Cnt) <= CYCLE_COUNT )
                return pool_merge( current->Cnt );
            else
                return (char*)&current->Cyc[current->Cnt = 0];
        } else {
           #if DEBUG>0
            printf( "%s(): everything collected!\n", __FUNCTION__ );
           #endif
          return NULL;
        }
    }
}

void POOL_VOIDCALL( cleanupCheckpoint )
{
    while( pool_cyclesPushed() ) {
        pool_pop();
    } pool_InitializeCycle_ex( inst );
}

void POOL_VOIDCALL( freeAllCycles )
{
   #if DEBUG>=1
    printf("%s():\n", __FUNCTION__);
    StringPool* ptrVal=0;
    while ( (ptrVal = pool_pop()) != inst )
        printf("%s(): next pop: %p\n",__FUNCTION__,ptrVal);
	printf("%s(): all pools poped!\n",__FUNCTION__);
   #endif
    while ( pool_pop() != inst );
}
#endif

#undef pool

#if defined( EXPERIMENTAL )
char* chunkReader(void* ptStream)
{
    PoolStream* str = (PoolStream*)ptStream;
    if(str->data) {
        char* t = str->data(str->pool);
        if( str->last == t )
            str->data = NULL;
        return str->last = t;
    } return NULL;
}
PoolStream POOL_FUNCTION( createStream, SimplePoolStream getfunc )
{
    PoolStream out = { getfunc, &chunkReader, NULL, inst };
    return out;
}
#endif

#undef writeCStringPool
#undef writeDataToCycle
#undef writeCharToCycle
#undef writePoolSlice
