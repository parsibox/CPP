#include"EsmeClient.hpp"
CEsmeClient::CEsmeClient()
{
}
CEsmeClient::~CEsmeClient()
{
}
void CEsmeClient::mcfn_initilise(std::string CL_systemId,std::string CL_passwd,std::string CL_systemType,int iL_interfaceVer)
{
	meC_systemId=CL_systemId;
	meC_password=CL_passwd;
	meC_serviceType=CL_systemType;
	mesi_interfaceVersion=iL_interfaceVer;	
	mesi_seqNum=0;
}
short CEsmeClient::mcfn_getSeqNumber()
{
	++mesi_seqNum;
	if(mesi_seqNum>=0x7FFF){
		mesi_seqNum=0;
	}
	return mesi_seqNum;
}

int CEsmeClient::mcfn_getReqNumber()
{
	++mesi_reqId;
	if(mesi_reqId>=0x7FFFFFFF){
		mesi_reqId=0;
	}
	return mesi_reqId;
}

std::string CEsmeClient::mcfn_convertToString(int iL_data)
{
	std::stringstream CL_Value;
	CL_Value<<iL_data;
	return CL_Value.str();
}

bool CEsmeClient::mcfn_sendXmlMsg(std::string CL_xmlMsg,long iL_seqNum,int iL_operation)
{
	printf("sending data:%s\n",CL_xmlMsg.c_str());
	CXmlHeader CL_hdr(CL_xmlMsg.length()+OWN_HEADER_LENGTH,iL_seqNum);
        CL_hdr.mcsi_operationType = iL_operation;
	u8 pu8L_data[OWN_HEADER_LENGTH];
	memset(pu8L_data,0x00,OWN_HEADER_LENGTH);
	CL_hdr.mcfn_getHeader(&pu8L_data[0]);
	return (mcfn_sendMsgToServer((u8*)pu8L_data,OWN_HEADER_LENGTH) & mcfn_sendMsgToServer((u8*)CL_xmlMsg.c_str(),CL_xmlMsg.length()) );	

}

bool CEsmeClient::mcfn_sendUssdAbort(CXmlHeader& CL_hdr)
{
     u8 pu8L_hdr[OWN_HEADER_LENGTH];
     CL_hdr.mcfn_getHeader(pu8L_hdr);
    return mcfn_sendMsgToServer((u8*)pu8L_hdr,OWN_HEADER_LENGTH) ;
}
bool CEsmeClient::mcfn_sendUssdBind()
{
	CXmlUssdBind CL_bind;
	CL_bind.mcC_requestId = mcfn_convertToString(mcfn_getReqNumber()); 
	CL_bind.mcC_userName = meC_systemId;
	CL_bind.mcC_password =  meC_password;
	CL_bind.mcC_applicationId =  mcfn_convertToString(mesi_interfaceVersion);
	printf("%s\n",CL_bind.mcfn_encode().c_str());
	return mcfn_sendXmlMsg(CL_bind.mcfn_encode(),mesi_reqId,XML_LOGIN);
}

bool CEsmeClient::mcfn_sendUssdBindResp(long sL_seqId,int iL_status,std::string CL_authMsg,int iL_reqId)
{
	CXmlUssdBindResp CL_pdu;
	CL_pdu.mcC_requestId= mcfn_convertToString(iL_reqId);
	CL_pdu.mcC_authMsg= CL_authMsg;
	CL_pdu.mcC_errorCode = (iL_status<=0)?"":mcfn_convertToString(iL_status);
	return mcfn_sendXmlMsg(CL_pdu.mcfn_encode(),sL_seqId,XML_LOGIN_RESP);
}

bool CEsmeClient::mcfn_processLoginRes(std::string CL_loginRes,int iL_len)
{
	CXmlUssdBindResp CL_pdu;
	if(CL_pdu.mcfn_decode(CL_loginRes.c_str()))
	{ 
		return CL_pdu.mcC_errorCode=="0" ;
	}
	return false;
}

bool CEsmeClient::mcfn_sendUssdShake()
{
	//return mcfn_sendXmlMsg(CL_shake.mcfn_encode());
}
bool CEsmeClient::mcfn_sendUssdBegin(CXmlUssdMsg &CL_pdu)
{
	return mcfn_sendXmlMsg(CL_pdu.mcfn_encode(),mcfn_getReqNumber(),XML_START);
}
bool CEsmeClient::mcfn_sendUssdContinue(CXmlUssdMsg &CL_pdu,long sL_seqId)
{
	return mcfn_sendXmlMsg(CL_pdu.mcfn_encode(),sL_seqId,XML_CONTINUE);
}

bool CEsmeClient::mcfn_sendUssdEnd(CXmlUssdMsg &CL_pdu,long sL_seqId)
{
	return mcfn_sendXmlMsg(CL_pdu.mcfn_encode(),sL_seqId,XML_END);
}

bool CEsmeClient::mcfn_processReceivedMessage(CXmlHeader& CL_header,u8* u8_param,int iL_paramLen){
	CXmlUssdMsg CL_pdu;
        std::string CL_param;
        CL_param.assign((const char *)u8_param,iL_paramLen);
	if(CL_pdu.mcfn_decode(CL_param) == false)
		return false;
	printf("Message Received :%s\n",CL_pdu.mcfn_ShortDebugString().c_str());
	switch((Operations)CL_header.mcsi_operationType)	
	{
		case XML_START:
			return mcfn_onUssdBegin(CL_header,CL_pdu);
		case XML_CONTINUE:
			return mcfn_onUsddContinue(CL_header,CL_pdu);
		case XML_END:	
			return mcfn_onUssdEnd(CL_header,CL_pdu);
		case XML_ABORT:
			return mcfn_onUssdAbort(CL_header);
		case XML_ENQ_LINK:
			return mcfn_onUssdShake(CL_header);
		case XML_ENQ_LINK_RESP:
			return mcfn_onUssdShakeResp(CL_header);
                default:
                      break;
	}	
	return false;
}
