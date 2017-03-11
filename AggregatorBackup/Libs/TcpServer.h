/*
 * TcpConnection.h
 *
 *  Created on: May 12, 2013
 *      Author: surya
 */


#ifndef __TCPSERVER__H
#define __TCPSERVER__H
#include<STSSocket.h>
#include<vector>
/**
 * Common fuctionalitys of Server and Client will be here ,we are Inheriting the STSSocket library.
 */
class CTcpServer:public CSTSSocket{
		//Application name for Event Log
		std::string	meC_AppName;
		//Connection status
		bool 	   	meb_isConnected;
		//function pointer for giving the data to application .
		bool (*pmefn_onReceiveFun)(std::string CL_recData,int iL_recDataLen);
		//thread handle for killing the thread once the object is destroyed			
		THREAD_HANDLE meC_ThreadHandle;
		 //TCP IP and Port details variable
                std::string meC_TcpIp;
                int         mesi_TcpPort;
	public: 
		//setting the application Name for EventLog 
		void mcfn_setAppName(std::string CL_AppName);
		//function for getting the Application Name
		std::string mcfn_getAppName();
		//getting the connection status
		bool mcfn_getConnectionStatus();
		//setting the connection status
		void mcfn_setConnectionStatus(bool bL_status);
		//sending the data to ohter side by using socket pointer
		bool mcfn_sendToTcpCommon(std::string CL_msgData,CSTSSocket *pCL_Socket);
		//constuctor with port where to start listing ,receiving function pointer and IP if required
		CTcpServer(int iL_port,std::string CL_Ip="");
		//destructor
		~CTcpServer();
		//sending the data to client by using the socket object
		bool mcfn_sendToTcp(CSTSSocket *pCL_Socket,std::string CL_msgData);
		//this setting the port and Ip,in case of constructor with out args
		bool mcfn_setServerPort(int iL_port,std::string CL_IP="");
		//Starting the Server thread ,it will start listing on given IP and port ,and accepts the clint connections
		void mcfn_StartServerThread();
		//once the the clint connected this function will be called to start the client thread
		void mcfn_StartClientThread(CSTSSocket *pCL_Socket);
		//staring the Server thread
		bool mcfn_Start();	
		
		//receiveing from other side using socket pointer
		virtual bool mcfn_receiveFromTcp(CSTSSocket *pCL_Socket);
	
		virtual bool mcfn_onReceiveData(std::string,int,CSTSSocket*);	
		virtual bool mcfn_onAccept(CSTSSocket *){ return true ;}
		virtual bool mcfn_onDisconnect(CSTSSocket *){return true;}

};

inline std::string CTcpServer::mcfn_getAppName(){
	return meC_AppName;
}
inline void CTcpServer::mcfn_setAppName(std::string CL_AppName){
	meC_AppName=CL_AppName;
}

inline bool CTcpServer::mcfn_getConnectionStatus(){
	return meb_isConnected;
}

inline void CTcpServer::mcfn_setConnectionStatus(bool bL_status){
	meb_isConnected=bL_status;
}


inline bool CTcpServer::mcfn_setServerPort(int iL_port,std::string CL_IP){
	meC_TcpIp=CL_IP;
	if(iL_port>0 && iL_port<65535){
		mesi_TcpPort=iL_port;
		return true;
	}
	return false;
}
#endif
