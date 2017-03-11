#ifndef ESME_CONNECTION_H
#define ESME_CONNECTION_H
#include"EsmeClient.hpp"
#include"TcpClient.h"
#include"STSMsgQue.h"
#include"Thread.h"
typedef std::vector<u8> Buffer;
typedef struct _receiveData{
	int mcsi_len;
	CXmlHeader mcC_header;
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
                bool mcfn_onUssdUnbindResp(CXmlHeader&);
                bool mcfn_onUssdBegin(CXmlHeader&,CXmlUssdMsg&);
                bool mcfn_onUsddContinue(CXmlHeader&,CXmlUssdMsg&);
                bool mcfn_onUssdEnd(CXmlHeader&,CXmlUssdMsg &);
                bool mcfn_onUssdAbort(CXmlHeader&);
                bool mcfn_onUssdShake(CXmlHeader&);
                bool mcfn_onUssdShakeResp(CXmlHeader&);

		bool mcfn_sendMsgToServer(u8* buf, int len);

		bool mcfn_run(int,void *);
		std::string mcfn_getThreadName(void*);
};
#endif
