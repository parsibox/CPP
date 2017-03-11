#include"CdrClient.hpp"
#include <sstream>
#include"EventLog.h"
#include"DBOperations.h"

extern CEventLog CG_EventLog;

bool CCdrClient::mcfn_sendMsgToCdr(DeliverToEsme *pCL_msg){
	CdrMsg *pCL_Msg= new CdrMsg();
	pCL_Msg->set_msg_type(CdrMsg::USSD_SUBMIT);
	pCL_Msg->mutable_submit_msg()->CopyFrom(*pCL_msg);
	if(0 == pCL_msg->status() || false == mcC_retryPlanner.mcfn_getCheckForRetry(pCL_msg))
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
		if(!pCL_Msg->submit_msg().service_url().empty()){
			mcfn_doHttpHit(pCL_Msg);
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

bool CCdrClient::mcfn_doHttpHit(CdrMsg *&pCL_Msg){
	DBG_INFO((CG_EventLog),("doing HTTP Hit URL Name:%s",pCL_Msg->submit_msg().service_url().c_str()));
	mysqlpp::StoreQueryResult CL_ResultSet;
	CDBOperations CL_dbObj;
	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
	CL_QueryStream<<"SELECT * from URL_DETAILS WHERE URL_NAME='"<<pCL_Msg->submit_msg().service_url()<<"' AND STATUS='A' limit 1";
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		if(CL_ResultSet.num_rows()>0){
		std::string CL_res="";
			std::stringstream CL_req;
			CL_req<<CL_ResultSet[0]["url"].c_str();
			char param[1024]={0x00};
			sprintf(param,CL_ResultSet[0]["param"].c_str(),pCL_Msg->submit_msg().da().c_str(),pCL_Msg->submit_msg().task_id());
			bool isHttpSucess = false;
			if(CL_ResultSet[0]["method"].compare("GET"))
			{
				CL_req<<param;
				pCL_Msg->mutable_submit_msg()->set_service_url(CL_req.str());
				isHttpSucess = meC_httpObj.mcfn_doHttpGetReq(pCL_Msg->submit_msg().service_url(),CL_res);
			}
			else
			{
				meC_httpObj.mcfn_initialise(CL_req.str());
				pCL_Msg->mutable_submit_msg()->set_service_url(CL_req.str());
				isHttpSucess = meC_httpObj.mcfn_doHttpPostReq(param,CL_res);
			}
			if(isHttpSucess){
				pCL_Msg->mutable_submit_msg()->set_service_url_status(atoi(CL_res.c_str()));
			}
			else{
				pCL_Msg->mutable_submit_msg()->set_service_url_status(1001);
			}

			return isHttpSucess;
		}
		else{
			DBG_CRITICAL((CG_EventLog),("No URL is configured :%s",pCL_Msg->submit_msg().service_url().c_str()));
		}
	}
	return false;
}

