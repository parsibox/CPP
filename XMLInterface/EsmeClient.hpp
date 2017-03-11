#ifndef __USSD_ESME_CLIENT_H
#define __USSD_ESME_CLIENT_H
#include"UssdBind.hpp"
#include"UssdSubmit.hpp"
enum CONN_STATUS{
        CONNECTED=0,
        NOT_CONNECTED=1
};

class CEsmeClient{
	private:
		u8 meu8_ussdVersion;
		CONN_STATUS meE_ConnStatus;
		int mesi_interfaceVersion;
		short mesi_seqNum;
		int   mesi_reqId;
		std::string meC_systemId;
		std::string meC_password;
		std::string meC_serviceType;
	protected:
		CONN_STATUS mdfn_getConnectionStatus(){return meE_ConnStatus;}	
	public:
		CEsmeClient();
		~CEsmeClient();
		void mcfn_initilise(std::string,std::string,std::string,int=0x10);	
		short mcfn_getSeqNumber();
		int mcfn_getReqNumber();
		std::string mcfn_convertToString(int iL_data);
		bool mcfn_sendXmlMsg(std::string CL_xmlMsg,short iL_seqNum);
		//clinet  functions	
		bool mcfn_sendUssdBind();
		bool mcfn_sendUssdShake();
		//common functions	
		bool mcfn_sendUssdBegin(CUssdMsg &);
		bool mcfn_sendUssdContinue(CUssdMsg &,short sL_seqId) ;
		bool mcfn_sendUssdEnd(CUssdMsg &,short sL_seqId);
		bool mcfn_processLoginRes(std::string CL_loginRes,int iL_len);
		//TODO::server functions need to do
		bool mcfn_sendUssdBindResp(short iL_seqId,int iL_status,std::string CL_accName,int iL_reqId);
		
		bool mcfn_processReceivedMessage(CHeader&,u8*,int);
	
		virtual bool mcfn_onUssdBegin(CHeader&,CUssdMsg &){}
		virtual bool mcfn_onUsddContinue(CHeader&,CUssdMsg &){}
		virtual bool mcfn_onUssdEnd(CHeader&,CUssdMsg&){}
		virtual bool mcfn_onUssdShake(CHeader&){}
		virtual bool mcfn_onUssdShakeResp(CHeader&){}

		virtual bool mcfn_sendMsgToServer(u8*,int)=0;//pure virtual function 

};
#endif
