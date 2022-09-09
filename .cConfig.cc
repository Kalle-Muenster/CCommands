/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      cConfig.cc                                 ||
||     Author:    Kalle                                      ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
#include ".cConfig.h"
#endif

#define COMMAND_LINER_SETTING(SETTING,VALUE) "\
#ifndef " ## SETTING ## "\n\
#define " ## SETTING ## " (" ## VALUE ## ")\n\
#endif\n"

#include <stdio.h>
#include ".commandLiner.h"

#ifndef  MAX_NAM_LEN
unsigned MAX_NAM_LEN=255;
#endif
#ifndef  MAX_NUM_GUM
unsigned MAX_NUM_GUM=8;
#endif
#ifndef  ARGUM_SLASHING
unsigned ARGUM_SLASHING=0;
#endif
#ifndef  QUOTE_ENDOUBLING
unsigned QUOTE_ENDOUBLING=0;
#endif
#ifndef  BRACKET_ECKING
unsigned BRACKET_ECKING=0;
#endif
#ifndef  SPACE_ENDASHING
unsigned SPACE_ENDASHING=0;
#endif

void RequestConfiguration(void);

CreateDoku(cConfig, "To change settings, use parameter \"set\",\n\
or pass options to directly set the values:\n\n\
    --n-<number>  :  Set maximum number on arguments\n\
    --a-<size>    :  Set maximum length of each argument.\n\
  For all commandline parameter Enable/Disable:\n\
    --p-<yes|no>  :  automatic slash conversion.\n\
    --q-<yes|no>  :  automatic quote-endoubling.\n\
    --b-<yes|no>  :  automatic bracket-ecking.\n\
    --u-<yes|no>  :  automatic spacen-dashing.\n\n\
  *(-n/-a must be numerical, -p/-q/-b/-u \"yes\" or \"no\"!)\n\n" );

int USAGE(cConfig)
{
    Synopsis("[set]|[options]\n");
    printf(DOKU(cConfig));
    return 0;
}

int ShowConfiguration(void)
{
    if( hasOption('h') || wasError() )
        USAGE(cConfig);

    uint system4cc = SYSTEM;
#ifndef LOCUS_POCUS
    const char *const LOCUS_POCUS = getPathOfTheCommander();
#endif

    printf("\nCurrent Command-Liner Configuration:\n\n");

    printf("Scripts configured for running on: %s\n",(char*)&system4cc);
    printf("CCommands scripts folder location: %s\n\n",LOCUS_POCUS);

    printf("Total bytes command line buffer: %i\n",COMMANDER_BUFFER);
    printf("Maximum length of each argument: %i\n",MAX_NAM_LEN);
    printf("Maximum number of all arguments: %i\n\n",MAX_NUM_GUM);

    printf("Commandline input conversion settings:\n");
    printf("auto-slashing:    %s\n",   ARGUM_SLASHING?"ENABLED":"DISABLED");
    printf("quote-endoubling: %s\n", QUOTE_ENDOUBLING?"ENABLED":"DISABLED");
    printf("bracket-ecking:   %s\n",   BRACKET_ECKING?"ENABLED":"DISABLED");
    printf("space-endashng:   %s\n\n",SPACE_ENDASHING?"ENABLED":"DISABLED");
    return getErrorCode();
}

void SaveCommandLinerConfiguration( int maxnumgum, int maxnamlen, int autoslash,
                                    int autoqoute, int autobrack, int autospack )
{
    if( CheckForError() )
        return;

    char  filename[MAX_NAM_LEN];
    char  SettingsBuffer[456+MAX_NAM_LEN];
    char* buffer = &SettingsBuffer[0];

    sprintf( &filename[0],"%s/eszentielle/.CommandLinerSetting.h",
             getPathOfTheCommander() );

    strcpy(buffer,COMMAND_LINER_SETTING("LOCUS_POCUS","\"%s\""));
    strcat(buffer,COMMAND_LINER_SETTING("MAX_NUM_GUM","%i"));
    strcat(buffer,COMMAND_LINER_SETTING("MAX_NAM_LEN","%i"));
    strcat(buffer,COMMAND_LINER_SETTING("ARGUM_SLASHING","%i"));
    strcat(buffer,COMMAND_LINER_SETTING("QUOTE_ENDOUBLING","%i"));
    strcat(buffer,COMMAND_LINER_SETTING("BRACKET_ECKING","%i"));
    strcat(buffer,COMMAND_LINER_SETTING("SPACE_ENDASHING","%i"));
    strcat(buffer,COMMAND_LINER_SETTING("SYSTEM","%s"));

    FILE* f = fopen( &filename[0], "w" );
    fprintf(f,buffer,getPathOfTheCommander(), maxnumgum, maxnamlen,
             autoslash, autoqoute, autobrack, autospack, SystemFourCC );
    fflush(f); fclose(f);
}

void RequestCommandLinerConfiguration(void)
{
    int maxnumgum = MAX_NUM_GUM;
    int maxnamlen = MAX_NAM_LEN;

    #define BeginUserinputRequest CommandLinerSettings
    #include "eszentielle/.CommandlinerInputs.h"

    InitDatasetRequest(5);

    CreateNumericInputRequest("\nSet maximum number on arguments: ",maxnumgum,'n',2);
    CreateNumericInputRequest("\nSet maximum length of arguments: ",maxnamlen,'a',3);

    CreateBooleanDecision("\nEnable backslash/slash conversion for input arguments? [y/n]: ",ARGUM_SLASHING,'p',"y\0n");
    CreateBooleanDecision("\nEnable single/double quote conversion for input arguments? [y/n]: ",QUOTE_ENDOUBLING,'q',"y\0n");
    CreateBooleanDecision("\nEnable bracket conversion \"[->(\" for input arguments? [y/n]: ",BRACKET_ECKING,'b',"y\0n");
    CreateBooleanDecision("\nEnable underdash to space conversion for input arguments? [y/n]: ",SPACE_ENDASHING,'u',"y\0n");

    #define EndOfUserinputRequest CommandLinerSettings
    #include "eszentielle/.CommandlinerInputs.h"
}

int InputLooping(void)
{
    int maxnumgum = MAX_NUM_GUM;
    int maxnamlen = MAX_NAM_LEN;
    int autoslash = ARGUM_SLASHING;
    int autoqoute = QUOTE_ENDOUBLING;
    int autobrack = BRACKET_ECKING;
    int autospack = SPACE_ENDASHING;


    if(search('n'))
        if(!sscanf(getName('n'),"%i",&maxnumgum))
            setErrorText("input -n was not a number!\n");

    if(search('a'))
        if(!sscanf(getName('a'),"%i",&maxnamlen))
            setErrorText("input -a was not a number!\n");



    if(hasOption('p')) {
        char* aso = getName('p');
        char YesOrNo = aso[0]=='y'?1:aso[0]=='n'?0:-1;
        if(YesOrNo<0)
            autoslash = SYSTEM == WIN;
        else
            autoslash = YesOrNo;
    }

    if(hasOption('q')) {
        char* aqd = getName('q');
        char YesOrNo = aqd[0]=='y'?1:aqd[0]=='n'?0:-1;
        if(YesOrNo<0)
            autoqoute = SYSTEM == WIN;
        else
            autoqoute = YesOrNo;
    }

    if(hasOption('b')) {
        char* abe = getName('b');
        char YesOrNo = abe[0]=='y'?1:abe[0]=='n'?0:-1;
        if(YesOrNo<0)
            autobrack = SYSTEM == LNX;
        else
            autobrack = YesOrNo;
    }

    if(hasOption('u')) {
        char* asd = getName('u');
        char YesOrNo = asd[0]=='y'?1:asd[0]=='n'?0:-1;
        if( YesOrNo<0 )
            autospack = true;
        else
            autospack = YesOrNo;
    }


    if(hasOption('n')||hasOption('a')||hasOption('p')||hasOption('q')||hasOption('b')||hasOption('u'))
        SaveCommandLinerConfiguration(maxnumgum,maxnamlen,autoslash,autoqoute,autobrack,autospack);

    if(!numGum()||hasOption('h')||wasError()||hasOption('v'))
        return ShowConfiguration();

    return 0;
}

int main( int argc, char** argv )
{
    InitCommandLiner( argc, argv );

    if( hasOption('v') )
        showOptions();

    if( isModus("set") )
        do { clearAllErrors();
          RequestCommandLinerConfiguration();
        } while( InputLooping() );
    else
        exit( InputLooping() );

    exit( getErrorCode() );
}
