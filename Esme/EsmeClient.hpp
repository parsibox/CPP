#ifndef ESME_CILENT_H
#define ESME_CLIENT_H

#include "smpp.hpp"

#define SMPP_INTERFACE_VERSION 0x34
enum CONN_STATUS{
	CONNECTED=0,
	NOT_CONNECTED=1
};


class CEsmeClient
{
	private:
		std::string meC_systemId;
		std::string meC_password;
		std::string meC_SysType;
		std::string meC_ConnType;
		int mesi_ton;
		int mesi_npi;
		std::string meC_addrRange;
		CONN_STATUS meE_ConnStatus;
		int mesi_seqNumber;
	protected:
		int mdsi_len;
		Smpp::Uint8 *pmdu8_dataPkt;
	protected:
		CONN_STATUS mdfn_getConnectionStatus();
	public:
		CEsmeClient();
		~CEsmeClient();
		int mcfn_getSeqNumber();
		bool mcfn_initialise(std::string,std::string,std::string,std::string ="TR",int =0,int=0,std::string="");
		
		bool mcfn_setConnectionStatus(CONN_STATUS);
		bool mcfn_sendBind();
		bool mcfn_sendUnbind();
		bool mcfn_sendEnqLink();
		bool mcfn_sendSubmitSm(Smpp::SubmitSm &,int &);
		bool mcfn_sendSubmitMultiSm(Smpp::SubmitMulti  &);
		bool mcfn_sendDataSm(Smpp::DataSm &);
		bool mcfn_sendDlrSmRes(int);
		bool mcfn_sendCancelSm(Smpp::CancelSm &);
		bool mcfn_sendQuerySm(Smpp::QuerySm &);
		bool mcfn_sendReplaceSm(Smpp::ReplaceSm &);
		bool mcfn_processResponceMessage(Smpp::Uint8* u8L_Msg,int iL_length);
		
		virtual bool mcfn_processBindRes();	
		virtual bool mcfn_processUnbindRes();
		virtual bool mcfn_processEnqLinkRes();
		virtual bool mcfn_processSubmitSmRes();
		virtual bool mcfn_processSubmitMultiSmRes();
		virtual bool mcfn_processDataSmRes();
		virtual bool mcfn_processDlrSm();
		virtual bool mcfn_processDlrSmRes();
		virtual bool mcfn_processCancelSmRes();
		virtual bool mcfn_processQuerySmRes();
		virtual bool mcfn_processReplaceSmRes();
		virtual bool mcfn_sendRequestMessage(const Smpp::Uint8* buf, int len)=0;//pure virtual function
};
#endif
