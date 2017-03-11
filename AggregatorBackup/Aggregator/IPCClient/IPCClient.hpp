#ifndef __IPCCLIENT__
#define __IPCCLIENT__

#include<STSMsgQue.h>
#include<TcpClient.h>
#include<IPCType.hpp>

class CIPCClient : public CTcpClient
{
	private:
		short mesi_moduleId;
		short mesi_ipcRouterId;

		CMsgQue<IPCMsg>* pmeC_RecQue;
	public:
		CIPCClient(short ,short ,CMsgQue<IPCMsg> *);
		bool mcfn_receiveData();
		bool mcfn_onConnect();
		bool mcfn_sendMessage(short ,std::string& );
};
#endif
