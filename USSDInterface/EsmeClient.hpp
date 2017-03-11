#ifndef __USSD_ESME_CLIENT_H
#define __USSD_ESME_CLIENT_H
#include"UssdBind.hpp"
#include"UssdSubmit.hpp"
#include"UssdCharging.hpp"
enum CONN_STATUS{
        CONNECTED=0,
        NOT_CONNECTED=1
};

class CEsmeClient{
	private:
		u8 meu8_ussdVersion;
		CONN_STATUS meE_ConnStatus;
		int mesi_interfaceVersion;
		int mesi_seqNum;
		std::string meC_systemId;
		std::string meC_password;
		std::string meC_serviceType;
	protected:
		CONN_STATUS mdfn_getConnectionStatus(){return meE_ConnStatus;}	
                int mdsi_handShakeCount;
	public:
		CEsmeClient();
		~CEsmeClient();
		void mcfn_initilise(std::string,std::string,std::string,int=0x10);	
		int mcfn_getSeqNumber();
		//clinet  functions	
		bool mcfn_sendUssdBind();
		bool mcfn_sendUssdUnbind();
		bool mcfn_sendUssdShake();
		bool mcfn_sendUssdChargingInd(CHeader &,int,int,u8*,u8);
		//common functions	
		bool mcfn_sendUssdBegin(CHeader&,std::string,std::string,u8,std::string,u8);	
		bool mcfn_sendUssdContinue(CHeader&,std::string,std::string,u8,std::string,u8);	
		bool mcfn_sendUssdEnd(CHeader &,std::string,std::string,u8,std::string,u8);	
		bool mcfn_sendUssdAbort(CHeader &);	
		bool mcfn_sendUssdSwitch(CHeader &,u8,std::string,std::string,std::string);
		bool mcfn_sendUssdShakeResp(CHeader &);	
		//TODO::server functions need to do
		bool mcfn_sendUssdBindResp(CHeader &CL_header,int iL_status,std::string CL_accName);
		
		bool mcfn_processReceivedMessage(CHeader&,u8*,int);
	
		virtual bool mcfn_onUssdBind(CHeader&,u8*,int){}
		virtual bool mcfn_onUssdBindResp(CHeader&,u8*,int){}
		virtual bool mcfn_onUssdUnbind(CHeader&){}
		virtual bool mcfn_onUssdUnbindResp(CHeader&){}
		virtual bool mcfn_onUssdBegin(CHeader&,u8*,int){}
		virtual bool mcfn_onUsddContinue(CHeader&,u8*,int){}
		virtual bool mcfn_onUssdEnd(CHeader&,u8*,int){}
		virtual bool mcfn_onUssdAbort(CHeader&){}
		virtual bool mcfn_onUssdSwitch(CHeader&,u8*,int){}
		virtual bool mcfn_onUssdChargingInd(CHeader&,u8*,int){}
		virtual bool mcfn_onUssdChargingIndRes(CHeader&){}
		virtual bool mcfn_onUssdShake(CHeader&){}
		virtual bool mcfn_onUssdShakeResp(CHeader&){};

		virtual bool mcfn_sendMsgToServer(u8*,int)=0;//pure virtual function 

};
#endif
