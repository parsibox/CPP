#include "STSException.h"
#include <string.h>
	STSGenException::STSGenException(int code, const char* resn)
	{
		
		if(strlen(resn)<99)
			strcpy(meca_Reason,resn);
		else
		{
			memcpy(meca_Reason, resn, 99);
				meca_Reason[99]=0;
		}
		mei_ErrCode = code;
		
	}
	char* STSGenException::reasonString(char* res)
	{
		strcpy(res,meca_Reason);
		return res;
	}
	char* STSGenException::reasonString()
	{
		return  meca_Reason;
	}
	int STSGenException::reasonCode()
	{
		return mei_ErrCode;
	}
