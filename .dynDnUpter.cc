/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      dynDnUpter.cc                              ||
||     Author:    Kalle                                      ||
||     Generated: 30.09.2017                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
  #include ".dynDnUpter.h"
#else
  #include <.commandLiner.h>
  #include <.stringPool.h>

  int USAGE(dynDnUpter)
  {
     if( isModus("verbose") ) {
         showOptions();
         showPersistente(LOCAL);
     }

    Synopsis("--n-<update-name> --<4|6>-<ip4-adress|ip6-adress>");
    printf("\nOptions:\n\n"
           "    --name, --n-<dns-target-name> :  set name of route endpoint\n"
           "    --ipv6, --6-<update ip6>      :  pass ip6 address up to date.\n"
           "    --ipv4, --4-<update ip4>      :  pass ip4 address up to date\n\n");
    printf("\nPersisted Option:\n\n"
           "    --setSocketier <path to http poster>\n"
           "                   set is: " DYN_UPDATE_SOCKETIER "\n"
           "    --setServiceUrl <url of DynDNS service>\n"
           "                   set is: " DYN_SERVICE_ADRESS "\n"
           "    --addRouteEndpoint <computer name>=<domain name>\n"
           "    --setAuthToken <autentication tokken>\n\n");
    return CheckForError()
        || !hasOption('h');
  }
#endif

// regular includes here:
#include <stdlib.h>

char* _passPhrase(const char* pass)
{
    tempf("&password=%s",pass);
    return getTemp();
}

const char* dynDnUpter_PrepareSlot(const char* updateSlot)
{
    const char* endpoint = getVariable( pool_setf("ENDPOINT_%s",updateSlot) );
    DEBUGFMT("found endpoint: %s",endpoint);
    pool_setCheckpoint();
   #if WINDOWS
    pool_set( DYN_UPDATE_SOCKETIER " \"" );
   #else
    pool_set( "exec " DYN_UPDATE_SOCKETIER " \"" );
   #endif
    pool_set( DYN_SERVICE_ADRESS );
    pool_set( endpoint );
    pool_setf( "&password=%s", DYN_SERVICE_SESSION );
    if( hasOption('4') )
        pool_setf( "&ip4=%s", getName('4') );
    if( hasOption('6') )
        pool_setf( "&ip6=%s", getName('6') );
    pool_set( "\"" );

    ExitOnError( "missing value" );
    return updateSlot;
}

int dynDnUpter_DoUpdate( const char* updateSlot )
{
    printf( "\n\n will update ddn route for: %s\n", updateSlot );
    char* command = pool_collectCheckpoint();
    DEBUGFMT("will execute: %s",command);
    return system(command);
}

const char* dynDnUpter_CheckForSettingChanges(CommanderState* cmndstate)
{
      DEBUGLOG("enter")

      CommanderState* ctmp = c.running;
      if(cmndstate)
        c.running = cmndstate;

      beginPersistChange(LOCAL);
      if(isModus("setSocketier")) {
        if(!rawNext('s')[0])
            setErrorText("need path to some web socket accessing app given");
        else
            setPersistEntry("DYN_UPDATE_SOCKETIER",pool_setf("\"%s\"",rawNext('s')));
      }
      if(isModus("setServiceUrl")) {
        if(!rawNext('s')[0])
            setErrorText("enter DDNS service's address");
        else
            setPersistEntry("DYN_SERVICE_ADRESS",pool_setf("\"%s\"",rawNext('s')));
      }
      if(isModus("setAuthToken")) {
        if(!rawNext('s')[0])
            setErrorText("enter passphrase for ddns account");
        else
            setPersistEntry("DYN_SERVICE_SESSION",pool_setf("\"%s\"",rawNext('s')));
      }
      if(isModus("addRouteEndpoint")) {
        if(!rawNext('a')[0])
            setErrorText("missing definition: <local host name>=<dynamic domain name>");
        else {
          char* inp = rawNext('a');
          const int end = strlen(inp);
          int i=-1;
          while(inp[++i]!='='&&i<end);
          if(i>=end)
            setErrorText("missing definition: <local host name>=<dynamic domain name>");
          else {
            inp[i++]='\0';
            tempf( "\"%s\"", &inp[i] );
                setPersistEntry( pool_setf("ENDPOINT_%s",inp), getTemp() );
            printf( "Set endpoint: %s for dyn domain name: %s", pool_get(), getTemp() );
          }
        }
      } commitPersistChange();
      pool_set( getError() );
      c.running = ctmp;
      return pool_get();
}

void CheckGivenParameters(void)
{
    if(!ensure('n',"missing: update slot name"))
        ExitOnError("argument");
    getNameByIndex(indexOfName("name"))[0]='\0';

    if(isModus("verbose")){
        showOptions();
        showPersistente(LOCAL);
    }

    if(hasOption('4')) {
        if(!hasOption('i'))
            setOption('i',"ipv4");
    } else if(isModus("ipv4")) {
        setOption('4',rawNext('i'));
    } else
    if(hasOption('6')) {
        if(!isModus("ipv4"))
            setOption('i',"ipv6");
    } else if(isModus("ipv6")) {
        setOption('6',rawNext('i'));
    } else setErrorText("need either 'ipv4' or 'ipv6' ip address given");

    ExitOnError("input");
}

#ifdef dynDnUpter
int main(int argc,char**argv)
{
    if(!InitCommandLiner(argc,argv)
      ||hasOption('h'))
        exit(USAGE(dynDnUpter));

    if(hasOption('v')||DEBUG)
        setOption('v',"verbose");

    if(isAnyModus("set*")||isAnyModus("add*"))
        exit(dynDnUpter_CheckForSettingChanges(c.running));

    CheckGivenParameters();

    int returnvalue = dynDnUpter_DoUpdate( dynDnUpter_PrepareSlot(getName('n')) );
    exit( returnvalue );
 }
 #endif

