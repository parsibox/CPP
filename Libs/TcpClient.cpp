/*
 * TcpConnection.cpp
 *
 *  Created on: May 12, 2013
 *      Author: surya
 */

#include"TcpClient.h"
#include<EventLog.h>
#include"signalHandler.h"
/**
 * using the same object created in application
 */
extern CEventLog CG_EventLog;

static FNVOID fnG_Wrapper_ThConnectToServer(void *pvL_This) {
	try {

		(static_cast<CTcpClient*> (pvL_This))->mcfn_connectToTcpThread();
	}
      catch(std::bad_cast& x)
      {
	 DBG_CRITICAL((CG_EventLog), ("Bad cast while starting fnG_Wrapper_ThConnectToServer"));
      }
      catch(...) {
		DBG_ERROR((CG_EventLog),("Thread stoped running"));
                throw;
	}
	RETURN;
}

CTcpClient::CTcpClient(std::string CL_AppName,int iL_noOftrys){
	mcfn_initialize(CL_AppName);
	mesi_numberOftrys=iL_noOftrys;
}
/**
 *clinet destructor
 */
CTcpClient::~CTcpClient(){
	meC_TcpConnDetails.clear();
	DBG_VERBOSE((CG_EventLog),("Exited the thread with status:[%s]",meC_AppName.c_str()));
	mcfn_setConnectionStatus(false);
	if(meC_ThreadHandle){
	  int result = pthread_cancel(meC_ThreadHandle);
	  DBG_VERBOSE((CG_EventLog),("Exited the thread with status:%d[%s]",result,meC_AppName.c_str()));
	}
}
/**
*clinet connection initilizer function for giving application name and receive function
*/
void CTcpClient::mcfn_initialize(std::string CL_AppName){
	mcfn_setAppName(CL_AppName);
	mcfn_setConnectionStatus(false);
	mesi_currentIdx=0;
	mesi_totalEntrys=0;
	meC_TcpConnDetails.clear();
}
/**
*	Adding the tcp server connecion details primary and secondary
*/
bool  CTcpClient::mcfn_addTcpDetails(std::string CL_PriIp,int iL_PriPort,std::string CL_SecIp,int iL_SecPort){
	mcfn_addTcpDetails(CL_PriIp,iL_PriPort);
	mcfn_addTcpDetails(CL_SecIp,iL_SecPort);
	return mcfn_Start();
}
/**
*starting the clinet connecion to server connetion thread
*/
bool CTcpClient::mcfn_Start(){
	mesi_totalEntrys=meC_TcpConnDetails.size();
	if(mesi_totalEntrys>0){
		CSTSOSUtil::mcfn_beginAndDetachThread(meC_ThreadHandle,fnG_Wrapper_ThConnectToServer,this);	
		return true;
	}
	DBG_CRITICAL((CG_EventLog),("Before Adding Tcp Details you are Traing to run the Thread that is not possible,Add the Details by using mcfn_addTcpDetails(\"IP\",port_num)"));	
	return false;
}
/**
*connection to server thread function
*/

void CTcpClient::mcfn_connectToTcpThread(){
	DBG_INFO((CG_EventLog),("Started mcfn_connectTo%sThread",mcfn_getAppName().c_str()));	
	int siL_Counter=0;
	std::string CL_TcpIp;
	int 	iL_Port;
	while(1){
		CL_TcpIp.clear();
		iL_Port=0;
		mcfn_close();
		siL_Counter=0;
		try{
			while(1){
				if(CL_TcpIp.empty()||siL_Counter>=mesi_numberOftrys){
					mefn_getNextTcpDetails(CL_TcpIp,iL_Port);	
					siL_Counter=0;
				}
				if(mefn_Connect(CL_TcpIp,iL_Port,siL_Counter)){
					siL_Counter=0;
					break;
				}
				CSTSOSUtil::mcfn_sleep(1);
				siL_Counter++;
			}
			if(!mcfn_onConnect()){
				DBG_ERROR((CG_EventLog),("Connection Rejected with %s",mcfn_getAppName().c_str()));

			}
			else{
				mcfn_setConnectionStatus(true);
				while(1){
					if(!mcfn_receiveData()||mcfn_getConnectionStatus()==false){
						mcfn_setConnectionStatus(false);	
						DBG_ERROR((CG_EventLog),("Connection Lost with %s",mcfn_getAppName().c_str()));	
						break;
					}
				}
			}
		}
		catch(CSignalException e){
			DBG_ERROR((CG_EventLog),("Exception:%s",e.what()));	
		}
		/*catch(...){
			DBG_ERROR((CG_EventLog),("Some unknown Exception came in mcfn_connectTo%sThread",mcfn_getAppName().c_str()));
                        break; 
		}*/		
	}
	DBG_CRITICAL((CG_EventLog),("Exiting from mcfn_connectTo%sThread",mcfn_getAppName().c_str()));
}
/**
 *If user gives the receive function is will be invoked other wise server connection will be handled by library
 */
bool CTcpClient::mcfn_receiveData(){
	DBG_VERBOSE((CG_EventLog),("Receiveing From %s",mcfn_getAppName().c_str()));
	int ulL_RecvLen = 0x00;
	//receiveing the length of data first
	if(!mcfn_receive((char*)&ulL_RecvLen, sizeof(ulL_RecvLen)))
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
	if(!mcfn_receive(pscL_Bytes,ulL_RecvLen)){
		DBG_ERROR((CG_EventLog),("%s Socket Disconnected : %d",mcfn_getAppName().c_str(),mcfn_getLastErrorCode()));
		delete []pscL_Bytes;	
		return false;	
	}
	//if the function pointer is set then we are giving the data and length
	std::string CL_receBytes="";
	CL_receBytes.assign(pscL_Bytes,ulL_RecvLen);
	delete []pscL_Bytes;
	return mcfn_onReceiveData(CL_receBytes,ulL_RecvLen);

}
/**
*trying to connect to given ip and port
*/
bool CTcpClient::mefn_Connect(std::string CL_Ip,int iL_Port,int iL_Count){
	if(!mcfn_create(iL_Port)){
		DBG_ERROR((CG_EventLog), ("[%d]Error Creating Socket for %s Port:%d",iL_Count,mcfn_getAppName().c_str(),iL_Port));
		return false;
	}
	if(!mcfn_connect((char *)CL_Ip.c_str(),30)) {
		DBG_ERROR((CG_EventLog),("[%d]Error in Connecting to %s [%s:%d]",iL_Count,mcfn_getAppName().c_str(),CL_Ip.c_str(),iL_Port));
		mcfn_close();
		return false;
	}
	DBG_VERBOSE((CG_EventLog),("[%d]Connected to %s[%s:%d]",iL_Count,mcfn_getAppName().c_str(),CL_Ip.c_str(),iL_Port));
	return true;
}
/**
 *sending the data to server ,if the connection is not available it will be going to sleep for 1 second,idally this should take care of application
 */
bool CTcpClient::mcfn_sendToTcp(std::string CL_msgData){
	if(mcfn_getConnectionStatus()==false){
		DBG_ERROR((CG_EventLog),("%s Connection is down Not Able to Send the Message",mcfn_getAppName().c_str()));
		//CSTSOSUtil::mcfn_sleep(1);
		return false;
	}

	int iL_len=CL_msgData.length();
	DBG_VERBOSE((CG_EventLog),("Sending Length:%d",iL_len));
	//sending the length firt
	if(!mcfn_send((char *)&iL_len,sizeof(int))){
		DBG_ERROR((CG_EventLog), ("Error in sending message to %s",mcfn_getAppName().c_str()));
		mcfn_setConnectionStatus(false);
		return false;	
	}
	//sendting the data
	if (!mcfn_send((char *)CL_msgData.c_str(),CL_msgData.length())) {
		DBG_ERROR((CG_EventLog), ("Error in sending message to %s",mcfn_getAppName().c_str()));
		mcfn_setConnectionStatus(false);
		return false;
	}
}


bool CTcpClient::mcfn_onReceiveData(std::string CL_data,int iL_Len){
	DBG_VERBOSE((CG_EventLog),("Receved Data from %s:%s[%d]",meC_AppName.c_str(),CL_data.c_str(),iL_Len));
	return true;
}
