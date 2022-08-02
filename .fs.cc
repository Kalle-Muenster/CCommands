/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      .fs.cc                                     ||
||     Author:    Kalle                                      ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
#include ".fs.h"
#endif


#define WITH_STRGPOOL
#define WITH_JUNKYARD
#include ".environMentor.h"
#include ".commandLiner.h"

#include <stdio.h>
#include <stdlib.h>
#if LINUX
  #include <unistd.h>
#endif

#include <dirent.h>

#define invokeGetter(pneptr,fu) pneptr-> get ## fu (pneptr)
#define invokeSetter(pneptr,fu,arg) pneptr-> set ## fu (pneptr,arg)

#include ".stringPool.h"
#include ".junkYard.h"

typedef enum PNE_FLAGS {
    IS_UNDEFINED = 0x0,
    IS_FILE      = 0x01,
    IS_FOLDER    = 0x02,
    FULL_NAME    = 0x04,
    PNE_NAMES    = 0x08,
    IS_PATTERN   = 0x10,
} PNE_FLAGS;

typedef struct PNE PNE;
typedef cmLn*(*PNE_getFunc)(void*);
typedef PNE*(*PNE_setFunc)(void*,cmLn);
typedef void*(*PNE_CtorFunc)(void*,cmLn);
void* PNE_Init(void*,cmLn);
typedef struct PNE {
    int pth,nam,ext,end;
    char split[4];
    PNE_FLAGS flags;
    PNE_getFunc getPath;
    PNE_getFunc getName;
    PNE_getFunc getExte;
    PNE_getFunc getFull;
    PNE_setFunc setPath;
    PNE_setFunc setName;
    PNE_setFunc setExte;
    char buf[ (2 * MAX_NAM_LEN) + MAX_EXT_LEN  +1 ];
    PNE* pne;
} PNE, *ptPNE;


cmLn PNE_getName(PNE* _pne)
{
    _pne->buf[_pne->nam] = _pne->split[0];
    _pne->split[1] = _pne->buf[_pne->ext];
    _pne->buf[_pne->ext] = '\0';
    return &_pne->buf[_pne->nam];
}

cmLn PNE_getExte(PNE* _pne)
{
    _pne->buf[_pne->ext] = _pne->split[1];
    return &_pne->buf[_pne->ext];
}

cmLn PNE_getPath(PNE* _pne)
{
    _pne->split[0] = _pne->buf[_pne->nam];
    _pne->buf[_pne->nam] = '\0';
    return &_pne->buf[_pne->pth];
}

cmLn PNE_getFull(PNE* _pne)
{
    if(!(_pne->flags & FULL_NAME))
    {
        _pne->end=-1; _pne->pth=0;
        while(_pne->buf[++_pne->end]);
        _pne->buf[_pne->end]=_pne->split[0];

        do _pne->pth++; while(
            _pne->buf[_pne->end+_pne->pth] =
            _pne->buf[_pne->nam+_pne->pth]
        );

        _pne->nam=_pne->end;
        _pne->end+=_pne->pth;
        _pne->pth=0;
        _pne->buf[_pne->end]=_pne->split[1];

        do _pne->pth++; while(
            _pne->buf[_pne->end+_pne->pth] =
            _pne->buf[_pne->ext+_pne->pth]
        );

        _pne->ext=_pne->end;
        _pne->end+=_pne->pth;
        _pne->pth=0;
        _pne->split[0]=_pne->buf[_pne->nam];
        _pne->split[1]=_pne->buf[_pne->ext];
        _pne->flags = (_pne->flags &~ PNE_NAMES)
                    | FULL_NAME;
    }
    return &_pne->buf[0];
}

ptPNE PNE_setExte(PNE* _pne,cmLn newExte)
{
    if(_pne->flags & FULL_NAME) {
       _pne->end=2*MAX_NAM_LEN-1;
        while( _pne->buf[++_pne->end] = *(newExte++) );
       _pne->ext=2*MAX_NAM_LEN;
    } else {
        strcpy(&_pne->buf[_pne->ext=2*MAX_NAM_LEN],newExte);
       _pne->end = _pne->ext+strlen(newExte);
    }
    _pne->split[1]=_pne->buf[_pne->ext];
    _pne->flags = (_pne->flags &~ FULL_NAME)
                | PNE_NAMES;
    return _pne;
}

ptPNE PNE_setName(PNE* _pne,cmLn newName)
{
    if(_pne->flags & FULL_NAME) {
        setTemp(&_pne->buf[_pne->ext]);
        _pne->setExte(_pne,getTemp());
    } strcpy(&_pne->buf[_pne->nam=MAX_NAM_LEN],newName);
    _pne->split[0]=_pne->buf[_pne->nam];
    _pne->flags = (_pne->flags &~ FULL_NAME)
                | PNE_NAMES;
    return _pne;
}

ptPNE PNE_setPath(PNE* _pne,cmLn newPath)
{
    if(_pne->flags & FULL_NAME) {
        setTemp(&_pne->buf[_pne->ext]);
        _pne->setExte(_pne,getTemp());
        setTemp(&_pne->buf[_pne->nam]);
        _pne->setName(_pne,getTemp());
    } strcpy(&_pne->buf[_pne->pth=0],newPath);
    _pne->flags = (_pne->flags &~ FULL_NAME)
                | PNE_NAMES;
    return _pne;
}

const PNE newPNE = {
    0,MAX_NAM_LEN,(MAX_NAM_LEN*2),
    ((MAX_NAM_LEN*2)+MAX_EXT_LEN),
    {"PNEe"},IS_UNDEFINED,
    &PNE_getPath, &PNE_getName,
    &PNE_getExte, &PNE_getFull,
    &PNE_setPath, &PNE_setName,
    &PNE_setExte, {0}, NULL
};

void* PNE_Init(void* allocated,cmLn fromCmdLineArg)
{
    DEBUGFMT("enter: %s",fromCmdLineArg);
    PNE* pne = (PNE*)memcpy(allocated,&newPNE,sizeof(PNE));
    pne->pne = pne;
    DEBUGFMT("address: %i",pne->pne);

    char initBuffer[MAX_NAM_LEN];
    char* tmpBuf=&initBuffer[0];
    char* pathName = (char*)fromCmdLineArg;
    int isPath = false;
    int hasExt = false;


    int i = EMPTY;  while(pathName[++i]!='\0') {
        if(pathName[i]=='/')
            isPath = true;
        if(pathName[i]=='.'&&i)
            if(pathName[i-1]!='/')
                hasExt = true;
    } pne->end=i;
    if(hasOption('x')) {
        pathName[i]=" "; pathName[++i]='\0';
    }
    if(hasExt) {
        char spacer[4]={2105376}; // <- drei spaces und ne null
        int extender = i;
        while(pathName[i]!='.'&&i>=0)i--;
        extender -= i;
        extender = extender < 4
                 ? extender : 3;
        strcpy(tmpBuf,&pathName[i]);
        strcat(tmpBuf,&spacer[3-(4-extender)]);
        strcpy(&pne->buf[pne->ext],tmpBuf);
        pathName[i]='\0';
    } else {
        pne->buf[pne->end=pne->ext]='\0';
    }
    if(isPath) {
        while(pathName[--i]!='/');
        strcpy(&pne->buf[pne->nam],&pathName[++i]);
        pathName[i]='\0';
        strcpy(&pne->buf[pne->pth=0],&pathName[0]);
        pathName[i] = pne->split[0] = (&pne->buf[pne->nam])[0];
    } else {
        strcpy(&pne->buf[pne->pth=0],fs_CD());
        strcpy(&pne->buf[pne->nam],&pathName[0]);
        pne->split[0] = (&pne->buf[pne->nam])[0];
    } i--;
    while(pathName[++i]!='\0');
    if(hasExt)
        pathName[i] = pne->split[1] = (&pne->buf[pne->ext])[0];
    pne->flags = pne->buf[pne->end-1] == '/'
               ? IS_FOLDER
               : IS_FILE;
    DEBUGFMT("seems %s",(pne->flags&IS_FILE)?"IS_FILE":"IS_FOLDER");
    DEBUGFMT("found path: %s",&pne->buf[pne->pth]);
    DEBUGFMT("found name: %s",&pne->buf[pne->nam]);
    DEBUGFMT("found .ext: %s",&pne->buf[pne->ext]);
    return pne;
}

ptPNE fs_getPNEcopy(ptPNE pne)
{
    DEBUGLOG("enter");
    char pneBuffer[MAX_NAM_LEN];
    strcpy(&pneBuffer[0],getName('P'));
    strcat(&pneBuffer[0],getName('N'));
    strcat(&pneBuffer[0],getName('E'));
    PNE_Init(pne,&pneBuffer[0]);
    DEBUGLOG("return");
    return pne;
}

PNE* PNE_new(cmLn path)
{
    return PNE_Init( malloc( ( (size_t)&newPNE.pne
                              -(size_t)&newPNE.pth )
                             + sizeof(size_t) ),
                         path );
}

void PNE_del(PNE* pne)
{
    free(pne);
    pne=NULL;
}


typedef struct ThereAreFourCharPointer {
    char* first;
    char* second;
    char* third;
    char* fourth;
} ThereAreFourCharPointer;

typedef struct NameSplit {
    union {
        char * array[4];
        ThereAreFourCharPointer four;
    };
} NameSplit;


int USAGE(fs)
{
    Synopsis("[opt] [./rel/path*]");
    printf("  -.   -   only lists FOLDER (...but as well extension-less files)\n\
   -*    -  only lists FILES (...but only these which got extension. )\n\
   -p    -  output always full path, even if entries are in cwd\n\
   --cpy -<FOLDER>  -  rather then just outputting list of found matches to stdout,\n\
                    also copy all these found items to destination target <FOLDER>");
    return CheckForError()
        || !hasOption('h');
}


const char* fs_CD(void)
{
    int find=-1;
    char currentWorkingDirectory[MAX_NAM_LEN];
    char* cwd = getcwd(&currentWorkingDirectory[0],MAX_NAM_LEN);
#if SYSTEM==WIN
    while((cwd[++find]=cwd[find]=='\\'?'/':cwd[find])!='\0');
#endif
    tempf( "%s",cwd);
    DEBUGFMT("found current working directory: %s",getTemp())
    return getTemp();
}

const char* fs_fitDestinationPattern(const char* dstPattern,const char* srcFilePath)
{
    char* srcTmp = (char*)srcFilePath;
    int l = strlen(srcTmp);
    while(srcTmp[--l]=='\n')
        srcTmp[l]='\0';

    DEBUGFMT("enter1 dstPattern: %s",dstPattern)
    DEBUGFMT("enter2 srcFilePath: %s",srcTmp)

    PNE Pattern;
    PNE Pne;

    DEBUGFMT("pattern: %s",dstPattern)
    PNE* pat = (PNE*)PNE_Init(&Pattern,dstPattern);
    DEBUGFMT("src: %s",srcFilePath)
    PNE* src = (PNE*)PNE_Init(&Pne,srcTmp);

    char bu[MAX_NAM_LEN];

    DEBUGLOG("extension")
    strcpy(&bu[0],pat->getExte(pat));
    int zwei = strlen(&bu[0])+1;
    strcpy(&bu[zwei],src->getExte(src));
    DEBUGFMT("pat extension: %s",&bu[0])
    DEBUGFMT("src extension: %s",&bu[zwei])
    tempf(&bu[0],&bu[zwei+1]);
    DEBUGFMT("resulting dst-extension: %s",getTemp())

    DEBUGLOG("name")
    DEBUGFMT("pat name: %s",pat->getName(pat))
    DEBUGFMT("src name: %s",src->getName(src))
    sprintf(&bu[0],pat->getName(pat),src->getName(src));
    pat->setName(pat,&bu[0]);
    DEBUGFMT("resulting dst-name: %s",pat->getName(pat))

    pat->setExte(pat,getTemp());
    DEBUGLOG("return")
    setTemp(pat->getFull(pat));
    DEBUGLOG(getTemp())
    return getTemp();
}

int fs_fileCopy(const char* src, const char* dst)
{
#if DEBUG
    printf("fs_fileCopy(%s,%s): enter\n",src,dst);
#endif
    if(!strcmp(src,dst)) {
        printf(setErrorText("copy destination equals copy source"));
        return getErrorCode();
    }
    FILE* fSrc,* fDst;
    PNE PneSrc,PneDst;
    PNE* pneDst = (PNE*)PNE_Init(&PneDst,dst);
    PNE* pneSrc = (PNE*)PNE_Init(&PneSrc,src);
    if(pneSrc->flags & IS_FILE) {
        fSrc = fopen(invokeGetter(pneSrc,Full),"rb");
        if(pneDst->flags & IS_FILE) {
            char* dstExt = invokeGetter(pneDst,Exte);
            const int extLen = strlen(dstExt);
            for(int i=0;i<extLen;i++) {
                if( dstExt[i]=='*' )
                    dstExt[i] = invokeGetter(pneSrc,Exte)[i];
            }
            fDst = fopen(invokeGetter(pneDst,Full),"wb");
        } else {
            setTemp(invokeGetter(pneDst,Full));
            invokeSetter(pneDst,Name,invokeGetter(pneSrc,Name));
            invokeSetter(pneDst,Exte,invokeGetter(pneSrc,Exte));
            invokeSetter(pneDst,Path,getTemp());
            fDst = fopen(invokeGetter(pneDst,Full),"wb");
        } unsigned buffer; int chunkBytes; int copiedBytes=0;
        while(chunkBytes=fread(&buffer,1,4,fSrc))
            copiedBytes+=fwrite(&buffer,1,chunkBytes,fDst);
        return copiedBytes;
    }
    return 0;
}

// checks if given starsplit 'parts' match
// the given dirent* 'ent' directory entry
// - if match: it stores the entry name to
//  cycle buffer and returns it as a char*
// - if not match: it stores nothing to the
//  cycle and returns NULL as char* value
// - if given 'ent' may be NULL, it return
//  also NULL char*, and also it genrates
//  commandLiner error "no files!" then.
const char* matchEntry(struct dirent* ent, ThereAreFourCharPointer* parts)
{
    if( !ent ) { setErrorText("no files!");
        return NULL; }
    else {
        int ok = true;
        bool hasExtension = hasOption('E');
        char* curEnt[2];
        if( ent->d_name[0]=='.' && (ent->d_name[1]=='\0' || ent->d_name[1]=='.') )
            ok = false; // this filters out '.' and '..' entries
        else {
            // split current entry into 'name' and 'extension' parts
            curEnt[0] = &ent->d_name[0];
            curEnt[1] = &ent->d_name[0] + strlen(curEnt[0]);
            while( (*curEnt[1]!='.') && (curEnt[1]!=curEnt[0]) ) curEnt[1]--;
            if( curEnt[0] == curEnt[1] ) {
                curEnt[1] = NULL;
                if( hasExtension ) // filter out any entries without extensions if current
                    ok = false;   // search may for some name which got extension.
            } else if( !hasExtension ) {
                ok = false; // filter out any entries with extensions if currently
            } else {       // searching for some name without extension
                *curEnt[1] = '\0';
            }
        }

        if(ok) { // build a 'merged' name-part string:
            char buN[2*MAX_NAM_LEN] = {NULL};
            int l = (parts->second-parts->first)-1;
            for(int i=0;i<l;i++)
                buN[i]=parts->first[i];
            int end = strlen(curEnt[0])-strlen(parts->second);
            for(int i=l;i<end;i++)
                buN[i]=curEnt[0][i];
            int t = end;
            end = strlen(curEnt[0]);
            for(int i=t;i<end;i++)
                buN[i]=parts->second[i-t];
            buN[end]='\0';
            char* buE = &buN[MAX_NAM_LEN+MAX_NAM_LEN/2];

            if(curEnt[1]&&hasExtension)
           {// build a 'merged' extension-part string..
            *curEnt[1]='.';
            l = (parts->fourth-parts->third);//-1;
            for(int i=0;i<l;i++)
                buE[i]=parts->third[i];
            end = strlen(curEnt[1])-strlen(parts->fourth);
            for(int i=l;i<end;i++)
                buE[i]=curEnt[1][i];
            t = end;
            end = strlen(curEnt[1]);
            for(int i=t;i<end;i++)
                buE[i]=parts->fourth[i-t];
            buE[end]='\0';
           }

            // check if 'merged' strings would match:
            int checkN = 0; while(buN[checkN]==curEnt[0][checkN]) checkN++;
            checkN -= strlen(&buN[0]);
            int checkE = 0;
            if(checkN>=0) { // write found entry into the stringCycle...
                if(hasExtension) {
                    while(buE[checkE]==curEnt[1][checkE]) checkE++;
                    checkE -= strlen(&buE[0]);
                }
                if (checkE>=0) {
                    int l = strlen(strcat(strcpy(&buN[0],getName('P')),&ent->d_name[0]));
                    buN[l++]='\n';
                    buN[l]='\0';
                    return pool_set(&buN[0]);
                }
            }  // or return NULL if not matching...
        } return NULL;
    }
}

// split given directory entry string into
// 'path', 'name' and 'extension' parts.
// returns true on files, false on directory names
//
// valid arguments are:
// - filename in the current work directory containing '*'s
// - full pathname containing '*' in the '/name.ext' part.
//
// stores each part by command line options 'P','N' and 'E'
int fs_PNE( const char* path )
{
    char* pathName = (char*)path;
    int isPath = false;
    int hasExt = false;
    int i = EMPTY;  while(pathName[++i]!='\0') {
        if(pathName[i]=='/')
            isPath = true;
        if(pathName[i]=='.'&&i)
            if(pathName[i-1]!='/')
                hasExt = true;
    }
    if(hasOption('x')) {
        pathName[i]=" "; pathName[++i]='\0';
    }
    if(hasExt) {
        char spacer[4] = {' ',' ',' ','\0'};
        int extender = i;
        while(pathName[i]!='.'&&i>=0)i--;
        extender -= i;
        extender = extender < 4 ? extender : 3;
        setTemp( &pathName[i] );
        setOption('E',strcat( getTemp(), &spacer[3-(4-extender)] ));
        pathName[i]='\0';
    } else {
        noOption('E');
    }
    if(isPath) {
        while(pathName[--i]!='/');
        setOption('N',&pathName[++i]);
        pathName[i]='\0';
        setOption('P',&pathName[0]);
        pathName[i]=getName('N')[0];
    } else {
        DEBUGLOG("using path of cwd!")
        tempf("%s/",fs_CD());
        setOption('P',getTemp());
        DEBUGFMT("path is: %s",path)
        DEBUGFMT("&pathName[0] is: %s",&pathName[0])
        if( isEmptyArg(path) ) {
            if( !hasOption('*') ) {
                setOption('N',"*");
                setOption('E',".*");
            } else
                setOption('N',"");
        } else
            setOption('N',&pathName[0]);
    } i--;
    while(pathName[++i]!='\0');
    if(hasExt) pathName[i] = getName('E')[0];
    return !isPath;
}

NameSplit splitAtStars(char* Name, char* Extension)
{
    DEBUGFMT("enter name: %s",Name)
    DEBUGFMT("enter extension: %s",Extension)

    NameSplit parts = { Name, NULL, Extension, NULL };

    // split the 'name' part at '*' sign,
    while(*parts.array[0]!='\0') {
        if(*parts.array[0]=='*') {
           *parts.array[0]='\0';
            parts.array[1]=parts.array[0]+1;
        } parts.array[0]++;
    } parts.array[0]=Name;
    if(!parts.array[1])
        parts.array[1] = (parts.array[0] + strlen(parts.array[0]));

    // split the 'extension' part at '*'...
    while(*parts.four.third!='\0') {
        if(*parts.four.third=='*') {
           *parts.four.third='\0';
            parts.four.fourth=parts.four.third+1;
        } parts.four.third++;
    } parts.four.third=Extension;
    if(!parts.four.fourth)
        parts.four.fourth = parts.four.third + strlen(parts.four.third);

#if DEBUG>0
    printf( "%s(): results:\nnameA: %s\nnameB: %s\nextA: %s\nextB: %s\n",__FUNCTION__,
            parts.array[0],parts.array[1],parts.four.third,parts.four.fourth );
#endif
    return parts;
}

// resolve '*' signs from given wildcard and returns first matching entry
const char* firstMatchFromPNE(void)
{
                DIR* dir = opendir( getName('P') );
    NameSplit split = splitAtStars( getName('N'),
                                    getName('E') );
    if( isSwitch('p') )
        noOption('P');

    pool_set("no match!");
    while( !matchEntry( readdir(dir), &split.four) ) {
        if( catchError("no files!") )
            break;
    } return pool_get();
}

// resolve '*' signs from given wildcard and
// writes found matches separated by '\n's
// into the given output stream 'outStream'
void filesListFromPNE(void* outStream)
{
    DEBUGLOG("enter")

                DIR* dir = opendir( getName('P') );
    NameSplit split = splitAtStars( getName('N') ,
                                    getName('E') );

    struct dirent* ent = readdir( dir );
    if( !ent ) {
        setErrorText("no files!");
        fwrite( getError(), 1, strlen(getError()), outStream );
        return;
    }

    char* output;
    if( !pool_setCheckpoint() ) {
        setErrorText("increase string pool size!");
    } ExitOnError("buffer");
    printf("\n");

    if( hasOption('c') ) {
        do{ const char* searchMatch = matchEntry( ent, &split.four );
        if( searchMatch ) {
            const char* fitsDestination = fs_fitDestinationPattern( getName('c'), searchMatch );
            printf( "copied %i byte from '%s' to '%s'\n",
                    fs_fileCopy( searchMatch, fitsDestination ),
                                 searchMatch, fitsDestination );
            }
        } while( ent = readdir(dir) );
    } else {
        DEBUGLOG("writing found entries to outputstream")
        if( isSwitch('p') )
            noOption('P');
        do matchEntry( ent, &split.four );
        while( ent = readdir(dir) );
        int dbgcycpsht = pool_cyclesPushed();
        DEBUGFMT("%i new pools have been pushed!",dbgcycpsht)
        if ( dbgcycpsht > 1 ) {
            DEBUGLOG("several pools have been pushed")
            while( output = pool_collectCheckpoint() )
                fwrite( output, 1, strlen(output), (FILE*)outStream );
            pool_freeAllCycles();
        } else {
            DEBUGLOG("einer ist keiner!,... no pools pushed")
            output = pool_collectCheckpoint();
            fwrite( output, 1, strlen(output), (FILE*)outStream );
        }
    }
}

const char* fs_getFirstMatch( const char* wildcard )
{
    fs_PNE( wildcard );
    return firstMatchFromPNE();
}

// generate a match list for given wildcard
const char* fs_getAllMatches( const char* wildcard )
{
    fs_PNE( wildcard );
    filesListFromPNE( stdout );
    setTemp("\n");
    return getTemp();
}

void setConfig(const char* key,const char* val)
{
    int number = 0;
    if(!strcmp(key,"buffer")) {
        setMacroNum("CYCLE_SIZE",atoi(val));
        printf("set buffersize: %s\n",val);
    }
    if(!strcmp(key,"slices")) {
        setMacroNum("CYCLE_COUNT",atoi(val));
        printf("set slicecount: %s\n",val);
    }
    if(!strcmp(key,"extlen")) {
        setMacroNum("MAX_EXT_LEN",atoi(val));
        printf("set extension length: %s\n",val);
    }
    exit(EXIT_SUCCESS);
}

const char* fs_prepareDestinationPattern(const char* path)
{
    DEBUGFMT("enter: %s",path)
    PNE Dst; char funcBuf[MAX_NAM_LEN]; char* buf=&funcBuf[0];
    PNE* dst = (PNE*)PNE_Init(&Dst,path);
    if(dst->flags & IS_FILE) { DEBUGLOG("IS_FILE");
        const char* nam = dst->getName(dst);
        const char* ext = dst->getExte(dst);
        NameSplit split = splitAtStars(nam,ext);
        sprintf(buf,"%s%%s%s",split.array[0], split.array[1]);
        dst->setName(dst,buf);
        if (strlen(split.four.fourth)||strlen(split.four.third)) {
            sprintf(buf,"%s%%s%s",split.array[2], split.array[3]);
            dst->setExte(dst,buf);
        }
    } else if (dst->flags & IS_FOLDER) {
        DEBUGLOG("IS_FOLDER")
        strcpy(buf,dst->getPath(dst));
        strcat(buf,dst->getName(dst));
        DEBUGFMT("input pth: %s",buf)
        dst->setPath(dst,buf);
        dst->setName(dst,"%s");
        dst->setExte(dst,".%s");
    }
        DEBUGFMT("pth: %s",dst->getPath(dst))
        DEBUGFMT("nam: %s",dst->getName(dst))
        DEBUGFMT("ext: %s",dst->getExte(dst))
        pool_set( dst->getFull(dst) );
        DEBUGFMT("ret: %s",pool_get())
        return pool_get();
}

#ifdef fs
void resetPNE(void)
{
    noOption('P');
    noOption('N');
    noOption('E');
}
int _listmode=NULL;
char _noRawName1=false;
int do_ListMode(int listmode)
{
    #if DEBUG>0
        pool_setCheckpoint();
        if( listmode & IS_FOLDER )
            pool_set( "IS_FOLDER" );
        if( listmode & IS_FILE )
            pool_setf("%sIS_FILE",pool_slicesSinceCheckpoint()?"|":"");
        DEBUGFMT( "listmode=%s", pool_collectCheckpoint() )
    #endif

    if( isEmptyArg(rawName(1)) || _noRawName1 ) {
        _noRawName1=true;
        if( listmode & IS_FOLDER ) {
            DEBUGLOG("will list FOLDERS")
            pool_set("*");
        } else if( listmode & IS_FILE ) {
            DEBUGLOG("will list FILES")
            pool_setf( "%s/*.*", fs_CD() );
        }
    } else if( !_noRawName1 ) {
        DEBUGFMT("will list %s",rawName(1))
        pool_set( rawName(1) );
        listmode = false;
    }

    if( isSwitch('f') )
        setOption('f',pool_get());
    else
        setOption('d',pool_get());

    if( isModus("cpy") && listmode<3 ) {
        DEBUGLOG("COPY-MODUS")
        const char* destDef = rawName(2);
        printf("WILL COPY TO: %s\n",destDef);
        const char* preparedPattern = fs_prepareDestinationPattern( destDef );
        setOption('c', preparedPattern );
    }

    if( hasOption('d') ) {
        printf( fs_getAllMatches(getName('d')) );
        setSwitch('d');
    }
    else if(hasOption('f')) {
        printf( fs_getFirstMatch(getName('f')) );
        setSwitch('f');
    }
    resetPNE();
    return _listmode
           =listmode&IS_FOLDER
         ? (listmode&~IS_FOLDER)
          : listmode&IS_FILE
         ? (listmode&~IS_FILE)
          : false;
}


int main(int argc,char**argv)
{
    if(!InitCommandLiner(argc,argv)) {
        setSwitch('d');
    }

    if(isModus("lnk")) {
        char* fullPathnameO = fs_getFirstMatch(rawNext('l'));
        char* fullPathnameT = nextRaw();
        if(fullPathnameT==NoString) {
            pool_setf("lnk.cmd -show %s\n",fullPathnameO);
            printf("will execute: %s\n",fullPathnameO=pool_get());
            system(fullPathnameO);
        } else {
            pool_setf("lnk.cmd %s ",fullPathnameO);
            pool_set(fullPathnameT);
            printf("will execute: %s\n",fullPathnameT=pool_merge(2));
            system(fullPathnameT);
        } exit(CheckForError());
    }

    // everything which was happening in PNE before 'p' check used full path names

    if(hasOption('p'))
        noOption('p');
    else
        setSwitch('p');

    // evrything below now is handled as cwd local 'file.ext' file or directory name


    if(isModus("set"))
        setConfig(rawNext('s'),nextRaw());

    _listmode = hasOption('*') ? IS_FOLDER
              : hasOption('.') ? IS_FILE
              : IS_FILE|IS_FOLDER;

    if( hasOption('v') )
        showOptions();
    if( hasOption('h') )
        exit( USAGE(fs) );

    while( do_ListMode(_listmode) );
    printf("\n");//%c",(char)0x04);

    exit( CheckForError() );
}
#endif