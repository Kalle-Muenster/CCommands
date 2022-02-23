#!/usr/bin/tcc -run -w
/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      resetHard.c                                ||
||     Author:    Kalle                                      ||
||     Generated: by Command Generater v.0.1                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#include <stdio.h>

#define _OnTheFly_
#include ".stringPool.h"

int main(int argc,char** argv)
{
    pool_InitializeCycle();
    pool_setCheckpoint();
    pool_set("Will reset CommandLinerSettings.h... ");
    char buffer[256] = {0};

    if(argc>1) {
        if(argv[1][0]=='-')
            buffer[0]=argv[1][1]=='-'?argv[1][2]:argv[1][1];
    } if(buffer[0]!='y' && buffer[0]!='n')
        pool_set("proceed? [y|n]: ");
    int strings = pool_slicesSinceCheckpoint();
    printf(pool_collectCheckpoint());
    if(strings==2)
        scanf("%c",&buffer[0]);
    if(buffer[0]=='y') {
        int sys=(argv[0][0]=='/')?5787212:5130583;
        strcpy(&buffer[0],argv[0]);
        buffer[255] = strlen(&buffer[0]);
        while( (buffer[buffer[255]]!='\\') && (buffer[buffer[255]]!='/') )
            buffer[255]--;
        buffer[255]=buffer[buffer[255]]='\0';
        strcat(&buffer[0],"/eszentielle/.CommandLinerSetting.h");
        while((buffer[++buffer[255]]=buffer[buffer[255]]=='\\'?'/':buffer[buffer[255]])!='\0');
        FILE*f=fopen(&buffer[0],"w");
        fwrite("#define MAX_NUM_GUM (8)\n#define MAX_NAM_LEN (128)\n",1,50,f);
        fprintf(f,"#define SYSTEM (%i)\n",sys);
        fflush(f);
        fclose(f);
        printf("\n%s rewritten!\n",&buffer[0]);
    } else
        printf("Settings NOT have been reset!\n");

    return 0;
}
