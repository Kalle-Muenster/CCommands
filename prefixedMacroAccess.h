#ifdef find_macro_name
#undef find_macro_name
#endif
#ifdef found_name
#undef found_name
#endif

#if  defined(find_name)
# define find_macro_name (find_name)
#else 
# define  prefixed_name LOCAL_PREFIX ## _ ## find_name
# if	  defined(prefixed_name)
#  define find_macro_name (prefixed_name)
# endif
#endif

#ifdef  find_macro_name
#define found_name find_macro_name
#endif

