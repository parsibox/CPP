#include"CdrClient.hpp"
#include <sstream>
#include"EventLog.h"
extern CEventLog CG_EventLog;

bool CCdrClient::mcfn_sendMsgToCdr(DeliverToEsme *pCL_msg){
	CdrMsg *pCL_Msg= new CdrMsg();
	pCL_Msg->set_msg_type(CdrMsg::USSD_SUBMIT);
	pCL_Msg->mutable_submit_msg()->CopyFrom(*pCL_msg);
	meC_cdrQue.mcfb_insertIntoQue(pCL_Msg);
	return true;
}
bool CCdrClient::mcfn_run(int iL_threadId,void *vL_arg){
	while(1){
		CdrMsg *pCL_Msg=NULL;
		if(meC_cdrQue.mcfb_getNextMessage(pCL_Msg,true)==false){
			DBG_DEBUG((CG_EventLog), ("No data Found to Send Cdr going for sleep for 2 sec"));
			sleep(2);
			continue;
		}
		if(pCL_Msg->submit_msg().service_url().empty()==false){
			std::string CL_res="";
			std::stringstream CL_req;
			CL_req<<pCL_Msg->submit_msg().service_url()<<pCL_Msg->submit_msg().da();
			pCL_Msg->mutable_submit_msg()->set_service_url(CL_req.str());
			if(meC_httpObj.mcfn_doHttpGetReq(pCL_Msg->submit_msg().service_url(),CL_res)){
				pCL_Msg->mutable_submit_msg()->set_service_url_status(atoi(CL_res.c_str()));
			}
			else{
				pCL_Msg->mutable_submit_msg()->set_service_url_status(1001);
			}
		}
		std::string CL_msg="";
		if(pCL_Msg->SerializeToString(&CL_msg)){
			if(mcfn_sendToTcp(CL_msg)==false){
				meC_cdrQue.mcfb_insertIntoQue(pCL_Msg);
				sleep(1);
				DBG_INFO((CG_EventLog),("Send Fail Happend to CdrServer"));
			}
			else{
				DBG_INFO((CG_EventLog),("[%d]Sent to Cdr Server:%s",iL_threadId,pCL_Msg->ShortDebugString().c_str()));
				delete pCL_Msg;
				pCL_Msg=NULL;
			}	
		}
		else{
			DBG_ERROR((CG_EventLog),("Failed to Serialise:%s",pCL_Msg->ShortDebugString().c_str()));
			delete pCL_Msg;
			pCL_Msg=NULL;
		}	
	}
}
