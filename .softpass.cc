/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      softpass.cc                                ||
||     Author:    Autogenerated                              ||
||     Generated: 13.03.2022                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
  #include ".softpass.h"
#else
  #define WITH_STRGPOOL
  #include <.environMentor.h>

  int USAGE(softpass)
  {
    Synopsis("[option] <location> [password]");
    printf("Options:\n\n\
    --get : get the password for <location>\n\
    --add : add new <location> and set a <password> for it\n\
    --rem : remove stored <location> and the password for it\n\
    --lst : print a list of all stored locations\n\n");
    return CheckForError()
        || !hasOption('h');
  }
#endif

// regular includes here:
#include <stdlib.h>


// default values of featured macros

#ifndef SOFTPASS_CRYPTCOMMAND
#define SOFTPASS_CRYPTCOMMAND "crypt64"
#endif

#ifndef SOFTPASS_PASSWORDSDIR
#define SOFTPASS_PASSWORDSDIR ""
#endif

#ifndef SOFTPASS_MASTERPHRASE
#define SOFTPASS_MASTERPHRASE ""
#endif


// configuration functions for modifying featured macros

const char*
softpass_set_master( const char* oldkey, const char* newkey )
{
    cmBl valid = 0;
    if ( isEmptyArg( SOFTPASS_MASTERPHRASE ) ) {
        valid = 1;
    } else {
        valid = !stringCompare( SOFTPASS_MASTERPHRASE, oldkey );
    } if ( valid ) {
        setPrefixed( "MASTERPHRASE", newkey );
        pool_setf( "master password has been reset to: %s\n", newkey );
    } else {
        setError(
            pool_setf( "invalid phrase '%s'. master password not changed.\n", oldkey ),
            FourCC("pass")
        );
    } return pool_get();
}

const char*
softpass_set_folder( const char* phrase, const char* folder )
{
    if ( isEmptyArg( SOFTPASS_MASTERPHRASE ) ) {
        setError( pool_set("no master password set yet"), FourCC("pass") );
    } else {
        if( !stringCompare( SOFTPASS_MASTERPHRASE, phrase ) ) {
            setPrefixed( "PASSWORDSDIR", toQuoted( folder ) );
            pool_setf("storage location has been set to: %s\n", folder);
        } else setError(
            pool_setf( "invalid phrase '%s'. storage location not set.\n", phrase ),
            FourCC("pass")
        );
    } return pool_get();
}

const char*
softpass_set_crypte( const char* phrase, const char* crypte )
{
    if ( isEmptyArg( SOFTPASS_MASTERPHRASE ) ) {
        setError( pool_set("no master password set yet"), FourCC("pass") );
    } else {
        if( !stringCompare( SOFTPASS_MASTERPHRASE, phrase ) ) {
            setPrefixed( "CRYPTCOMMAND", toQuoted( crypte ) );
            pool_setf("crypt command has been set to: %s\n", crypte );
        } else setError(
            pool_setf( "invalid phrase '%s'. crypt command was not reset.\n", phrase ),
            FourCC("pass")
        );
    } return pool_get();

}


// commandline string generators for crypt calls

const char*
softpass_get_command( const char* xwort )
{
    return pool_setf( SOFTPASS_CRYPTCOMMAND " -d " SOFTPASS_PASSWORDSDIR "%s.txt.yps -o conout$ -k " SOFTPASS_MASTERPHRASE, xwort );
}

const char*
softpass_add_command( const char* name, const char* xwort )
{
    pool_setf( SOFTPASS_CRYPTCOMMAND " -t -e %s --o-", xwort );
    pool_setf( SOFTPASS_PASSWORDSDIR "%s.txt.yps -k " SOFTPASS_MASTERPHRASE, name );
    return pool_merge(2);
}

const char*
softpass_rem_command( const char* name )
{
    pool_setf( "del /f /s /q " SOFTPASS_PASSWORDSDIR "%s.txt.yps",name);
    pool_setf( ">nul&&echo removed pass '%s'", name );
    return pool_merge(2);
}

const char*
softpass_lst_command( void )
{
    return pool_set( "fs.c -. " SOFTPASS_PASSWORDSDIR "*.yps" );
}



/* api calls for retrieving password
#include <.crypt64.h>

const char*
softpass_get( const char* xort )
{
    crypt64_Initialize( 1 );
    MakeArray( char, filepath, strlen(SOFTPASS_PASSWORDSDIR)+strlen(xort)+9 );
    byte decrypselt[MAX_NAM_LEN];
    byte* dst = &decrypselt[0];

    K64F* file = crypt64_createFileStream( SOFTPASS_MASTERPHRASE, strcat( strcpy( &filepath[0], SOFTPASS_PASSWORDSDIR ), xort ), "rd" );
    k64Chunk frame;
    while ( !(frame = crypt64_getYps( file )).u8[3] ) {
        *dst = frame.u32;
        dst += 3;
    } *dst = '\0';
    junk_drop( file );
    return pool_set( &decrypselt[0] );
}

const char*
softpass_add( const char* ort, const char* xwort )
{
    system( softpass_add_command( ort, xwort ) );
    return pool_set( "done\n" );
}

const char*
softpass_rem( const char* ort )
{
    system( softpass_rem_command( ort ) );
    return pool_set( "done\n" );
}

const char*
softpass_lst( void )
{
    system( softpass_lst_command() );
    return pool_set( "done\n" );
}
*/

#ifdef softpass
int main( int argc, char**argv )
{
    if( !InitCommandLiner(argc,argv) ) {
        exit(USAGE(softpass));
    }

    if( !isAnyModusAtAll() ) {
        setModus( "get" );
    }

    if ( isEmptyArg( SOFTPASS_MASTERPHRASE ) ) {
        setError("no master passphrase set!", FourCC("pass") );
        exit(USAGE(softpass));
    }

    if ( isEmptyArg( SOFTPASS_PASSWORDSDIR ) ) {
        setError("storage location not set!", FourCC("dir") );
        exit(USAGE(softpass));
    }

    const char* command =
         isModus("get") ? softpass_get_command( rawName(1) )
       : isModus("add") ? softpass_add_command( rawName(1), rawName(2) )
       : isModus("rem") ? softpass_rem_command( rawName(1) )
       : isModus("lst") ? softpass_lst_command()
       : NoString;

    if ( isValidArg( command ) ) system( command );
    else setError( "modus", FourCC("mode") );

    exit( CheckForError() );
}
#endif