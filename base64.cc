#ifndef _OnTheFly_ 
#include "base64.h"
#endif

#include <stdio.h>
#include "commandLiner.h"

enum HEADER {
	
	NO_HEADERS = -1,	
	BASE64_BEGIN, 
	ENDOF_BASE64, 
	PLAIN_TEXT_BEGIN, 
	ENDOF_PLAIN_TEXT,
		
NUMBER_OF_HEADER};

byte data[MAXIMUM_BASE64_DATA_SIZE];
unsigned _loadedSize=0;
char _codeTableBuffer[65];

const char* header[NUMBER_OF_HEADER] = 
			{ "-------BEGIN_BASE64-------\n",
			"\n-------ENDOF_BASE64-------",
			  "-----BEGIN_PLAIN_TEXT-----\n",
			"\n-----ENDOF_PLAIN_TEXT-----" };

const char* hartKodierteTabelle = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/=";
		   


const char* CodeTable = (const char*)&hartKodierteTabelle[0];
			

// fills the end of datastring by zeros so it's length will be multiplicate of 4
byte* fillEndByZeros(byte* pData,unsigned* cbData)
{
	byte rest = (*cbData)%4;
	if(rest)
	{
		rest = 4-rest;
		for(int i=0;i<rest;i++)
			pData[*cbData+i]='\0';
	}
	
	for(int i=0;i<4;i++)
		pData[*cbData + rest + i] = '\0';
	
	return pData;
}
					
//removes the: "-----SOME TEXT-----" heders from data if there may contained such.
byte* removeHeader(byte* pData,unsigned *cbData)
{
	if(!pData)
		return 0;
	
	// look for intro-header...
	short i=0,begin=0;
	while(pData[i]=='-')i++;
	if(i>3)	{
		while(pData[++i]!='-');
		while(pData[++i]=='-');
		while(pData[i]=='\n')i++;
		begin=i;
	} else// return if no header was found...
	{
		pData[*cbData]='\0';
		return &pData[0];
	}

	while(i<*cbData)
	{// find and remove end-header...
		if(pData[++i]=='-')
			i=(pData[i+1]=='-' && pData[i+2]=='-' && pData[i+3]=='-') ? 
														  *cbData = i : i;
	}// and agjust the data's size	
	pData[*cbData]='\0';
	*cbData -= begin;
	
	if(hasOption('v'))
		printf("removed headers: datasize now is %i bytes\n",*cbData);
	
	return &pData[begin];
}

// removes any line-breaks from data string and returns it manipulated..
byte* removeLineBreaks(byte* pData,unsigned *cbData)
{
	if(!pData)
		return 0;
	
	// when loading plain-text, no need for removing \n's
	if(hasOption('e'))
		return &pData[0];
	
	// remove linebreaks and rewrite data...
	short w,r,c;
	w=r=c=0;
	while(pData[w]=='\n')w++;
	short begin = w;
	r=begin;
	while( (r<*cbData) && (char)pData[r]!='\0' )
	{
		r=c+w;
		if(pData[r]=='\n') w++;
		else 
		{
			pData[c++]=pData[r];
			if(pData[r]=='=')
		        pData[r]='\0';
		}
	}
	pData[c]='=';
	pData[*cbData = ++c]='\0'; 
	*cbData-=begin;
	
	if(hasOption('v'))
		printf("removed line breaks: datasize now is %i bytes\n",*cbData);
	
	return &pData[begin];
}

// load a file.
byte* loadeFile(char* fileName)
{
	if(!hasOption('s'))
		printf("\nloading file: %s\n\n",fileName);
	_loadedSize=0;
	
	byte* dat = readFile(fileName);

	dat =  fillEndByZeros(
			removeLineBreaks(
				   removeHeader( dat, &_loadedSize ),
				 &_loadedSize), 
			   &_loadedSize);
			   
	if(hasOption('v'))
		printf("\nloaded Data:\n%s\n\n",(char*)dat);
	
	return dat;
}





// encodes one frame (3 byte binary-data to 4 chars base64-data)
Frame encodeFrame(unsigned threeByte)
{
	Frame out;
	for(int i=0;i<4;i++) {
		out.u8[i] = CodeTable[threeByte%64];
		threeByte>>=6; }
	return out;
}

// decode one frame (4 chars base64-data to 3 byte binary data)
Frame decodeFrame(char* fourChars)
{
	Frame out;
	out.u32=0;
	for(int i=0;i<4;i++)
	{
		if(fourChars[i]=='=') {
			out.u32+=(64<<(i*6));
			out.i8[3]=(4-i);
			return out;
		} else for(unsigned c = 0;c<65;c++)	{
			if(c==64) { 
				setError("Input is not base64-data!"); 
				out.i8[3] = fourChars[i];
				return out; 
			} else if(CodeTable[c]==fourChars[i]) {
				out.u32+=(c<<(i*6));
				c=65;
			}
		}
	}
	return out;
}


int encodeString(char* dst,const char* src)
{
	int lB,iS,iD;
	lB=iS=iD=0;

	while( src[iS]!='\0' )
	{		
		uInt(&dst[lB+iD]) = encodeFrame(uInt( &src[iS] )).u32;
		iD+=4; iS+=3;	
		
		if(iD%64 == 0)
			dst[iD+lB++]='\n';
	};

    iD += lB;
	dst[iD]='=';
	dst[++iD] ='\0';
	return iD;
}

int decodeString(char* dst,const char* src)
{
	int iS = -4; 
	int	iD = -3;
	int	iT =  0;

	while (! (iT = ( 
		asFrame(&dst[iD+=3]) = decodeFrame(&src[iS+=4])
		    ).i8[3]) );

	dst[iD+=(4-iT)]='\0';
	return iD;
}



int encodeFile(char* dst,const char* inputFileName)
{
	setOption('e',inputFileName);
	return encodeString(dst,(char*)loadeFile(inputFileName));
}
int decodeFile(char* dst,const char* inputFileName)
{
	setOption('d',inputFileName);
	return decodeString(dst,(char*)loadeFile(inputFileName));
}



//main kram...
const char* 	loadeCode(void);
byte* 			readFile(char* fileName);
int 			streamOut(FILE*,byte*,char);


int main(int argc, char **argv)
{
	CommandLineArgs(argc,argv);

	if(!hasOption('o'))
		setOption('o',"stdout");
	
	if(hasOption('v'))
		showOptions();
	
	if( ((!hasOption('e')) && (!hasOption('d'))) || hasOption('h'))
	{ 
		printf("\nUsage:\n\n");
		printf("    base64 [options] <[--e/--d]-SourceFile> [--o-OutputFile]\n\n\n");
		printf("    options:\n\n");
		printf("      --e-<input>       : encode: file <input> to base64.\n");
		printf("      --d-<input>       : decode: file <input> from base64 back to Utf-8\n");
		printf("      --o-<output>      : writes the ouput to file <output> instead to stdout.\n");
		printf("      --c-<codetable>   : define own coding table (64 chars + terminating '=' sign!)\n");
		printf("       -t               : interpret -d/-e/-c as string-input (rather then filenames).\n");
		printf("       -v               : print verbose details to stdout.\n");
		printf("       -s               : silent de-/en-coding.\n");
		printf("       -h               : print this help-message.\n\n");
		return !hasOption('h');
	}

	char Mode = hasOption('e') ? 'e'
			  : hasOption('d') ? 'd'  
			  : 0;
	
	CodeTable = loadeCode();
	
	if(hasOption('v'))
		printf("\nusing base64 code table:\n%s\n\n", CodeTable);
	
	int bytesWritten,bytesCoded;
	char outbuffer[4096];
	
	bytesCoded  = hasOption('t') ? Mode=='e' ? encodeString(&outbuffer[0],getName(Mode))
								 : Mode=='d' ? decodeString(&outbuffer[0],getName(Mode)) 
							:0	 : Mode=='e' ?   encodeFile(&outbuffer[0],getName(Mode))  
								 : Mode=='d' ?   decodeFile(&outbuffer[0],getName(Mode)):0;
	
	if((!hasOption('s')) && (!isModus("stdout")))
		printf("\n\n%s\n\n",&outbuffer[0]);
	
	bytesWritten = streamOut( isModus("stdout")
	                         ? stdout 
	                         : fopen(getName('o'),"w"),
			                  &outbuffer[0],
			                  (Mode=='d') * 2 );
	if(!hasOption('s'))
		printf( "\n\n%i bytes written to %s (with line breaks and headers)\n"
				"%i byte of data de/en coded (wihout headers or line breaks)\n",
				bytesWritten, getName('o'), bytesCoded );
	
	return 0;
}




int streamOut(FILE* outstream, byte* data, char writeHeader)
{
	int written = 0;
	if(writeHeader>=0)
	{
		written+=strlen(header[writeHeader]);
		fwrite(header[writeHeader++],1,written,outstream);
	}
	
	int size = strlen(&data[0]);
	fwrite(&data[0],1,size,outstream);
	written+=size;
	
	if(writeHeader>0)
	{
		size = strlen(header[writeHeader]);
		fwrite(header[writeHeader],1,size,outstream);
		written+=size;
	}
	
	if(!isModus("stdout")){
		fflush(outstream);
		fclose(outstream);
	}return written;
}

// open the file fileName, reads it's content and retuns a pointer to the data it red, 
// writes the file's size to the _loadedSize variable. 
byte* readFile(char* fileName)
{
	FILE* file = fopen(fileName,"r");
	if(file)
	{	
		fseek(file,0,SEEK_END);
		_loadedSize = ftell(file);
		fseek(file,0,SEEK_SET);
		fread(&data[0],1,_loadedSize,file);
		fflush(file);
		fclose(file);
		data[_loadedSize++]='\0';


		if(hasOption('v'))
			printf("raw data read from file: %i bytes\n",_loadedSize);
		
		return &data[0];
	}
	else
	{
		char xeption[64];
		sprintf(&xeption[0],"file: %s not found !\n",fileName);
		setError(&xeption[0]);
		return 0;
	}
}


const char* saveTableFile(char* b64table)
{
	if( hasOption('e')
	&&  hasOption('c') )
	{
		char codeFileName[128] = {'\0'};
		sprintf(&codeFileName[0],"%s_table.dat",getName('e'));
		FILE* b64TableFile = fopen(&codeFileName[0],"w");
		fwrite(b64table,1,65,b64TableFile);
		fflush(b64TableFile);
		fclose(b64TableFile);
		
		if(!hasOption('s'))
			printf("Saved given base64 table to file: %s\n",&codeFileName[0]);
	}
	
	return (const char*)b64table;
}

const char* loadeCode(void)
{
	char* coderLoader = hasOption('c') ? 
						  getName('c') : &hartKodierteTabelle[0];
	int i=-1;
	while(coderLoader[++i]!='\0')
		if((coderLoader[i]=='=')&&(i>=64))
			return saveTableFile(&coderLoader[i-64]);
	
	FILE* codeFile = fopen(coderLoader,"r");
	if(codeFile) {
		fread(&_codeTableBuffer[0],1,65,codeFile);
		fflush(codeFile);
		fclose(codeFile);
		return &_codeTableBuffer[0];
	} else
		return &hartKodierteTabelle[0];
}


