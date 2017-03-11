#include"Receiver.hpp"
#include"Externs.h"
CReceiver::CReceiver(int iL_port,int iL_throwttalSize,std::string CL_ip):CTcpServer(iL_port,CL_ip){
	mesi_ThrowttleSize=iL_throwttalSize;
	meb_isThrowtteled=false;
	mcfn_setAppName("Publisher");
}
CReceiver::~CReceiver(){
}
//virtual function from CTcpServer class
bool CReceiver::mcfn_onAccept(CSTSSocket *pCL_conn){
	CAutoLocker CL_Lock(meC_ActiveListLocker);
	int iL_throw=0;
	if(pCL_conn->mcfn_send((char*)&iL_throw,sizeof(iL_throw))){
		meC_connVec.push_back((unsigned long)pCL_conn);
		return true;
	}
	return false;
}
bool CReceiver::mcfn_onReceiveData(std::string CL_data,int,CSTSSocket* pCL_conn){
	DeliverToEsme *pCL_Msg=new DeliverToEsme();
	if(pCL_Msg->ParseFromArray(CL_data.c_str(),CL_data.length())==false){
		 DBG_ERROR((CG_EventLog), ("Failed to Parse from Array:%s",pCL_Msg->ShortDebugString().c_str()));
		 delete pCL_Msg;
	}	
	else{
		 DBG_VERBOSE((CG_EventLog), ("Received Data:%s",pCL_Msg->ShortDebugString().c_str()));
		 CG_ReceiverQ.mcfb_insertIntoQue(pCL_Msg);
		if(!meb_isThrowtteled && CG_ReceiverQ.mcfb_getCount()>mesi_ThrowttleSize){
			mcfn_sendThrottleMessage(1);
			meb_isThrowtteled=true;
		}		 
	}
	
	return true;
}
bool CReceiver::mcfn_onDisconnect(CSTSSocket* pCL_clientSocket){
	//TODO::need to remove from here
	DBG_INFO((CG_EventLog),("Client disconnected with Deliver module:%u",pCL_clientSocket));
	DBG_INFO((CG_EventLog),("Removing connection : %u Size Of Vector:%d", pCL_clientSocket,meC_connVec.size()));
	bool  bL_return=false;
	CAutoLocker CL_Lock(meC_ActiveListLocker);
	for(int siL_Idx=0;siL_Idx<meC_connVec.size();siL_Idx++){
		if(meC_connVec[siL_Idx]==(unsigned long)pCL_clientSocket){
			meC_connVec.erase(meC_connVec.begin()+siL_Idx);
			bL_return=true;
		}
	}
	DBG_INFO((CG_EventLog),("After Removing Size Of Vector:%d",meC_connVec.size()));
	return bL_return;
}

bool CReceiver::mcfn_sendThrottleMessage(int iL_throttleStatus){
	//TODO::here need to send the messsage to all connected clinets;	
	CAutoLocker CL_Lock(meC_ActiveListLocker);
	for(int siL_Idx=0;siL_Idx<meC_connVec.size();siL_Idx++){

		if(((CSTSSocket*)meC_connVec[siL_Idx])->mcfn_send((char*)&iL_throttleStatus,sizeof(iL_throttleStatus))){
			DBG_DEBUG((CG_EventLog),("Sent Thortteling Message  SucessFull to client %u",meC_connVec[siL_Idx]));
		}
		else{
			DBG_ERROR((CG_EventLog),("Failed to send the Thortteling closing connection:%u",meC_connVec[siL_Idx]));
			((CSTSSocket*)meC_connVec[siL_Idx])->mcfn_close();
			meC_connVec.erase(meC_connVec.begin()+siL_Idx);
		}
	}
}

