#include"EsmeConnection.hpp"
#include "Externs.h"
#include "Menu.hpp"
#include"SMPP.hpp"
#include"ErrorCodes.hpp"
CEsmeConnection::CEsmeConnection():CTcpClient("XML",10){
	bL_isLoginSucess=false;	
	mcfn_start();
}

CEsmeConnection::~CEsmeConnection(){

}

//virtual function from CTcpClinet class
bool CEsmeConnection::mcfn_onConnect(){
	sleep(1);
	if(!mcfn_sendUssdBind()){
		DBG_ERROR((CG_EventLog),("Error in sending login request reconnecting"));
		return false;
	}
	// receive the responce and check responce is fine or not if it is not fine return false ,
	//so that connection will be closed and reconnected by TcpClinet class	
	bL_isLoginSucess=mcfn_receiveData();
	return bL_isLoginSucess;
}
bool CEsmeConnection::mcfn_receiveData(){
	if(!bL_isLoginSucess && !mcfn_isDataAvailable(30000000)){
		DBG_ERROR((CG_EventLog),("Login Responce didn't came in 30 sec so reconnecting"));
		return false;	
	}
	ReceviedData CL_recData;
	CL_recData.pmcu8_data.resize(HEADER_LENGTH);
	if(mcfn_receive((char*)&CL_recData.pmcu8_data[0],HEADER_LENGTH)){//receiving header
		CL_recData.mcC_header.mcfn_setHeader((u8*)&CL_recData.pmcu8_data[0]);
		CL_recData.mcsi_len= CL_recData.mcC_header.mcsi_cmdLength;	
		if(CL_recData.mcsi_len>HEADER_LENGTH){
			 CL_recData.pmcu8_data.resize(CL_recData.mcsi_len);
			 if(mcfn_receive((char*)&CL_recData.pmcu8_data[HEADER_LENGTH],CL_recData.mcsi_len-HEADER_LENGTH)){
				 DBG_INFO((CG_EventLog),("Receiveing the data from SMPP[%ld]:%s",CL_recData.mcsi_len,CL_recData.mcC_header.mcfn_ShortDebugString().c_str()));
				 CG_EventLog.mcfn_logU8Message(&CL_recData.pmcu8_data[0],CL_recData.mcsi_len);
				 if(!bL_isLoginSucess){
				 	DBG_INFO((CG_EventLog),("Login Responce: %s",CL_recData.mcC_header.mcfn_ShortDebugString().c_str()));
					 return CEsmeClient::mcfn_processLoginRes((const char*)&CL_recData.pmcu8_data[HEADER_LENGTH],CL_recData.mcsi_len);
				}
				else{
					meC_RecDataQue.mcfb_insertIntoQue(CL_recData);	
				}	
			}
		}
	}
	return true;
}

bool CEsmeConnection::mcfn_run(int,void *){
	while(1){
	 	ReceviedData CL_recData;
		if(!meC_RecDataQue.mcfb_getNextMessage(CL_recData,false)){	
			sleep(1);
		}
		else{
			//todo::need to check retry errors 
			mcfn_processReceivedMessage(CL_recData.mcC_header,&CL_recData.pmcu8_data[HEADER_LENGTH],CL_recData.mcsi_len);
		}
	}
	return true;
}
std::string CEsmeConnection::mcfn_getThreadName(void*){
	return "PrecessReceivedSmppPacketThread";
}
//virtual function from EsmeClinet Class
bool CEsmeConnection::mcfn_sendMsgToServer( u8* buf, int len){
	CG_EventLog.mcfn_logU8Message(buf,len);
	if(mcfn_send((const char*)buf,len))
		return true;

	mcfn_close();
	return false;

}

bool CEsmeConnection::mcfn_onUssdBegin(CHeader& CL_header,CUssdMsg &CL_pdu){
	return true;
}
bool CEsmeConnection::mcfn_onUsddContinue(CHeader& CL_header,CUssdMsg &CL_pdu){
	int iL_temp=0;
	//swaping 
	MsgTypes *pcL_Msg=NULL;
	bool bL_isLeaf=false;
	std::string CL_url="";
	std::string CL_msg="";
	DBG_VERBOSE((CG_EventLog), ("Received Continue Message:%s",CL_pdu.mcfn_ShortDebugString().c_str()));
	//TODO::need to find the next menu according to user input
	if(CG_seqMap.mcfb_findElement(CL_pdu.mcC_requestId,pcL_Msg)){
		DBG_INFO((CG_EventLog), ("Session found :%s,Analysing User Input:%s", CL_pdu.mcC_requestId.c_str(),CL_pdu.mcC_userData.c_str()));
		pcL_Msg->meC_traversolPath.append("*");
		pcL_Msg->meC_traversolPath.append(CL_pdu.mcC_userData.c_str());

		CL_msg=CMenu::mcfnS_getLeafData(pcL_Msg->pmcC_Session,CL_pdu.mcC_userData.c_str(),pcL_Msg->pmcC_EsmeMsg->username().c_str(),pcL_Msg->pmcC_EsmeMsg->srv_code().c_str(),bL_isLeaf,CL_url);

		pcL_Msg->pmcC_EsmeMsg->set_receiver_sessionid(atoi(CL_pdu.mcC_requestId.c_str()));
		CTimerManeger::mcfnS_GetInstance()->mcfn_unregisterTimer((ITimer*)pcL_Msg);	
		if(CL_msg.empty()||bL_isLeaf){
			CL_msg.assign("Thanks for using this service");
			DBG_INFO((CG_EventLog), ("Session found :%s,Sending Continue Release Message :%s", CL_pdu.mcC_requestId.c_str(),CL_msg.c_str()));	
			CL_pdu.mcC_userData=CL_msg;
			mcfn_sendUssdEnd(CL_pdu,CL_header.mcsi_cmdSessionId);
			pcL_Msg->pmcC_EsmeMsg->set_session_endtime(time(NULL));
			pcL_Msg->pmcC_EsmeMsg->set_status(0);
			pcL_Msg->pmcC_EsmeMsg->set_msg(pcL_Msg->meC_traversolPath);
			CG_seqMap.mcfb_removeElement(CL_pdu.mcC_requestId.c_str());
			pcL_Msg->pmcC_EsmeMsg->set_service_url(CL_url);
			CG_CdrClient.mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg);
			delete pcL_Msg;
		}
		else{
			DBG_INFO((CG_EventLog), ("Session found :%s,Sending Continue Message :%s", CL_pdu.mcC_requestId.c_str(),CL_msg.c_str()));
			CL_pdu.mcC_userData=CL_msg;	
			mcfn_sendUssdContinue(CL_pdu,CL_header.mcsi_cmdSessionId);
			CTimerManeger::mcfnS_GetInstance()->mcfn_registerTimer((ITimer*)pcL_Msg,CG_Cfg.mcfn_getUssdTimeout(),NULL);
		}
	}
	else{
		DBG_CRITICAL((CG_EventLog), ("Session is not Active Sending  Abort [%s]",CL_pdu.mcC_requestId.c_str()));	
		//mcfn_sendUssdAbort(CL_header);	
	}
	return true;
}
bool CEsmeConnection::mcfn_onUssdEnd(CHeader& CL_header,CUssdMsg& CL_pdu){
	MsgTypes *pcL_Msg=NULL;
	DBG_VERBOSE((CG_EventLog), ("Received End Message:%s",CL_pdu.mcfn_ShortDebugString().c_str()));
	if(CG_seqMap.mcfb_getElement(CL_pdu.mcC_requestId.c_str(),pcL_Msg)){
		DBG_INFO((CG_EventLog), ("Session found :%s,Releaseing the session:%s", CL_pdu.mcC_requestId.c_str(), CL_pdu.mcC_userData.c_str()));
		pcL_Msg->pmcC_EsmeMsg->set_session_endtime(time(NULL));
		pcL_Msg->pmcC_EsmeMsg->set_status(0);
		CG_CdrClient.mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg);
	}
	else{
		DBG_INFO((CG_EventLog), ("Session not found :%s,Session Already Releaseed:%s", CL_pdu.mcC_requestId.c_str(),CL_pdu.mcC_userData.c_str()));
	}
	return true;
}
bool CEsmeConnection::mcfn_onUssdAbort(CHeader& CL_header){
	/*MsgTypes *pcL_Msg=NULL;
	DBG_CRITICAL((CG_EventLog), ("Ussd Abort Cause:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
	if(CG_seqMap.mcfb_getElement(CL_header.mcsi_senderCB,pcL_Msg)){
		DBG_INFO((CG_EventLog), ("Session found :%d", CL_header.mcsi_senderCB));
		pcL_Msg->pmcC_EsmeMsg->set_session_endtime(time(NULL));
		pcL_Msg->pmcC_EsmeMsg->set_status(CL_header.mcsi_cmdStatus);
		CG_CdrClient.mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg);
	}
	else{
		DBG_INFO((CG_EventLog), ("Session not found :%d,Session Already Releaseed", CL_header.mcsi_senderCB));
	}	*/
}
bool CEsmeConnection::mcfn_onUssdShake(CHeader& CL_header){
	DBG_CRITICAL((CG_EventLog), ("Ussd Shake:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
}
bool CEsmeConnection::mcfn_onUssdShakeResp(CHeader& CL_header){
	DBG_CRITICAL((CG_EventLog), ("Ussd Shake Resp:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
}
