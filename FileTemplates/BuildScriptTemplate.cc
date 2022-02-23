const byte IncludeHeaderer[] = {
    "#ifndef IncludesFor_^COMMANDNAME^\n"
    "#define IncludesFor_^COMMANDNAME^ (1)\n"
    "\n"
    "\n"
    "#define PREFER_LOCAL (1)\n"
    "#include <.environMentor.h>\n"
    "\n"
    "#ifndef SETFIX\n"
    "#define SETFIX ^COMMANDNAME^\n"
    "#endif\n"
    "\n"
    "#ifndef TARGET\n"
    "#define TARGET SETFIX ## _TARGET_\n"
    "#endif\n"
    "\n"
    "#ifndef ACTION\n"
    "#define ACTION SETFIX ## _ACTION_\n"
    "#endif\n"
    "\n"
    "\n"
    "#ifdef _OnTheFly_\n"
    "#include \"^COMMANDNAME^.cc\"\n"
    "#endif\n"
    "#endif\n"
};

const byte FiloPeilo[] = {
    "#ifndef _OnTheFly_\n"
    "#include \"^COMMANDNAME^.h\"\n"
    "#endif \n\n"
    "#include <stdlib.h>\n"
    "#include \".commandLiner.h\"\n"
    "#include \".stringPool.h\"\n\n"
    "#define LocalScopeName  _OnTheFly_\n"
    "const char* DOKU(LocalScopeName)={\"\\nhelper tool for manually setting \\\n"
    "definitions for compiler suites:\\n\\n  options:\\n\\n    -p : prefix\\n\\\n"
    "    -n : name\\n    -c : command\\n    -e : export\\n    --get [Name] \\\n"
    ": gets copy of sets feature header (<Name>_features.h) into the CWD.\\n\"};\n"
    "UsageDefinition(LocalScopeName,\"--[add|rem|set] <Name> <Value>\")\n"
    "#include \".buildWalker.h\"\n\n\n"
    "int main(int argc,char**argv)\n"
    "{\n"
    "    InitCommandLiner(argc,argv);\n\n"
    " // change ConfigureActions here by:\n"
    " // build_setConfigureAction(Add|Set|Build,function);\n\n"
    "    exit(InitBuildWalker());\n"
    "}\n"
};

const byte BuildCommand[] = {
    "#define _OnTheFly_ ^COMMANDNAME^\n"
    "#define ^COMMANDNAME^ \"\"\n"
    "#include \"^COMMANDNAME^.h\"\n"
    "int main(int argc,char** argv);\n"
};

// define generater functions:
void createIncludeHeader(char* outputDir)
{
    pool_set(outputDir);
    pool_set(Tokken[COMMANDNAME]);
    pool_set(Tokken[EXTENSION]=".h");
    generateFile( &IncludeHeaderer[0], pool_merge(3) );
}

void createFiloPeilo(char* outputDir,char insertSnippet)
{
    pool_set(outputDir);
    pool_set(Tokken[COMMANDNAME]);
    pool_set(Tokken[EXTENSION]=".cc");
    generateFile( &FiloPeilo[0], pool_merge(3) );
}

void createBuildCommand(char* outputDir)
{
    pool_set(outputDir);
    pool_set(Tokken[COMMANDNAME]);
    pool_set(Tokken[EXTENSION]=".c");

    generateFileConditionalInsert( "#!/usr/bin/tcc -run -w\n",
                                   BuildCommand, InfoHeader,
                                   INSERT_NOINFO, pool_merge(3) );
}

void InitBuildScriptGenerator(void)
{
    generateHeaderFile = &createIncludeHeader;
    generateCodeFile   = &createFiloPeilo;
    generateExtraFile  = &createBuildCommand;

    if(!hasOption('b'))
        setErrorText("inpropper usage of 'InitBuildScriptGenerator()'");
    else {
        if(!hasOption('c')) {
            if(!isSwitch('b')) {
                setOption('c',getName('b'));
            } else
                setOption('c',"buildo");
        }
    } assignTokken(&Tokken[0],COMMANDNAME,getName('c'));
}