#ifndef __COMMON_HANLDLIN_H
#define __COMMON_HANLDLIN_H
#include"UssdData.pb.h"
#include<string>
#include"StsUserTypes.h"
class CCommonHandling
{
	private:
		int mesi_seqNum;
	public:
		CCommonHandling();
		~CCommonHandling();
		int mcfn_getSeqNumber();
                void mcfn_initilise();

		bool mcfn_processReceivedMessage(std::string &,void* =NULL);
		virtual bool mcfn_sendUssdMessage(UssdData &CL_pdu) =0 ;

		virtual bool mcfn_onUssdBind(UssdData &,void *p){}
		virtual bool mcfn_onUssdBindResp(UssdData &){}
		virtual bool mcfn_onUssdUnbind(UssdData &,void *p){}
		virtual bool mcfn_onUssdUnbindResp(UssdData &){}
		virtual bool mcfn_onUssdShake(UssdData &){}
		virtual bool mcfn_onUssdShakeResp(UssdData&){}
		//pure virtual function 
		virtual bool mcfn_encode(UssdData&,std::string &) =0 ;
		virtual bool mcfn_decode(std::string &,UssdData&) =0 ;
		virtual bool mcfn_sendMsgToServer(u8 *,int,UssdData& )=0;

};
#endif
