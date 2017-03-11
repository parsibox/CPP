#include"EsmeConnection.hpp"
#include "Externs.h"

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
	//std::string err=CErrorCodes::mcfnS_GetInstance()->mcfn_getErrorString(CL_recData.mcC_header.mcsi_cmdStatus).c_str();
	DBG_INFO((CG_EventLog),("Receiveing the data from SMPP[%ld]:%s",CL_recData.mcsi_len,CL_recData.mcC_header.mcfn_ShortDebugString().c_str()));//,err.c_str()));
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
bool CEsmeConnection::mcfn_setUssdMsg(CUssdBegin &CL_ussdMsg,UssdData  &CL_ussdData)
{
	CL_ussdData.set_version(1);
	CL_ussdData.set_dcs(CL_ussdMsg.mcu8_codeSchema);
	CL_ussdData.set_oa((char*)CL_ussdMsg.pmcu8_serviceCode);
	CL_ussdData.set_da((char*)CL_ussdMsg.pmcu8_msisdn);
	CL_ussdData.set_msg((char*)CL_ussdMsg.pmcu8_ussdString);
	//msg_type	--this will be set in diffent messages
	CL_ussdData.set_srv_code((char*)CL_ussdMsg.pmcu8_serviceCode);
	mcfn_setHeaderData((CHeader&)CL_ussdMsg,CL_ussdData);
	return true;
}
bool CEsmeConnection::mcfn_setHeaderData(CHeader& CL_header,UssdData  &CL_ussdData)
{
	CL_ussdData.set_status(CL_header.mcsi_cmdStatus);
	CL_ussdData.set_sender_sessionid(CL_header.mcsi_senderCB);
	CL_ussdData.set_receiver_sessionid(CL_header.mcsi_receiverCB);
	return true;
}

bool CEsmeConnection::mcfn_onUssdBegin(CHeader& CL_header,u8* pu8L_param,int){
	CUssdBegin CL_begin(CL_header);
	CL_begin.mcfn_setParam(pu8L_param);
	 DBG_VERBOSE((CG_EventLog), ("Received Begin Message:%s",CL_begin.mcfn_ShortDebugString().c_str()));
	UssdData  CL_ussdData;
	mcfn_setUssdMsg(CL_begin,CL_ussdData);
	switch(CL_begin.mcu8_ussdOptype){
		case USSD_OPTYPE::USSD_REQUEST:
			CL_ussdData.set_msg_type(UssdData::PUSSR);
			//TODO::this will come in case of PSSR
			break;
		case USSD_OPTYPE::USSD_NOTIFY:
			//TODO::This will never come to CMS. 
			CL_ussdData.set_msg_type(UssdData::USSN);
			break;
		default:
			break;
	}	
	return true;
}
bool CEsmeConnection::mcfn_onUsddContinue(CHeader& CL_header,u8* pu8L_param,int iL_len){
	CUssdContinue CL_continue(CL_header);
	CL_continue.mcfn_setParam(pu8L_param);
	 DBG_VERBOSE((CG_EventLog), ("Received Continue Message:%s",CL_continue.mcfn_ShortDebugString().c_str()));
	UssdData  CL_ussdData;
	mcfn_setUssdMsg(CL_continue,CL_ussdData);

	 switch(CL_continue.mcu8_ussdOptype){
                case USSD_OPTYPE::USSD_REQUEST:
                        //TODO::this will come in case of PSSR
			 CL_ussdData.set_msg_type(UssdData::USSR);
                        break;
                case USSD_OPTYPE::USSD_NOTIFY:
                        //TODO::This will never come to CMS.
			CL_ussdData.set_msg_type(UssdData::USSN);
                        break;
		case  USSD_OPTYPE::USSD_RESPONCE:
			//TODO::need to find the next menu according to user input
			CL_ussdData.set_msg_type(UssdData::USSR_ACK);
			break;
                default:
                        break;
        }
	return true;
}
bool CEsmeConnection::mcfn_onUssdEnd(CHeader& CL_header,u8* pu8L_param,int iL_len){
	CUssdEnd CL_end(CL_header);
	CL_end.mcfn_setParam(pu8L_param);
	 DBG_VERBOSE((CG_EventLog), ("Received End Message:%s",CL_end.mcfn_ShortDebugString().c_str()));
	UssdData  CL_ussdData;
	mcfn_setHeaderData(CL_end,CL_ussdData);

	switch(CL_end.mcu8_ussdOptype){
		case  USSD_OPTYPE::USSD_RESPONCE:
			CL_ussdData.set_session_status(0);
			break;
		case  USSD_OPTYPE::USSD_RELEASE:
			CL_ussdData.set_session_status(1);
			break;
                default:
			break;
	}
}
bool CEsmeConnection::mcfn_onUssdAbort(CHeader& CL_header){
	UssdData  CL_ussdData;
	mcfn_setHeaderData(CL_header,CL_ussdData);
	CL_ussdData.set_session_status(1);
}
bool CEsmeConnection::mcfn_onUssdChargingIndRes(CHeader& CL_header){
		
	DBG_CRITICAL((CG_EventLog), ("Ussd Charging :[%s]",CL_header.mcfn_ShortDebugString().c_str()));
}
bool CEsmeConnection::mcfn_onUssdShake(CHeader& CL_header){
	DBG_CRITICAL((CG_EventLog), ("Ussd Shake:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
}
bool CEsmeConnection::mcfn_onUssdShakeResp(CHeader& CL_header){
	DBG_CRITICAL((CG_EventLog), ("Ussd Shake Resp:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
}
bool  CEsmeConnection:: mcfn_onUssdSwitch(CHeader&,u8*,int){
}
