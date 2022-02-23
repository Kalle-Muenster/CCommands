// generator Template for deriving new Stepflow Controllermode
// implementation from the Stepflow UsermodeControl base class

const byte ControllerImplementation[] = {"\
#ifndef _^COMMANDNAME^_hpp_\n\
#define _^COMMANDNAME^_hpp_\n\
\n\
#define READONLY_PINS\n\
#include <ControllerAccess.h>\n\
\n\
^Space.LOS^\
\n\
    enum ^COMMANDNAME^Pins { PIN_A = 0, PIN_B = 1 };\n\
    template<typename cT>\n\
    class ^COMMANDNAME^ : public STEPFLOW(VALUES,^SNIPPED.DATANAME^<cT>)\n\
    { DeclareTwoPins(cT,PIN_A,cT,PIN_B)\n\
    protected:\n\
        typedef STEPFLOW(VALUES,^SNIPPED.DATANAME^<cT>) BASE;\n\
        virtual unsigned modeCodeBase(void) const {\n\
         // Unique mode id (32bit typecode) here:\n\
        // ...short example:\n\
            return *(unsigned*)\"^SNIPPED.DATATYPE^\";\n\
        }\n\
		virtual unsigned usrModeSize() const {\n\
            return sizeof(^COMMANDNAME^);\n\
        }\n\
        virtual void Init(void) {\n\
         // Initialization logic here:\n\
        // ...short example:\n\
            BASE::Init();\n\
            PIN_A = MOV;\n\
            PIN_B = ((MAX-MIN)/2)+MIN;\n\
            PIN_COUNT += 2;\n\
        }\n\
    public:\n\
        virtual cT checkVALUE(cT* pVALUE) {\n\
         // Controller logic here:\n\
        // ...short example:\n\
            cT val = (*pVALUE+1);\n\
            val = val > PIN_B ? (PIN_A) : val;\n\
            return *pVALUE = val;\n\
        }\n\
    };\n\
\n\
^Space.END^\
\n\
#include <ControllerAccess.h>\n\
#endif\n\
"};

const byte ControllerTestFunctions[] = {"\
#ifndef _TEST_^COMMANDNAME^_hpp_\n\
#define _TEST_^COMMANDNAME^_hpp_\n\
#include <ControlledValues.h>\n\
#include \"^COMMANDNAME^.hpp\"\n\n\
#define PinGetterTest(pinnam,pindex,expect) std::cout << pinnam << \": \";\\\n\
    if ( testling.GetPin<cT>(pindex) == expect ) {\\\n\
        std::cout << \"PASS\\n\";\\\n\
    } else {\\\n\
        std::cout << \"FAIL\\n\";\\\n\
        failures++;\\\n\
    }\n\n\
^Space.LOS^\
\n\
template<typename cT>\n\
int run_^COMMANDNAME^_Pinning_Test(cT* testData=NULL)\n\
{\n\
    STEPFLOW(VALUES,VariableController<cT>) testling;\n\
    testling.SetUserMode<^COMMANDNAME^>(0,100,1,1);\n\
    testling.SetCheckAtGet(true);\n\
    testling.Active = true;\n\
    int failures = 0;\n\
    PinGetterTest(\"PIN_A\",PIN_A,1)\n\
    PinGetterTest(\"PIN_B\",PIN_B,50)\n\
    return failures;\n\
}\n\n\
template<typename cT>\n\
int run_^COMMANDNAME^_Value_Test(cT* testData=NULL)\n\
{\n\
    STEPFLOW(VALUES,VariableController<cT>) testling;\n\
    testling.SetUserMode<^COMMANDNAME^>(0,100,1,1);\n\
    testling.SetCheckAtGet(true);\n\
    testling.Active = true;\n\
    for(int i=0;i<100;++i) {\n\
        cT get = testling;\n\
        if( get != testData[i]) {\n\
            std::cout << \"Failure at testdata position \"\n\
                      << i << \": testling (\" << get << \") != \"\n\
                      << \" testData (\" << testData[i] << \")\\n\";\n\
            return i;\n\
        }\n\
    } return EMPTY;\n\
}\n\
\n\
^Space.END^\n\
#endif\n\
"};

const byte ControllerTestRun[] = {"\
#include \"^COMMANDNAME^_tst.hpp\"\n\
#include <iostream>\n\
\n\
// any other includes needed here:\n\
\n\
^Space.USE^\
\n\
int main(int argc,char** argv)\n\
{\n\
    const char* TestName = \"^COMMANDNAME^<float> test \";\n\
    float testdata[100];\n\
    for(int i=0;i<100;++i){ float I = i+2;\n\
        I = I > 100 ? I - 100 : I > 50 ? I - 50 : I;\n\
        testdata[i] = I;\n\
    }\n\
    int errorlevel = 0;\n\
    std::cout << \"\\nPinning Tests: \\n\";\n\
    if( errorlevel += run_^COMMANDNAME^_Pinning_Test<float>() ) {\n\
        std::cout << errorlevel << \"FAILS!\\n\";\n\
    }\n\
    std::cout << \"\\nController Test:\\n\";\n\
    if( run_^COMMANDNAME^_Value_Test<float>(&testdata[0]) == EMPTY ){\n\
        std::cout << TestName << \"PASS\\n\";\n\
    } else {\n\
        std::cout << TestName << \"FAIL\\n\";\n\
        errorlevel++;\n\
    }\n\
    return errorlevel;\n\
}\n\
"};


// define generator functions:
void createControllerImplementation(char* outputDir)
{
    char filenamebuffer[MAX_NAM_LEN]={'\0'};
    strcpy(Tokken[EXTENSION],".hpp");
    sprintf( &filenamebuffer[0],"%s%s%s", outputDir,
             Tokken[COMMANDNAME], Tokken[EXTENSION] );
    generateFile( &ControllerImplementation[0], &filenamebuffer[0] );
}

void createControllerTestFunctions(char* outputDir)
{
    char filenamebuffer[MAX_NAM_LEN]={'\0'};
    strcpy(Tokken[EXTENSION],"_tst.hpp");
        sprintf( &filenamebuffer[0],"%s%s%s", outputDir,
                 Tokken[COMMANDNAME], Tokken[EXTENSION] );
    generateFile( &ControllerTestFunctions[0], &filenamebuffer[0] );
}

void createControllerTestRun(char* outputDir)
{
    char filenamebuffer[MAX_NAM_LEN]={'\0'};
    strcpy(Tokken[EXTENSION],"_tst.cpp");
        sprintf( &filenamebuffer[0],"%s%s%s", outputDir,
                 Tokken[COMMANDNAME], Tokken[EXTENSION] );
    generateFile( &ControllerTestRun[0], &filenamebuffer[0] );
}

void InitControllerGenerator(void)
{
    if( !search('c') ) {
        setOption('c',"UserModeControl");
    }

    if( search('n') ) {
        generate_setNamespace((const byte*)getName('n'));
    } else {
        assignTokken( Space.Tokken, LOS, "#ifdef BEGIN_STEPFLOW_NAMESPACE\n\
       BEGIN_STEPFLOW( VALUES )\n#endif\n" );
        assignTokken( Space.Tokken, END, "#ifdef ENDOF_STEPFLOW_NAMESPACE\n\
       ENDOF_STEPFLOW( VALUES )\n#endif\n" );
        assignTokken( Space.Tokken, USE, "USING_STEPFLOW( VALUES )\n" );
        assignTokken( Space.Tokken, NAM, NoString );
        assignTokken( Space.Tokken, SEP, NoString );
    }

    char fourcc[5] = {0};
    *(unsigned*)&fourcc[0] = *(unsigned*)getName('C');
    assignTokken(&Tokken[0], COMMANDNAME,getName('C'));
    assignTokken(Snipped.Tokken,DATANAME,getName('c'));
    assignTokken(Snipped.Tokken,DATATYPE,&fourcc[0]);

    generateHeaderFile = &createControllerImplementation;
    generateCodeFile   = &createControllerTestRun;
    generateExtraFile  = &createControllerTestFunctions;
}




