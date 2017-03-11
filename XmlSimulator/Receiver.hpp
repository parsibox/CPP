#ifndef ___USSDRECEVIER_HPP__
#define ___USSDRECEVIER_HPP__
#include"TcpServer.h"
#include"EsmeClient.hpp"
typedef std::vector<u8> Buffer;

class CReceiver:public CTcpServer,public CEsmeClient{
	private:
		CSTSSocket *pmeC_conn;
		bool mcb_Login;	
	public:
		CReceiver(int iL_port,std::string CL_ip):CTcpServer(iL_port,CL_ip){mcb_Login=false;}
		~CReceiver(){}
		bool mcfn_onAccept(CSTSSocket *pCL_conn);
		bool mcfn_receiveFromTcp(CSTSSocket *pCL_Socket);
		bool CReceiver::mcfn_onDisconnect(CSTSSocket*){ return true;}
		
		bool mcfn_sendMsgToServer(u8*, int);
		bool mcfn_onUssdBind(CHeader& CL_header,u8* pu8L_param,int iL_len);
		bool mcfn_onUssdBegin(CHeader&,CUssdMsg &);
		bool mcfn_onUsddContinue(CHeader&,CUssdMsg &);
		bool mcfn_onUssdEnd(CHeader&,CUssdMsg &);
		bool mcfn_onUssdShake(CHeader&);
		bool mcfn_onUssdShakeResp(CHeader&);
};
#endif
