#include"Receiver.hpp"
#include"Externs.h"
CReceiver::CReceiver(int iL_port,int iL_throwttalSize,std::string CL_ip):CTcpServer(iL_port,CL_ip){
	mesi_ThrowttleSize=iL_throwttalSize;
	meb_isThrowtteled=false;
}
CReceiver::~CReceiver(){
}
//virtual function from CTcpServer class
bool CReceiver::mcfn_onAccept(CSTSSocket *pCL_conn){
	meC_connVec.push_back(pCL_conn);
	return true;
}
bool CReceiver::mcfn_onReceiveData(std::string CL_data,int,CSTSSocket* pCL_conn){
	DeliverToEsme *pCL_Msg=new DeliverToEsme();
	if(pCL_Msg->ParseFromArray(CL_data.c_str(),CL_data.length())){
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
bool CReceiver::mcfn_onDisconnect(CSTSSocket*){
	//TODO::need to remove from here
}

bool CReceiver::mcfn_sendThrottleMessage(int iL_throttleStatus){
	//TODO::here need to send the messsage to all connected clinets;	
}
