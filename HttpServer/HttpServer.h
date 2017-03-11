#ifndef __HTTP_SERVER__
#define __HTTP_SERVER__

#include "TcpServer.h"
#include "HttpMessage.h"

class HttpServer:public TcpServer
{
	public:
		HttpServer(int port);
		~HttpServer();
		virtual bool receiveFromTcp(Socket *pCL_Socket);
		bool registerService(std::string method,std::string service,bool (*functionPointer)(HttpRequest,HttpResponse&));
	private:

		std::string readLine(Socket *pCL_Socket);
		std::string readHttpHeader(Socket *pCL_Socket);
		bool decodeHttpHeader(std::string hdr,HttpRequest &httpMsg);
		bool callService(HttpRequest httpMsg,HttpResponse& httpResp);

	private:
		CCriticalSection serviceMapLock;
		std::map< std::string, std::map< std::string, bool (*)(HttpRequest,HttpResponse&) >* > serviceMap;
};

#endif
