#include"EsmeClient.hpp"
CEsmeClient::CEsmeClient(){
}
CEsmeClient::~CEsmeClient(){
}
void CEsmeClient::mcfn_initilise(std::string CL_systemId,std::string CL_passwd,std::string CL_systemType,int iL_interfaceVer){
	meC_systemId=CL_systemId;
	meC_password=CL_passwd;
	meC_serviceType=CL_systemType;
	mesi_interfaceVersion=iL_interfaceVer;	
	mesi_seqNum=0;
        mdsi_handShakeCount=0;
}
int CEsmeClient::mcfn_getSeqNumber(){
	++mesi_seqNum;
	if(mesi_seqNum>=0x7FFFFFFF){
		mesi_seqNum=0;
	}
	return mesi_seqNum;
}
bool CEsmeClient::mcfn_sendUssdBind(){
	CUssdBind CL_bind;
	strcpy((char*)CL_bind.pmcu8_AccountName,meC_systemId.c_str());//,11);
	strcpy((char*)CL_bind.pmcu8_password,meC_password.c_str());//,9);
	strcpy((char*)CL_bind.pmcu8_systemType,meC_serviceType.c_str());//,13);
	CL_bind.mcsi_interfaceVersion=mesi_interfaceVersion;
	CL_bind.mcsi_cmdLength=COMMAND_ID::USSD_BIND_LEN;//if it is fixed lenght we can define in Headers.
	CL_bind.mcsi_cmdId=COMMAND_ID::USSD_BIND;
	u8 pu8L_data[COMMAND_ID::USSD_BIND_LEN]={0x00};
	CL_bind.mcfn_getParam(&pu8L_data[20]);
	CL_bind.mcfn_getHeader(&pu8L_data[0]);
        mdsi_handShakeCount =0;
	return mcfn_sendMsgToServer((u8*)pu8L_data,CL_bind.mcsi_cmdLength);	
}
bool CEsmeClient::mcfn_sendUssdBindResp(CHeader &CL_header,int iL_status,std::string CL_accName){
	CUssdBindResp CL_pdu(CL_header);
	strcpy((char*)CL_pdu.pmcu8_AccountName,CL_accName.c_str());	
	 CL_pdu.mcsi_cmdLength=COMMAND_ID::USSD_BINDRESP_LEN;//if it is fixed lenght we can define in Headers.
        CL_pdu.mcsi_cmdId=COMMAND_ID::USSD_BINDRESP;
	CL_pdu.mcsi_cmdStatus=iL_status;
        u8 pu8L_data[COMMAND_ID::USSD_BINDRESP_LEN]={0x00};
        CL_pdu.mcfn_getParam(&pu8L_data[20]);
        CL_pdu.mcfn_getHeader(&pu8L_data[0]);
        return mcfn_sendMsgToServer((u8*)pu8L_data,CL_pdu.mcsi_cmdLength);
}
bool CEsmeClient::mcfn_sendUssdUnbind(){
	CUssdUnbind CL_unbind;
	CL_unbind.mcsi_cmdLength=COMMAND_ID::USSD_UNBIND_LEN;//if it is fixed lenght we can define in Headers.
	CL_unbind.mcsi_cmdId=COMMAND_ID::USSD_UNBIND;
	u8 pu8L_data[COMMAND_ID::USSD_UNBIND_LEN]={0x00};
	CL_unbind.mcfn_getHeader(&pu8L_data[0]);
	return mcfn_sendMsgToServer((u8*)pu8L_data,CL_unbind.mcsi_cmdLength);

}
bool CEsmeClient::mcfn_sendUssdShake(){

       if(mdsi_handShakeCount++ >5 )
       {
           mdsi_handShakeCount = 0;
           return false;
       } 
	CUssdShake CL_shake;
	CL_shake.mcsi_cmdLength=COMMAND_ID::USSD_HANDSHAKE_LEN;
	CL_shake.mcsi_cmdId=COMMAND_ID::USSD_HANDSHAKE;
	u8 pu8L_data[COMMAND_ID::USSD_HANDSHAKE_LEN]={0x00};
	CL_shake.mcfn_getHeader(&pu8L_data[0]);
	return mcfn_sendMsgToServer((u8*)pu8L_data,CL_shake.mcsi_cmdLength);
}

bool CEsmeClient::mcfn_sendUssdShakeResp(CHeader &CL_header){
	CL_header.mcsi_cmdId=COMMAND_ID::USSD_HANDSHAKERESP;
        CL_header.mcsi_cmdLength=COMMAND_ID::USSD_HANDSHAKERESP_LEN;
        u8 pu8L_data[COMMAND_ID::USSD_HANDSHAKERESP_LEN]={0x00};
        CL_header.mcfn_getHeader(&pu8L_data[0]);
	return mcfn_sendMsgToServer((u8*)pu8L_data,CL_header.mcsi_cmdLength);
}

bool CEsmeClient::mcfn_sendUssdBegin(CHeader& CL_Header,std::string CL_msisdn,std::string CL_serviceCode,u8 u8L_codeScema,std::string CL_ussdString,u8 u8L_ussdOptype){
	CUssdBegin CL_begin(CL_Header);	
	memcpy(CL_begin.pmcu8_msisdn,CL_msisdn.c_str(),CL_msisdn.length());
	if(!CL_serviceCode.empty())
		memcpy(CL_begin.pmcu8_serviceCode,CL_serviceCode.c_str(),CL_serviceCode.length());	
	memcpy(CL_begin.pmcu8_ussdString,CL_ussdString.c_str(),CL_ussdString.length());
	CL_begin.mcu8_ussdOptype=u8L_ussdOptype;
	CL_begin.mcu8_codeSchema=u8L_codeScema;
	CL_begin.mcu8_ussdVersion=mesi_interfaceVersion;
	CL_begin.mcsi_cmdId=COMMAND_ID::USSD_BEGIN;
	CL_begin.mcsi_cmdLength=COMMAND_ID::USSD_BEGIN_LEN;
	u8 pu8L_data[COMMAND_ID::USSD_BEGIN_LEN]={0x00};
	CL_begin.mcfn_getParam(&pu8L_data[20]);
	CL_begin.mcfn_getHeader(&pu8L_data[0]);
	return mcfn_sendMsgToServer((u8*)pu8L_data,CL_begin.mcsi_cmdLength);	
}
bool CEsmeClient::mcfn_sendUssdContinue(CHeader& CL_header,std::string CL_msisdn,std::string CL_serviceCode,u8 u8L_codeScema,std::string CL_ussdString,u8 u8L_ussdOptype){
	CUssdContinue CL_continue(CL_header);
	memcpy(CL_continue.pmcu8_msisdn,CL_msisdn.c_str(),CL_msisdn.length());
	if(!CL_serviceCode.empty())
		memcpy(CL_continue.pmcu8_serviceCode,CL_serviceCode.c_str(),CL_serviceCode.length());
	memcpy(CL_continue.pmcu8_ussdString,CL_ussdString.c_str(),CL_ussdString.length());
	CL_continue.mcu8_codeSchema=u8L_codeScema;
	CL_continue.mcsi_cmdId=COMMAND_ID::USSD_CONTINUE;
	CL_continue.mcsi_cmdLength=COMMAND_ID::USSD_CONTINUE_LEN;
	CL_continue.mcu8_ussdOptype=u8L_ussdOptype;
	CL_continue.mcu8_ussdVersion=mesi_interfaceVersion;
	u8 pu8L_data[COMMAND_ID::USSD_CONTINUE_LEN]={0x00};
	CL_continue.mcfn_getParam(&pu8L_data[20]);
	CL_continue.mcfn_getHeader(&pu8L_data[0]);
	return mcfn_sendMsgToServer((u8*)pu8L_data,CL_continue.mcsi_cmdLength);
}
bool CEsmeClient::mcfn_sendUssdEnd(CHeader& CL_header,std::string CL_msisdn,std::string CL_serviceCode,u8 u8L_codeScema,std::string CL_ussdString,u8 u8L_ussdOptype){
	CUssdEnd CL_end(CL_header);
	memcpy(CL_end.pmcu8_msisdn,CL_msisdn.c_str(),CL_msisdn.length());
	if(!CL_serviceCode.empty())
		memcpy(CL_end.pmcu8_serviceCode,CL_serviceCode.c_str(),CL_serviceCode.length());
	memcpy(CL_end.pmcu8_ussdString,CL_ussdString.c_str(),CL_ussdString.length());
	CL_end.mcu8_codeSchema=u8L_codeScema;
	CL_end.mcsi_cmdId=COMMAND_ID::USSD_END;
	CL_end.mcsi_cmdLength=COMMAND_ID::USSD_END_LEN;
	CL_end.mcu8_ussdOptype=u8L_ussdOptype;
	CL_end.mcu8_ussdVersion=mesi_interfaceVersion;
	u8 pu8L_data[COMMAND_ID::USSD_END_LEN]={0x00};
	CL_end.mcfn_getParam(&pu8L_data[20]);
	CL_end.mcfn_getHeader(&pu8L_data[0]);
	return mcfn_sendMsgToServer((u8*)pu8L_data,CL_end.mcsi_cmdLength);
}
bool CEsmeClient::mcfn_sendUssdAbort(CHeader &CL_header){
	CL_header.mcsi_cmdId=COMMAND_ID::USSD_ABORT;
        CL_header.mcsi_cmdLength=COMMAND_ID::USSD_ABORT_LEN;
        u8 pu8L_data[COMMAND_ID::USSD_ABORT_LEN]={0x00};
        CL_header.mcfn_getHeader(&pu8L_data[0]);
	return mcfn_sendMsgToServer((u8*)pu8L_data,CL_header.mcsi_cmdLength);
}
bool CEsmeClient::mcfn_sendUssdSwitch(CHeader &CL_header,u8 u8L_switchMode,std::string CL_msisdn,std::string CL_serviceCode,std::string CL_ussdStr){
	CUssdSwitch CL_switch(CL_header);
	CL_switch.mcu8_switchMode=u8L_switchMode;
	memcpy(CL_switch.pmcu8_msisdn,CL_msisdn.c_str(),CL_msisdn.length());	
	if(!CL_serviceCode.empty())
	memcpy(CL_switch.pmcu8_serviceCode,CL_serviceCode.c_str(),CL_serviceCode.length());	
	memcpy(CL_switch.pmcu8_ussdString,CL_ussdStr.c_str(),CL_ussdStr.length());
	
	 CL_switch.mcsi_cmdId=COMMAND_ID::USSD_SWITCH;
        CL_switch.mcsi_cmdLength=COMMAND_ID::USSD_SWITCH_LEN;
        u8 pu8L_data[COMMAND_ID::USSD_SWITCH_LEN]={0x00};
        CL_switch.mcfn_getHeader(&pu8L_data[0]);
        CL_switch.mcfn_getParam(&pu8L_data[20]);
	return mcfn_sendMsgToServer((u8*)pu8L_data,CL_switch.mcsi_cmdLength);
}
bool CEsmeClient::mcfn_sendUssdChargingInd(CHeader &CL_header,int iL_chargeRatio,int iL_chargeType,u8 *pu8L_chargeSource,u8 u8_chargeLocation){
	CUssdChargingInd CL_pdu(CL_header);
	CL_pdu.mcsi_chargeRation=iL_chargeRatio;
	CL_pdu.mcsi_chargeType=iL_chargeType;
	memcpy(CL_pdu.pmcu8_chargeSource,pu8L_chargeSource,21);
	CL_pdu.mcu8_chargeLocation=u8_chargeLocation;
	
	  CL_pdu.mcsi_cmdId=COMMAND_ID::USSD_CHARGINGRINDESP;
        CL_pdu.mcsi_cmdLength=COMMAND_ID::USSD_CHARGINGRINDESP_LEN;
        u8 pu8L_data[COMMAND_ID::USSD_CHARGINGRINDESP_LEN]={0x00};
        CL_pdu.mcfn_getHeader(&pu8L_data[0]);
        CL_pdu.mcfn_getParam(&pu8L_data[20]);
	return mcfn_sendMsgToServer((u8*)pu8L_data,CL_pdu.mcsi_cmdLength);
}

bool CEsmeClient::mcfn_processReceivedMessage(CHeader& CL_header,u8* u8_param,int iL_paramLen){
	
		switch(CL_header.mcsi_cmdId){
                        case COMMAND_ID::USSD_BIND:
					return mcfn_onUssdBind(CL_header,u8_param,iL_paramLen);	
                        case COMMAND_ID::USSD_BINDRESP:
					return mcfn_onUssdBindResp(CL_header,u8_param,iL_paramLen);
                        case COMMAND_ID::USSD_UNBIND:
					return mcfn_onUssdUnbind(CL_header);
                        case COMMAND_ID::USSD_UNBINDRESP:
					return mcfn_onUssdUnbindResp(CL_header);
                        case COMMAND_ID::USSD_BEGIN:
					return mcfn_onUssdBegin(CL_header,u8_param,iL_paramLen);
                        case COMMAND_ID::USSD_CONTINUE:
					return mcfn_onUsddContinue(CL_header,u8_param,iL_paramLen);
                        case COMMAND_ID::USSD_END:
					return mcfn_onUssdEnd(CL_header,u8_param,iL_paramLen);
                        case COMMAND_ID::USSD_ABORT:
					return mcfn_onUssdAbort(CL_header);
                        case COMMAND_ID::USSD_SWITCH:
					return mcfn_onUssdSwitch(CL_header,u8_param,iL_paramLen);
                        case COMMAND_ID::USSD_CHARGINGIND:
					return mcfn_onUssdChargingInd(CL_header,u8_param,iL_paramLen);
                        case COMMAND_ID::USSD_CHARGINGRINDESP:
					return mcfn_onUssdChargingIndRes(CL_header);
                        case COMMAND_ID::USSD_HANDSHAKE:
					return mcfn_onUssdShake(CL_header);
                        case COMMAND_ID::USSD_HANDSHAKERESP:
					return mcfn_onUssdShakeResp(CL_header);
				break;
			default:
				
				break;
		}
	return false;
}
