#ifndef __ERRORCODES_HPP_
#define __ERRORCODES_HPP_
#include<string>
#include"MsgMap.h"
#include"AutoLocker.h"
typedef struct  _ErrorDetails{
	std::string mcC_errDescription;
	int mcsi_isRetry;
}ErrorDetails;
class CErrorCodes{
	
	private:
		CMsgMap<int,ErrorDetails> meC_errorDetailsMap;
		static CErrorCodes *pmeC_Instance;
		CCriticalSection meC_errLock;
	private:
		CErrorCodes(){}
	public:
		static CErrorCodes* mcfnS_GetInstance();
		std::string mcfn_getErrorString(int iL_errorCode);
		int mcfn_getErrorFlags(int iL_errorCode);
		void mcfn_loadErrorDetails();
};

#endif
