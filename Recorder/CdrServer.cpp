#include"CdrServer.hpp"
#include"EventLog.h"
extern CEventLog CG_EventLog;

extern CMsgQue<CdrMsg*> CG_ReceiverQ;

CCdrServer::CCdrServer(int iL_port,std::string CL_ip):CTcpServer(iL_port,CL_ip){
}
CCdrServer::~CCdrServer(){
}
//virtual function from CTcpServer class
bool CCdrServer::mcfn_onAccept(CSTSSocket *pCL_conn){
	return true;
}
bool CCdrServer::mcfn_onReceiveData(std::string CL_data,int,CSTSSocket* pCL_conn){
	CdrMsg *pCL_Msg=new CdrMsg();
	if(pCL_Msg->ParseFromArray(CL_data.c_str(),CL_data.length())){
		 DBG_ERROR((CG_EventLog), ("Failed to Parse from Array:%s",pCL_Msg->ShortDebugString().c_str()));
		 delete pCL_Msg;
	}	
	else{
		 DBG_VERBOSE((CG_EventLog), ("Received Data:%s",pCL_Msg->ShortDebugString().c_str()));
		 CG_ReceiverQ.mcfb_insertIntoQue(pCL_Msg);
	}
	
	return true;
}
bool CCdrServer::mcfn_onDisconnect(CSTSSocket* pCL_sock){
	DBG_ERROR((CG_EventLog), ("Clinet Disconnected :%u",pCL_sock));
	return true;	
}
