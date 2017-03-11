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
        
	DBG_INFO((CG_EventLog),("Login :%s",bL_isLoginSucess?"Successful":"Failed"));
	return bL_isLoginSucess;
}
bool CEsmeConnection::mcfn_receiveData(){
	if(!bL_isLoginSucess && !mcfn_isDataAvailable(30000000)){
		DBG_ERROR((CG_EventLog),("Login Responce didn't came in 30 sec so reconnecting"));
		return false;	
	}
	ReceviedData CL_recData;
	CL_recData.pmcu8_data.resize(OWN_HEADER_LENGTH);
	if(mcfn_receive((char*)&CL_recData.pmcu8_data[0],OWN_HEADER_LENGTH)){//receiving header
		CL_recData.mcC_header.mcfn_setHeader((u8*)&CL_recData.pmcu8_data[0]);
	        CG_EventLog.mcfn_logU8Message((u8*)&CL_recData.pmcu8_data[0],OWN_HEADER_LENGTH);
		DBG_INFO((CG_EventLog),("Receiveing the Header from SMPP:%s",CL_recData.mcC_header.mcfn_ShortDebugString().c_str()));
		CL_recData.mcsi_len= CL_recData.mcC_header.mcsi_cmdLength;	
		if(CL_recData.mcsi_len>OWN_HEADER_LENGTH){
			 CL_recData.pmcu8_data.resize(CL_recData.mcsi_len+1);
			 if(mcfn_receive((char*)&CL_recData.pmcu8_data[OWN_HEADER_LENGTH],CL_recData.mcsi_len-OWN_HEADER_LENGTH)){
                                  CL_recData.pmcu8_data[CL_recData.mcsi_len] =0x00;
				 DBG_INFO((CG_EventLog),("Receiveing the data from SMPP[%ld]:%s",CL_recData.mcsi_len,CL_recData.mcC_header.mcfn_ShortDebugString().c_str()));
				 CG_EventLog.mcfn_logU8Message(&CL_recData.pmcu8_data[0],CL_recData.mcsi_len);
				 if(!bL_isLoginSucess){
				 	DBG_INFO((CG_EventLog),("Login Responce: %s",CL_recData.mcC_header.mcfn_ShortDebugString().c_str()));
					 return CEsmeClient::mcfn_processLoginRes((const char*)&CL_recData.pmcu8_data[OWN_HEADER_LENGTH],CL_recData.mcsi_len);
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
			mcfn_processReceivedMessage(CL_recData.mcC_header,&CL_recData.pmcu8_data[OWN_HEADER_LENGTH],CL_recData.mcsi_len);
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

bool CEsmeConnection::mcfn_onUssdBegin(CXmlHeader& CL_header,CXmlUssdMsg &CL_pdu){
	MsgTypes *pcL_Msg= new MsgTypes;
	pcL_Msg->pmcC_Session=new Session();
	pcL_Msg->pmcC_EsmeMsg =new DeliverToEsme();
        pcL_Msg->pmcC_EsmeMsg->set_username(mcfn_getUserName());
        pcL_Msg->pmcC_EsmeMsg->set_srv_code(CL_pdu.mcC_starCode);
	std::string CL_msg=CMenu::mcfnS_getRootMenuData(mcfn_getUserName().c_str(),CL_pdu.mcC_starCode.c_str(),"",pcL_Msg->pmcC_Session);
	if(CL_msg.empty()){
		DBG_VERBOSE((CG_EventLog),("Menu Not cofigure for this message:%s[%s]",CL_pdu.mcC_starCode.c_str(),mcfn_getUserName().c_str()));
		delete pcL_Msg;
                return false;
	}
	CL_pdu.mcC_userData=CL_msg;
	DBG_VERBOSE((CG_EventLog),("Message sending to server:%s",CL_msg.c_str()));
	CG_seqMap.mcfb_insert( CL_header.mcsi_sessionId[0],pcL_Msg);
	mcfn_sendUssdContinue(CL_pdu,CL_header.mcsi_sessionId);
	return true;
}
bool CEsmeConnection::mcfn_onUsddContinue(CXmlHeader& CL_header,CXmlUssdMsg &CL_pdu){
	int iL_temp=0;
	//swaping 
	MsgTypes *pcL_Msg=NULL;
	bool bL_isLeaf=false;
	std::string CL_url="";
	std::string CL_msg="";
	DBG_VERBOSE((CG_EventLog), ("Received Continue Message:%s",CL_pdu.mcfn_ShortDebugString().c_str()));
	//TODO::need to find the next menu according to user input
	if(CG_seqMap.mcfb_findElement(CL_header.mcsi_sessionId[0],pcL_Msg)){
		DBG_INFO((CG_EventLog), ("Session found :%ld,Analysing User Input:%s", CL_pdu.mcsi_sessionId[0],CL_pdu.mcC_userData.c_str()));
		pcL_Msg->meC_traversolPath.append("*");
		pcL_Msg->meC_traversolPath.append(CL_pdu.mcC_userData.c_str());

		CL_msg=CMenu::mcfnS_getLeafData(pcL_Msg->pmcC_Session,CL_pdu.mcC_userData.c_str(),pcL_Msg->pmcC_EsmeMsg->username().c_str(),pcL_Msg->pmcC_EsmeMsg->srv_code().c_str(),bL_isLeaf,CL_url);

		pcL_Msg->pmcC_EsmeMsg->set_receiver_sessionid(CL_pdu.mcsi_sessionId[0]);
		CTimerManeger::mcfnS_GetInstance()->mcfn_unregisterTimer((ITimer*)pcL_Msg);	
		if(CL_msg.empty()||bL_isLeaf){

			if(CL_msg.empty())
				CL_msg.assign("Thanks for using this service");
			CL_pdu.mcC_userData=CL_msg;
			if(pcL_Msg->meb_isCdrGenerated == true)
			{
				DBG_INFO((CG_EventLog), ("Session found :%ld,Sending Continue Release Message :%s", CL_pdu.mcsi_sessionId[0],CL_msg.c_str()));	
				CL_pdu.mcC_msgType = "4";//mefn_getString(RELEASE); 
				mcfn_sendUssdEnd(CL_pdu,CL_header.mcsi_sessionId);
			}
			else
			{
			       DBG_INFO((CG_EventLog), ("Session found :%ld,Sending Continue Notify Message :%s", CL_pdu.mcsi_sessionId[0],CL_msg.c_str()));	
				CL_pdu.mcC_msgType = "2";//mefn_getString(NOTIFY); 
				mcfn_sendUssdContinue(CL_pdu,CL_header.mcsi_sessionId);
			}
			pcL_Msg->pmcC_EsmeMsg->set_session_endtime(time(NULL));
			pcL_Msg->pmcC_EsmeMsg->set_status(0);
			pcL_Msg->pmcC_EsmeMsg->set_msg(pcL_Msg->meC_traversolPath);
			pcL_Msg->pmcC_EsmeMsg->set_service_url(CL_url);
			if(pcL_Msg->meb_isCdrGenerated == false)
			{
				CG_CdrClient.mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg);
				pcL_Msg->meb_isCdrGenerated=true;

			}
			else
			{
				CG_seqMap.mcfb_removeElement(CL_pdu.mcsi_sessionId[0]);
				delete pcL_Msg;
			}
		}
		else{
			DBG_INFO((CG_EventLog), ("Session found :%ld,Sending Continue Message :%s", CL_pdu.mcsi_sessionId[0],CL_msg.c_str()));
			CL_pdu.mcC_userData=CL_msg;	
                        CL_pdu.mcC_msgType = "1";//mefn_getString(REQUEST); 
			mcfn_sendUssdContinue(CL_pdu,CL_header.mcsi_sessionId);
			CTimerManeger::mcfnS_GetInstance()->mcfn_registerTimer((ITimer*)pcL_Msg,CG_Cfg.mcfn_getUssdTimeout(),NULL);
		}
	}
	else{
		DBG_CRITICAL((CG_EventLog), ("Session is not Active Sending  Abort [%ld]",CL_pdu.mcsi_sessionId[0]));	
		mcfn_sendUssdAbort(CL_header);	
	}
	return true;
}
bool CEsmeConnection::mcfn_onUssdEnd(CXmlHeader& CL_header,CXmlUssdMsg& CL_pdu){
	MsgTypes *pcL_Msg=NULL;
	DBG_VERBOSE((CG_EventLog), ("Received End Message:%s",CL_pdu.mcfn_ShortDebugString().c_str()));
	if(CG_seqMap.mcfb_getElement(CL_pdu.mcsi_sessionId[0],pcL_Msg)){
		DBG_INFO((CG_EventLog), ("Session found :%ld,Releaseing the session:%s", CL_pdu.mcsi_sessionId[0], CL_pdu.mcC_userData.c_str()));
		pcL_Msg->pmcC_EsmeMsg->set_session_endtime(time(NULL));
		pcL_Msg->pmcC_EsmeMsg->set_status(0);
		CG_CdrClient.mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg);
	}
	else{
		DBG_INFO((CG_EventLog), ("Session not found :%ld,Session Already Releaseed:%s", CL_pdu.mcsi_sessionId[0],CL_pdu.mcC_userData.c_str()));
	}
	return true;
}
bool CEsmeConnection::mcfn_onUssdAbort(CXmlHeader& CL_header){
	MsgTypes *pcL_Msg=NULL;
	DBG_CRITICAL((CG_EventLog), ("Ussd Abort Cause:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
	if(CG_seqMap.mcfb_getElement(CL_header.mcsi_sessionId[0],pcL_Msg)){
		DBG_INFO((CG_EventLog), ("Session found :%d", CL_header.mcsi_sessionId[0]));
		pcL_Msg->pmcC_EsmeMsg->set_session_endtime(time(NULL));
		pcL_Msg->pmcC_EsmeMsg->set_status(CL_header.mcsi_sessionStatus);
		CG_CdrClient.mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg);
	}
	else{
		DBG_INFO((CG_EventLog), ("Session not found :%d,Session Already Releaseed", CL_header.mcsi_sessionId[0]));
	}	
}
bool CEsmeConnection::mcfn_onUssdShake(CXmlHeader& CL_header){
	DBG_CRITICAL((CG_EventLog), ("Ussd Shake:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
}
bool CEsmeConnection::mcfn_onUssdShakeResp(CXmlHeader& CL_header){
	DBG_CRITICAL((CG_EventLog), ("Ussd Shake Resp:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
}
