#include"CommonHandling.hpp"
#include"Externs.h"
CCommonHandling::CCommonHandling(){
}
CCommonHandling::~CCommonHandling(){
}
void CCommonHandling::mcfn_initilise(){
	mesi_seqNum=0;
}
int CCommonHandling::mcfn_getSeqNumber(){
	++mesi_seqNum;
	if(mesi_seqNum>=0x7FFFFFFF){
		mesi_seqNum=0;
	}
	return mesi_seqNum;
}

bool CCommonHandling::mcfn_processReceivedMessage(std::string& CL_msg,void *pCL_ptr)
{
	UssdData CL_pdu;
	mcfn_decode(CL_msg,CL_pdu);
	DBG_INFO((CG_EventLog),("Received data[%s]",CL_pdu.ShortDebugString().c_str()));
	switch(CL_pdu.op_type())
	{
		case UssdData::LOGIN :
			return mcfn_onUssdBind(CL_pdu,pCL_ptr);
		case UssdData::LOGIN_RESP:
			return mcfn_onUssdBindResp(CL_pdu);
		case UssdData::LOGOUT:
			return mcfn_onUssdUnbind(CL_pdu,pCL_ptr);
		case UssdData::LOGOUT_RESP:
			return mcfn_onUssdUnbindResp(CL_pdu);
		case UssdData::ENQ_LINK:
			return mcfn_onUssdShake(CL_pdu);
		case UssdData::ENQ_LINK_RESP:
			return mcfn_onUssdShakeResp(CL_pdu);
			break;
		default:
			return mcfn_sendUssdMessage(CL_pdu);	
			break;
	}
	return false;
}
