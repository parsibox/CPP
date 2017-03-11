#ifndef ESME_SERVER_H
#define ESME_SERVER_H
#include"TcpServer.h"
#include"STSMsgQue.h"
#include"Thread.h"
#include<UssdData.pb.h>
#include<CommonHandling.hpp>
#include<MsgMap.h>
typedef std::vector<u8> Buffer;
class CEsmeClient;

class CEsmeServer : public CTcpServer,public CThread,public CCommonHandling 
{
	private:
		bool bL_isLoginSucess;
		CMsgQue<std::string> meC_RecDataQue;
                int mesi_headerLen;
                CMsgMap<CSTSSocket*,std::list<CEsmeClient*>*> meC_socketMap;
	public:
		CEsmeServer(int,int,std::string ="");
		virtual ~CEsmeServer();
		//virtual function from CTcpServer class
		bool mcfn_receiveFromTcp(CSTSSocket *pCL_Socket);
		bool mcfn_onAccept(CSTSSocket *) ;
		bool mcfn_onDisconnect(CSTSSocket *);

                bool mcfn_sendUssdMessage(UssdData &CL_pdu);
		bool mcfn_sendMsgToServer(u8* buf, int len,UssdData &CL_pdu);
		
		bool mcfn_run(int,void *);
		std::string mcfn_getThreadName(void*);
             //virtual fucntions
                 
		bool mcfn_onUssdBind(UssdData &,void*);
		bool mcfn_onUssdUnbind(UssdData &,void*);
		bool mcfn_onUssdShake(UssdData &);
		bool mcfn_onUssdShakeResp(UssdData&);

		//pure virtual function

                virtual int mcfn_getDataLen(u8* ) = 0;

};

#endif
