/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      cConfig.h                                  ||
||     Author:    Kalle                                      ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef using_cConfig
#define using_cConfig .cConfig.cc

void SaveCommandLinerConfiguration( int maxnumgum, int maxnamlen, int autoslash,
                                    int autoquote, int autobrack, int autospack );

#ifdef _OnTheFly_
#include ".cConfig.cc"
#undef maxnum
#endif
#endif
