#ifndef ESME_CONNECTION_H
#define ESME_CONNECTION_H
#include"EsmeClient.hpp"
#include"TcpClient.h"
typedef std::vector<Smpp::Uint8> Buffer;

class CEsmeConnection:public CTcpClient,public CEsmeClient
{
	private:
		
	public:
		CEsmeConnection();
		~CEsmeConnection();
		//virtual function from CTcpClinet class
		bool mcfn_onConnect();
		bool mcfn_receiveData();
		//virtual function from EsmeClinet Class
		bool mcfn_processBindRes();
		bool mcfn_processEnqLinkRes();
		bool mcfn_processUnbindRes();
		bool mcfn_processSubmitSmRes();
		bool mcfn_processDlrSm();
		bool mcfn_sendRequestMessage(const Smpp::Uint8* buf, int len);
};
#endif
