/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      .fs.h                                      ||
||     Author:    Kalle                                      ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef IncludesFor_fs
#define IncludesFor_fs (1)


#define PREFER_LOCAL (1)
#include ".environMentor.h"

#ifndef MAX_EXT_LEN
#define MAX_EXT_LEN (MAX_NAM_LEN/16)
#endif

const char* fs_CD(void);
int         fs_PNE(const char* fullpath);
int         fs_fileCopy(const char* src, const char* dst);
const char* fs_getFirstMatch(const char* wildcard);
const char* fs_getAllMatches(const char* wildcard);


#ifdef _OnTheFly_
#include ".fs.cc"
#endif
#endif
