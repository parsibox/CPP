#ifndef __SERVER_H
#define __SERVER_H

#include<STSSocket.h>
#include<STSMsgQue.h>
#include<StsUserTypes.h>
#include<EventLog.h>

extern CEventLog CG_EventLog;

class CServer;
struct SClientHandlerParam{
	CServer		*pmcC_This;
	CSTSSocket  *pmcC_ConnSocket;
	SClientHandlerParam(CServer *pCL_This, CSTSSocket  *pCL_ConnSocket){
		pmcC_This = pCL_This;
		pmcC_ConnSocket = pCL_ConnSocket;
	}
};

class CServer:public CSTSSocket{
	private:
		u16		meu16_ServerPort;
		u8		meu8_NumOfLenBytes;
		char	mesc_ServerIp[30];
	protected:
		ByteStreamQ mdC_ProcessQ;
	public:
		CServer(u16 u16L_ServerPort, u8 u8L_NumLenBytes = 4, bool bL_StartServerProcess=true);
		bool mcfn_StartServer();
		void mcfn_ServerThread();
		virtual void mcfn_ClientHandler(CSTSSocket *pCL_Conn);
		virtual void mcfn_InternalProcessor();
//		Pure virtual function, sub class should put app specific 
//		login in side this function to process the received data.
//		ByteStream is an STS typedef in file StsUserTypes
		virtual bool mcfn_OnRx(SByteStream SL_ByteStream)=0x00;
};

#endif //__SERVER_H
