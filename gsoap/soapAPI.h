//gsoap ns service name: soapAPI
//gsoap ns service style: rpc 

int ns__add(int num1, int num2, int* result );
int ns__sub(int num1, int num2, int* result );
int ns__mult( int num1, int num2, int *result);
int ns__divid( int num1, int num2, int *result);
int ns__UpdateNotice(std::string& result);
int ns__PublishNotice(char* str,int* result );

