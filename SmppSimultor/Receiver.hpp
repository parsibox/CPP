#ifndef ___USSDRECEVIER_HPP__
#define ___USSDRECEVIER_HPP__
#include"TcpServer.h"
#include"EsmeClient.hpp"
typedef std::vector<u8> Buffer;

class CReceiver:public CTcpServer,public CEsmeClient{
	private:
		CSTSSocket *pmeC_conn;	
	public:
		CReceiver(int iL_port,std::string CL_ip):CTcpServer(iL_port,CL_ip){}
		~CReceiver(){}
		bool mcfn_onAccept(CSTSSocket *pCL_conn);
		bool mcfn_receiveFromTcp(CSTSSocket *pCL_Socket);
		bool CReceiver::mcfn_onDisconnect(CSTSSocket*){ return true;}
		
		bool mcfn_sendMsgToServer(u8*, int);
		bool mcfn_onUssdBind(CHeader&,u8*,int);
		bool mcfn_onUssdUnbindResp(CHeader&);
		bool mcfn_onUssdBegin(Smpp::SubmitSm*);
		bool mcfn_onUssdContinue(Smpp::SubmitSm*);
		bool mcfn_onUssdEnd(Smpp::SubmitSm*);
		bool mcfn_onUssdAbort(CHeader&);
		bool mcfn_onUssdSwitch(CHeader&,u8*,int);
		bool mcfn_onUssdChargingInd(CHeader&,u8*,int);
		bool mcfn_onUssdChargingIndRes(CHeader&);
		bool mcfn_onUssdShake(CHeader&);
		bool mcfn_onUssdShakeResp(CHeader&);
};
#endif
