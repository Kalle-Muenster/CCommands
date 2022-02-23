#if !( LINUX || WINDOWS )
#define LNX (5787212)
#define WIN (5130583)
#ifndef LINUX 
    #define LINUX (SYSTEM==LNX) 
#endif
#ifndef WINDOWS
    #define WINDOWS (SYSTEM==WIN)
#endif
#endif
