#ifdef using_stringPool
#define BottomCounter (using_stringPool+1)
#undef using_stringPool
#define using_stringPool (BottomCounter)

#ifdef pool_set
#undef pool_push
#undef pool_pop
#undef pool_detach
#undef pool_attach
#undef pool_set
#undef pool_sets
#undef pool_setb
#undef pool_seti
#undef pool_setc
#undef pool_setf
#undef pool_setfi
#undef pool_sizePlan
#undef pool_ensure
#undef pool_overlap
#undef pool_get
#undef pool_last
#undef pool_merge
#undef pool_slic
#undef pool_slice
#undef pool_slices
#undef pool_setCheckpoint
#undef pool_collectCheckpoint
#undef pool_cleanupCheckpoint
#undef pool_slicesTillPointOfNoReturn
#undef pool_slicesSinceCheckpoint
#undef pool_byteSinceCheckpoint
#undef pool_getSliceSize
#undef pool_cyclesPushed
#undef pool_freeState
#undef pool_freeAllCycles
#if defined( DEBUG )
 #undef pool_PrintStatistics
#endif
#if defined( EXPERIMENTAL )
 #undef poolStream
 #undef pntrStream
 #undef pool_createStream
#endif
#endif

#ifdef  PREPARE_POOLBOTTOM
#define BOTTOM (PREPARE_POOLBOTTOM)
#else
#define BOTTOM (DEFAULT_POOLBOTTOM)
#endif

#define pool_push() pool_(push)(BOTTOM)
#define pool_pop() pool_(pop)(BOTTOM)
#define pool_detach() pool_(detach)(BOTTOM)
#define pool_attach(newpool) pool_(attach)(BOTTOM,newpool)
#define pool_set(srcstr) pool_(set)(BOTTOM,srcstr)
#define pool_sets(srcstr) pool_(sets)(BOTTOM,srcstr)
#define pool_setb(ptDat,cbLen) pool_(setb)(BOTTOM,ptDat,cbLen)
#define pool_seti(uiDat) pool_(seti)(BOTTOM,uiDat)
#define pool_set8(longi) pool_(set8)(BOTTOM,longi)
#define pool_setc(bChar,cbLen) pool_(setc)(BOTTOM,bChar,cbLen)
#define pool_setf(strFmt,strSrc) pool_(setf)(BOTTOM,strFmt,strSrc)
#define pool_setfi(strFmt,intSrc) pool_(setfi)(BOTTOM,strFmt,intSrc)
#define pool_sizePlan(planedsize) pool_(sizePlan)(BOTTOM,planedsize)
#define pool_ensure(size) pool_(ensure)(BOTTOM,size)
#define pool_overlap() pool_(overlap)(BOTTOM)
#define pool_get() pool_(get)(BOTTOM)
#define pool_last(number) pool_(last)(BOTTOM,number)
#define pool_merge(count) pool_(merge)(BOTTOM,count)
#define pool_slic() pool_(slic)(BOTTOM)
#define pool_slice(atpos) pool_(slice)(BOTTOM,atpos)
#define pool_slices(merget) pool_(slices)(BOTTOM,merget)
#define pool_setCheckpoint() pool_(setCheckpoint)(BOTTOM)
#define pool_collectCheckpoint() pool_(collectCheckpoint)(BOTTOM)
#define pool_cleanupCheckpoint() pool_(cleanupCheckpoint)(BOTTOM)
#define pool_slicesTillPointOfNoReturn() pool_(slicesTillPointOfNoReturn)(BOTTOM)
#define pool_slicesSinceCheckpoint() pool_(slicesSinceCheckpoint)(BOTTOM)
#define pool_byteSinceCheckpoint() pool_(byteSinceCheckpoint)(BOTTOM)
#define pool_getSliceSize(number) pool_(getSliceSize)(BOTTOM,number)
#define pool_cyclesPushed() pool_(cyclesPushed)(BOTTOM)
#define pool_freeState(state) pool_(freeState)(BOTTOM,state)
#define pool_freeAllCycles() pool_(freeAllCycles)(BOTTOM)

#if defined( DEBUG )
    #define pool_PrintStatistics() pool_(PrintStatistics)(BOTTOM)
#endif

#if defined( EXPERIMENTAL )
	#define poolStream(plStrm) plStrm.read(&plStrm)
	#define pntrStream(ptPStr) ptPStr->read(ptPlStr)
	#define pool_createStream(getfu) pool_(createStream)(BOTTOM,getfu)
#endif

#ifdef  PREPARE_POOLBOTTOM
 #undef PREPARE_POOLBOTTOM
#endif

#undef BOTTOM
#define BOTTOM Pool
#endif
