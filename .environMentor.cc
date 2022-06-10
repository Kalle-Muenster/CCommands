/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      environMentor.cc                           ||
||     Author:    Kalle                                      ||
||     Generated: by ClassFileGenerator v0.0                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
#include "settings.h"
#include ".environMentor.h"
#endif

// regular includes here:
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <memory.h>

#ifndef  NUMBER_OF_FEATURES
 #define NUMBER_OF_FEATURES (0)
  const static char* DUMMY[MIN_ARR_SIZ];
 #define FEATURE_NAMES DUMMY
#endif
#ifndef  NUMBER_OF_MACROS
 #define NUMBER_OF_MACROS (0)
  #if    NUMBER_OF_FEATURES > 0
   const static char* DUMMY[MIN_ARR_SIZ];
  #endif
 #define MACRO_NAMES DUMMY
#endif

#define ENV_BUFFER ((NUMBER_OF_MACROS+2) * MAX_NAM_LEN)
#define MOST_PER_REFLECTOR ((NUMBER_OF_MACROS > NUMBER_OF_FEATURES) \
                           ? NUMBER_OF_MACROS : NUMBER_OF_FEATURES)

#define GetReflectorSize(scopi) scopi==0 ? NUMBER_OF_MACROS \
                              : scopi==1 ? NUMBER_OF_FEATURES \
                                         : EMPTY

MentorState mentor = {
    (short)0, (short)0, 0,
    {(char)0},{(char)0},DISABLED, //{},DISABLED,
    {NULL},{NULL}
};

char**      getReflector(int);
const char* reflect(const char*);

int hasFeatures(void)
{
   #ifdef _OnTheFly_
     return !strcmp( getNameOfTheCommander(), _OnTheFly_ );
   #else
    return false;
   #endif
}

void InitializeEnvironMentor(void)
{
    sprintf( &mentor.globalsFile[0], "%s/persistente/Macros.h", LOCUS_POCUS );
    sprintf( &mentor.featureFile[0],"%s/persistente/%s_features.h",
             LOCUS_POCUS, commanderUseExtension(false) );
    mentor.isInitialized = true;
}

const char* reflect(const char* macro)
{
    while( *(macro++) );
    return macro;
}

void showPersistente(REFLECTOR whichOne)
{
    if(!mentor.isInitialized)
        InitializeEnvironMentor();
#if DEBUG
    printf(" number of features: %i\n", NUMBER_OF_FEATURES );
#endif
    if( hasFeatures() && ( whichOne==LOCAL || whichOne==DISABLED ) ) {
        printf( "Features of Command: %s\n\n",getNameOfTheCommander());
        for( int i=0; i<NUMBER_OF_FEATURES; ++i )
            printf( "%i - %s %s\n", i, FEATURE_NAMES[i],
                    reflect(FEATURE_NAMES[i]) );
    printf("\n\n"); }

    if( whichOne==GLOBAL || whichOne==DISABLED ) {
        printf( "Macros defined in Macros.h:\n\n" );
        for( int i=0; i<NUMBER_OF_MACROS; ++i )
            printf( "%i - %s %s\n", i, MACRO_NAMES[i],
                    reflect(MACRO_NAMES[i]) );
    printf("\n\n"); }
#if DEBUG
    whichOne = DISABLED;
#endif
    if( whichOne == DISABLED ) {
        printf("Macros defined in CommandLinerSettings.h:\n\n"
               "MAX_NAM_LEN=%i\nMAX_NUM_GUM=%i\nARGUM_SLASHING=%s\n\n",
               MAX_NAM_LEN, MAX_NUM_GUM, ARGUM_SLASHING ?
                                               "ENABLED":"DISABLED" );
    printf("\n\n"); }
}

////////////////////////////////////////////////////////////////
// searching through reflectors:...



int indexOfMacro( const char* name )
{
    for(int i=0; i < NUMBER_OF_MACROS ; ++i)
        if( stringCompare(name,MACRO_NAMES[i]) == 0 )
            return i;
    return NUMBER_OF_MACROS;
}

int indexOfFeature( const char* name )
{
    for(int i=0; i < NUMBER_OF_FEATURES; i++)
        if( stringCompare(name,FEATURE_NAMES[i]) == 0 )
            return i;
    return NUMBER_OF_FEATURES;
}

uint isDefined( const char* name )
{
    uint index = 0;
    if( (index = indexOfMacro(name)) != NUMBER_OF_MACROS ) {
        return index + 1;
    } else if((index = indexOfFeature(name)) != NUMBER_OF_FEATURES ) {
        return ((index+1)<<16);
    } return 0;
}

uint isReflected( REFLECTOR where, const char* macronym )
{
    uint idx;
    switch(where)
    {
case GLOBAL:
        return ((idx = indexOfMacro(macronym)) < NUMBER_OF_MACROS)
             ? idx+1 : false;
case LOCAL:
        return ((idx = indexOfFeature(macronym)) < NUMBER_OF_FEATURES)
             ? idx+1 : false;
case ANY:
        return isDefined(macronym);
    default: return false;
    }
}

const char* getReflected( REFLECTOR where, const char* macronym )
{
    uint idx;
    switch(where)
    {
case GLOBAL:
        return ((idx = indexOfMacro(macronym)) < NUMBER_OF_MACROS)
             ? reflect(MACRO_NAMES[idx]) : NoString;
case LOCAL:
        return ((idx = indexOfFeature(macronym)) < NUMBER_OF_FEATURES)
             ? reflect(FEATURE_NAMES[idx]) : NoString;
case ANY:
        if( idx = isDefined(macronym) ) {
            if( idx & 0x0000ffff ) {
                return reflect(MACRO_NAMES[idx-1]);
            } else {
                return reflect(FEATURE_NAMES[(idx>>16)-1]);
            }
        }
    } return NoString;
}

char* makePrefix( char* anyCase )
{
    char* toUpper = anyCase;
    do { char c = *anyCase;
              c = ( c >= 'a' && c <= 'z' )
                ? c - 32
                : ( c == '.' )
                ? '\0' : c;
            if(*anyCase = c)
                anyCase++;
    } while ( *anyCase );
    return toUpper;
}

int checkForPrefix( char* macronym )
{
    char* prefix = getNameOfTheCommander();
    MakeArray( char, workbuffer, strlen(prefix) + strlen(macronym) + 2 )
    prefix = makePrefix( strcpy( workbuffer, prefix ) );
    int exists = isDefined( macronym );
    if( exists == false ) {
        if ( stringCompare( macronym, prefix ) < strlen( prefix ) ) {
            strcat( prefix, "_" );
            strcat( prefix, macronym );
            strcpy( macronym, prefix );
        exists = isDefined( macronym ); }
    }
    if( exists ) {
        exists = (exists > 0x0000ffff) ? LOCAL : GLOBAL;
    }
    CleanArray( workbuffer )
    return exists;
}

const char* getPrefixed( REFLECTOR inscope, const char* macronym )
{
    const char* val = getReflected( inscope, macronym );
    if( isEmptyArg( val ) ) {
        char prfxbuffer[MAX_NAM_LEN];
        strcpy( &prfxbuffer[0], getNameOfTheCommander() );
        char* toUpper = makePrefix( &prfxbuffer[0] );
        strcat( &prfxbuffer[0], "_" );
        strcat( &prfxbuffer[0], macronym );
        if( isEmptyArg( val = getReflected( inscope, &prfxbuffer[0] ) ) )
            return NULL;
    } return val;
}

int setPrefixed( const char* macronym, const char* macroexpr )
{
    char* prefixed = setOption( '_', macronym );
    int exists = checkForPrefix( prefixed );
    if( exists )
        exists = (exists > 0x0000ffff) ? LOCAL : GLOBAL;
    int addprefix = stringCompare( prefixed, macronym );
    if( addprefix ) macronym = prefixed;
    if( nonCommittedChanges() ) {
        if( exists && (exists != mentor.scope) )
            removeMacro( macronym );
        setPersistEntry( macronym, macroexpr );
    } else
        setMacroRaw( macronym, macroexpr );
    noOption( '_' );
    return addprefix;
}

const char* getVariable( const char* name )
{
    int has = hasVariable( name );
    if( has-- )
    return mentor.scope == DISABLED
         ? getenv(name) : reflect(
           mentor.scope == GLOBAL
         ? MACRO_NAMES[has]
         : FEATURE_NAMES[has] );
    else return NoString;
}

int hasVariable(const char* name)
{
    #if DEBUG > 0
printf("checking for envar %s...\n",name);
    #endif
    uint foundInEnv = (uint)getenv(name);
    if( foundInEnv ) {
        mentor.scope = DISABLED;
        return NUMBER_OF_MACROS+1;
    }
    #if DEBUG > 0
printf("checking for macro %s...\n",name);
    #endif
    for(int i=0;i<NUMBER_OF_MACROS;i++)
        if(!strcmp(name,MACRO_NAMES[i])) {
            mentor.scope = GLOBAL;
            return i+1;
        }
    #if DEBUG > 0
printf("checking for feature %s...\n",name);
    #endif
    if( hasFeatures() )
        for(int i=0;i<NUMBER_OF_FEATURES;i++)
            if(!strcmp(name,FEATURE_NAMES[i])) {
                mentor.scope = LOCAL;
                return i+1;
            }
    #if DEBUG > 0
printf("%s not found at all...\n",name);
    #endif
    return 0;
}

FeatureGet getFeatured( const char* name )
{
    const char* varval;
    uint state = mentor.scope;
    FeatureGet result;
    varval = getVariable( name );
    result.name = name;
    if (varval == NoString) {
		varval = getPrefixed(LOCAL,name);
	}
	if (varval == NULL) {
		varval = getPrefixed(GLOBAL,name);
	} 
	if (varval == NULL) {
        result.type.flags = Type_NOTHIN;
        result.value.string = NoString;
        setErrorText( name );
        return result;
    }
    result.type.flags = mentor.scope!=DISABLED
                      ? mentor.scope==GLOBAL
                      ? Type_MACROG : Type_MACROL
                      : Type_ENVVAR;
    result.type.size[1] = 0;
    uint minlen = strlen( varval );
    MakeArray( char, work, minlen*2 + 2 );
    if( result.type.flags&Type_ENVVAR ) {
        sprintf( &work[0],"\"%s\"",varval);
        varval = &work[0];
    } else {
        varval = strcpy( &work[0], varval);
    } result.value.string = varval;
    char* workA = &work[0];
    char* workB = &work[minlen+1];
    char workF = 'A';
    if( !isEmptyArg( varval ) ) {
        while ( *varval == '\"' ) {
            int l=strlen(varval);
            if( varval[l-1]=='\"' ) {
                result.type.size[0] = (unsigned short)( (result.type.flags&Type_STRING)
                                                      ? ((result.type.flags&~Type_STRING)|Type_PREPRO)
                                                      : (result.type.flags|Type_STRING) );
                char* t = &varval[1];
                t[l-2]='\0';
                varval = strcpy(workF=='A'?workB:workA,t);
                workF = workF=='A' ? 'B':'A';
            } else {
                result.type.size[0] = (unsigned short)( (result.type.flags&Type_STRING)
                                                      ? ((result.type.flags&~Type_STRING)|Type_PREPRO)
                                                      : (result.type.flags|Type_ESCAPE) );
                varval = strcpy(workF=='A'?workB:workA,varval);
                workF = workF=='A' ? 'B':'A';
            }
        }
        if ( *varval == '(' ) {
            char* t = strcpy(workF=='A'?workB:workA,&varval[1]);
            workF = workF=='A' ? 'B':'A';
            t[strlen(t)-1]='\0';
            result.type.flags = (result.type.flags|Type_NUMBER);
        #if DEBUG > 0
            printf( "Found feature of: Type_NUMBER\n" );
        #endif
            const char* end = (const char*)t+strlen(t);
            result.value.number = strtod(t,&end);
        #if DEBUG > 0
            printf("...which has value: %f\n", result.value.number);
        #endif
        } else
        if( result.type.flags & Type_STRING ) {
            char* t = varval-1;
            char sep = result.type.flags&Type_ENVVAR?WINDOWS?';':':':',';
            while( *++t ) {
                if(*t == sep) {
                    result.type.size[0] = (unsigned short)((unsigned)result.type.size[0]|Type_LISTER);
                    result.type.size[1] = (unsigned short)sep;
              break; }
            } varval = strcpy(workF=='A'?workB:workA,varval);
               workF = workF=='A' ? 'B':'A';
        } if( result.type.flags & Type_LISTER ) {
        #if DEBUG > 0
            printf( "Found feature of: Type_LISTER\n" );
        #endif
            result.type.size[0] = (unsigned short)((unsigned)result.type.size[0] &~ Type_STRING);
            char* elms = strcpy(workF=='A'?workB:workA,&varval[0]);
        #if DEBUG > 0
            printf( "...which has value: %s\n", elms );
        #endif
            workF = workF=='A' ? 'B':'A';
            int cbSize = strlen(elms);
            int guests = toSplitList( elms, (char*)&result.type.size[1] );
            const uint nameLength = strlen(result.name);
            char* transfaere = (char*)junk_allocateJunkChunk(
                   2+cbSize+sizeof(ptval)*guests + nameLength);
            result.name = (const char*)strcpy( transfaere, result.name );
            transfaere += (nameLength+1);
            char** faerenSeats = (char**)transfaere;
            char* faerenGuests = transfaere + ( sizeof(ptval) * guests );
            memccpy( &faerenGuests[0], &elms[0], 1, cbSize );
            faerenGuests[cbSize] = '\0';
            --faerenGuests;
            for( int g = 0; g < guests; ++g ) {
                faerenSeats[g] = ++faerenGuests;
                while( *++faerenGuests );
            } result.type.size[1] = (word)guests;
            result.value.lister = faerenSeats;
        } else if( result.type.flags & Type_STRING ) {
        #if DEBUG > 0
            printf( "Found feature of: Type_STRING\n" );
        #endif
            result.type.size[1] = (unsigned short)strlen(&varval[0]);
        #if DEBUG > 0
            printf( "...which has value: %s\n", &varval[0] );
        #endif
            const uint nameLength = strlen( result.name );
            result.value.string = (char*)junk_allocateJunkChunk(
                           result.type.size[1] + nameLength + 2 );
	
            result.name = (const char*)strcpy(
			 result.value.string, result.name );
	
            result.value.string += (nameLength+1);
            strcpy( result.value.string, &varval[0] );
        }
    } else {
        setErrorText( "feature" );
        result.value.string = NoString;
        result.type.flags = Type_NOTHIN;
    }
	CleanArray( work );
    mentor.scope = state;
    return result;
}

int setFeatured( FeatureGet* store )
{
    if ( store->type.flags ) {
               if (store->type.flags&Type_NUMBER) {
            setPersistEntry( store->name, setTempf( "(%d)", (char*)(ptval)*(ulong*)&store->value.number ) );
        } else if (store->type.flags&Type_STRING) {
            setPersistEntry( store->name, toQuoted( store->value.string ) );
        } else if (store->type.flags&Type_LISTER) {
            char fromto[] = {'\0',SEPPEL,'\0'};
            toSplitList( store->value.lister, &fromto[0] );
            setPersistEntry( store->name, toQuoted( *store->value.lister ) );
        } else if (store->type.flags&Type_PREPRO) {
            setPersistEntry( store->name, unQuoted( store->value.string ) );
        }
    }
}

int setFeaturedList( const char* feature, char* values )
{
    char sep[2] = {(SYSTEM==WIN ? ';' : ':'),','};
    if( toSplitList( values, sep ) ) {
        setPrefixed( feature, setTempf("\"%s\"",values) );
    } else if ( checkForPrefix( setTemp(feature) ) ) {
        if ( nonCommittedChanges() )
            removePersistEntry( getTemp() );
        else removeMacro( getTemp() );
    } return true;
}

int addFeaturedList( const char* feature, char* value )
{
    int len = strlen(value);
    const char* macroval = getPrefixed( ANY, feature );
    MakeArray( char, convertedValue, strlen(macroval) + len + 4 );
	char* setter = &convertedValue[0];
    *setter++ ='\"';
    if( macroval ) {
        ++macroval;
        len = strlen(macroval)-1;
        memcpy(setter,macroval,len);
        setter+=len;
        *setter++ = ',';
    } strcpy(setter,value);
    setter+= strlen(value);
    *setter++ ='\"';
    *setter = '\0';
    setPrefixed( feature, &convertedValue[0] );
	CleanArray( convertedValue );
    return true;
}

int remFeaturedList(const char* feature, char* value)
{
    checkForPrefix( setOption('_',feature) );
    FeatureGet fet = getFeatured( getName('_') );
    if ( fet.type.flags & Type_STRING ) {
        if( !stringCompare( fet.value.string, value ) ) {
            if ( nonCommittedChanges() )
                removePersistEntry( fet.name );
            else removeMacro( fet.name );
        } else setErrorText(setTempf("feature %s not is a 'list'",fet.name));
    } else if ( fet.type.flags & Type_LISTER ) {
        int len = strlen(value);
        MakeArray( char, convertedValue, (fet.type.size[0]-len) + 2 );
        char* setter = &convertedValue[0];
        *setter++ = '\"';
        int wasEntryBefore = false;
        for( int i = 0; i < fet.type.size[1]; ++i ) {
            const char* entry = fet.value.lister[i];
            if( stringCompare( entry, value ) ) {
                int ents = strlen(entry);
                if ( ents ) {
                    if ( wasEntryBefore ) *setter++ = ',';
                    wasEntryBefore = true;
                    strcpy(setter,entry);
                    setter += ents; }
            } else printf("Removed '%s' entry: %s",fet.name,entry);
        } *setter++ = '\"';
        if( nonCommittedChanges() ) {
            if( wasEntryBefore ) {
                setPersistEntry( fet.name, &convertedValue[0] );
            } else {
                removePersistEntry( fet.name );
            }
        } else {
            if( wasEntryBefore ) {
                setMacroRaw( fet.name, &convertedValue[0] );
            } else {
                removeMacro( fet.name );
            }
        } CleanArray( convertedValue );
    } noOption('_');
    return true;
}

int listedFeature( unsigned listOperation, const char* listName, char* setValue )
{
    switch( listOperation ) {
        case SET_AS_LIST:
            return setFeaturedList( listName, setValue );
        case ADD_TO_LIST:
            return addFeaturedList( listName, setValue );
        case REMOVE_FROM:
            return remFeaturedList( listName, setValue );
        default: {
            setErrorText( "List Operation" );
            return false;
        }
    }
}

// sets a variable "name" in the environment to given "value".
void setVariable(const char* name,const char* value)
{
    char buffer[MAX_NAM_LEN*2];
    sprintf(&buffer[0],"%s=%s",name,value);
    putenv(&buffer[0]);
}

/////////////////////////////////////////////////////////////////
//writing stuff...

void writeNamesOfMacros(FILE* f, const char* commandName,
                         char** names, int number)
{
    const char * Typus = (commandName[0]=='\0')
                       ? "MACRO"
                       : "FEATURE";

    DEBUGFMT("writing %s reflector!",Typus)

    fprintf(f,"///[%sReflector]\n", &commandName[0]);
    fprintf(f,"#define NUMBER_OF_%sS (%i)\n", Typus, number);
    fprintf(f,"const static int %s_NUMBER_OF_MACROS = NUMBER_OF_%sS;\n",
              commandName, Typus );
    fprintf(f,"const static char* %s_NAMES[NUMBER_OF_%sS] = {\n",
              Typus, Typus );
    number+=mentor.removalsCount;
    DEBUGLOG("entering loop!")
    for(int i=0;i<number;i++) {
        if(names[i][0]!='\0') {
            fprintf(f,"    \"%s\\0",names[i]);
        #if DEBUG>0
            printf( "%s(): will reflect macro: %s\n",
                   __FUNCTION__, names[i] );
        #endif
            char* value = (char*)reflect(names[i]);
            int end = strlen(value);
            int q = 0;
            if(*value=='\"') {
                 if( value[end-1]=="\"" ) { q = 1;
                     value[--end] = '\0';
                 }
            } for( int i=0; i<end; ++i ) {
                 if( value[i]=='\\' || value[i]=='\"' )
                     fwrite("\\",1,1,f);
                 fwrite(&value[i],1,1,f);
            } if ( q==1 )
                fprintf(f, "\\\"\",\n");
            else
                fprintf(f, "\",\n");
        }
    } fprintf(f,"};\n");
}

int entrySize( char* entry )
{
    int p = strlen(entry);
    p += (strlen(&entry[p+1])+2);
    return p;
}

void copyNameAndValue( char* dst, const char* srcName, const char* srcVal )
{
    strcpy( dst, srcName );
    strcpy( &dst[strlen(dst)+1], srcVal );
}

void cloneEntry( char* dst, const char*src )
{
    while(*dst++=*src++);
    while(*dst++=*src++);
}

char** allocateReflector( int newCount, uint maxOnReflector, const char** names )
{
    const int pointerSize = sizeof(char*);
    void* rawAllocated = malloc((newCount*pointerSize)+(newCount*MAX_NAM_LEN*2));
    char* data = (char*)((char*)rawAllocated + (newCount*pointerSize));
    char** newAllocated = (char**)rawAllocated;
    const uint stride = MAX_NAM_LEN+MAX_NAM_LEN;
    for( uint i=0; i<newCount; ++i ) {
        newAllocated[i] = &data[i*stride];
        if(i<maxOnReflector)
            cloneEntry( newAllocated[i], names[i] );
    } return newAllocated;
}

char** getReflector(int ID)
{ if(!mentor.isInitialized) InitializeEnvironMentor();
    if(mentor.reflector[ID]!=NULL) {
        return mentor.reflector[ID];
    }
    switch(ID) {
        case GLOBAL: {
            mentor.reflector[ID] = allocateReflector(
                NUMBER_OF_MACROS+5,
                NUMBER_OF_MACROS,
                MACRO_NAMES
            ); break;
        }
        case LOCAL: {
            if( hasFeatures() )
                mentor.reflector[ID] = allocateReflector(
                    NUMBER_OF_FEATURES+5,
                    NUMBER_OF_FEATURES,
                    FEATURE_NAMES
                );
            break;
        }
    }
    DEBUGFMT("will return reflector: %i",ID)
    return mentor.reflector[ID];
}

//----------------------------------------------------------------------------------------------------------------------

void writeMacroInternal(const char* name, const char* value,
                        const char* fileName, int count,
                        int index, const char** NAMES,
                        const char* commandName )
{
DEBUGFMT("beginn writing macro: %s",name);
DEBUGFMT("at index %i!",index);

    FILE* f = fopen(fileName,"w+");
   #if defined(ADD_NORMALISATOR)
    if(commandName[0]=='\0')
        fprintf(f,"#if NormalizeNamespace==1\n");
   #endif
   #if defined(__TINYC__)
    char NewBuffer[MAX_NAM_LEN*2*count];
    char* newOrder[count];
    char* newBuffer = &NewBuffer[0];
   #else
    char* newBuffer = (char*)malloc(MAX_NAM_LEN*2*count);
    char** newOrder = (char**)malloc(sizeof(char*)*count);
   #endif
    for(int l=0;l<count;l++)
    {
        newOrder[l] = &newBuffer[l*512];
        if(l==index)
            copyNameAndValue( newOrder[l], name, value );
        else
            cloneEntry( newOrder[l], NAMES[l] );

        fprintf( f, "#define %s %s\n", newOrder[l],
                 reflect( newOrder[l] ) );
    }

    writeNamesOfMacros(f,commandName,newOrder,count);
   #if defined(ADD_NORMALISATOR)
    if(commandName[0]=='\0')
        fprintf(f,"#undef  NormalizeNamespace\n"
                  "#undef  %s\n#define %s\n"
                  "#endif\n", commandName,
                  commandName );
   #endif
    fflush(f); fclose(f);
   #if !defined(__TINYC__)
    free(newBuffer);
    free(newOrder);
   #endif
}


//////////////////////////////////////////////////////
//functions for doing changes to included macro header
void setMacroRaw(const char* name, const char* value)
{
    if(!mentor.isInitialized) InitializeEnvironMentor();
    const char** Names;
    char* fileName;
    char* commandName;
    char isFeature = false;
    int count = NUMBER_OF_MACROS;
    int TheOne;
DEBUGFMT("Checking for Macro: %s",name);
    int index = TheOne = indexOfMacro(name);
    if( index == NUMBER_OF_MACROS ) {
        index = indexOfFeature(name);
        if( index == NUMBER_OF_FEATURES ) { // means:
            // 'name' does not yet exist at all...
            if( hasFeatures() && PREFER_LOCAL ) {
                isFeature = true;
                count = ( index = NUMBER_OF_FEATURES ) + 1;
            } else {
                count = ( index = NUMBER_OF_MACROS ) + 1;
            }
        } else { isFeature = true;
            count = NUMBER_OF_FEATURES;
        }
    } if(isFeature) {
DEBUGFMT("Feature! : the \"One\" is = %i",TheOne);
        Names = FEATURE_NAMES;
        fileName = &mentor.featureFile[0];
        commandName = (char*)commanderUseExtension(false);
    } else {
        index = TheOne;
DEBUGFMT("Macro! : the \"One\" is = %i\n",TheOne);
        Names = MACRO_NAMES;
        fileName = &mentor.globalsFile[0];
        commandName = "";
    }
    remove(fileName);
DEBUGFMT("File %s removed!\n",fileName);
    writeMacroInternal( name, value, fileName,
                        count, index, Names,
                        commandName );
}

void setMacroNum(const char* name, int value)
{
    char buffer[18];
    sprintf( &buffer[0],"(%i)",value );
    setMacroRaw( name, &buffer[0] );
}

void setMacroStr(const char* name, const char* value)
{
   #if defined(__TINYC__)
    char buffer[strlen(value)+4];
   #else
    char* buffer = (char*)malloc(strlen(value)+4);
   #endif
    setMacroRaw( name, strcpy( &buffer[0], toQuoted(value) ) );
   #if !defined(__TINYC__)
    free(buffer);
   #endif
}

void removeMacro(const char* name)
{
    DEBUGLOG("Begin removeMacro(MacroName):");
    uint where;
    if( where = isDefined(name) )
    {
        char isFeature = (where & 0xffff0000)?1:0;
        int index = ( isFeature ?
                      where>>16 : where ) -1;

        char* fileName = isFeature
                       ? &mentor.featureFile[0]
                       : &mentor.globalsFile[0];

        int THE_NUMBER = isFeature
                   ? NUMBER_OF_FEATURES
                   : NUMBER_OF_MACROS;

        char** Names = isFeature
                     ? (char**)FEATURE_NAMES
                     : (char**)MACRO_NAMES;

        remove( fileName );
    DEBUGFMT("File %s removed!\n",fileName);
        FILE* f = fopen(fileName,"w+");
       #if defined(ADD_NORMALISATOR)
        if(isFeature)
            fprintf(f,"#if NormalizeNamespace==1\n");
       #endif
        for( short i=0; i<THE_NUMBER; i++ )
            if( i != index )
                fprintf( f, "#define %s %s\n", Names[i],
                         reflect( Names[i] ) );
            else
                copyNameAndValue( Names[i], "\0", "\0" );

        char* commandername = isFeature
                            ? (char*)commanderUseExtension(false)
                            : (char*)"\0";

        mentor.removalsCount = 1;
        writeNamesOfMacros(f, isFeature ?
                              commandername
                         :"", Names,
                              THE_NUMBER - 1 );
       #if defined(ADD_NORMALISATOR)
        if(isFeature)
            fprintf(f,"#else\n#undef NormalizeNamespace\n"
                      "#undef %s\n#define %s\n#endif\n",
                      commandername, commandername );
       #endif
        fflush(f);
        fclose(f);
    }
}


///////////////////////////////////////////////////////
// functions for enabling, setting up and initializing
// header files for commands, so they can include these
// for using reflected feature macros.
void enableFeaturesForCommander(void);
void setCommandoFeature(const char*,const char*);

void setLocalMacro(char* featureString)
{
    char *commandName,
         *featureName,
         *featureValue;

    if(!hasFeatures())
        enableFeaturesForCommander();

    short c=0;
    short len = strlen(featureString);
    featureString[len]=' ';
    commandName = &featureString[0];
    while(featureString[c++]!=' ');
    commandName[c-1]='\0';
    featureName = &featureString[c];
    int n = 0;
    while(featureString[++n + c]!=' ');
    featureName[n]='\0';
    if((c+=n)>=len) {
        featureValue = featureName;
        featureName = commandName;
        commandName = (char*)getNameOfTheCommander();
        commandName[strlen(commandName)-2]='\0';
    } else {
        featureValue = &featureString[c+1];
        featureValue[len]='\0';}

    sprintf(&mentor.featureFile[0],
            "%s/persistente/%s_features.h",
            getPathOfTheCommander(),
            commandName );

    FILE* f=fopen(&mentor.featureFile[0],"w");

    featureName[strlen(featureName)]=' ';

    setCommandoFeature(commandName,featureName);
}

void setCommandoFeature( const char* commandName,
                         const char* feature )
{
    char fName[MAX_NAM_LEN];
    sprintf( &fName[0],"%s/persistente/%s_features.h",
             getPathOfTheCommander(), commandName );

    remove(&fName[0]);

    char Feature[512];
    strcpy(&Feature[0],feature);
    int c = 0;
    while(Feature[++c]!=' ');
    Feature[c]='\0';
    char* val = &Feature[c+1];

    short index = indexOfFeature(&Feature[0]);

        int count = index==NUMBER_OF_FEATURES
                         ? NUMBER_OF_FEATURES + 1
                         : NUMBER_OF_FEATURES;

    writeMacroInternal( &Feature[0], val, &fName[0],
                        count, index, FEATURE_NAMES,
                        commandName );
}

void enableFeaturesForCommand(const char* commandName)
{
    char fName[MAX_NAM_LEN];
    sprintf( &fName[0],
             "%s/persistente/CommandoFeatures.h",
             LOCUS_POCUS );
    DEBUGLOG("will open CommandoFeatures.h....");
    FILE* f = fopen(&fName[0],"r");
    fseek(f,0,SEEK_END);
    int fSize = ftell(f);
    DEBUGFMT("filesize: %i",fSize);
    MakeArray( char, featureIncluder, fSize+512 );
    fseek(f,0,SEEK_SET);

    fread(&featureIncluder[0],1,fSize,f);
    featureIncluder[fSize-1]='\0';
    fflush(f);
    fclose(f);
    remove(fName);

    FILE* fo = fopen(&fName[0],"w+");
    fprintf(fo,"%s",&featureIncluder[0]);
    fprintf(fo,"\n#if defined IncludesFor_%s\n#undef %s\n",
              commandName,commandName );
    fprintf(fo, "#define %s \"%s\"\n#endif\n",
              commandName,commandName );
    fflush(fo);
    fclose(fo);

    sprintf( &fName[0],"%s/persistente/%s_features.h",
             LOCUS_POCUS, commandName );

    if(!(f=fopen(&fName[0],"r"))) {
        f=fopen(&fName[0],"w+");
        fprintf(f,"#define ENVIRONMENTIONED \"this is the %s teststring\"\n",commandName);
        fprintf(f,"#define NUMBER_OF_FEATURES (1)\n",commandName);
        fprintf(f,"static const int %s_NUMBER_OF_MACROS = NUMBER_OF_FEATURES;\n",commandName);
        fprintf(f,"static const char* FEATURE_NAMES[NUMBER_OF_FEATURES] = {\n",commandName);
        fprintf(f,"    \"ENVIRONMENTIONED\\0\\\"this is %s teststring\\\"\",\n",commandName);
        fprintf(f,"};\n");
    } fflush(f); fclose(f);
	CleanArray( featureIncluder );
}

void enableFeaturesForCommander(void)
{
    char buffer[MAX_NAM_LEN];
    strcpy( &buffer[0], commanderUseExtension(false) );
    enableFeaturesForCommand( &buffer[0] );
}

//////////////////////////////////////////////////////////////
//another approach for doing changes to macro include headers
//this works more saftly, but doesnt support changing feature
//headers for commands other then the currently executing one.
char beginPersistChange( REFLECTOR scope )
{
    DEBUGFMT("Beginning %s change!",scope==FEATURE?"FEATURE":scope==MACRO?"MACRO":"ANY");
    if(!mentor.isInitialized)
        InitializeEnvironMentor();
    if( mentor.scope == DISABLED ) {
        mentor.scope = scope;
    } else if( scope != mentor.scope )
        setErrorText("scope missmatch of uncommitted changes!");
    return (mentor.scope == scope);
}

void removePersistEntry( const char* name )
{
    DEBUGFMT("Removing Macro: %s ...",name)
    int index =
        mentor.scope==GLOBAL ? indexOfMacro(name)   :
        mentor.scope==LOCAL  ? indexOfFeature(name) : -1;
    copyNameAndValue( getReflector(mentor.scope)[index],
                      "\0", "\0" );
}

void setPersistEntry( const char* name, const char* value )
{
    DEBUGFMT("will set macro: '%s'",name)
    DEBUGFMT("   ...to value: '%s'",value)

    if (mentor.scope == DISABLED) {
        setErrorText("beginPersitChange() was not called before!");
        return;
    }
    char** features;
    uint reflidx = isDefined(name);
    if( reflidx ) {
        features = getReflector(mentor.scope);
        int featurIndex = ( mentor.scope==LOCAL ? (reflidx>>16) : reflidx ) -1;
        DEBUGFMT( "featurIndex: %i", featurIndex );
        char* destination = &features[featurIndex][0];
        DEBUGFMT( "write address: %p", destination );
        copyNameAndValue( destination, name, value );
    } else {
        DEBUGLOG( "not defined" )
        const int THE_NUMBER = mentor.scope==LOCAL
                         ? NUMBER_OF_FEATURES
                         : NUMBER_OF_MACROS;

        if(mentor.featuresAdd % 6 == 5) {
            char** tempreflector = allocateReflector (
                                       THE_NUMBER +mentor.featuresAdd + 5,
                                       THE_NUMBER +mentor.featuresAdd,
                                       mentor.reflector[mentor.scope]
                                   );
            free( mentor.reflector[mentor.scope] );
            mentor.reflector[mentor.scope] = tempreflector;
        }
        features = getReflector(mentor.scope);
        copyNameAndValue( mentor.reflector[mentor.scope][THE_NUMBER +mentor.featuresAdd],
                          name, value );
        ++mentor.featuresAdd;
    }
}

int nonCommittedChanges(void)
{
    return mentor.scope!=DISABLED;
}

void commitPersistChange(void)
{
    DEBUGFMT( "Committing '%s' persist changes!", mentor.scope==LOCAL?"LOCAL":"GLOBAL" )

    if(nonCommittedChanges())
    {
        char** features = getReflector(mentor.scope);
        const int count = mentor.scope==LOCAL
                        ? NUMBER_OF_FEATURES +mentor.featuresAdd
                        : NUMBER_OF_MACROS +mentor.featuresAdd;
        int r=0;
        remove(&mentor.featureFile[0]);
        FILE* f = fopen(&mentor.featureFile[0],"w+");

      #if defined(ADD_NORMALISATOR)
        if(mentor.scope==LOCAL)
            fprintf(f, "#ifndef NormalizeNamespace\n" );
      #endif
        for(int i=0;i<count;i++) {
            if(features[i][0]=='\0') {
              #if DEBUG>0
                printf( "%s(): removed %s...\n", __FUNCTION__,
                       mentor.scope==GLOBAL ? MACRO_NAMES[i]
                                      : FEATURE_NAMES[i] );
              #endif
                ++mentor.removalsCount;
            } else {
                char* value = reflect( features[i] );
              #if DEBUG>0
                printf( "writing %s=%s\n", features[i], value );
              #endif
                fprintf( f, "#define %s %s\n", features[i], value );
            }
        } writeNamesOfMacros( f, commanderUseExtension(false),
                              features, count - mentor.removalsCount );
      #if defined(ADD_NORMALISATOR)
        if(mentor.scope==LOCAL)
            fprintf( f, "#else\n#undef  %s\n#define %s\n"
                        "#undef  NormalizeNamespace\n"
                        "#endif\n", getNameOfTheCommander(),
                        getNameOfTheCommander() );
      #endif
        commanderUseExtension( !WINDOWS );
        fflush(f); fclose(f);
        free( features );
        mentor.scope = DISABLED;
    }
}

// checks if currently executing command includes
// an existing "features" header file (which defines
// a command's depending local macros + reflectors
// for these). If not, it creates the header file
// and adds an include statement to the currently
// executing .cc file. if this happens during commandLiner.h
// Initialisation, the command, when done, automatically
// will terminate and reload itself with header now included.
char environMentor_CheckAndEnable(void)
{
    char featuresAreEnabled = hasFeatures();
    #if DEBUG > 0
    if(!featuresAreEnabled)
        printf("features for %s not enabled yet!\n",
                commanderUseExtension( false ) );
    else
        printf("features for %s are enabled!\n",
               _OnTheFly_ );

    commanderUseExtension( !WINDOWS );
    #endif
    if(!featuresAreEnabled)
        enableFeaturesForCommander();
    return featuresAreEnabled;
}

void exportAll(void)
{
#if DEBUG > 0
printf("begin exporting...\n");
#endif

    if(!mentor.isInitialized)
        InitializeEnvironMentor();

#if DEBUG > 0
printf("... exporting macros...\n");
#endif
    for(int i=0;i<NUMBER_OF_MACROS;i++)
        setVariable( MACRO_NAMES[i],
                     unQuoted(reflect(MACRO_NAMES[i])) );
#if DEBUG > 0
printf("... exporting features...\n");
#endif
    if(hasFeatures())
        for(int i=0;i<NUMBER_OF_FEATURES;i++)
            setVariable( FEATURE_NAMES[i],
                         unQuoted(reflect(FEATURE_NAMES[i])) );

system( CONSOLA " \"echo.\"" );
}

void exportMacros(REFLECTOR scope)
{
#if DEBUG > 0
printf("begin exporting...\n");
#endif
    if(!mentor.isInitialized)
        InitializeEnvironMentor();

    if((scope==GLOBAL)||(scope==DISABLED)) {
#if DEBUG > 0
printf("... exporting macros...\n");
#endif
    for(int i=0;i<NUMBER_OF_MACROS;i++)
        setVariable( MACRO_NAMES[i],
                     unQuoted(reflect(MACRO_NAMES[i])) );
    }
    if((scope==LOCAL)||(scope==DISABLED)) {
#if DEBUG > 0
printf("... exporting features...\n");
#endif
    if(hasFeatures())
        for(int i=0;i<NUMBER_OF_FEATURES;i++)
            setVariable( FEATURE_NAMES[i],
                         unQuoted(reflect(FEATURE_NAMES[i])) );
    }
system( CONSOLA " \"echo.\"" );
}

/*
#define NormalizeNamespace (1)
#include COMMAND_DEPENDENT_HEADER
*/