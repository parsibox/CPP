#include"EsmeConnection.hpp"
#include "Externs.h"
#include"SMPP.hpp"
#include"ErrorCodes.hpp"
CEsmeConnection::CEsmeConnection():CTcpClient("SMPP",10){
	mcfn_start();
}

CEsmeConnection::~CEsmeConnection(){

}
void CEsmeConnection::mcfn_initilise(std::string CL_systemId,std::string CL_passwd,std::string CL_systemType,int iL_interfaceVer){
	meC_systemId=CL_systemId;
	meC_password=CL_passwd;
	meC_serviceType=CL_systemType;
	mesi_interfaceVersion=iL_interfaceVer;	
	mesi_seqNum=0;
}

//virtual function from CTcpClinet class
bool CEsmeConnection::mcfn_onConnect(){
	sleep(1);
	bL_isLoginSucess=false;	
	if(!mcfn_sendBind()){
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
			if(!mcfn_processReceivedMessage(CL_recData.mcC_header,&CL_recData.pmcu8_data[0],CL_recData.mcsi_len))
			{
				DBG_ERROR((CG_EventLog),("Failed to process message sending Invalid commad id  to server"));
			}
		}
	}
	return true;
}
std::string CEsmeConnection::mcfn_getThreadName(void*){
	return "PrecessReceivedSmppPacketThread";
}
int CEsmeConnection::mcfn_getSeqNumber(){
	++mesi_seqNum;
	if(mesi_seqNum>=0x7FFFFFFF){
		mesi_seqNum=0;
	}
	return mesi_seqNum;
}

bool CEsmeConnection::mcfn_sendMsgToServer( u8* buf, int len){
	CG_EventLog.mcfn_logU8Message(buf,len);
	if(mcfn_send((const char*)buf,len))
		return true;

	mcfn_close();
	return false;

}

bool CEsmeConnection::mcfn_sendBind(){
	try{
	if(meC_serviceType.compare("T")==0)
	{
		Smpp::BindTransmitter pdu;
		pdu.system_id(meC_systemId.c_str());
		pdu.password(meC_password.c_str());
		pdu.system_type(meC_serviceType.c_str());
		pdu.interface_version(mesi_interfaceVersion);
		return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());	
	}
	else if(meC_serviceType.compare("R")==0)
	{
		Smpp::BindReceiver pdu;	
		pdu.system_id(meC_systemId.c_str());
		pdu.password(meC_password.c_str());
		pdu.system_type(meC_serviceType.c_str());
		pdu.interface_version(mesi_interfaceVersion);
		return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());	
	}
	else
	{
		Smpp::BindTransceiver pdu;
		pdu.system_id(meC_systemId.c_str());
		pdu.password(meC_password.c_str());
		pdu.system_type(meC_serviceType.c_str());
		pdu.interface_version(mesi_interfaceVersion);
		return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());	
	}
	}
	catch(const Smpp::Error &e){
		DBG_CRITICAL((CG_EventLog), ("Bind messages creation failed :%s",e.what()));
		return false;
	}
}

bool CEsmeConnection::mcfn_sendBindResp(int iL_status,std::string CL_accName,std::string CL_sysType){
	if(CL_sysType.compare("T")==0)
	{
		Smpp::BindTransmitterResp pdu;
		pdu.system_id(CL_accName.c_str());
		return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());
	}
	else if(CL_sysType.compare("R")==0)
	{
		Smpp::BindReceiverResp pdu;
		pdu.system_id(CL_accName.c_str());
		return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());
	}
	else
	{
		Smpp::BindTransceiverResp pdu;
		pdu.system_id(CL_accName.c_str());
		return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());
	}
}
bool CEsmeConnection::mcfn_sendUnbind(){
	Smpp::Unbind pdu(mcfn_getSeqNumber());
        return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());
}
bool CEsmeConnection::mcfn_sendShake(){
        Smpp::EnquireLink pdu(mcfn_getSeqNumber());
        return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());
}
bool CEsmeConnection::mcfn_sendSubmitSm(int iL_seqNum,std::string CL_msisdn,std::string CL_oa,u8 u8L_codeScema,std::string CL_ShortMsg,u8 u8L_reg_del)
{
	Smpp::SubmitSm pdu;
	pdu.sequence_number(iL_seqNum);
	pdu.source_addr(Smpp::Address(CL_oa.c_str()));
	pdu.destination_addr( Smpp::SmeAddress(Smpp::Ton(Smpp::Ton::International),
				Smpp::Npi(Smpp::Npi::E164),
				Smpp::Address(CL_msisdn.c_str())));
	pdu.registered_delivery(u8L_reg_del);
	pdu.short_message( reinterpret_cast<const Smpp::Uint8*>(CL_ShortMsg.c_str()),
			CL_ShortMsg.length());
	pdu.data_coding(Smpp::DataCoding(u8L_codeScema));
	return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());
}

bool CEsmeConnection::mcfn_onBindResp(CHeader& CL_header,u8* pu8L_param,int iL_len){
	if(CL_header.mcsi_cmdStatus==0x00){
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

		DBG_INFO((CG_EventLog), ("Login Sucess With :%s",systemId.c_str()));
		meE_ConnStatus = CONNECTED;
		sleep(1);	
		return true;	
	}
	else{
		DBG_CRITICAL((CG_EventLog), ("Login Failed"));
		sleep(1);	
		return false;
	}
}
bool CEsmeConnection::mcfn_onUnbindResp(CHeader& CL_pdu) {
	 if(CL_pdu.mcsi_cmdStatus==0x00){
                 DBG_INFO((CG_EventLog), ("Logout Sucess"));
                return true;
        }
	else{
		DBG_CRITICAL((CG_EventLog), ("Logout Failed ,Error Cause:%d",CL_pdu.mcsi_cmdStatus));	
		return false;	
	}
	
}

bool CEsmeConnection::mcfn_onShake(CHeader& CL_header){
	DBG_VERBOSE((CG_EventLog), ("Shake:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
	return true;
}

bool CEsmeConnection::mcfn_onShakeResp(CHeader& CL_header){
	DBG_VERBOSE((CG_EventLog), ("Shake Resp:[%s]",CL_header.mcfn_ShortDebugString().c_str()));
	return true;
}

bool CEsmeConnection::mcfn_onSubmitSm(u8* u8_param, int iL_paramLen)
{
	DBG_VERBOSE((CG_EventLog), ("Submit Sm received Length:[%d]",iL_paramLen));
	return true;
}

bool CEsmeConnection::mcfn_onSubmitSmResp(u8* u8_param, int iL_paramLen)
{
	DBG_VERBOSE((CG_EventLog), ("Submit Sm Resp received Length:[%d]",iL_paramLen));
	Smpp::SubmitSmResp pdu;
	pdu.decode(&u8_param[0]);
	MsgTypes *pcL_Msg=NULL;
	if(CG_seqMap.mcfb_findElement(pdu.sequence_number(),pcL_Msg)){
		DBG_CRITICAL((CG_EventLog), ("sequence number  found [%d][%d][%s]",pdu.sequence_number(),pdu.command_status(),std::string(pdu.message_id()).c_str()));	
	        pcL_Msg->pmcC_EsmeMsg->set_status(pdu.command_status());
		pcL_Msg->pmcC_EsmeMsg->set_sms_submit_msgid(pdu.message_id());
		pCG_CdrClient->mcfn_sendMsgToCdr(pcL_Msg->pmcC_EsmeMsg);
		 CG_seqMap.mcfb_removeElement(pdu.sequence_number());
		 delete pcL_Msg;
	}
	else{
		DBG_CRITICAL((CG_EventLog), ("sequence number not found [%d]",pdu.sequence_number()));	
	}
	return true;
}

bool CEsmeConnection::mcfn_onDeliverSm(u8* u8_param, int iL_paramLen)
{
	DBG_VERBOSE((CG_EventLog), ("Deliver Sm received Length:[%d]",iL_paramLen));
	return true;
}

bool CEsmeConnection::mcfn_onDeliverSmResp(u8* u8_param, int iL_paramLen)
{
	DBG_VERBOSE((CG_EventLog), ("Deliver Sm  Resp received Length:[%d]",iL_paramLen));
	return true;
}

bool CEsmeConnection::mcfn_onUnbind(CHeader&)
{
	DBG_VERBOSE((CG_EventLog), ("Unbind received"));
	return true;
}

bool CEsmeConnection::mcfn_onBind(CHeader&,u8*,int iL_paramLen)
{
	DBG_VERBOSE((CG_EventLog), ("Bind  Resp received Length:[%d]",iL_paramLen));
	return true;
}

bool CEsmeConnection::mcfn_processReceivedMessage(CHeader& CL_header,u8* u8_param,int iL_paramLen){
	try{	
		switch(CL_header.mcsi_cmdId){
			case Smpp::CommandId::BindReceiver :
			case Smpp::CommandId::BindTransmitter :
			case Smpp::CommandId::BindTransceiver :
				return mcfn_onBind(CL_header,u8_param,iL_paramLen);	
			case Smpp::CommandId::BindReceiverResp :
			case Smpp::CommandId::BindTransmitterResp :
			case Smpp::CommandId::BindTransceiverResp :
				return mcfn_onBindResp(CL_header,u8_param,iL_paramLen);
			case Smpp::CommandId::Unbind:
				return mcfn_onUnbind(CL_header);
			case Smpp::CommandId::UnbindResp:
				return mcfn_onUnbindResp(CL_header);
			case Smpp::CommandId::SubmitSm:
				return mcfn_onSubmitSm(u8_param,iL_paramLen);
			case Smpp::CommandId::DeliverSm:
				return mcfn_onDeliverSm(u8_param,iL_paramLen);
			case Smpp::CommandId::EnquireLink:
				return mcfn_onShake(CL_header);
			case Smpp::CommandId::EnquireLinkResp:
				return mcfn_onShakeResp(CL_header);
			case Smpp::CommandId::DeliverSmResp:
				return mcfn_onDeliverSmResp(u8_param,iL_paramLen);
			case Smpp::CommandId::SubmitSmResp:
				return mcfn_onSubmitSmResp(u8_param,iL_paramLen);
			default:
				break;
		}
	}

	catch(const Smpp::Error &e){
		DBG_CRITICAL((CG_EventLog), ("Exception came to process  :%s",e.what()));
	}
	return false;
}
