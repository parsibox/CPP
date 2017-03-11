#ifndef ESME_CLIENT_H
#define ESME_CLIENT_H
#include"TcpClient.h"
#include"STSMsgQue.h"
#include"Thread.h"
#include<UssdData.pb.h>
#include<CommonHandling.hpp>
typedef std::vector<u8> Buffer;
class CEsmeClient:public CTcpClient,public CThread,public CCommonHandling 
{
	private:
		bool bL_isLoginSucess;
		CMsgQue<std::string> meC_RecDataQue;
                int mesi_headerLen;
                CSTSSocket *pmeC_serverSock;
        protected:
                std::string mdC_name;
                std::string mdC_password;
                std::string mdC_applicationId;
                int mdsi_version; 
	public:
    
		CEsmeClient(int ,CSTSSocket*);
		virtual ~CEsmeClient();
		//virtual function from CTcpClinet class
		bool mcfn_onConnect();
		bool mcfn_receiveData();

		bool mcfn_sendUssdMessage(UssdData &CL_pdu) ;
		bool mcfn_sendMsgToServer(u8 *,int,UssdData& CL_pdu);

		bool mcfn_run(int,void *);
		std::string mcfn_getThreadName(void*);
		void mcfn_setDetails(std::string username,std::string password,std::string appId,int version);
		//pure virtual function
		virtual bool mcfn_sendUssdBind() = 0;
		virtual int mcfn_getDataLen(u8* ) = 0;
};
#endif
