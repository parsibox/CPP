#ifndef ESME_CONNECTION_H
#define ESME_CONNECTION_H
#include"EsmeClient.hpp"
#include"TcpClient.h"
#include"STSMsgQue.h"
#include"Thread.h"
#include<UssdData.pb.h>
typedef std::vector<u8> Buffer;
typedef struct _receiveData{
	int mcsi_len;
	CHeader mcC_header;
	Buffer  pmcu8_data;
	_receiveData(){
		mcsi_len=0;
	}
}ReceviedData;

class CEsmeConnection:public CTcpClient,public CEsmeClient,public CThread
{
	private:
		bool bL_isLoginSucess;
		CMsgQue<ReceviedData> meC_RecDataQue;
	public:
		CEsmeConnection();
		~CEsmeConnection();
		//virtual function from CTcpClinet class
		bool mcfn_onConnect();
		bool mcfn_receiveData();
		//virtual function from EsmeClinet Class
                bool mcfn_onUssdBindResp(CHeader&,u8*,int);
                bool mcfn_onUssdUnbindResp(CHeader&);
                bool mcfn_onUssdBegin(CHeader&,u8*,int);
                bool mcfn_onUsddContinue(CHeader&,u8*,int);
                bool mcfn_onUssdEnd(CHeader&,u8*,int);
                bool mcfn_onUssdAbort(CHeader&);
                bool mcfn_onUssdSwitch(CHeader&,u8*,int);
                bool mcfn_onUssdChargingIndRes(CHeader&);
                bool mcfn_onUssdShake(CHeader&);
                bool mcfn_onUssdShakeResp(CHeader&);

		bool mcfn_sendMsgToServer(u8* buf, int len);
		
		bool mcfn_setUssdMsg(CUssdBegin &CL_ussdMsg,UssdData  &CL_ussdData);	
		bool mcfn_setHeaderData(CHeader& CL_header,UssdData  &CL_ussdData);
		bool mcfn_run(int,void *);
		std::string mcfn_getThreadName(void*);
};
#endif
