#include<UssdInterface.hpp>
#include<Externs.h>
#include<HauwaiHeaders.hpp>
#define USSD_HUWAI_HEADERLEN 20
 
UssdInterface::UssdInterface(CSTSSocket* pCL_srvSock):CEsmeClient(USSD_HUWAI_HEADERLEN,pCL_srvSock)
{
}
UssdInterface::~UssdInterface()
{
}

bool UssdInterface::mcfn_sendUssdBind()
{
	CUssdBind CL_bind;
	strcpy((char*)CL_bind.pmcu8_AccountName,mdC_name.c_str());//,11);
	strcpy((char*)CL_bind.pmcu8_password,mdC_password.c_str());//,9);
	strcpy((char*)CL_bind.pmcu8_systemType,mdC_applicationId.c_str());//,13);
	CL_bind.mcsi_interfaceVersion=mdsi_version;
	CL_bind.mcsi_cmdLength=COMMAND_ID::USSD_BIND_LEN;//if it is fixed lenght we can define in Headers.
	CL_bind.mcsi_cmdId=COMMAND_ID::USSD_BIND;
	u8 pu8L_data[COMMAND_ID::USSD_BIND_LEN]={0x00};
	CL_bind.mcfn_getParam(&pu8L_data[20]);
	CL_bind.mcfn_getHeader(&pu8L_data[0]);
	return mcfn_send((char*)pu8L_data,CL_bind.mcsi_cmdLength);	
}
bool UssdInterface::mcfn_sendUssdShake(){
	CUssdShake CL_shake;
	CL_shake.mcsi_cmdLength=COMMAND_ID::USSD_HANDSHAKE_LEN;
	CL_shake.mcsi_cmdId=COMMAND_ID::USSD_HANDSHAKE;
	u8 pu8L_data[COMMAND_ID::USSD_HANDSHAKE_LEN]={0x00};
	CL_shake.mcfn_getHeader(&pu8L_data[0]);
	return mcfn_send((char*)pu8L_data,CL_shake.mcsi_cmdLength);
}


bool UssdInterface::mcfn_onUssdBindResp(UssdData &CL_pdu)
{
	DBG_INFO((CG_EventLog),("Receive Huwai Ussd Bind Responce :%s",CL_pdu.ShortDebugString().c_str()));
        return CL_pdu.session_status()==0;
}

bool UssdInterface::mcfn_onUssdUnbindResp(UssdData &CL_pdu)
{
	DBG_INFO((CG_EventLog),("Receive Huwai Ussd Unbin Responce :%s",CL_pdu.ShortDebugString().c_str()));
        return false;
}

bool UssdInterface::mcfn_onUssdShake(UssdData &CL_pdu)
{
	CHeader CL_hdr;
	CL_hdr.mcsi_senderCB = CL_pdu.sender_session_id();
	CL_hdr.mcsi_receiverCB = mcfn_getSeqNumber();
	CL_hdr.mcsi_cmdId = COMMAND_ID::USSD_HANDSHAKERESP;
	CL_hdr.mcsi_cmdLength = COMMAND_ID::USSD_HANDSHAKERESP_LEN ;
        u8 pu8L_data[COMMAND_ID::USSD_HANDSHAKERESP_LEN]={0x00};
        CL_hdr.mcfn_getHeader(&pu8L_data[0]);
        return mcfn_sendMsgToServer(pu8L_data,CL_hdr.mcsi_cmdLength,CL_pdu); 
}
bool UssdInterface::mcfn_onUssdShakeResp(UssdData& CL_pdu)
{
	DBG_INFO((CG_EventLog),("Receive Huwai Ussd Shake Responce :%s",CL_pdu.ShortDebugString().c_str()));
        return true;
}
//pure virtual function 
bool UssdInterface::mcfn_encode(UssdData& CL_pdu, std::string &CL_data)
{
	DBG_INFO((CG_EventLog),("encoding data :%s",CL_pdu.ShortDebugString().c_str()));
	CUssdBegin CL_pduData;
	CL_pduData.mcsi_senderCB = CL_pdu.sender_session_id();
	CL_pduData.mcsi_receiverCB =  CL_pdu.receiver_session_id();
	CL_pduData.mcsi_cmdStatus = CL_pdu.session_status();
	CL_pduData.mcsi_cmdId = mefn_encodeOperationType(CL_pdu.op_type());
	CL_pduData.mcsi_cmdLength = mefn_getCommandLen( CL_pduData.mcsi_cmdId );
	u8 pu8L_data[CL_pduData.mcsi_cmdLength];
	if(CL_pduData.mcsi_cmdLength > USSD_HUWAI_HEADERLEN)
	{
		if(CL_pdu.has_version()) CL_pduData.mcu8_ussdVersion = CL_pdu.version();
		if(CL_pdu.has_dcs()) CL_pduData.mcu8_codeSchema = CL_pdu.dcs();
		if(CL_pdu.has_oa()) memcpy(CL_pduData.pmcu8_msisdn,CL_pdu.oa().c_str(),CL_pdu.oa().length());
		if(CL_pdu.has_msg()) memcpy(CL_pduData.pmcu8_ussdString, CL_pdu.msg().c_str(), CL_pdu.msg().length());
		if(CL_pdu.has_srv_code()) memcpy(CL_pduData.pmcu8_serviceCode,CL_pdu.srv_code().c_str(), CL_pdu.srv_code().length());
		CL_pduData.mcu8_ussdOptype = mefn_encodeMessageType(CL_pdu.msg_type());
		CL_pduData.mcfn_getParam(&pu8L_data[20]);
	}
	CL_pduData.mcfn_getHeader(&pu8L_data[0]);
        CL_data.assign((char*)&pu8L_data[0],CL_pduData.mcsi_cmdLength);
        //mcfn_send((char*)pu8L_data,CL_pduData.mcsi_cmdLength);
	DBG_INFO((CG_EventLog),("Lenght of message and sending Message Lenght %d-%d",CL_pduData.mcsi_cmdLength,CL_data.length()));
        return true;
}
bool UssdInterface::mcfn_decode(std::string& CL_data,UssdData& CL_pdu)
{
	CHeader CL_hdr;
	CL_hdr.mcfn_setHeader((u8*)CL_data.c_str());
	CL_pdu.set_op_type( (UssdData::operation_type) mefn_decodeOperationType(CL_hdr.mcsi_cmdId));
	CL_pdu.set_session_status(CL_hdr.mcsi_cmdStatus);
	CL_pdu.set_sender_session_id(CL_hdr.mcsi_senderCB);
	CL_pdu.set_receiver_session_id(CL_hdr.mcsi_receiverCB);
	CUssdBegin CL_pduData(CL_hdr);
	switch(CL_pdu.op_type())
	{
		case  UssdData::LOGIN:
			break; 
		case UssdData::LOGIN_RESP:
			CL_pdu.set_username(CL_data.substr(USSD_HUWAI_HEADERLEN,CL_data.length()));
			break;
		case UssdData::START:
		case UssdData::END:
		case UssdData::CONTINUE:
			CL_pduData.mcfn_setParam((u8*)CL_data.substr(USSD_HUWAI_HEADERLEN,CL_data.length()).c_str());

			CL_pdu.set_version(CL_pduData.mcu8_ussdVersion);
			CL_pdu.set_dcs(CL_pduData.mcu8_codeSchema);
			CL_pdu.set_oa((char*)CL_pduData.pmcu8_msisdn);
			// CL_pdu.set_da(CL_pduData.pmcu8_msisdn);
			CL_pdu.set_msg((char*)CL_pduData.pmcu8_ussdString);
			CL_pdu.set_msg_type( (UssdData::Msg_Type) mefn_decodeMessageType(CL_pduData.mcu8_ussdOptype));
			CL_pdu.set_srv_code((char*)CL_pduData.pmcu8_serviceCode);
			break;
		case UssdData::LOGOUT:
		case UssdData::LOGOUT_RESP:
		case UssdData::ABORT:
		case UssdData::ENQ_LINK:
		case UssdData::ENQ_LINK_RESP:
		default:
			break;
	}
	DBG_INFO((CG_EventLog),("decoded data :%s",CL_pdu.ShortDebugString().c_str()));
      return true;
}

int UssdInterface::mcfn_getDataLen(u8* pcL_header)
{
	CHeader CL_hdr;
	CL_hdr.mcfn_setHeader(pcL_header);
	return CL_hdr.mcsi_cmdLength;
}

int  UssdInterface::mefn_encodeOperationType(UssdData::operation_type eL_opType)
{
	switch(eL_opType)
	{
		case UssdData::LOGIN:
			return COMMAND_ID::USSD_BIND ;
		case UssdData::LOGIN_RESP:
			return COMMAND_ID::USSD_BINDRESP ;
		case UssdData::LOGOUT:
			return  COMMAND_ID::USSD_UNBIND ;
		case UssdData::LOGOUT_RESP:
			return  COMMAND_ID::USSD_UNBINDRESP ;
		case UssdData::START:
			DBG_INFO((CG_EventLog),("Begin messgae recieved "));
			return  COMMAND_ID::USSD_BEGIN;
		case UssdData::CONTINUE:
			DBG_INFO((CG_EventLog),("Continue messgae recieved "));
			return  COMMAND_ID::USSD_CONTINUE ;
		case  UssdData::END:
			DBG_INFO((CG_EventLog),("End messgae recieved "));
			return  COMMAND_ID::USSD_END;
		case UssdData::ABORT:
			return  COMMAND_ID::USSD_ABORT ;
		case UssdData::ENQ_LINK:
			return COMMAND_ID::USSD_HANDSHAKE ;
		case UssdData::ENQ_LINK_RESP:
			return  COMMAND_ID::USSD_HANDSHAKERESP;
	}
}

int UssdInterface::mefn_getCommandLen(u8 u8L_cmd)
{
	switch(u8L_cmd)
	{
		case COMMAND_ID::USSD_BIND :
			return COMMAND_ID::USSD_BIND_LEN;
		case COMMAND_ID::USSD_BINDRESP :
			return COMMAND_ID::USSD_BINDRESP_LEN;
		case COMMAND_ID::USSD_UNBIND :
			return COMMAND_ID::USSD_UNBIND_LEN;
		case COMMAND_ID::USSD_UNBINDRESP :
			return COMMAND_ID::USSD_UNBINDRESP_LEN;
		case COMMAND_ID::USSD_BEGIN:
			DBG_INFO((CG_EventLog),("Begin messgae recieved "));
			return COMMAND_ID::USSD_BEGIN_LEN;
		case COMMAND_ID::USSD_CONTINUE :
			DBG_INFO((CG_EventLog),("continue messgae recieved :%d",COMMAND_ID::USSD_CONTINUE_LEN));
			return COMMAND_ID::USSD_CONTINUE_LEN;
		case COMMAND_ID::USSD_END:
			DBG_INFO((CG_EventLog),("end messgae recieved "));
			return COMMAND_ID::USSD_END_LEN;
		case COMMAND_ID::USSD_ABORT :
			return COMMAND_ID::USSD_ABORT_LEN;
		case COMMAND_ID::USSD_HANDSHAKE :
			return COMMAND_ID::USSD_HANDSHAKE_LEN;
		case COMMAND_ID::USSD_HANDSHAKERESP:
			return COMMAND_ID::USSD_HANDSHAKERESP_LEN;
        }
}

int UssdInterface::mefn_decodeOperationType(int iL_operation)
{
	switch(iL_operation)
	{
		case COMMAND_ID::USSD_BIND :
			return UssdData::LOGIN;
		case COMMAND_ID::USSD_BINDRESP :
			return UssdData::LOGIN_RESP;
		case COMMAND_ID::USSD_UNBIND :
			return UssdData::LOGOUT;
		case COMMAND_ID::USSD_UNBINDRESP :
			return UssdData::LOGOUT_RESP;
		case COMMAND_ID::USSD_BEGIN:
			return UssdData::START;
		case COMMAND_ID::USSD_CONTINUE :
			return UssdData::CONTINUE;
		case COMMAND_ID::USSD_END:
			return UssdData::END;
		case COMMAND_ID::USSD_ABORT :
			return UssdData::ABORT;
		case COMMAND_ID::USSD_HANDSHAKE :
			return UssdData::ENQ_LINK;
		case COMMAND_ID::USSD_HANDSHAKERESP:
			return UssdData::ENQ_LINK_RESP; 
	}
}

u8  UssdInterface::mefn_encodeMessageType(UssdData::Msg_Type eL_msgType)
{
	switch(eL_msgType)
	{
		case UssdData::REQUEST:
			return USSD_OPTYPE::USSD_REQUEST;
		case UssdData::NOTIFY:
			return  USSD_OPTYPE::USSD_NOTIFY;
		case UssdData::RESPONCE:
			return  USSD_OPTYPE::USSD_RESPONCE;
		case UssdData::RELEASE:
			return  USSD_OPTYPE::USSD_RELEASE;
	}
}

int UssdInterface::mefn_decodeMessageType(u8 opType)
{
	switch (opType)
	{
             case USSD_OPTYPE::USSD_REQUEST:
                  return UssdData::REQUEST;
             case USSD_OPTYPE::USSD_NOTIFY:
                  return UssdData::NOTIFY;
             case USSD_OPTYPE::USSD_RESPONCE:
                  return UssdData::RESPONCE;
             case USSD_OPTYPE::USSD_RELEASE:
                  return UssdData::RELEASE;
	}
}
