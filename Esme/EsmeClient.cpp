
#include "EsmeClient.hpp"

CEsmeClient::CEsmeClient(){
}
CEsmeClient::~CEsmeClient(){
}
bool CEsmeClient::mcfn_initialise(std::string CL_sysId,std::string CL_passwd,std::string CL_sysType,std::string CL_connType,int iL_ton,int iL_npi,std::string CL_addRange){
	meC_systemId=CL_sysId;
	meC_password=CL_passwd;
	meC_SysType=CL_sysType;
	meC_ConnType=CL_connType;
	mesi_ton=iL_ton;
	mesi_npi=iL_npi;
	meC_addrRange=CL_addRange;
	mesi_seqNumber=0x00000000;
}

CONN_STATUS CEsmeClient::mdfn_getConnectionStatus(){
	return meE_ConnStatus;
}
bool CEsmeClient::mcfn_setConnectionStatus(CONN_STATUS eL_connStatus){
	meE_ConnStatus=eL_connStatus;
}
int CEsmeClient::mcfn_getSeqNumber(){
	//TODO::need to use lock here in case of MultiThreading
	++mesi_seqNumber;
	if(mesi_seqNumber>=0x7FFFFFFF)
		mesi_seqNumber=0x01;
	return mesi_seqNumber;		
}
bool CEsmeClient::mcfn_sendBind(){
	if(meC_ConnType=="T"){
		Smpp::BindTransmitter CL_bindT;
		CL_bindT.system_id(meC_systemId.c_str());
		CL_bindT.password(meC_password.c_str());
		CL_bindT.system_type(meC_SysType.c_str());
		CL_bindT.interface_version(SMPP_INTERFACE_VERSION);
		CL_bindT.addr_ton(mesi_ton);
		CL_bindT.addr_npi(mesi_npi);
		CL_bindT.address_range(meC_addrRange.c_str());
		CL_bindT.sequence_number(mcfn_getSeqNumber());
		return mcfn_sendRequestMessage(CL_bindT.encode(),CL_bindT.command_length());

	}
	else if(meC_ConnType=="R"){
		Smpp::BindReceiver CL_bindR;
		CL_bindR.system_id(meC_systemId.c_str());
		CL_bindR.password(meC_password.c_str());
		CL_bindR.system_type(meC_SysType.c_str());
		CL_bindR.interface_version(SMPP_INTERFACE_VERSION);
		CL_bindR.addr_ton(mesi_ton);
		CL_bindR.addr_npi(mesi_npi);
		CL_bindR.address_range(meC_addrRange.c_str());
		CL_bindR.sequence_number(mcfn_getSeqNumber());
		return mcfn_sendRequestMessage(CL_bindR.encode(),CL_bindR.command_length());

	}
	else{
		Smpp::BindTransceiver CL_bindTR;
		CL_bindTR.system_id(meC_systemId.c_str());
		CL_bindTR.password(meC_password.c_str());
		CL_bindTR.system_type(meC_SysType.c_str());
		CL_bindTR.interface_version(SMPP_INTERFACE_VERSION);
		CL_bindTR.addr_ton(mesi_ton);
		CL_bindTR.addr_npi(mesi_npi);
		CL_bindTR.address_range(meC_addrRange.c_str());
		CL_bindTR.sequence_number(mcfn_getSeqNumber());
		return mcfn_sendRequestMessage(CL_bindTR.encode(),CL_bindTR.command_length());
	}
	return false;
}
bool CEsmeClient::mcfn_sendUnbind(){
	Smpp::Unbind CL_unbind(mcfn_getSeqNumber());
	return mcfn_sendRequestMessage(CL_unbind.encode(),CL_unbind.command_length());
}
bool CEsmeClient::mcfn_sendEnqLink(){
	Smpp::EnquireLink CL_enqLink(mcfn_getSeqNumber());
	return mcfn_sendRequestMessage(CL_enqLink.encode(),CL_enqLink.command_length());
}

bool CEsmeClient::mcfn_sendSubmitSm(Smpp::SubmitSm &CL_submitSm,int &iL_seqNum){
	iL_seqNum=mcfn_getSeqNumber();
	CL_submitSm.sequence_number(iL_seqNum);
	CL_submitSm.service_type(meC_SysType.c_str());
	return mcfn_sendRequestMessage(CL_submitSm.encode(),CL_submitSm.command_length());
}

bool CEsmeClient::mcfn_sendSubmitMultiSm(Smpp::SubmitMulti  &CL_subMultiSm){
	CL_subMultiSm.sequence_number(mcfn_getSeqNumber());
	return mcfn_sendRequestMessage(CL_subMultiSm.encode(),CL_subMultiSm.command_length());
}
bool CEsmeClient::mcfn_sendDataSm(Smpp::DataSm &CL_dataSm){
	CL_dataSm.sequence_number(mcfn_getSeqNumber());
	return mcfn_sendRequestMessage(CL_dataSm.encode(),CL_dataSm.command_length());
}
bool CEsmeClient::mcfn_sendDlrSmRes(int iL_seqNum){
	Smpp::DeliverSmResp CL_dlrSmResp(Smpp::CommandStatus::ESME_ROK, iL_seqNum);
	return mcfn_sendRequestMessage(CL_dlrSmResp.encode(),CL_dlrSmResp.command_length());
}
bool CEsmeClient::mcfn_sendCancelSm(Smpp::CancelSm &){
}
bool CEsmeClient::mcfn_sendQuerySm(Smpp::QuerySm &){
}
bool CEsmeClient::mcfn_sendReplaceSm(Smpp::ReplaceSm &){
}

bool CEsmeClient::mcfn_processBindRes(){
	Smpp::BindReceiverResp CL_bindRecRes;
	Smpp::BindTransmitterResp CL_bindTranRes;
	Smpp::BindTransceiverResp CL_bindTransRecRes;
	switch(Smpp::get_command_id(pmdu8_dataPkt)){
		case Smpp::CommandId::BindReceiverResp:
			CL_bindRecRes.decode(pmdu8_dataPkt);
			break;
		case Smpp::CommandId::BindTransmitterResp:
			CL_bindTranRes.decode(pmdu8_dataPkt);
			break;
		case Smpp::CommandId::BindTransceiverResp:
			CL_bindTransRecRes.decode(pmdu8_dataPkt);
			break;
		defalt:
			break;
	}
	return true;
}
bool CEsmeClient::mcfn_processUnbindRes(){
	Smpp::UnbindResp CL_unbindRes;
        CL_unbindRes.decode(pmdu8_dataPkt);
	return true;
}
bool CEsmeClient::mcfn_processEnqLinkRes(){
	 Smpp::EnquireLinkResp CL_enqLinkRes;
        CL_enqLinkRes.decode(pmdu8_dataPkt);
	return true;
}
bool CEsmeClient::mcfn_processSubmitSmRes(){
	 Smpp::SubmitSmResp CL_SubmitSmRes;
        CL_SubmitSmRes.decode(pmdu8_dataPkt);
	return true;
}
bool CEsmeClient::mcfn_processSubmitMultiSmRes(){
	
	return true;
}
bool CEsmeClient::mcfn_processDataSmRes(){
	Smpp::DataSmResp CL_dataSmRes;
	CL_dataSmRes.decode(pmdu8_dataPkt);
	return true;
}
bool CEsmeClient::mcfn_processDlrSm(){
	
	return true;
}
bool CEsmeClient::mcfn_processDlrSmRes(){
	return true;

}
bool CEsmeClient::mcfn_processCancelSmRes(){
	return true;
}
bool CEsmeClient::mcfn_processQuerySmRes(){
	return true;
}
bool CEsmeClient::mcfn_processReplaceSmRes(){
	return true;
}

bool CEsmeClient::mcfn_processResponceMessage(Smpp::Uint8* u8L_Msg,int iL_length){
	pmdu8_dataPkt=u8L_Msg;
	mdsi_len=iL_length;
	try{	
		switch(Smpp::get_command_id(u8L_Msg)){
			case Smpp::CommandId::GenericNack:
				break;
			case Smpp::CommandId::BindReceiverResp:
			case Smpp::CommandId::BindTransmitterResp:
			case Smpp::CommandId::BindTransceiverResp:
				return mcfn_processBindRes();
				break;
			case Smpp::CommandId::UnbindResp:
				return mcfn_processUnbindRes();
				break;
			case Smpp::CommandId::QuerySmResp:
				return mcfn_processQuerySmRes();
				break;
			case Smpp::CommandId::SubmitSmResp:
				return mcfn_processSubmitSmRes();
				break;
			case Smpp::CommandId::DeliverSmResp:
				return mcfn_processDlrSmRes();	
				break;
			case Smpp::CommandId::ReplaceSmResp:
				return mcfn_processReplaceSmRes();
				break;
			case Smpp::CommandId::CancelSmResp:
				return mcfn_processCancelSmRes();
				break;
			case Smpp::CommandId::EnquireLinkResp:
				return mcfn_processEnqLinkRes();
				break;
			case Smpp::CommandId::SubmitMultiResp:
				return mcfn_processSubmitMultiSmRes();
				break;
			case Smpp::CommandId::DataSmResp:
				return mcfn_processDataSmRes();
				break;
			case Smpp::CommandId::BroadcastSmResp:
				break;
			case Smpp::CommandId::QueryBroadcastSmResp:
				break;
			case Smpp::CommandId::CancelBroadcastSmResp:
				break;
			default:
				printf("unhandled message came:%d\n",Smpp::get_command_id(u8L_Msg));
				break;
		}
		return true;
	}
	catch(Smpp::Error& e){
		printf("SMPP error: %s\n",e.what());
	}
	catch(std::exception& e) {
		printf("std::exception error:%s\n",e.what());
	} catch(...) {
		printf("Unknown exception\n");
	}
	return false;

}
