#include"EsmeConnection.hpp"
#include "Externs.h"
#include "Menu.hpp"
#include"SMPP.hpp"
#include"ErrorCodes.hpp"
CEsmeConnection::CEsmeConnection():CTcpClient("SMPP",10){
	mcfn_start();
}

CEsmeConnection::~CEsmeConnection(){

}

//virtual function from CTcpClinet class
bool CEsmeConnection::mcfn_onConnect(){
	sleep(1);
	bL_isLoginSucess=false;	
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
	if(bL_isLoginSucess == false  && !mcfn_isDataAvailable(30000000)){
		DBG_ERROR((CG_EventLog),("Login Responce didn't came in 30 sec so reconnecting"));
		return false;	
	}
	ReceviedData CL_recData;
	static int headerSize = sizeof(CL_recData.mcC_header);
	CL_recData.pmcu8_data.resize(headerSize);
	if(mcfn_receive((char*)&CL_recData.pmcu8_data[0],headerSize)){//receiving header
		CL_recData.mcC_header.mcfn_setHeader((u8*)&CL_recData.pmcu8_data[0]);
		CL_recData.mcsi_len= CL_recData.mcC_header.mcsi_cmdLength;	
		if(CL_recData.mcsi_len > headerSize){
			CL_recData.pmcu8_data.resize(CL_recData.mcsi_len);
			if(false == mcfn_receive((char*)&CL_recData.pmcu8_data[headerSize],CL_recData.mcsi_len-headerSize)){
				DBG_ERROR((CG_EventLog),("Receive failed so closing the connection"));
				return false;
			}
		}
		else if(CL_recData.mcsi_len < headerSize)
		{
			DBG_ERROR((CG_EventLog),("Receive Length is invalid so disconnecing connection :%d",CL_recData.mcsi_len));
			return false;
		}

	}
	else{
		DBG_ERROR((CG_EventLog),("Receive failed so closing the connection"));
		return false;
	}
	std::string err=CErrorCodes::mcfnS_GetInstance()->mcfn_getErrorString(CL_recData.mcC_header.mcsi_cmdStatus).c_str();
	DBG_INFO((CG_EventLog),("Receiveing the data from SMPP[%ld]:%s:%s",CL_recData.mcsi_len,CL_recData.mcC_header.mcfn_ShortDebugString().c_str(),err.c_str()));
	CG_EventLog.mcfn_logU8Message(&CL_recData.pmcu8_data[0],CL_recData.mcsi_len);
	if(!bL_isLoginSucess){
		return mcfn_processReceivedMessage(CL_recData.mcC_header,&CL_recData.pmcu8_data[0],CL_recData.mcsi_len);
	}
	else{
		meC_RecDataQue.mcfb_insertIntoQue(CL_recData);
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
			if(!mcfn_processReceivedMessage(CL_recData.mcC_header,&CL_recData.pmcu8_data[0],CL_recData.mcsi_len))
			{
				//				mcfn_sendUssdAbort(CL_recData.mcC_header);
				DBG_ERROR((CG_EventLog),("Failed to process message sending Invalid commad id  to server"));
				Smpp::SubmitSmResp resp(Smpp::CommandStatus::ESME_RINVCMDID,CL_recData.mcC_header.mcsi_sessionId,Smpp::MessageId("0"));
				mcfn_sendMsgToServer((u8*)resp.encode(),resp.command_length());
			}
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
bool CEsmeConnection::mcfn_onUssdBindResp(CHeader& CL_header,u8* pu8L_param,int iL_len){
	std::string systemId="";
	if(Smpp::CommandId::BindReceiver == CL_header.mcsi_cmdId)
	{
		Smpp::BindReceiverResp pdu; pdu.decode(pu8L_param);
		systemId=pdu.system_id();
	}
	else if(Smpp::CommandId::BindTransmitter == CL_header.mcsi_cmdId)
	{
		Smpp::BindTransceiverResp pdu; pdu.decode(pu8L_param);
		systemId=pdu.system_id();
	}
	else {//Smpp::CommandId::BindTransceiver :
		Smpp::BindTransmitterResp pdu; pdu.decode(pu8L_param);
		systemId=pdu.system_id();
	}
	if(CL_header.mcsi_cmdStatus==0x00){

		DBG_INFO((CG_EventLog), ("Login Sucess With :%s",systemId.c_str()));
		meE_ConnStatus = CONNECTED;
		sleep(1);	
		return true;	
	}
	else{
		DBG_CRITICAL((CG_EventLog), ("Login Failed With :%s",systemId.c_str()));
		sleep(1);	
		return false;
	}
}
bool CEsmeConnection::mcfn_onUssdUnbindResp(CHeader& CL_pdu) {
	 if(CL_pdu.mcsi_cmdStatus==0x00){
                 DBG_INFO((CG_EventLog), ("Logout Sucess"));
                return true;
        }
	else{
		DBG_CRITICAL((CG_EventLog), ("Logout Failed ,Error Cause:%d",CL_pdu.mcsi_cmdStatus));	
		return false;	
	}
	
}


bool CEsmeConnection::mcfn_onUssdBegin(Smpp::DeliverSm *pCL_pdu){

	char messageString[200]={0x00};
	std::copy(pCL_pdu->short_message().begin(), pCL_pdu->short_message().end(),messageString);
	std::string CL_msg="";
	CL_msg.assign(messageString,pCL_pdu->short_message().length());
	CHeader CL_header;
	CL_header.mcsi_sessionId = mcfn_getSeqNumber();
	std::string CL_shortCode = pCL_pdu->destination_addr().address();
	MsgTypes *pcL_Msg= new MsgTypes;
	pcL_Msg->pmcC_Session=new Session();
	pcL_Msg->pmcC_EsmeMsg =new DeliverToEsme();
	pcL_Msg->pmcC_EsmeMsg->set_srv_code(CL_shortCode.c_str());
	pcL_Msg->pmcC_EsmeMsg->set_submit_time(time(NULL));
	pcL_Msg->pmcC_EsmeMsg->set_submit_msgid(CL_header.mcsi_sessionId);
	pcL_Msg->pmcC_EsmeMsg->set_sender_sessionid(CL_header.mcsi_sessionId);
	pcL_Msg->pmcC_EsmeMsg->set_da(pCL_pdu->source_addr().address());
	//removing # and assiging user input
	std::string srvWithOutHash="";
	srvWithOutHash.assign(CL_msg.substr(0,CL_msg.find('#')));
	pcL_Msg->meC_traversolPath.assign(srvWithOutHash);

	MenuDetails CL_menuDetails; 
	CL_menuDetails.mcC_menuServiceCode.assign((char*)pcL_Msg->meC_traversolPath.c_str()); 
	DBG_VERBOSE((CG_EventLog),("MO message checking for config:%s[%s][%s][%s]",CL_shortCode.c_str(),CL_msg.c_str(),"MO",CL_menuDetails.mcC_menuServiceCode.c_str()));
	CL_msg=CMenu::mcfnS_getRootMenuData(CL_menuDetails.mcC_userName,CL_menuDetails.mcC_menuServiceCode,"",pcL_Msg->pmcC_Session);
	if(CL_msg.empty()){
		DBG_VERBOSE((CG_EventLog),("Menu Not cofigure for this message:%s[%s][%s]",CL_shortCode.c_str(),CL_msg.c_str(),"MO"));
		delete pcL_Msg;
		return false;
	}
	pcL_Msg->pmcC_EsmeMsg->set_srv_code(CL_menuDetails.mcC_menuServiceCode);
	pcL_Msg->pmcC_EsmeMsg->set_username(CL_menuDetails.mcC_userName);

	DBG_VERBOSE((CG_EventLog),("Message sending to server:%s",CL_msg.c_str()));
	pcL_Msg->pmcC_EsmeMsg->set_receiver_sessionid(CL_header.mcsi_sessionId);//receiver id which we are generating in mo case
	pcL_Msg->pmcC_EsmeMsg->set_msg_type(2);
	pcL_Msg->pmcC_EsmeMsg->set_task_id(0);
	pcL_Msg->pmcC_EsmeMsg->set_msg_id(CL_header.mcsi_sessionId);
	pcL_Msg->pmcC_EsmeMsg->set_oa("MO");
	pcL_Msg->pmcC_EsmeMsg->set_interface_id(10);
        MsgTypes *pcL_MsgTemp=NULL;
	if(CG_seqMap.mcfb_findElement(pcL_Msg->pmcC_EsmeMsg->da(),pcL_MsgTemp)){
		pcL_MsgTemp->pmcC_EsmeMsg->set_session_endtime(time(NULL));
		pcL_MsgTemp->pmcC_EsmeMsg->set_status(1011);
		pCG_CdrClient->mcfn_sendMsgToCdr(pcL_MsgTemp->pmcC_EsmeMsg,pcL_MsgTemp->pmcC_Session->mcC_userInputs);
		CG_seqMap.mcfb_removeElement(pcL_Msg->pmcC_EsmeMsg->da());
		CTimerManeger::mcfnS_GetInstance()->mcfn_unregisterTimer((ITimer*)pcL_MsgTemp);
		delete pcL_MsgTemp;	
	}
	CG_seqMap.mcfb_insert(pcL_Msg->pmcC_EsmeMsg->da(),pcL_Msg);
	mcfn_sendUssdContinue(pcL_Msg->pmcC_EsmeMsg->da().c_str(),srvWithOutHash,0x00,CL_msg,USSD_OPTYPE::USSD_REQUEST);
	CTimerManeger::mcfnS_GetInstance()->mcfn_registerTimer((ITimer*)pcL_Msg,CG_Cfg.mcfn_getUssdTimeout(),NULL);
	return true;
}

bool CEsmeConnection::mcfn_onSubmitSmResp(CHeader& CL_pdu)
{
	MsgTypes *pcL_Msg=NULL;
	if(CG_submitSmMap.mcfb_findElement(CL_pdu.mcsi_sessionId,pcL_Msg)){
		pcL_Msg->pmcC_EsmeMsg->set_session_endtime(time(NULL));
		pcL_Msg->pmcC_EsmeMsg->set_status(CL_pdu.mcsi_cmdStatus);
		if(pcL_Msg->meb_isCdrGenerated == false)
		{	
			pCG_CdrClient->mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg,pcL_Msg->pmcC_Session->mcC_userInputs);
			pcL_Msg->meb_isCdrGenerated=true;
			CG_submitSmMap.mcfb_removeElement(CL_pdu.mcsi_sessionId);
			delete pcL_Msg;
		}
	}
	return true;
}
bool CEsmeConnection::mcfn_onUssdContinue(Smpp::DeliverSm *pCL_pdu){
	MsgTypes *pcL_Msg=NULL;
	bool bL_isLeaf=false;
	std::string CL_url="";
	std::string CL_msg="";
	std::string CL_msisdn=pCL_pdu->source_addr().address();
	char messageString[200]={0x00};
	std::copy(pCL_pdu->short_message().begin(), pCL_pdu->short_message().end(),messageString);

	DBG_VERBOSE((CG_EventLog), ("Received Continue Message:%s,UserInput:%s",CL_msisdn.c_str(),messageString));
	if(CG_seqMap.mcfb_findElement(CL_msisdn,pcL_Msg)){
		if(pcL_Msg->pmcC_EsmeMsg->msg_type() == 2 ){
			DBG_INFO((CG_EventLog), ("USSR Session found :%s,Analysing User Input:%s", CL_msisdn.c_str(),messageString));

			bool bL_isValidInput = false;
			CL_msg=CMenu::mcfnS_getLeafData(pcL_Msg->pmcC_Session,(const char*)messageString,pcL_Msg->pmcC_EsmeMsg->username().c_str(),pcL_Msg->pmcC_EsmeMsg->srv_code().c_str(),bL_isLeaf,CL_url,bL_isValidInput);
			if(bL_isValidInput)
			{
				pcL_Msg->meC_traversolPath.append("*");
				pcL_Msg->meC_traversolPath.append((char*)messageString);
			}
		}
		CTimerManeger::mcfnS_GetInstance()->mcfn_unregisterTimer((ITimer*)pcL_Msg);	
		if(CL_msg.empty()||bL_isLeaf){
			if(CL_msg.empty())
				CL_msg.assign(CG_Cfg.mcfn_getEndMessage());
			DBG_INFO((CG_EventLog), ("Session found :%s[cdr:%d],Sending Continue Message :%s", CL_msisdn.c_str(),pcL_Msg->meb_isCdrGenerated ,CL_msg.c_str()));
			/*if(pcL_Msg->pmcC_EsmeMsg->msg_type() == 1 || pcL_Msg->meb_isCdrGenerated )	
				mcfn_sendUssdEnd(pcL_Msg->pmcC_EsmeMsg->da().c_str(),pcL_Msg->pmcC_EsmeMsg->srv_code(),(u8)pcL_Msg->pmcC_EsmeMsg->dcs(),"",USSD_OPTYPE::USSD_RESPONCE);
			else
				mcfn_sendUssdContinue(pcL_Msg->pmcC_EsmeMsg->da().c_str(),pcL_Msg->pmcC_EsmeMsg->srv_code(),0x00,CL_msg,USSD_OPTYPE::USSD_NOTIFY);
*/
			 // mcfn_sendUssdEnd(pcL_Msg->pmcC_EsmeMsg->da().c_str(),CG_Cfg.mcfn_getOpeartorServiceCode(),(u8)pcL_Msg->pmcC_EsmeMsg->dcs(),CL_msg,USSD_OPTYPE::USSD_RELEASE);
			 mcfn_sendUssdContinue(pcL_Msg->pmcC_EsmeMsg->da().c_str(),CG_Cfg.mcfn_getOpeartorServiceCode(),(u8)pcL_Msg->pmcC_EsmeMsg->dcs(),CL_msg,USSD_OPTYPE::USSD_NOTIFY);

			pcL_Msg->pmcC_EsmeMsg->set_session_endtime(time(NULL));
			pcL_Msg->pmcC_EsmeMsg->set_status(0);
			if(pcL_Msg->meC_traversolPath.empty() == false )
				pcL_Msg->pmcC_EsmeMsg->set_msg(pcL_Msg->meC_traversolPath);
			if(pcL_Msg->pmcC_EsmeMsg->service_url().empty())
			{
				pcL_Msg->pmcC_EsmeMsg->set_service_url(CL_url);
			}
		/*	if(pcL_Msg->meb_isCdrGenerated || pcL_Msg->pmcC_EsmeMsg->msg_type() == 1 )
			{
				CG_seqMap.mcfb_removeElement(CL_msisdn);
				delete pcL_Msg;
			}*/
			if(pcL_Msg->meb_isCdrGenerated == false)
			{	
				pCG_CdrClient->mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg,pcL_Msg->pmcC_Session->mcC_userInputs);
				pcL_Msg->meb_isCdrGenerated=true;
				CG_seqMap.mcfb_removeElement(CL_msisdn);
				delete pcL_Msg;
			}
		}
		else{
			DBG_INFO((CG_EventLog), ("Session found :%s,Sending Continue Message :%s", CL_msisdn.c_str(),CL_msg.c_str()));	
			mcfn_sendUssdContinue(pcL_Msg->pmcC_EsmeMsg->da().c_str(),CG_Cfg.mcfn_getOpeartorServiceCode(),0x00,CL_msg, USSD_OPTYPE::USSD_REQUEST ) ;
			CTimerManeger::mcfnS_GetInstance()->mcfn_registerTimer((ITimer*)pcL_Msg,CG_Cfg.mcfn_getUssdTimeout(),NULL);
		}
	}
	else{
		DBG_CRITICAL((CG_EventLog), ("Session is not Active Sending  Abort [%s]",CL_msisdn.c_str()));	
		mcfn_sendUssdAbort(CL_msisdn,"",0x00,"Session is not active",USSD_OPTYPE::USSD_RELEASE);	
	}
	return true;
}
bool CEsmeConnection::mcfn_onUssdEnd(Smpp::SubmitSm* pCL_pdu){
	MsgTypes *pcL_Msg=NULL;
	std::string CL_msisdn = pCL_pdu->destination_addr().address();
	std::string CL_msg="";
	//	CL_msg.assign(pCL_pdu->short_message().begin());
	char messageString[200]={0x00};
	std::copy(pCL_pdu->short_message().begin(), pCL_pdu->short_message().end(),messageString);
	CL_msg.assign(messageString,pCL_pdu->short_message().length());
	if(CG_seqMap.mcfb_getElement(CL_msisdn,pcL_Msg)){
		DBG_INFO((CG_EventLog), ("Session found :%s,Releaseing the session:%s", CL_msisdn.c_str(),CL_msg.c_str()));
		pcL_Msg->pmcC_EsmeMsg->set_session_endtime(time(NULL));
		pcL_Msg->pmcC_EsmeMsg->set_status(pCL_pdu->command_status());
		if(pcL_Msg->meC_traversolPath.empty() == false )
			pcL_Msg->pmcC_EsmeMsg->set_msg(pcL_Msg->meC_traversolPath);
		pCG_CdrClient->mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg,pcL_Msg->pmcC_Session->mcC_userInputs);
		CG_seqMap.mcfb_removeElement(CL_msisdn);
		delete pcL_Msg;

	}
	else{
		DBG_INFO((CG_EventLog), ("Session not found :%s,Session Already Releaseed:%s",CL_msisdn.c_str(),CL_msg.c_str()));
	}
}
/*bool CEsmeConnection::mcfn_onUssdAbort(CHeader& CL_header){
  MsgTypes *pcL_Msg=NULL;
  DBG_CRITICAL((CG_EventLog), ("Ussd Abort Cause:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
  if(CG_seqMap.mcfb_getElement(CL_header.mcsi_receiverCB,pcL_Msg)){
  DBG_INFO((CG_EventLog), ("Session found :%d", CL_header.mcsi_receiverCB));
  if(pcL_Msg->meC_traversolPath.empty() == false )
  pcL_Msg->pmcC_EsmeMsg->set_msg(pcL_Msg->meC_traversolPath);
		pcL_Msg->pmcC_EsmeMsg->set_session_endtime(time(NULL));
		pcL_Msg->pmcC_EsmeMsg->set_status(CL_header.mcsi_cmdStatus);
		pCG_CdrClient->mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg);
	}
	else{
		DBG_INFO((CG_EventLog), ("Session not found :%d,Session Already Releaseed", CL_header.mcsi_receiverCB));
	}	
}*/
bool CEsmeConnection::mcfn_onUssdChargingIndRes(CHeader&){
		
}
bool CEsmeConnection::mcfn_onUssdShake(CHeader& CL_header){
	DBG_VERBOSE((CG_EventLog), ("Ussd Shake:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
	return true;
}
bool CEsmeConnection::mcfn_onUssdShakeResp(CHeader& CL_header){
	DBG_VERBOSE((CG_EventLog), ("Ussd Shake Resp:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
        //mdsi_handShakeCount = 0;
	return true;
}
bool  CEsmeConnection:: mcfn_onUssdSwitch(CHeader&,u8*,int){
}
