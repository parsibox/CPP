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

bool CEsmeClient::mcfn_sendXmlMsg(std::string CL_xmlMsg,short iL_seqNum)
{
	printf("sending data:%s\n",CL_xmlMsg.c_str());
	CHeader CL_hdr(CL_xmlMsg.length(),iL_seqNum);
	u8 pu8L_data[HEADER_LENGTH];
	memset(pu8L_data,0x00,HEADER_LENGTH);
	CL_hdr.mcfn_getHeader(&pu8L_data[0]);
	return (mcfn_sendMsgToServer((u8*)pu8L_data,HEADER_LENGTH) & mcfn_sendMsgToServer((u8*)CL_xmlMsg.c_str(),CL_xmlMsg.length()) );	

}

bool CEsmeClient::mcfn_sendUssdBind()
{
	CUssdBind CL_bind;
	CL_bind.mcC_requestId = mcfn_convertToString(mcfn_getReqNumber()); 
	CL_bind.mcC_userName = meC_systemId;
	CL_bind.mcC_password =  meC_password;
	CL_bind.mcC_applicationId =  mcfn_convertToString(mesi_interfaceVersion);
	printf("%s\n",CL_bind.mcfn_encode().c_str());
	return mcfn_sendXmlMsg(CL_bind.mcfn_encode(),mcfn_getSeqNumber());
}

bool CEsmeClient::mcfn_sendUssdBindResp(short sL_seqId,int iL_status,std::string CL_authMsg,int iL_reqId)
{
	CUssdBindResp CL_pdu;
	CL_pdu.mcC_requestId= mcfn_convertToString(iL_reqId);
	CL_pdu.mcC_authMsg= CL_authMsg;
	CL_pdu.mcC_errorCode = (iL_status<=0)?"":mcfn_convertToString(iL_status);
	return mcfn_sendXmlMsg(CL_pdu.mcfn_encode(),sL_seqId);
}

bool CEsmeClient::mcfn_processLoginRes(std::string CL_loginRes,int iL_len)
{
	CUssdBindResp CL_pdu;
	if(CL_pdu.mcfn_decode(CL_loginRes.c_str()))
	{	
		if(CL_pdu.mcC_errorCode.empty())
			return true;
	}
	return false;
}

bool CEsmeClient::mcfn_sendUssdShake()
{
	//return mcfn_sendXmlMsg(CL_shake.mcfn_encode());
}
bool CEsmeClient::mcfn_sendUssdBegin(CUssdMsg &CL_pdu)
{
	return mcfn_sendXmlMsg(CL_pdu.mcfn_encode(),mcfn_getSeqNumber());
}
bool CEsmeClient::mcfn_sendUssdContinue(CUssdMsg &CL_pdu,short sL_seqId)
{
	if(CL_pdu.mcC_msgType.empty())
	CL_pdu.mcC_msgType=mcfn_convertToString(USSR);
	CL_pdu.mcC_endOfSession = "0"	;
	return mcfn_sendXmlMsg(CL_pdu.mcfn_encode(),sL_seqId);
}

bool CEsmeClient::mcfn_sendUssdEnd(CUssdMsg &CL_pdu,short sL_seqId)
{
	if(CL_pdu.mcC_msgType.empty())
	CL_pdu.mcC_msgType=mcfn_convertToString(USSN);
	CL_pdu.mcC_endOfSession = "1"	;
	return mcfn_sendXmlMsg(CL_pdu.mcfn_encode(),sL_seqId);
}

bool CEsmeClient::mcfn_processReceivedMessage(CHeader& CL_header,u8* u8_param,int iL_paramLen){
	CUssdMsg CL_pdu;
	if(CL_pdu.mcfn_decode((const char *)u8_param) == false)
		return false;
	printf("Message Received :%s\n",CL_pdu.mcfn_ShortDebugString().c_str());
	switch((XmlMsgTypes)atoi(CL_pdu.mcC_msgType.c_str()))	
	{
		case PSSR:
		case USSR:
			return mcfn_onUssdBegin(CL_header,CL_pdu);
		case USSRACK:
		case PSSRACK:
			return 	mcfn_onUsddContinue(CL_header,CL_pdu);	
		case USSN:
		case USSNACK:
			return mcfn_onUssdEnd(CL_header,CL_pdu);
	}	
	return false;
}
