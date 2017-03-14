#ifndef ESME_CONNECTION_H
#define ESME_CONNECTION_H
#include"Header.hpp"
#include"TcpClient.h"
#include"STSMsgQue.h"
#include"Thread.h"
typedef std::vector<u8> Buffer;

typedef struct _receiveData{
	int mcsi_len;
	CHeader mcC_header;
	Buffer  pmcu8_data;
	_receiveData(){
		mcsi_len=0;
	}
}ReceviedData;


enum CONN_STATUS{
        CONNECTED=0,
        NOT_CONNECTED=1
};


class CEsmeConnection:public CTcpClient,public CThread
{
	private:
		u8 meu8_ussdVersion;
		int mesi_interfaceVersion;
		int mesi_seqNum;
		std::string meC_systemId;
		std::string meC_password;
		std::string meC_serviceType;

		bool bL_isLoginSucess;
		CMsgQue<ReceviedData> meC_RecDataQue;
		CONN_STATUS meE_ConnStatus;
	public:
		CEsmeConnection();
		~CEsmeConnection();
		void mcfn_initilise(std::string,std::string,std::string,int=0x10);	
		//virtual function from CTcpClinet class
		bool mcfn_onConnect();
		bool mcfn_receiveData();
		//virtual function from EsmeClinet Class
		bool mcfn_onBind(CHeader&,u8*,int);
		bool mcfn_onBindResp(CHeader&,u8*,int);
		bool mcfn_onUnbind(CHeader&);
		bool mcfn_onUnbindResp(CHeader&);
		bool mcfn_onShake(CHeader&);
		bool mcfn_onShakeResp(CHeader&);

		bool mcfn_onSubmitSm(u8* u8_param, int iL_paramLen);
		bool mcfn_onSubmitSmResp(u8* u8_param, int iL_paramLen);
		bool mcfn_onDeliverSm(u8* u8_param, int iL_paramLen);
		bool mcfn_onDeliverSmResp(u8* u8_param, int iL_paramLen);


		bool mcfn_processReceivedMessage(CHeader&,u8*,int);
		bool mcfn_sendMsgToServer(u8* buf, int len);

		int mcfn_getSeqNumber();
		bool mcfn_sendBind();
		bool mcfn_sendUnbind();
		bool mcfn_sendShake();
		bool mcfn_sendBindResp(int iL_status,std::string CL_accName,std::string CL_sysType="TR");
		bool mcfn_sendSubmitSm(int iL_seqNum,std::string CL_msisdn,std::string CL_oa,u8 u8L_codeScema,std::string CL_ShortMsg,u8 u8L_reg_del);

		bool mcfn_run(int,void *);
		std::string mcfn_getThreadName(void*);
		CONN_STATUS mcfn_getConnectionStatus(){return meE_ConnStatus;}	
};
#endif
