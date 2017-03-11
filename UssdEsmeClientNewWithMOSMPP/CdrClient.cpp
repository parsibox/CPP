#include"CdrClient.hpp"
#include <sstream>
#include"EventLog.h"
#include"DBOperations.h"
#include<template.h>

extern CEventLog CG_EventLog;

bool CCdrClient::mcfn_sendMsgToCdr(DeliverToEsme *pCL_msg,std::vector<std::string>& userInputs){
	CdrMsg *pCL_Msg= new CdrMsg();

	if(!userInputs.empty())
	{
		std::string userInputStr="";
		for(int i=0;i<userInputs.size();i++)
		{
			DBG_INFO((CG_EventLog), ("User Input %d:%s",i,userInputs[i].c_str()));	
			if(!userInputStr.empty())
			userInputStr.append("*");
			userInputStr.append(userInputs[i]);
		}
		pCL_msg->set_user_input(userInputStr);
	}
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
		if(pCL_Msg->submit_msg().service_url().empty()==false){
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
//			char param[1024]={0x00};
			std::string param;
			ctemplate::TemplateDictionary dict("url");
			dict.SetValue("DA", pCL_Msg->submit_msg().da().c_str());
			dict.SetValue("OA",pCL_Msg->submit_msg().oa().c_str());
			char taskId[10]={0x00};
 			sprintf(taskId,"%03d",pCL_Msg->submit_msg().task_id());
			dict.SetValue("TASK_ID",taskId);
			dict.SetIntValue("UNIQUE_ID", pCL_Msg->submit_msg().msg_id());
			dict.SetIntValue("STATUS",pCL_Msg->submit_msg().status());
			dict.SetValue("TRAVERSOL_PATH", pCL_Msg->submit_msg().msg().c_str());
			ctemplate::StringToTemplateCache(pCL_Msg->submit_msg().service_url(),CL_ResultSet[0]["param"].c_str(), ctemplate::DO_NOT_STRIP);
			ctemplate::ExpandTemplate(pCL_Msg->submit_msg().service_url(), ctemplate::DO_NOT_STRIP, &dict, &param);	
			//sprintf(param,CL_ResultSet[0]["param"].c_str(),pCL_Msg->submit_msg().da().c_str(),pCL_Msg->submit_msg().task_id());
			bool isHttpSucess = false;
			CHttpInterface CL_httpObj;
			if(CL_ResultSet[0]["method"].compare("GET")==0)
			{
				CL_req<<"?"<<param;
				pCL_Msg->mutable_submit_msg()->set_service_url(CL_req.str());
				isHttpSucess = CL_httpObj.mcfn_doHttpGetReq(pCL_Msg->submit_msg().service_url(),CL_res);
			}
			else
			{
				CL_httpObj.mcfn_initialise(CL_req.str());
				//CL_httpObj.mcfn_setHeader("Content-Type: application/json");
				if(CL_ResultSet[0]["content_type"].length()>0)
				CL_httpObj.mcfn_setHeader(CL_ResultSet[0]["content_type"].c_str());
				pCL_Msg->mutable_submit_msg()->set_service_url(CL_req.str());
				isHttpSucess = CL_httpObj.mcfn_doHttpPostReq(param,CL_res);
			}
			DBG_CRITICAL((CG_EventLog),("Responce from HTTP server  :%s",CL_res.c_str()));
			if(isHttpSucess){
				pCL_Msg->mutable_submit_msg()->set_service_url_status(CL_res.c_str());
			}
			else{
				pCL_Msg->mutable_submit_msg()->set_service_url_status("1001");
			}
			return isHttpSucess;
		}
		else{
			DBG_CRITICAL((CG_EventLog),("No URL is configured :%s",pCL_Msg->submit_msg().service_url().c_str()));
		}
	}
	return false;
}

