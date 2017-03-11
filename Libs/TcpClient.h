/*
 * TcpConnection.h
 *
 *  Created on: May 12, 2013
 *      Author: surya
 */


#ifndef __TCPCLIENT__H
#define __TCPCLIENT__H
#include<STSSocket.h>
#include<vector>
/** 
 *	Structure for Tcp Ip and port
 *
 */
typedef struct {
	std::string meC_TcpIp;
	int 	    mesi_TcpPort;	
}TCP_DETAILS;

/**
 * Common fuctionalitys of Server and Client will be here ,we are Inheriting the STSSocket library.
 */
class CTcpClient:public CSTSSocket{
	private:	
		//Application name for Event Log
		std::string	meC_AppName;
		//Connection status
		bool 	   	meb_isConnected;

		//current index will be used to connect the secondary connection if the primary is down
		int 		mesi_currentIdx;
		//total number of connections added
		int		mesi_totalEntrys;
		//total number of trys for secondary connection
		int 		mesi_numberOftrys;	
		//vector for having the server connecion details 
		std::vector<TCP_DETAILS> meC_TcpConnDetails;

		//thread handle for killing the thread once the object is destroyed			
		THREAD_HANDLE meC_ThreadHandle;

		//connection to ip and port ,count for printing how many times the connect has failed
		bool mefn_Connect(std::string CL_Ip, int iL_Port,int iL_Count);
		//getting the next server details in the case of primary is down
		bool mefn_getNextTcpDetails(std::string &CL_Ip,int &iL_Port);
	public:
		//constructor with Application name 
		CTcpClient(std::string CL_AppName,int iL_noOftrys=30);
		//destructor
		~CTcpClient();
		//passing the application name and receive function ater creataion of object
		void mcfn_initialize(std::string CL_AppName);

		//setting the application Name for EventLog 
		void mcfn_setAppName(std::string CL_AppName);
		//function for getting the Application Name
		std::string mcfn_getAppName();
		//getting the connection status
		bool mcfn_getConnectionStatus();
		//setting the connection status
		void mcfn_setConnectionStatus(bool bL_status);
		//adding the Tcp server ip and port details
		void mcfn_addTcpDetails(std::string CL_Ip,int iL_port);
		//adding the primaty and secondary server IP and port details
		bool mcfn_addTcpDetails(std::string CL_PriIp,int iL_PriPort,std::string CL_SecIp,int iL_SecPort);
		//thread for connecting to server 
		void mcfn_connectToTcpThread();
		//starting the thread connecton thread
		bool mcfn_Start();
		//sending the the data to server 
		bool mcfn_sendToTcp(std::string CL_msgData);
		
		virtual bool mcfn_onReceiveData(std::string,int);
		//this function will call the user function or it will act like connection monitor
		virtual bool mcfn_receiveData();

		virtual bool mcfn_onConnect(){}
};

inline std::string CTcpClient::mcfn_getAppName(){
	return meC_AppName;
}
inline void CTcpClient::mcfn_setAppName(std::string CL_AppName){
	meC_AppName=CL_AppName;
}

inline bool CTcpClient::mcfn_getConnectionStatus(){
	return meb_isConnected;
}

inline void CTcpClient::mcfn_setConnectionStatus(bool bL_status){
	meb_isConnected=bL_status;
}


inline bool CTcpClient::mefn_getNextTcpDetails(std::string &CL_Ip,int &iL_Port){
	CL_Ip=meC_TcpConnDetails[mesi_currentIdx].meC_TcpIp;
	iL_Port=meC_TcpConnDetails[mesi_currentIdx].mesi_TcpPort;	
	++mesi_currentIdx;
	if(mesi_currentIdx>=mesi_totalEntrys)	{
		mesi_currentIdx=0;
	}

}
inline void CTcpClient::mcfn_addTcpDetails(std::string CL_Ip,int iL_port){
	TCP_DETAILS SL_TcpDetails;
	SL_TcpDetails.meC_TcpIp=CL_Ip;
	SL_TcpDetails.mesi_TcpPort=iL_port;
	if(!SL_TcpDetails.meC_TcpIp.empty() &&  (SL_TcpDetails.mesi_TcpPort>0&&iL_port<65535)){
		meC_TcpConnDetails.push_back(SL_TcpDetails);
	}
}
#endif
