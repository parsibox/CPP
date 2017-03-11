#ifndef _STS_EXCEPTION_H
#define _STS_EXCEPTION_H

class   STSGenException
{
public:
	
	STSGenException(int code, const char* resn="");
	char* reasonString(char* res);
	char* reasonString();
	int reasonCode();
	
protected:
private:
	char meca_Reason[100];
	int mei_ErrCode;
};
class STSTimeoutException
{

};
#endif
