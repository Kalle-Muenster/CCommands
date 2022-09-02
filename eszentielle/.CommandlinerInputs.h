#ifdef  BeginUserinputRequest
#define DatasetRequestName BeginUserinputRequest

# ifdef EndOfUserinputRequest
    #undef EndOfUserinputRequest
    #undef DatasetRequestName
    #undef BeginUserinputRequest
    #undef InitDatasetRequest
    #undef CreateNumericInputRequest
    #undef CreateBooleanDecision
# else

#define InitDatasetRequest(MaxInputLength) const unsigned DatasetRequestName ## MAX_DIG_NUM = MaxInputLength; \
char DatasetRequestName ## _inputBuffer[MaxInputLength+1] = {'\0'}

#define CreateNumericInputRequest(request,defaultvalvar,option,digits) DatasetRequestName ## _inputBuffer[ DatasetRequestName ## MAX_DIG_NUM ]=digits; \
DatasetRequestName ## _inputBuffer[DatasetRequestName ## MAX_DIG_NUM-1]='\0'; printf(request); scanf(" %c",&DatasetRequestName ## _inputBuffer[0]); \
while( DatasetRequestName ## _inputBuffer[ DatasetRequestName ## MAX_DIG_NUM ] \
    && DatasetRequestName ## _inputBuffer[ digits-DatasetRequestName ## _inputBuffer[DatasetRequestName ## MAX_DIG_NUM] ]>='0' \
    && DatasetRequestName ## _inputBuffer[ digits-DatasetRequestName ## _inputBuffer[DatasetRequestName ## MAX_DIG_NUM] ]<='9' ) { \
       DatasetRequestName ## _inputBuffer[DatasetRequestName ## MAX_DIG_NUM]--; \
       scanf("%c",&DatasetRequestName ## _inputBuffer[ digits-DatasetRequestName ## _inputBuffer[DatasetRequestName ## MAX_DIG_NUM] ]); \
} if( !DatasetRequestName ## _inputBuffer[DatasetRequestName ## MAX_DIG_NUM] ) sscanf(&DatasetRequestName ## _inputBuffer[0],"%i",&defaultvalvar); \
sprintf(&DatasetRequestName ## _inputBuffer[0],"%i",defaultvalvar); \
setOption(option,&DatasetRequestName ## _inputBuffer[0])

#define CreateBooleanDecision(request,default,option,values) printf(request); \
scanf(" %c",setOption(option,default?&values[0]:&values[2]))

#define CreateStringInputRequest(request,defaultVar,option) printf(request); \
scanf(" %s",defaultVar=(char*)setOption(option,defaultVar))

# endif
#endif
