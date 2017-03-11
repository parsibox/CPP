/*
 * TcpConnection.cpp
 *
 *  Created on: May 12, 2013
 *      Author: surya
 */

#include"TcpServer.h"
#include<EventLog.h>
#include"signalHandler.h"
/**
 * using the same object created in application
 */
extern CEventLog CG_EventLog;
/*Structure for passing as thread argument*/
struct SParam{
	//tcp server object to call the function 
	CTcpServer* pmcC_Obj;
	//socket object for receiving the data
	CSTSSocket * pmcC_Sock;
	SParam(CTcpServer* pCL_Obj, CSTSSocket* pCL_Sock){
		pmcC_Obj  = pCL_Obj;
		pmcC_Sock = pCL_Sock;
	}
};
/**
 *	thread fucntion for staring the client thread
 */
static FNVOID fnG_Wrapper_ServerClientHandlerThread(void *pvL_Param){
	try{
		SParam SL_Param = *(static_cast<SParam*>(pvL_Param));
		SL_Param.pmcC_Obj->mcfn_StartClientThread(SL_Param.pmcC_Sock);
	}catch(...){
		DBG_CRITICAL((CG_EventLog), ("Bad cast while starting mcfn_StartClientThread"));
	}
	RETURN;
}
/**
 *	thread function for starting the server
 */
static FNVOID fnG_Wrapper_ThStartServerThread(void *pvL_This) {
	try {

		(static_cast<CTcpServer*> (pvL_This))->mcfn_StartServerThread();
	}catch(...) {
		DBG_ERROR((CG_EventLog),("Unable to start connection Thread,Reason:Bad Cast"));
	}
	RETURN; 
}              
/**
 *function for sending the data to other side,
 *param1:Data with out the length
 *param2:socket object to which connection data need to send
 *Returns:true if the send is sucess ,false ohter wise
 */
bool CTcpServer::mcfn_sendToTcpCommon(std::string CL_msgData,CSTSSocket *pCL_Socket){

	if(CL_msgData.empty()){
		DBG_INFO((CG_EventLog),("Message is empty we are not sending the message "));
	}
	else 
	{
		int iL_len=CL_msgData.length();
		DBG_VERBOSE((CG_EventLog),("Sending Length:%d",iL_len));
		//sending the length firt
		if(!pCL_Socket->mcfn_send((char *)&iL_len,sizeof(int))){
			DBG_ERROR((CG_EventLog), ("Error in sending message to %s",mcfn_getAppName().c_str()));
			mcfn_setConnectionStatus(false);
			return false;	
		}
		//sendting the data
		if (!pCL_Socket->mcfn_send((char *)CL_msgData.c_str(),CL_msgData.length())) {
			DBG_ERROR((CG_EventLog), ("Error in sending message to %s",mcfn_getAppName().c_str()));
			mcfn_setConnectionStatus(false);
			return false;
		}
	}
	return true;
}
/**
 *function for receiveing the data to other side,
 *param1:socket object from  which connection data need to receive
 *Returns:true if the send is sucess ,false ohter wise
 * If on receive function pointer is set it will give the data to that function
 */

bool CTcpServer::mcfn_receiveFromTcp(CSTSSocket *pCL_Socket){
	DBG_VERBOSE((CG_EventLog),("Receiveing From %s",mcfn_getAppName().c_str()));
	int ulL_RecvLen = 0x00;
	//receiveing the length of data first
	if(!pCL_Socket->mcfn_receive((char*)&ulL_RecvLen, sizeof(ulL_RecvLen)))
	{
		DBG_ERROR((CG_EventLog),("%s Socket Disconnected : %d",mcfn_getAppName().c_str(),mcfn_getLastErrorCode()));
		return false;
	}
	//if the length of the data is more that 10K we are disconnecting the connection
	if(ulL_RecvLen>10000){
		DBG_CRITICAL((CG_EventLog),("Packet length is coming more that 10000 disconnecting the %s socket",mcfn_getAppName().c_str()));
		return false;	
	}
	//creating the charecter bytes of received length
	char *pscL_Bytes = new char[ulL_RecvLen];
	//receiving the actual data 
	if(!pCL_Socket->mcfn_receive(pscL_Bytes,ulL_RecvLen)){
		DBG_ERROR((CG_EventLog),("%s Socket Disconnected : %d",mcfn_getAppName().c_str(),mcfn_getLastErrorCode()));
		delete []pscL_Bytes;	
		return false;	
	}
	//if the function pointer is set then we are giving the data and length
	std::string CL_receBytes="";
	CL_receBytes.assign(pscL_Bytes,ulL_RecvLen);
	delete []pscL_Bytes;
	return mcfn_onReceiveData(CL_receBytes,ulL_RecvLen,pCL_Socket);
}
/*
 *using the common send fuction to send the data to respective clinet
 *param1:clinet socket to whcih we are sending the data
 *param2:data to send
 */

bool CTcpServer::mcfn_sendToTcp(CSTSSocket *pCL_Socket,std::string CL_msgData){
	return mcfn_sendToTcpCommon(CL_msgData,pCL_Socket);	
}
/*
 *Server object with port,recevive function and ip if req.
 *param1: port where server has to start listen
 *param2: IP 
 */
CTcpServer::CTcpServer(int iL_port,std::string CL_ip){
	//if given port and ip is valid then only server will start listing
	if(mcfn_setServerPort(iL_port,CL_ip)){
		mcfn_Start();	
	}
	else{//exiting from the process beacuse of invalid port or ip
		DBG_CRITICAL((CG_EventLog),("port number is out of range :[%s]%d",CL_ip.c_str(),iL_port));
		exit(0);
	}
}
/**
 *Destructr of server class
 */
CTcpServer::~CTcpServer(){
	if(meC_ThreadHandle){
		int result = pthread_cancel(meC_ThreadHandle);
		DBG_VERBOSE((CG_EventLog),("Exited the thread with status:%d[%s]",result,meC_AppName.c_str()));
	}
}
/**
 *starting the server port listening
 */
bool CTcpServer::mcfn_Start(){
	CSTSOSUtil::mcfn_beginAndDetachThread(meC_ThreadHandle,fnG_Wrapper_ThStartServerThread,this);
	return true;
}
/**
 *once server theread started bases on the arguments at creation of object server threads starts listening
 */

void CTcpServer::mcfn_StartServerThread(){
	DBG_INFO((CG_EventLog), ("%sStartServerThread started...",mcfn_getAppName().c_str()));
	while(true){
		try{
			while(1)
			{	//closing the socket connection
				mcfn_close();
				//checking IP is passed by user,if not listening on port  
				if(meC_TcpIp.empty()){
					if(!mcfn_create(mesi_TcpPort)){
						DBG_ERROR((CG_EventLog), ("Error in creating socket :%d",mesi_TcpPort));
						CSTSOSUtil::mcfn_sleep(2);
						continue;
					}
				}
				else{//is Ip is present in the configuration 
					if(!mcfn_create(mesi_TcpPort,(char*)meC_TcpIp.c_str())){
						DBG_ERROR((CG_EventLog), ("Error in creating socket %s:%d",meC_TcpIp.c_str(),mesi_TcpPort));
						CSTSOSUtil::mcfn_sleep(2);
						continue;
					}   
				}
				if(!mcfn_bind()){
					DBG_ERROR((CG_EventLog), ("Error in binding on port %s:%d",meC_TcpIp.c_str(),mesi_TcpPort));
					CSTSOSUtil::mcfn_sleep(2);
					continue;
				}
				break;
			}
			mcfn_listen(5);
			while(true){
				CSTSSocket *pCL_Conn = new CSTSSocket;
				if(!(mcfn_accept(*pCL_Conn)&& mcfn_onAccept(pCL_Conn))){
					DBG_ERROR((CG_EventLog), ("Accept failed %s",mcfn_getAppName().c_str()));
					mcfn_close();
					delete pCL_Conn;
					break;
				}else{
					DBG_INFO((CG_EventLog), (" Client connected from %s",mcfn_getAppName().c_str()));
					SParam SL_Param(this, pCL_Conn);
					//after clinet connecction starting the client handler thread
					CSTSOSUtil::mcfn_beginAndDetachThread(fnG_Wrapper_ServerClientHandlerThread, &SL_Param);
				}
			}
		}
		catch(CSignalException e){
			DBG_ERROR((CG_EventLog),("Exception:%s",e.what()));
		}
		catch(...){
			DBG_ERROR((CG_EventLog),("Some unknown Exception came in mcfn_StartServer%sThread",mcfn_getAppName().c_str()));
		}

	}
	DBG_CRITICAL((CG_EventLog),("mcfn_StartServerThread down"));
}
/**
*	stating the client handling thread  if the user has givent the receiver functon pointer invoking that otherwise using hadling the clinet connection in library itself
*/
void CTcpServer::mcfn_StartClientThread(CSTSSocket *pCL_Socket){
	DBG_INFO((CG_EventLog),("Started %sClientThread[%p]",mcfn_getAppName().c_str(),pCL_Socket));
	bool is_needToClose=false;
	while(1){
		try{
			if(mcfn_receiveFromTcp(pCL_Socket)==false){
				DBG_ERROR((CG_EventLog),("Clint Has Discoonected From %s",mcfn_getAppName().c_str()));
				mcfn_onDisconnect(pCL_Socket);
				if(pCL_Socket!=NULL){
					pCL_Socket->mcfn_close();
					delete pCL_Socket;
				}
				break;
			}
		}
		catch(CSignalException e){
			DBG_ERROR((CG_EventLog),("Exception:%s",e.what()));
		}
		catch(...){
			DBG_ERROR((CG_EventLog),("Some unknown Exception came in mcfn_startClinet%sThread",mcfn_getAppName().c_str()));
		}

	}	
	DBG_CRITICAL((CG_EventLog),("Clinet %s Disconnected",mcfn_getAppName().c_str()));
}



bool CTcpServer::mcfn_onReceiveData(std::string CL_data,int iL_Len,CSTSSocket *pCL_Socket){
        DBG_VERBOSE((CG_EventLog),("[%ld]Receved Data from %s:%s[%d]",pCL_Socket,meC_AppName.c_str(),CL_data.c_str(),iL_Len));
        return true;
}
