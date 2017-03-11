#ifndef __TCPSERVER__H
#define __TCPSERVER__H
#include<Thread.h>
#include<Socket.h>
#include<string>

class TcpServer:public Socket
{
	public: 
		TcpServer(int iL_port,std::string CL_Ip="");
		virtual ~TcpServer();
		bool setServerPort(int iL_port,std::string CL_IP="");
		void StartServerThread();

		void StartClientThread(Socket *pCL_Socket);
		virtual bool receiveFromTcp(Socket *pCL_Socket)=0;
	private:
		THREAD_HANDLE serverThreadHandle;
		std::string tcpIp;
		int         tcpPort;

};

inline bool TcpServer::setServerPort(int iL_port,std::string CL_IP){
	tcpIp=CL_IP;
	if(iL_port>0 && iL_port<65535){
		tcpPort=iL_port;
		return true;
	}
	return false;
}
#endif
