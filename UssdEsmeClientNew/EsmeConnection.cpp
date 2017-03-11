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
	CL_recData.pmcu8_data.resize(20);
	if(mcfn_receive((char*)&CL_recData.pmcu8_data[0],20)){//receiving header
		CL_recData.mcC_header.mcfn_setHeader((u8*)&CL_recData.pmcu8_data[0]);
		CL_recData.mcsi_len= CL_recData.mcC_header.mcsi_cmdLength;	
		if(CL_recData.mcsi_len >20){
			CL_recData.pmcu8_data.resize(CL_recData.mcsi_len);
			if(false == mcfn_receive((char*)&CL_recData.pmcu8_data[20],CL_recData.mcsi_len-20)){
				DBG_ERROR((CG_EventLog),("Receive failed so closing the connection"));
				return false;
			}
		}
		else if(CL_recData.mcsi_len < 20 )
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
		return mcfn_processReceivedMessage(CL_recData.mcC_header,&CL_recData.pmcu8_data[20],CL_recData.mcsi_len);
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
			mcfn_processReceivedMessage(CL_recData.mcC_header,&CL_recData.pmcu8_data[20],CL_recData.mcsi_len);
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
	CUssdBindResp CL_bindResp(CL_header);
	CL_bindResp.mcfn_setParam(pu8L_param);
	if(CL_bindResp.mcsi_cmdStatus==0x00){
		DBG_INFO((CG_EventLog), ("Login Sucess With :%s",CL_bindResp.mcfn_ShortDebugString().c_str()));
		sleep(1);	
		return true;	
	}
	else{
		DBG_CRITICAL((CG_EventLog), ("Login Failed With :%s",CL_bindResp.mcfn_ShortDebugString().c_str()));
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
bool CEsmeConnection::mcfn_onUssdBegin(CHeader& CL_header,u8* pu8L_param,int){
	CUssdBegin CL_begin(CL_header);
	CL_begin.mcfn_setParam(pu8L_param);
	switch(CL_begin.mcu8_ussdOptype){
		case USSD_OPTYPE::USSD_REQUEST:
			//TODO::this will come in case of PSSR
			break;
		case USSD_OPTYPE::USSD_NOTIFY:
			//TODO::This will never come to CMS. 
			break;
		default:
			break;
	}	
	return true;
}
bool CEsmeConnection::mcfn_onUsddContinue(CHeader& CL_header,u8* pu8L_param,int iL_len){
	CUssdContinue CL_continue(CL_header);
	CL_continue.mcfn_setParam(pu8L_param);
	int iL_temp=0;
	//swaping 
	iL_temp=CL_header.mcsi_receiverCB;
	CL_header.mcsi_receiverCB=CL_header.mcsi_senderCB;
	CL_header.mcsi_senderCB=iL_temp;
	MsgTypes *pcL_Msg=NULL;
	bool bL_isLeaf=false;
	std::string CL_url="";
	std::string CL_msg="";
	 DBG_VERBOSE((CG_EventLog), ("Received Continue Message:%s",CL_continue.mcfn_ShortDebugString().c_str()));
	 switch(CL_continue.mcu8_ussdOptype){
                case USSD_OPTYPE::USSD_REQUEST:
                        //TODO::this will come in case of PSSR
                        break;
                case USSD_OPTYPE::USSD_NOTIFY:
                        //TODO::This will never come to CMS.
                        break;
		case  USSD_OPTYPE::USSD_RESPONCE:
			//TODO::need to find the next menu according to user input
			if(CG_seqMap.mcfb_findElement(CL_header.mcsi_senderCB,pcL_Msg)){
				if(pcL_Msg->pmcC_EsmeMsg->msg_type() == 2 ){
					CL_continue.pmcu8_ussdString[1]=0x00;
					DBG_INFO((CG_EventLog), ("USSR Session found :%d,Analysing User Input:%s", CL_header.mcsi_senderCB,CL_continue.pmcu8_ussdString));

					bool bL_isValidInput = false;
					CL_msg=CMenu::mcfnS_getLeafData(pcL_Msg->pmcC_Session,(const char*)CL_continue.pmcu8_ussdString,pcL_Msg->pmcC_EsmeMsg->username().c_str(),pcL_Msg->pmcC_EsmeMsg->srv_code().c_str(),bL_isLeaf,CL_url,bL_isValidInput);
					if(bL_isValidInput)
					{
						pcL_Msg->meC_traversolPath.append("*");
						pcL_Msg->meC_traversolPath.append((char*)CL_continue.pmcu8_ussdString);
                                        }
				}
				
				pcL_Msg->pmcC_EsmeMsg->set_receiver_sessionid(CL_header.mcsi_senderCB);
				CTimerManeger::mcfnS_GetInstance()->mcfn_unregisterTimer((ITimer*)pcL_Msg);	
				if(CL_msg.empty()||bL_isLeaf){
					if(CL_msg.empty())
						CL_msg.assign(CG_Cfg.mcfn_getEndMessage());
					DBG_INFO((CG_EventLog), ("Session found :%d,Sending Continue Message :%s", CL_header.mcsi_senderCB,CL_msg.c_str()));
					if(pcL_Msg->pmcC_EsmeMsg->msg_type() == 1 || pcL_Msg->meb_isCdrGenerated )	
						mcfn_sendUssdEnd(CL_header,pcL_Msg->pmcC_EsmeMsg->da().c_str(),pcL_Msg->pmcC_EsmeMsg->srv_code(),(u8)pcL_Msg->pmcC_EsmeMsg->dcs(),CL_msg,USSD_OPTYPE::USSD_RELEASE);
					else
						mcfn_sendUssdContinue(CL_header,pcL_Msg->pmcC_EsmeMsg->da().c_str(),"",0x0f,CL_msg,USSD_OPTYPE::USSD_NOTIFY);

					pcL_Msg->pmcC_EsmeMsg->set_session_endtime(time(NULL));
					pcL_Msg->pmcC_EsmeMsg->set_status(0);
					if(pcL_Msg->meC_traversolPath.empty() == false )
						pcL_Msg->pmcC_EsmeMsg->set_msg(pcL_Msg->meC_traversolPath);
					pcL_Msg->pmcC_EsmeMsg->set_service_url(CL_url);
					if(pcL_Msg->meb_isCdrGenerated || pcL_Msg->pmcC_EsmeMsg->msg_type() == 1 )
					{
						CG_seqMap.mcfb_removeElement(CL_header.mcsi_senderCB);
						delete pcL_Msg;
					}
					if(pcL_Msg->meb_isCdrGenerated == false)
					{
						pCG_CdrClient->mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg);
						pcL_Msg->meb_isCdrGenerated=true;
						
					}
				}
				else{
					DBG_INFO((CG_EventLog), ("Session found :%d,Sending Continue Message :%s", CL_header.mcsi_senderCB,CL_msg.c_str()));	
					mcfn_sendUssdContinue(CL_header,pcL_Msg->pmcC_EsmeMsg->da().c_str(),"",0x0f,CL_msg,USSD_OPTYPE::USSD_REQUEST);
					CTimerManeger::mcfnS_GetInstance()->mcfn_registerTimer((ITimer*)pcL_Msg,CG_Cfg.mcfn_getUssdTimeout(),NULL);
				}
			}
			else{
				DBG_CRITICAL((CG_EventLog), ("Session is not Active Sending  Abort [%d]",CL_header.mcsi_senderCB));	
				mcfn_sendUssdAbort(CL_header);	
			}
			break;
                default:
                        break;
        }
	return true;
}
bool CEsmeConnection::mcfn_onUssdEnd(CHeader& CL_header,u8* pu8L_param,int iL_len){
	CUssdEnd CL_end(CL_header);
	CL_end.mcfn_setParam(pu8L_param);
	MsgTypes *pcL_Msg=NULL;
	 DBG_VERBOSE((CG_EventLog), ("Received End Message:%s",CL_end.mcfn_ShortDebugString().c_str()));
	switch(CL_end.mcu8_ussdOptype){
		case  USSD_OPTYPE::USSD_RESPONCE:

			break;
		case  USSD_OPTYPE::USSD_RELEASE:
			if(CG_seqMap.mcfb_getElement(CL_header.mcsi_receiverCB,pcL_Msg)){
				DBG_INFO((CG_EventLog), ("Session found :%d,Releaseing the session:%s", CL_header.mcsi_receiverCB, CL_end.pmcu8_ussdString));
				pcL_Msg->pmcC_EsmeMsg->set_session_endtime(time(NULL));
				pcL_Msg->pmcC_EsmeMsg->set_status(CL_end.mcsi_cmdStatus);
				if(pcL_Msg->meC_traversolPath.empty() == false )
					pcL_Msg->pmcC_EsmeMsg->set_msg(pcL_Msg->meC_traversolPath);
				pCG_CdrClient->mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg);
			}
			else{
				DBG_INFO((CG_EventLog), ("Session not found :%d,Session Already Releaseed:%s", CL_header.mcsi_receiverCB, CL_end.pmcu8_ussdString));
			}
			break;
                default:
			break;
	}
}
bool CEsmeConnection::mcfn_onUssdAbort(CHeader& CL_header){
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
}
bool CEsmeConnection::mcfn_onUssdChargingIndRes(CHeader&){
		
}
bool CEsmeConnection::mcfn_onUssdShake(CHeader& CL_header){
	DBG_VERBOSE((CG_EventLog), ("Ussd Shake:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
}
bool CEsmeConnection::mcfn_onUssdShakeResp(CHeader& CL_header){
	DBG_VERBOSE((CG_EventLog), ("Ussd Shake Resp:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
        mdsi_handShakeCount = 0;
}
bool  CEsmeConnection:: mcfn_onUssdSwitch(CHeader&,u8*,int){
}
