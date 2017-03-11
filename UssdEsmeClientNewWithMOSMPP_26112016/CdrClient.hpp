#ifndef __CCDRCLIENT_HPP_
#define __CCDRCLIENT_HPP_
#include"TcpClient.h"
#include"Thread.h"
#include"DeliverToEsme.pb.h"
#include"CdrMsg.pb.h"
#include"STSMsgQue.h"
#include"HttpInterface.h"
#include"RetryPlaner.hpp"
class CCdrClient:public CTcpClient,public CThread
{
	private:
		CMsgQue <CdrMsg*> meC_cdrQue;
		CHttpInterface meC_httpObj;
	public:
		CRetryPlanner 	mcC_retryPlanner;	
	public:
		CCdrClient():CTcpClient("CdrServer"){
			mcfn_start();
		}
		bool mcfn_sendMsgToCdr(DeliverToEsme *);
		//virtual function
		bool mcfn_onConnect(){
			return true;
		}
		bool mcfn_run(int iL_threadId,void *vL_arg);
		std::string mcfn_getThreadName(void*){
			return "CdrSendThread";
		}

};
#endif
