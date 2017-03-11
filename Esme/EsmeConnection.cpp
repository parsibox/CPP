#include"EsmeConnection.hpp"
#include"EventLog.h"
extern CEventLog CG_EventLog;
CEsmeConnection::CEsmeConnection():CTcpClient("SMPP",10){
		
}

CEsmeConnection::~CEsmeConnection(){

}

//virtual function from CTcpClinet class
bool CEsmeConnection::mcfn_onConnect(){
	mcfn_sendBind();
	// receive the responce and check responce is fine or not if it is not fine return false ,
	//so that connection will be closed and reconnected by TcpClinet class	
	return mcfn_receiveData();
}
bool CEsmeConnection::mcfn_receiveData(){
	Buffer CL_receiveData;
	CL_receiveData.resize(16);	
	if(mcfn_receive((char*)&CL_receiveData[0],16)){//receiving header
		 Smpp::Uint32 u32L_len = Smpp::get_command_length((Smpp::Uint8*)&CL_receiveData[0]);
		if(u32L_len>16){
			 CL_receiveData.resize(u32L_len);
			 if(mcfn_receive((char*)&CL_receiveData[16],u32L_len-16)){
				 DBG_INFO((CG_EventLog),("Receiveing the data from SMPP:%d",u32L_len));
				 Smpp::chex_dump(&CL_receiveData[0],CL_receiveData.size(), stdout);
				return mcfn_processResponceMessage(&CL_receiveData[0],u32L_len);
			}
		}
	}
	return false;
}
//virtual function from EsmeClinet Class
bool CEsmeConnection::mcfn_sendRequestMessage(const Smpp::Uint8* buf, int len){
	return mcfn_send((const char*)buf,len);
}
bool CEsmeConnection::mcfn_processBindRes(){
	Smpp::BindReceiverResp CL_bindRecRes;
	Smpp::BindTransmitterResp CL_bindTranRes;
	Smpp::BindTransceiverResp CL_bindTransRecRes;
	std::string CL_systemId="";
	switch(Smpp::get_command_id(pmdu8_dataPkt)){
		case Smpp::CommandId::BindReceiverResp:
			CL_bindRecRes.decode(pmdu8_dataPkt);
			CL_systemId=CL_bindRecRes.system_id();
			DBG_INFO((CG_EventLog),("Received BindReceiverResp from SMPP:%s",CL_systemId.c_str()));
			break;
		case Smpp::CommandId::BindTransmitterResp:
			CL_bindTranRes.decode(pmdu8_dataPkt);
			CL_systemId=CL_bindTranRes.system_id();
			DBG_INFO((CG_EventLog),("Received Bind Transmiter Resp from SMPP:%s",CL_systemId.c_str()));
			break;
		case Smpp::CommandId::BindTransceiverResp:
			CL_bindTransRecRes.decode(pmdu8_dataPkt);
			CL_systemId=CL_bindTransRecRes.system_id();
			DBG_INFO((CG_EventLog),("Received Bind TransReceiver Resp from  SMPP:%d",CL_systemId.c_str()));
			break;
		defalt:
			break;
	}
	return true;
}
bool CEsmeConnection::mcfn_processEnqLinkRes(){
	  Smpp::EnquireLinkResp CL_enqLinkRes;
        CL_enqLinkRes.decode(pmdu8_dataPkt);
}
bool CEsmeConnection::mcfn_processUnbindRes(){
	Smpp::UnbindResp CL_unbindRes;
        CL_unbindRes.decode(pmdu8_dataPkt);
}
bool CEsmeConnection::mcfn_processSubmitSmRes(){
	 Smpp::SubmitSmResp CL_SubmitSmRes;
        CL_SubmitSmRes.decode(pmdu8_dataPkt);
	
}
bool CEsmeConnection::mcfn_processDlrSm(){
	
}
