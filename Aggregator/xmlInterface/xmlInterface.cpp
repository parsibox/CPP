#include<xmlInterface.hpp>
#include<Externs.h>
#include<OwnHeaders.hpp>


xmlInterface::xmlInterface(int iL_port):CEsmeServer(USSD_OWN_HEADERLEN,iL_port)
{
}
xmlInterface::~xmlInterface()
{
}
//pure virtual function 
int xmlInterface::mcfn_getDataLen(u8* pcL_hdr)
{
 CXmlHeader CL_header;
 CL_header.mcfn_setHeader(pcL_hdr);
 return CL_header.mcsi_cmdLength;
}

bool xmlInterface::mcfn_encode(UssdData& CL_pdu,std::string &CL_data)
{

        DBG_INFO((CG_EventLog),("encoding data[%s]",CL_pdu.ShortDebugString().c_str()));
	CXmlHeader CL_hdr;
	CL_hdr.mcsi_sessionId[0] = CL_pdu.sender_session_id();
	CL_hdr.mcsi_sessionId[1] =  CL_pdu.receiver_session_id();
	CL_hdr.mcsi_sessionStatus = CL_pdu.session_status();
	CL_hdr.mcsi_operationType = mefn_encodeOperationType(CL_pdu.op_type());
	std::string CL_param= "";
	if(CL_hdr.mcsi_operationType == XML_LOGIN )
	{
		CXmlUssdBind CL_bind;
		CL_bind.mcC_userName = CL_pdu.username();
		CL_bind.mcC_password = CL_pdu.password();
		CL_bind.mcC_applicationId = CL_pdu.app_id();
		CL_param = CL_bind.mcfn_encode();
	}
	else if(CL_hdr.mcsi_operationType == XML_LOGIN_RESP )
	{
		CXmlUssdBindResp CL_bindResp;
		CL_bindResp.mcC_authMsg = CL_pdu.msg();
                char errorCode[4];
                sprintf(errorCode,"%d",CL_pdu.status());
		CL_bindResp.mcC_errorCode = errorCode;
		CL_param = CL_bindResp.mcfn_encode();
	}
	else
	{

		CXmlUssdMsg CL_pduData(CL_hdr);
		if(CL_pdu.has_version()) CL_pduData.mcC_phase = mefn_convertString(CL_pdu.version());
		if(CL_pdu.has_dcs()) CL_pduData.mcC_dcs = mefn_convertString(CL_pdu.dcs());
		if(CL_pdu.has_oa()) CL_pduData.mcC_msisdn = CL_pdu.oa();
		if(CL_pdu.has_msg()) CL_pduData.mcC_userData = CL_pdu.msg();
		if(CL_pdu.has_srv_code()) CL_pduData.mcC_starCode = CL_pdu.srv_code();
		CL_pduData.mcC_msgType = mefn_encodeMessageType(CL_pdu.msg_type());
		if(CL_pdu.has_imsi()) CL_pduData.mcC_imsi = CL_pdu.imsi();
		CL_param = CL_pduData.mcfn_encode();
	}

	CL_hdr.mcsi_cmdLength = USSD_OWN_HEADERLEN + CL_param.length();
        DBG_INFO((CG_EventLog),("encoding data[%s][%s]",CL_hdr.mcfn_ShortDebugString().c_str(),CL_param.c_str()));
	u8 pu8L_hdr[USSD_OWN_HEADERLEN];
	CL_data.assign((char*)CL_hdr.mcfn_getHeader(pu8L_hdr),USSD_OWN_HEADERLEN) ;
        CL_data+= CL_param;
	return true;
}

bool xmlInterface::mcfn_decode(std::string &CL_data,UssdData& CL_pdu)
{
	CXmlHeader CL_hdr;
	DBG_INFO((CG_EventLog),("Received data Length[%d]",CL_data.length()));
	CL_hdr.mcfn_setHeader((u8*)CL_data.c_str());
	if(CL_hdr.mcsi_cmdLength > USSD_OWN_HEADERLEN)
	{
		DBG_INFO((CG_EventLog),("Received data[%s]",CL_data.substr(USSD_OWN_HEADERLEN,CL_data.length()).c_str()));
		if(CL_hdr.mcsi_operationType == XML_LOGIN )
		{
			CXmlUssdBind CL_bind;
			CL_bind.mcfn_decode(CL_data.substr(USSD_OWN_HEADERLEN,CL_data.length()).c_str());
			CL_pdu.set_username(CL_bind.mcC_userName);
			CL_pdu.set_password(CL_bind.mcC_password);
			CL_pdu.set_app_id(CL_bind.mcC_applicationId);
		}
		else if(CL_hdr.mcsi_operationType == XML_LOGIN_RESP )
		{
			CXmlUssdBindResp CL_bindResp;
			CL_bindResp.mcfn_decode(CL_data.substr(USSD_OWN_HEADERLEN,CL_data.length()).c_str());
                        CL_pdu.set_msg(CL_bindResp.mcC_authMsg);
			CL_pdu.set_status(atoi(CL_bindResp.mcC_errorCode.c_str()));
		}
		else
		{
			CXmlUssdMsg CL_pduData;
			CL_pduData.mcfn_decode(CL_data.substr(USSD_OWN_HEADERLEN,CL_data.length()).c_str());
			CL_pdu.set_version(atoi(CL_pduData.mcC_phase.c_str()));
			CL_pdu.set_dcs(atoi(CL_pduData.mcC_dcs.c_str()));
			CL_pdu.set_oa(CL_pduData.mcC_msisdn);
			// CL_pdu.set_da(CL_pduData.pmcu8_msisdn);
			CL_pdu.set_msg(CL_pduData.mcC_userData);
			CL_pdu.set_msg_type( (UssdData::Msg_Type) mefn_decodeMessageType(CL_pduData.mcC_msgType));
			CL_pdu.set_srv_code(CL_pduData.mcC_starCode);
		}
	}
	DBG_INFO((CG_EventLog),("After decode [%s]",CL_pdu.ShortDebugString().c_str()));
	CL_pdu.set_op_type( (UssdData::operation_type) mefn_decodeOperationType(CL_hdr.mcsi_operationType));
	CL_pdu.set_session_status(CL_hdr.mcsi_sessionStatus);
        //swaping logic applying here
	CL_pdu.set_sender_session_id(CL_hdr.mcsi_sessionId[1]);
	CL_pdu.set_receiver_session_id(CL_hdr.mcsi_sessionId[0]);
	return true;
}

int  xmlInterface::mefn_encodeOperationType(UssdData::operation_type eL_opType)
{
	switch(eL_opType)
	{
		case UssdData::LOGIN:
			return XML_LOGIN ;
		case UssdData::LOGIN_RESP:
			return XML_LOGIN_RESP;
		case UssdData::LOGOUT:
			return  XML_LOGOUT ;
		case UssdData::LOGOUT_RESP:
			return  XML_LOGOUT_RESP;
		case UssdData::START:
			return  XML_START;
		case UssdData::CONTINUE:
			return  XML_CONTINUE ;
		case  UssdData::END:
			return  XML_END;
		case UssdData::ABORT:
			return  XML_ABORT ;
		case UssdData::ENQ_LINK:
			return XML_ENQ_LINK ;
		case UssdData::ENQ_LINK_RESP:
			return  XML_ENQ_LINK_RESP;
	}
}

int xmlInterface::mefn_getCommandLen(int iL_cmd)
{
	switch(iL_cmd)
	{
		case XML_LOGIN:
		case XML_LOGIN_RESP :
		case XML_LOGOUT :
		case XML_LOGOUT_RESP :
		case XML_START:
		case XML_CONTINUE :
		case XML_END:
                    return USSD_OWN_HEADERLEN +1; 
		case XML_ABORT :
		case XML_ENQ_LINK :
		case XML_ENQ_LINK_RESP:
                     return USSD_OWN_HEADERLEN;
	}
}

int xmlInterface::mefn_decodeOperationType(int iL_operation)
{
	switch(iL_operation)
	{
		case XML_LOGIN: 
			return UssdData::LOGIN;
		case XML_LOGIN_RESP :
			return UssdData::LOGIN_RESP;
		case XML_LOGOUT :
			return UssdData::LOGOUT;
		case XML_LOGOUT_RESP :
			return UssdData::LOGOUT_RESP;
		case XML_START:
			return UssdData::START;
		case XML_CONTINUE:
			return UssdData::CONTINUE;
		case XML_END:
			return UssdData::END;
		case XML_ABORT:
			return UssdData::ABORT;
		case XML_ENQ_LINK :
			return UssdData::ENQ_LINK;
		case XML_ENQ_LINK_RESP:
			return UssdData::ENQ_LINK_RESP; 
	}
}

std::string   xmlInterface::mefn_encodeMessageType(UssdData::Msg_Type eL_msgType)
{
	switch(eL_msgType)
	{
		case UssdData::REQUEST:
			return "1";
		case UssdData::NOTIFY:
			return  "2";
		case UssdData::RESPONCE:
			return  "3";
		case UssdData::RELEASE:
			return  "4";
	}
}

int xmlInterface::mefn_decodeMessageType(std::string opType)
{
	switch (opType[0])
	{
		case '1':
			return UssdData::REQUEST;
		case '2':
			return UssdData::NOTIFY;
		case '3':
			return UssdData::RESPONCE;
		case '4':
			return UssdData::RELEASE;
	}
}

std::string xmlInterface::mefn_convertString(int input)
{
	std::stringstream CL_out; 
	CL_out<<input;
	return CL_out.str();
}
