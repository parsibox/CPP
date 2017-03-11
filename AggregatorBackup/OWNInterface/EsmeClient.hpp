#ifndef __USSD_ESME_CLIENT_H
#define __USSD_ESME_CLIENT_H
#include"XmlUssdBind.hpp"
#include"XmlUssdSubmit.hpp"
#include<string>
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
		long   mesi_reqId;
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
		bool mcfn_sendXmlMsg(std::string CL_xmlMsg,long iL_seqNum,int iL_operation);
		//clinet  functions	
		bool mcfn_sendUssdBind();
		bool mcfn_sendUssdShake();
		//common functions	
		bool mcfn_sendUssdBegin(CXmlUssdMsg &);
		bool mcfn_sendUssdContinue(CXmlUssdMsg &,long sL_seqId) ;
		bool mcfn_sendUssdEnd(CXmlUssdMsg &,long sL_seqId);
                bool mcfn_sendUssdAbort(CXmlHeader&);
		bool mcfn_processLoginRes(std::string CL_loginRes,int iL_len);
		//TODO::server functions need to do
		bool mcfn_sendUssdBindResp(long iL_seqId,int iL_status,std::string CL_accName,int iL_reqId);
		
		bool mcfn_processReceivedMessage(CXmlHeader&,u8*,int);
	
		virtual bool mcfn_onUssdBegin(CXmlHeader&,CXmlUssdMsg &){}
		virtual bool mcfn_onUsddContinue(CXmlHeader&,CXmlUssdMsg &){}
		virtual bool mcfn_onUssdEnd(CXmlHeader&,CXmlUssdMsg&){}
		virtual bool mcfn_onUssdAbort(CXmlHeader&){}
		virtual bool mcfn_onUssdShake(CXmlHeader&){}
		virtual bool mcfn_onUssdShakeResp(CXmlHeader&){}

		virtual bool mcfn_sendMsgToServer(u8*,int)=0;//pure virtual function
                std::string mcfn_getUserName(){ return meC_systemId; } 

};
#endif
