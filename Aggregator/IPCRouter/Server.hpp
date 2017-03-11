#ifndef __IPCSERVER__
#define __IPCSERVER__

#include<TcpServer.h>
#include<MsgMap.h>
#include<STSMsgQue.h>

#include<IPCType.hpp>

class CServer : public CTcpServer
{
	private:
		CMsgMap<short,CSTSSocket*>	meC_srcMap;
		CMsgMap<CSTSSocket*,short>	meC_connMap;
		CMsgQue<IPCMsg>* 			pmeC_RecQue;	
	public:
		CServer(int iL_port,CMsgQue<IPCMsg>*,std::string CL_ip="");
		~CServer();
		
		bool mcfn_sendMessage(IPCMsg &CL_msg);
		
		//virtual function from CTcpServer class
		bool mcfn_onAccept(CSTSSocket *);
		bool mcfn_receiveFromTcp(CSTSSocket*);
		bool mcfn_onDisconnect(CSTSSocket*);


};
#endif
