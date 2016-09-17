#ifndef BASE_64_ENCODER
#define BASE_64_ENCODER base64.cc

#define uInt(fromChars) 			(*(unsigned*)fromChars)
#define asFrame(fromChars)			(*(Frame*)fromChars)
#define MAXIMUM_BASE64_DATA_SIZE 	(4096)

typedef unsigned char byte;

typedef union Frame {
	unsigned u32;
	byte 	 u8[4];
	char 	 i8[4];
} Frame;

int				encodeFile(char* dst,const char* inputFileName);
int				decodeFile(char* dst,const char* inputFileName);
int				encodeString(char* dst,const char* src);
int				decodeString(char* dst,const char* src);
Frame			encodeFrame(unsigned threeByte);
Frame			decodeFrame(char* fourChars);
byte*			removeHeader(byte* data,unsigned* cbData);
byte*			removeLineBreaks(byte* data,unsigned* cbData);

#ifdef _OnTheFly_
#include "base64.cc"
#endif
#endif