/*///////////////////////////////////////////////////////////*\
||                                                           ||
||     File:      xmlNow.cc                                  ||
||     Author:    Kalle                                      ||
||     Generated: 22.12.2017                                 ||
||                                                           ||
\*\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\*/
#ifndef _OnTheFly_
  #include ".xmlNow.h"
#else
//  #include <.environMentor.h>
  int checkEnableState(cmLn);
  int USAGE(xmlNow)
  {
    if( !isAnyOtherModusThen("help") ) {
        Synopsis("<XMLfile> <operation> [parameters]");
        printf("operations:\n\n");
        printf("   --add      -  adds a new element to the document\n");
        printf("   --get      -  gets an existing element from the document\n");
        printf("   --rem      -  removes an element from the document\n");
        printf("   --set      -  modify/overwrite existing element in the document\n\n");
		printf("   --format   -  set document formating to <on|off> (persitently)\n");
		printf("\n append '--help' to get more detailed information for one of the operations\n");
		clearAllErrors();
    }
    if( hasOption('h') ) {
        if( isModus("add") ) {
            Synopsis("<XMLfile> --add <xpath> < element [[@attribute=value]...n] [~content] >");
            printf("\n    <xpath>          : Expression pointing that existing element where to insert.");
            printf("\n    <element>        : Description of the element to be add. begins with the tag name.");
            printf("\n    [@attribute=val] : For each attribute to be set, '@name=value' pairs can follow.");
            printf("\n    [~content]       : Contend to be contained, can be defined following a tilde '~'");
            printf("\n                       given as last parameter (e.g. 'element@attr=name~content' ).\n");
        }
        if( isModus("rem") ) {
            Synopsis("<XMLfile> --rem <xpath>");
            printf("\n    <xpath>          : Expression pointing that existing element which to remove.\n");
        }
        if( isModus("set") ) {
            Synopsis("<XMLfile> --set <xpath> < element [[@attribute=value]...n] [~content] >");
		    Synopsis("<XMLfile> --set <xpath[@]> < [ @name | @name=value | =value ] >");
            Synopsis("<XMLfile> --set <xpath/text()> <~content>");
			printf("\n  <xpath>              : Expression pointing that existing element which to replace by <element>");
			printf("\n  <xpath[@n[=v]]>      : When pointing an attribute node, by either @name or =value or both,");
            printf("\n                         depending on if [@nam], [=val] or [@nam=val] is passed as parrameter.");	
			printf("\n  <xpath[@[=]]/text()> : When <xpath> is pointing to some content node, it is set to <~content>.\n");
        }
        if( isModus("get") ) {
            Synopsis("<XMLfile> --get <xpath>");
            printf("\n    <xpath>          : Simpli gets evaluation result of given <xpath> expression.\n");
        }
		if( isModus("format") ) {
			printf("\npersistent %s options:\n\n",getNameOfTheCommander());
			printf("   --format,-f : can be set [1|0] or [yes|no] or [on|off] for enabling\n");
			printf("                 or disabling writing 'formated' (by tab's and spaces)\n");
		    printf("                 xmlfiles or even by writing out everything 'minimized'\n");
            printf("                 to one long, single line into the output document...\n");
		    printf("                     actual state is: %s\n",
					checkEnableState(getVariable("XMLNOW_FORMATING"))?"ENABLED":"DISABLED");
		}
    } return CheckForError()
          || !hasOption('h');
  }

#endif

#define CONTENT_NODES (ROXML_CMT_NODE|ROXML_ELM_NODE)
/*
typedef struct node node_t;
#define ROXML_ATTR_NODE 0x008
#define ROXML_STD_NODE  0x010
#define ROXML_ELM_NODE  0x010
#define ROXML_TXT_NODE  0x020
#define ROXML_CMT_NODE  0x040
#define ROXML_PI_NODE   0x080
#define ROXML_NS_NODE   0x100
#define ROXML_NSDEF_NODE    (ROXML_NS_NODE | ROXML_ATTR_NODE)
#define ROXML_CDATA_NODE    (ROXML_TXT_NODE | 0x200)
#define ROXML_DOCTYPE_NODE  0x400
#define ROXML_ALL_NODES (ROXML_PI_NODE | ROXML_CMT_NODE | ROXML_TXT_NODE | ROXML_ATTR_NODE | ROXML_ELM_NODE)
#define ROXML_ALL_NODE  ROXML_ALL_NODES
#define ROXML_NODE_TYPES    0x05f8
#define RELEASE_ALL (void*)-1
#define RELEASE_LAST    (void*)-2

node_t *ROXML_API roxml_load_buf(char *buffer);
node_t *ROXML_API roxml_load_doc(char *filename);
node_t *ROXML_API roxml_load_fd(int fd);
void ROXML_API roxml_close(node_t *n);
node_t *ROXML_API roxml_get_next_sibling(node_t *n);
node_t *ROXML_API roxml_get_prev_sibling(node_t *n);
node_t *ROXML_API roxml_get_parent(node_t *n);
node_t *ROXML_API roxml_get_root(node_t *n);
node_t *ROXML_API roxml_get_ns(node_t *n);
node_t *ROXML_API roxml_set_ns(node_t *n, node_t * ns);
node_t *ROXML_API roxml_get_cmt(node_t *n, int nth);
int ROXML_API roxml_get_cmt_nb(node_t *n);
node_t *ROXML_API roxml_get_chld(node_t *n, char *name, int nth);
int ROXML_API roxml_get_chld_nb(node_t *n);
node_t *ROXML_API roxml_get_pi(node_t *n, int nth);
int ROXML_API roxml_get_pi_nb(node_t *n);
char *ROXML_API roxml_get_name(node_t *n, char *buffer, int size);
char *ROXML_API roxml_get_content(node_t *n, char *buffer, int bufsize, int *size);
int ROXML_API roxml_get_nodes_nb(node_t *n, int type);
node_t *ROXML_API roxml_get_nodes(node_t *n, int type, char *name, int nth);
int ROXML_API roxml_get_attr_nb(node_t *n);
node_t *ROXML_API roxml_get_attr(node_t *n, char *name, int nth);
node_t **ROXML_API roxml_xpath(node_t *n, char *path, int *nb_ans);
int ROXML_API roxml_get_type(node_t *n);
int ROXML_API roxml_get_node_position(node_t *n);
void ROXML_API roxml_release(void *data);
node_t *ROXML_API roxml_add_node(node_t *parent, int position, int type, char *name, char *value);
node_t *ROXML_API roxml_get_txt(node_t *n, int nth);
int ROXML_API roxml_get_txt_nb(node_t *n);
void ROXML_API roxml_del_node(node_t *n);
int ROXML_API roxml_commit_changes(node_t *n, char *dest, char **buffer, int human);
*/


#include <stdlib.h>


node_t* currentDocument = EMPTY;

const char* xmlNow_getNodeType(int nodetype)
{
    switch(nodetype) {
    case ROXML_ATTR_NODE: return "ATTRIBUTE";
    case ROXML_ELM_NODE:  return "ELEMENT";
    case ROXML_TXT_NODE:  return "TEXT";
    case ROXML_CMT_NODE:  return "COMMENT";
    case ROXML_PI_NODE:   return "PI";
    case ROXML_NS_NODE:   return "ATTRIBUTE";
    }
}

void AtXmlExitNow(void)
{
    if( currentDocument != EMPTY )
        roxml_close( currentDocument );
	currentDocument = EMPTY;
}

int checkEnableState( cmLn setting )
{
	if( !isValidArg( setting ) ) {
		setErrorText( setTempf( "invalid: %s",
		          setting ) );
		return XMLNOW_FORMATING;
	} else switch( AsInt(setting) ) {
		case FOURCC('o','n',0,0):
		case FOURCC('1',0,0,0):
		case FOURCC('(','1',')',0):
		case FOURCC('y','e','s',0):
			return 1;
		case FOURCC('0',0,0,0):
		case FOURCC('n','o',0,0):
		case FOURCC('(','0',')',0):
		case FOURCC('o','f','f',0):
			return 0;
	}
}

int xmlNow_setFormating( cmLn setting )
{
	int state = checkEnableState( setting );
	setMacroNum("XMLNOW_FORMATING", state );
	return state;
}

void xmlNow_commitChanges( node_t* xmldoc, const char* tempdocfile )
{
	FeatureGet fet = getFeatured( "XMLNOW_FORMATING" );
	if ( catchError( "XMLNOW_FORMATING" ) ) {
		fet.type.flags = Type_NUMBER;
		fet.value.number = 1;
	} int formating = (int)fet.value.number;
    roxml_commit_changes( 
		xmldoc, tempdocfile,
		  NULL, formating );
    roxml_close( xmldoc );
    xmldoc = NULL;
    switch( SYSTEM ) {
        case WIN:
			pool_setf( "del /s /f /q %s",  getName('x') );
			system( pool_get() ); 
			pool_setf( "rename %s ", tempdocfile );
        break;
        case LNX:
			pool_setf( "rm %s",  getName('x') );
			system( pool_get() ); 
	        pool_setf( "mv %s ", tempdocfile );
        break;
    } 
	pool_set( getName('x') );
    system( pool_merge(2) );
}

char* addElement(node_t* parent,const char* what)
{
    char* cntnt = NULL;
    int split = 0,nbAtr = 0;
    int length = strlen(what);
    char* elmnt = setTemp(what);
    while(++split<length) {
        if(elmnt[split]=='~')
            break;
    } if(split<length) {
        elmnt[split++]='\0';
        cntnt = &elmnt[split];
    }
    char* atrbs = &elmnt[--split];
    while(&atrbs[--split]!=elmnt) {
        if(atrbs[split]=='@') {
            nbAtr++; atrbs[split]='\0';
            pool_set(&atrbs[split+1]);
        }
    }
    node_t* newNode = roxml_add_node( parent, roxml_get_chld_nb(parent)+1,
                                      ROXML_ELM_NODE, &elmnt[0], cntnt );
    for(int i=0;i<nbAtr;i++) {
        atrbs = pool_last(nbAtr-i);
        split = 0;
        while(atrbs[++split]!='=');
        atrbs[split++]='\0';
        roxml_add_node( newNode, i+1,
                        ROXML_ATTR_NODE,
                        &atrbs[0],
                        &atrbs[split] );
    }
    setOption( 'c', "commit" );
    return elmnt;
}

PoolStream xmlNow_addElement( node_t* xmldoc, const char* where, const char* what )
{
	DEBUGLOG("enter");
    int insertPoints = 0;
    node_t** position = roxml_xpath( xmldoc,
                                     where, &insertPoints );
    if( insertPoints ) {
        pool_setf( "'%s' add to the document",
		           addElement( position[0], what ) );
    } else
        setErrorText( "insert point not clear" );
    return pool_createStream( &pool_get_ex );
}

int hasAttribut( node_t* node, const char* attr )
{
    int attrs=roxml_get_attr_nb(node);
    for(int c=0;c<attrs;c++)
        if(strcmp(roxml_get_attr(node,NULL,c),attr)<0)
            return true;
    return false;
}

char* setAttribute(int modeFourCC, void* nameOrNode, void* valueOrParent)
{
    char* name;
    char* value;
    node_t* parent;
    char workbuffer[MAX_NAM_LEN];

    switch( modeFourCC ) {
        case 1162104654: {
            node_t* n = (node_t*)nameOrNode;
            if(roxml_get_type(n)!=ROXML_ATTR_NODE)
                setErrorText("not attribute node");
            parent = roxml_get_parent(n);
            name = roxml_get_name( 
				   n,&workbuffer[0],MAX_NAM_LEN
									);
            value = (char*)valueOrParent;
        } break;
        case 1162690894: {
            parent = (node_t*)valueOrParent;
            name = (char*)nameOrNode;
            int split=-1;
            while(name[++split]!='=');
            name[split]='\0';
            value=&name[++split];
        } break;
    }

    roxml_add_node( parent, roxml_get_attr_nb(parent),
                    ROXML_ATTR_NODE, name, value );

    tempf( "set Attribute: %s=", name );
    setOption( 'c', "commit" );
    return strcat( getTemp(), value );
}

PoolStream xmlNow_setElement( node_t* xmldoc, const char* where, const char* what)
{
	DEBUGLOG("enter");
    int insertPoints = 0;
    node_t** position = roxml_xpath( xmldoc,
                                     where, &insertPoints );
    if( !insertPoints )
        setErrorText( "insert point not clear" );
    else {
        pool_setCheckpoint();
        for( int i=0; i<insertPoints; i++ ) {
            char workbuffer[MAX_NAM_LEN];
            node_t* node = position[i];
            switch(roxml_get_type(node)){
                case ROXML_ATTR_NODE: {
                    pool_set( setAttribute(AsInt("NODE"),node,what) );
                } break;
                case ROXML_ELM_NODE: {
                    node_t* parent = roxml_get_parent(node);
                    roxml_del_node(node);
                    pool_setf( "set node: '%s'\n", addElement(parent,what) );
                } break;
            case ROXML_TXT_NODE:
                pool_set( "TODO: setting text nodes" );
                break;
            } ExitOnError("set element");
        }
    }
	DEBUGLOG("next call: pool_createStream()")
    return pool_createStream( &pool_get_ex );
}

PoolStream xmlNow_remElement(node_t* xmldoc, const char* where,const char* what)
{
	DEBUGLOG("enter");
    int insertPoints = 0;
    node_t** position = roxml_xpath( xmldoc,
                                     where, &insertPoints );
    if( insertPoints ) {
        int namLen = 0;
        pool_setf (
            "removed: node '%s' from the document!\n",
             roxml_get_name( position[0],
                             (char*)what,
                             &namLen )
        );
        roxml_del_node( position[0] );
        setOption('c',"commit");
    } else
        setErrorText( "insert point not clear" );
    return pool_createStream( &pool_get_ex );
}

void attributeToTempCycle(node_t* atr)
{
    int length;
    pool_setf( " %s=", roxml_get_name( atr, NULL, NULL ) );
    roxml_release(RELEASE_LAST);
    pool_setf( "\"%s\"", roxml_get_content( atr, NULL, NULL, &length ) );
    roxml_release( RELEASE_LAST );
}

void attributesToTempCycle( node_t* node )
{
    int attrs=roxml_get_attr_nb(node);
    for(int c=0;c<attrs;c++)
        attributeToTempCycle( roxml_get_attr(node,NULL,c) );
}

void textToTempCycle(node_t* node)
{
    if(roxml_get_txt_nb(node)){
        int l=0;
        pool_set( roxml_get_content( roxml_get_txt(node,0),NULL,NULL,&l) );
        roxml_release(RELEASE_LAST);
    }
}

int subtreeToTempCycle( node_t* );
void printNodeToTempCycle( node_t* node )
{
	DEBUGLOG("enter");
    char ouputBuffer[MAX_NAM_LEN];
    int contentLength = 0;
    char* textbuffer = NULL;
    int nodetype = roxml_get_type(node);
    switch(nodetype) {
    case ROXML_ATTR_NODE:
        attributeToTempCycle( node );
        pool_set("\n");
        break;
    case ROXML_ELM_NODE:
        textbuffer = roxml_get_name( node, &ouputBuffer[0], MAX_NAM_LEN );
        pool_setf( "<%s", textbuffer );
        attributesToTempCycle(node);
        pool_set( ">" );
        textToTempCycle(node);
        subtreeToTempCycle(node);
        pool_setf( "</%s>\n", textbuffer );
        break;
    case ROXML_TXT_NODE:
        textbuffer = roxml_get_content( node, NULL, NULL, &contentLength );
        pool_set( textbuffer );
        roxml_release( RELEASE_LAST );
        break;
    }
}

int subtreeToTempCycle( node_t* parent )
{
    bool firstNode = true;
    const int count = roxml_get_nodes_nb( parent ,CONTENT_NODES );
    for(int i=0;i<count;i++) {
        node_t* node = roxml_get_chld( parent, NULL, i );
        if( firstNode ) {
            firstNode = false;
            pool_set( "\n\t" );
        } printNodeToTempCycle( node );
    } return count;
}

PoolStream xmlNow_evaluXPath( const char* fileName, const char* xPathExp )
{
    DEBUGLOG("enter");
    node_t* xmldoc = currentDocument = roxml_load_doc( fileName );
    int numberOfNodes = 0;
    node_t** nodes = roxml_xpath( xmldoc, xPathExp, &numberOfNodes );
    if(numberOfNodes) {
        pool_setCheckpoint();
        for( int n=0; n < numberOfNodes; n++ ) {
            printNodeToTempCycle( nodes[n] );
        } return pool_createStream( &pool_collectCheckpoint_ex );
    } setErrorText( "no nodes returned" );
    return pool_createStream( &pool_get_ex );
}

PoolStream xmlNow_perform( node_t* xmldoc, const char* action )
{
	DEBUGLOG("enter");
    if( isModus(action) ) {
        char modus = action[0];
		DEBUGFMT("modus: %s",action);
        if( !switchIdx( 'W', indexOfName(rawNext(modus)) ) ) {
            setErrorText(pool_setf("'%s' operation is missing 'where' argument",action));
        } else if( (!switchIdx('E',indexOfName(nextRaw())))  && (modus!='g') ) {
            setErrorText(pool_setf("'%s' operation is missing 'what' argument",action));
        } else switch (modus) {
            case 'a': return xmlNow_addElement( xmldoc, getName('W'), getName('E') );
            case 'g': return xmlNow_evaluXPath(         getName('x'), getName('W') );
            case 'r': return xmlNow_remElement( xmldoc, getName('W'), getName('E') );
            case 's': return xmlNow_setElement( xmldoc, getName('W'), getName('E') );
        } ExitOnError(action);
    } return pool_createStream( &pool_get_ex );
}


#ifdef xmlNow
int main(int argc,char**argv)
{
    if(( !InitCommandLiner(argc,argv) )
      || hasOption('h') )
        exit( USAGE(xmlNow) );

    setSwitch('c');
    ensure('x',"no xml to work on given");
	
	if( search('f') ) {
		printf( "document formating set to: %s\n",
				xmlNow_setFormating(getName('f'))
				? "ENABLED" : "DISABLED" );
	}
	
    if( hasOption('v') ) {
        setOption('v',"verbose" );
        showOptions();
    }

    ExitOnError( "Parameter" );

    currentDocument = roxml_load_doc( getName('x') );
	if ( !currentDocument ) {
		setErrorText( pool_setf( "file: %s", getName('x') ) );
	}
	
	ExitOnError( "Loading" );
	
    PoolStream xmlout;
	char* output="";
    if( search('p') ) {
        xmlout = xmlNow_evaluXPath( getName('x'),
		                            getName('p') );
    } else if( isAnyOtherModusThen( "verbose" ) ) {
		cmLn moddus = getModusNumber(1);
		DEBUGFMT( "will perform: %s", moddus )
        xmlout = xmlNow_perform( currentDocument, moddus );
	} while( output = poolStream( xmlout ) )
		printf( output );
		
    printf("\n");

    if( isModus("commit") )
        xmlNow_commitChanges( currentDocument, "xmlNow_tempfile.tmp" );
	else
		AtXmlExitNow();

    exit( CheckForError() );
 }
 #endif

