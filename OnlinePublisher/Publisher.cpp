/*
 * Publisher.cpp
 *
 *  Created on: Apr 14, 2014
 *      Author: surya
 */


#include"Publisher.h"
#include"EventLog.h"
extern CEventLog CG_EventLog;
CPublisher * CPublisher::pmeC_Instance = NULL;


bool CClientConnect::mcfn_onConnect(){
	 //CPublisher::mcfnS_GetInstance()->mcfn_addConnection(this);
	return true;
}
bool CClientConnect::mcfn_receiveData(){
	  int ulL_RecvLen = 0x00;
        if(!mcfn_receive((char*)&ulL_RecvLen, sizeof(ulL_RecvLen)))
        {
                DBG_ERROR((CG_EventLog),("%s Socket Disconnected : %d",mcfn_getAppName().c_str(),mcfn_getLastErrorCode()));
		return false;
	}
        DBG_ERROR((CG_EventLog),("%s Thowtteling Message Received %d",mcfn_getAppName().c_str(),ulL_RecvLen));
	if(ulL_RecvLen==1)//throwttled
	{
	 	CPublisher::mcfnS_GetInstance()->mcfn_deleteConnection(this);
	}
	else{
	 	CPublisher::mcfnS_GetInstance()->mcfn_addConnection(this);
	}
	return true;
}

CPublisher* CPublisher::mcfnS_GetInstance(){
        if(pmeC_Instance == NULL){
                DBG_INFO((CG_EventLog),("Creating Publisher object..."));
                pmeC_Instance = new CPublisher();
	}
        return pmeC_Instance;
}



bool CPublisher::mcfn_addConnection(CClientConnect *pCL_obj){
	CAutoLocker CL_auto(meC_connLock);
	for(int i=0;i<meC_ClientList.size();i++){
		if(meC_ClientList[i]==(unsigned long)pCL_obj){
			return true;
		}
	}
	meC_ClientList.push_back((unsigned long)pCL_obj);
	++mesi_totalConn;
	return true;
}
bool CPublisher::mcfn_deleteConnection(CClientConnect *pCL_obj){
	CAutoLocker CL_auto(meC_connLock);
	for(int siL_Idx=0;siL_Idx<meC_ClientList.size();siL_Idx++){
		if(meC_ClientList[siL_Idx]==(unsigned long)pCL_obj){
			meC_ClientList.erase(meC_ClientList.begin()+siL_Idx);
			--mesi_totalConn;
			return true;
		}
	}
	return false;
}
CClientConnect * CPublisher::mcfn_getConnection(){
	//in thread only we are locking don't lock again here
	if(mesi_totalConn==0)
		return NULL;	
	if(mesi_activeIdx>=mesi_totalConn)
		mesi_activeIdx=0;
	return (CClientConnect*)meC_ClientList[mesi_activeIdx++];
}
bool CPublisher::mcfn_isConnAvailable(){
	(meC_ClientList.empty()==false)?true:false;
}

//virtual function which started theread
bool CPublisher::mcfn_run(int iL_threadId,void *p){
	while(1){
		DeliverToEsme *pCL_Msg=NULL;
		if(meC_Que.mcfb_getNextMessage(pCL_Msg,false)==false){
			DBG_DEBUG((CG_EventLog),("Messages are not there to send to Esme"));
			sleep(1);
			continue;
		}
		else{
			CClientConnect *pCL_conn=NULL;
			std::string CL_msg="";
			if(pCL_Msg->SerializeToString(&CL_msg)){
				CAutoLocker CL_auto(meC_connLock);
				pCL_conn=mcfn_getConnection();
				if(pCL_conn==NULL||pCL_conn->mcfn_sendToTcp(CL_msg)==false){
					meC_Que.mcfb_insertIntoQue(pCL_Msg);
					meC_connLock.Unlock();
					sleep(1);
					DBG_INFO((CG_EventLog),("No Esme is Active"));
				}
				else{
					DBG_INFO((CG_EventLog),("[%d]Sent to Esme Client[%d]:%s",pCL_conn->mcfn_getEsmeId(),iL_threadId,pCL_Msg->ShortDebugString().c_str()));
					delete pCL_Msg;
					pCL_Msg=NULL;
				}
			}
			else{
				DBG_ERROR((CG_EventLog),("Failed to Serialise:%s",pCL_Msg->ShortDebugString().c_str()));
				delete pCL_Msg;
				pCL_Msg=NULL;
			}
		}
	}
}
