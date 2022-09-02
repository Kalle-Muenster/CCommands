/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      xmlNow.h                                   ||
||     Author:    Kalle                                      ||
||     Generated: 22.12.2017                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef IncludesFor_xmlNow
#define IncludesFor_xmlNow (1)

#define NO_SPACE_CHECKING (1)
#define EXPERIMENTAL (1)

#include "libRoXml.h"
#include <.environMentor.h>

#ifndef XMLNOW_FORMATING
#define XMLNOW_FORMATING (1)
#endif

void xmlNow_commitChanges( node_t* xmldoc, const char* tempFile );
PoolStream xmlNow_addElement( node_t* xmldoc, const char* where, const char* what );
PoolStream xmlNow_setElement( node_t* xmldoc, const char* where, const char* what);
PoolStream xmlNow_remElement(node_t* xmldox, const char* where,const char* what);
PoolStream xmlNow_perform( node_t* xmldoc, const char* action );

#ifdef _OnTheFly_
#include ".xmlNow.cc"
#endif
#endif
