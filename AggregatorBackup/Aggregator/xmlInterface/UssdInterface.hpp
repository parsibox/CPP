#ifndef __USSD_INTERFACE__
#define __USSD_INTERFACE__
#include<CommonHandling.hpp>
#include<EsmeClient.hpp>

class UssdInterface: public CEsmeClient 
{
	private :
		int mefn_getCommandLen(u8 u8L_cmd);
		int  mefn_encodeOperationType(UssdData::operation_type eL_opType);
		int mefn_decodeOperationType(int iL_operation);
		u8  mefn_encodeMessageType(UssdData::Msg_Type eL_msgType);
		int mefn_decodeMessageType(u8 opType);
	public:
		UssdInterface(CSTSSocket*);
		~UssdInterface();
		bool mcfn_onUssdBindResp(UssdData &);
		bool mcfn_onUssdUnbindResp(UssdData &);
		bool mcfn_onUssdShake(UssdData &);
		bool mcfn_onUssdShakeResp(UssdData&);
		//pure virtual function 
		bool mcfn_encode(UssdData&,std::string &);
		bool mcfn_decode(std::string &,UssdData&);
                bool mcfn_sendUssdBind();
		int mcfn_getDataLen(u8*);
};
#endif
