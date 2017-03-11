#include"EsmeClient.hpp"
const char PSSD_FLAG[1] ={0x00};
const char PSSR_FLAG[1] ={0x01};
const char USSR_REQ[1] = {0x02};
const char USSN_REQ[1] = {0x03};
const char PSSD_RES[1] = {0x10};
const char PSSR_RES[1] = {0x11};
const char USSR_ACK[1] = {0x12};
const char USSN_ACK[1] = {0x13};
int CEsmeClient::mesi_seqNum =0;
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
}
int CEsmeClient::mcfn_getSeqNumber(){
	++mesi_seqNum;
	if(mesi_seqNum>=0x7FFFFFFF){
		mesi_seqNum=0;
	}
	return mesi_seqNum;
}
bool CEsmeClient::mcfn_sendUssdBind(){
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
bool CEsmeClient::mcfn_sendUssdBindResp(int iL_status,std::string CL_accName,std::string CL_sysType){
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
bool CEsmeClient::mcfn_sendUssdUnbind(){
	Smpp::Unbind pdu(mcfn_getSeqNumber());
        return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());
}
bool CEsmeClient::mcfn_sendUssdShake(){
        Smpp::EnquireLink pdu(mcfn_getSeqNumber());
        return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());
}

bool CEsmeClient::sendDeliverSm(int iL_seqNum,std::string CL_msisdn,std::string CL_serviceCode,u8 u8L_codeScema,std::string CL_ussdString,u8 u8L_ussdOptype)
{
	Smpp::DeliverSm pdu;
	pdu.sequence_number(iL_seqNum);
	pdu.service_type("USSD");
	pdu.destination_addr(Smpp::Address(CL_serviceCode.c_str()));
	pdu.source_addr( Smpp::SmeAddress(Smpp::Ton(Smpp::Ton::International),
				Smpp::Npi(Smpp::Npi::E164),
				Smpp::Address(CL_msisdn.c_str())));
	//	pdu.registered_delivery(0x01);
	pdu.short_message( reinterpret_cast<const Smpp::Uint8*>(CL_ussdString.c_str()),
			CL_ussdString.length());
	pdu.data_coding(Smpp::DataCoding(u8L_codeScema));
	std::string ussdServiceOp = "";
	switch(u8L_ussdOptype)
	{
		case USSD_OPTYPE::USSD_REQUEST:
			ussdServiceOp.append(PSSR_FLAG);
			break;
		case USSD_OPTYPE::USSD_NOTIFY:
			ussdServiceOp.append(USSN_REQ);
			break;
		case USSD_OPTYPE::USSD_RESPONCE:
			ussdServiceOp.append(USSR_ACK);
			break;
		case USSD_OPTYPE::USSD_RELEASE:
			ussdServiceOp.append(PSSR_RES);
			break;
	}
	pdu.insert_array_tlv(
			Smpp::Tlv::ussd_service_op,
			0x01,
			reinterpret_cast<const Smpp::Uint8*>(ussdServiceOp.data()));
	return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());

}

bool CEsmeClient::sendSubmitSm(int iL_seqNum,std::string CL_msisdn,std::string CL_serviceCode,u8 u8L_codeScema,std::string CL_ussdString,u8 u8L_ussdOptype)
{
	Smpp::SubmitSm pdu;
	pdu.sequence_number(iL_seqNum);
	pdu.service_type("USSD");
	pdu.source_addr(Smpp::Address(CL_serviceCode.c_str()));
	pdu.destination_addr( Smpp::SmeAddress(Smpp::Ton(Smpp::Ton::International),
				Smpp::Npi(Smpp::Npi::E164),
				Smpp::Address(CL_msisdn.c_str())));
	//	pdu.registered_delivery(0x01);
	pdu.short_message( reinterpret_cast<const Smpp::Uint8*>(CL_ussdString.c_str()),
			CL_ussdString.length());
	pdu.data_coding(Smpp::DataCoding(u8L_codeScema));
	std::string ussdServiceOp = "";
	switch(u8L_ussdOptype)
	{
		case USSD_OPTYPE::USSD_REQUEST:
			ussdServiceOp.append(USSR_REQ);
			break;
		case USSD_OPTYPE::USSD_NOTIFY:
			ussdServiceOp.append(USSN_REQ);
			break;
		case USSD_OPTYPE::USSD_RESPONCE:
			ussdServiceOp.append(USSR_ACK);
			break;
		case USSD_OPTYPE::USSD_RELEASE:
			ussdServiceOp.append(PSSR_RES);
			break;
	}
	pdu.insert_array_tlv(
			Smpp::Tlv::ussd_service_op,
			ussdServiceOp.length(),
			reinterpret_cast<const Smpp::Uint8*>(ussdServiceOp.data()));
	return mcfn_sendMsgToServer((u8*)pdu.encode(),pdu.command_length());

}
bool CEsmeClient::mcfn_processDeliverSm(u8* pu8L_param,int iL_len)
{
	
	Smpp::DeliverSm pdu(&pu8L_param[0]);
	Smpp::DeliverSmResp resp(Smpp::CommandStatus::ESME_ROK,pdu.sequence_number());
	mcfn_sendMsgToServer((u8*)resp.encode(),resp.command_length());

	const Smpp::Tlv* ussdServiceOp = pdu.find_tlv(Smpp::Tlv::ussd_service_op);
	if(ussdServiceOp==NULL) return false;

	u8 ussdSerOp = *ussdServiceOp->value();
	switch(ussdSerOp)
	{
	/*	case 0x01:
			return 	mcfn_onUssdBegin(&pdu);
		case 0x12:
			return mcfn_onUssdContinue(&pdu);*/
		default:
			break;

	}
	return false;
}
bool CEsmeClient::mcfn_processSubmitSm(u8* pu8L_param,int iL_len)
{
	Smpp::SubmitSm pdu(&pu8L_param[0]);
	Smpp::SubmitSmResp resp(Smpp::CommandStatus::ESME_ROK,pdu.sequence_number(),Smpp::MessageId("0"));
	mcfn_sendMsgToServer((u8*)resp.encode(),resp.command_length());

	const Smpp::Tlv* ussdServiceOp = pdu.find_tlv(Smpp::Tlv::ussd_service_op);
	if(ussdServiceOp==NULL) return false;

	u8 ussdSerOp = *ussdServiceOp->value();
	switch(ussdSerOp)
	{
		case 0x11:
			return mcfn_onUssdEnd(&pdu);
		case 0x02:
			return mcfn_onUssdContinue(&pdu);

		defalut:
			return false;
	}
}

bool CEsmeClient::mcfn_sendUssdBegin(int iL_sessionId,std::string CL_msisdn,std::string CL_serviceCode,u8 u8L_codeScema,std::string CL_ussdString,u8 u8L_ussdOptype){
	return sendDeliverSm(iL_sessionId,CL_msisdn,CL_serviceCode,u8L_codeScema,CL_ussdString,u8L_ussdOptype);
}
bool CEsmeClient::mcfn_sendUssdContinue(std::string CL_msisdn,std::string CL_serviceCode,u8 u8L_codeScema,std::string CL_ussdString,u8 u8L_ussdOptype){
	return sendDeliverSm(mcfn_getSeqNumber(),CL_msisdn,CL_serviceCode,u8L_codeScema,CL_ussdString,u8L_ussdOptype);
}
bool CEsmeClient::mcfn_sendUssdEnd(std::string CL_msisdn,std::string CL_serviceCode,u8 u8L_codeScema,std::string CL_ussdString,u8 u8L_ussdOptype){
	return sendSubmitSm(mcfn_getSeqNumber(),CL_msisdn,CL_serviceCode,u8L_codeScema,CL_ussdString,u8L_ussdOptype);
}
bool CEsmeClient::mcfn_sendUssdAbort(std::string CL_msisdn,std::string CL_serviceCode,u8 u8L_codeScema,std::string CL_ussdString,u8 u8L_ussdOptype){
	return sendSubmitSm(mcfn_getSeqNumber(),CL_msisdn,CL_serviceCode,u8L_codeScema,CL_ussdString,u8L_ussdOptype);
}

bool CEsmeClient::mcfn_processReceivedMessage(CHeader& CL_header,u8* u8_param,int iL_paramLen){
	
		switch(CL_header.mcsi_cmdId){
			case Smpp::CommandId::BindReceiver :
			case Smpp::CommandId::BindTransmitter :
			case Smpp::CommandId::BindTransceiver :
					return mcfn_onUssdBind(CL_header,u8_param,iL_paramLen);	
                        case Smpp::CommandId::BindReceiverResp :
                        case Smpp::CommandId::BindTransmitterResp :
                        case Smpp::CommandId::BindTransceiverResp :
					return mcfn_onUssdBindResp(CL_header,u8_param,iL_paramLen);
			case Smpp::CommandId::Unbind:
					return mcfn_onUssdUnbind(CL_header);
			case Smpp::CommandId::UnbindResp:
					return mcfn_onUssdUnbindResp(CL_header);
                        case Smpp::CommandId::SubmitSm:
					return mcfn_processSubmitSm(u8_param,iL_paramLen);
                        case Smpp::CommandId::DeliverSm:
					return mcfn_processDeliverSm(u8_param,iL_paramLen);
                        case Smpp::CommandId::EnquireLink:
					return mcfn_onUssdShake(CL_header);
                        case Smpp::CommandId::EnquireLinkResp:
					return mcfn_onUssdShakeResp(CL_header);
                        case Smpp::CommandId::SubmitSmResp:
                        case Smpp::CommandId::DeliverSmResp:
					return true;
			default:
				break;
		}
	return false;
}
