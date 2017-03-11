#include"EsmeClient.hpp"
#include "Externs.h"
#include"MsgMap.h"
#include"EsmeServer.hpp"
extern CMsgMap<int,CEsmeClient*> CG_MoMap;

extern CEsmeServer  *pCG_xmlServer;
CEsmeClient::CEsmeClient(int iL_hdrLen,CSTSSocket* sock):CTcpClient("SMPP",10){
        mesi_headerLen = iL_hdrLen;
        pmeC_serverSock = sock;
	mcfn_start();
}

CEsmeClient::~CEsmeClient(){

}

void CEsmeClient::mcfn_setDetails(std::string username,std::string password,std::string appId,int version)
{
	mdC_name = username ;
	mdC_password = password;
	mdC_applicationId = appId;
	mdsi_version = version;
}
//virtual function from CTcpClinet class
bool CEsmeClient::mcfn_onConnect(){
	sleep(1);
	bL_isLoginSucess=false;	
	if(!mcfn_sendUssdBind()){
		DBG_ERROR((CG_EventLog),("Error in sending login request reconnecting"));
		return false;
	}
	// receive the responce and check responce is fine or not if it is not fine return false ,
	//so that connection will be closed and reconnected by TcpClinet class	
	bL_isLoginSucess=mcfn_receiveData();
	return bL_isLoginSucess;
}
bool CEsmeClient::mcfn_receiveData(){
	if(bL_isLoginSucess == false  && !mcfn_isDataAvailable(30000000)){
		DBG_ERROR((CG_EventLog),("Login Responce didn't came in 30 sec so reconnecting"));
		return false;	
	}
        int iL_dataLen=0;
        Buffer pu8_data;
	pu8_data.resize(mesi_headerLen);
	if(mcfn_receive((char*)&pu8_data[0],mesi_headerLen)){//receiving header
		iL_dataLen = mcfn_getDataLen((u8*)&pu8_data[0]);
		if(iL_dataLen >mesi_headerLen){
			pu8_data.resize(iL_dataLen);
			if(false == mcfn_receive((char*)&pu8_data[mesi_headerLen],iL_dataLen-mesi_headerLen)){
				DBG_ERROR((CG_EventLog),("Receive failed so closing the connection"));
				return false;
			}
		}
		else if(iL_dataLen < mesi_headerLen )
		{
			DBG_ERROR((CG_EventLog),("Receive Length is invalid so disconnecing connection :%d",iL_dataLen));
			return false;
		}

	}
	else{
		DBG_ERROR((CG_EventLog),("Receive failed so closing the connection"));
		return false;
	}
	DBG_INFO((CG_EventLog),("Receiveing the data Lagth[%ld]",iL_dataLen));
	CG_EventLog.mcfn_logU8Message(&pu8_data[0],iL_dataLen);
	std::string CL_recData;
	CL_recData.assign((char*)&pu8_data[0],iL_dataLen);
	if(!bL_isLoginSucess){
		return mcfn_processReceivedMessage(CL_recData);
	}
	else{
            
		meC_RecDataQue.mcfb_insertIntoQue(CL_recData);
	}	
	return true;	
}

bool CEsmeClient::mcfn_run(int,void *){
	while(1){
		std::string CL_recData;
		if(!meC_RecDataQue.mcfb_getNextMessage(CL_recData,false)){	
			sleep(1);
		}
		else{
			//todo::need to check retry errors 
			mcfn_processReceivedMessage(CL_recData);
		}
	}
	return true;
}
std::string CEsmeClient::mcfn_getThreadName(void*){
	return "PrecessReceivedSmppPacketThread";
}
//virtual function from EsmeClinet Class
bool CEsmeClient::mcfn_sendUssdMessage(UssdData &CL_pdu){

	if(CL_pdu.op_type() == UssdData::START)
	{
		CG_MoMap.mcfb_insert(CL_pdu.receiver_session_id(),this);
	}
        else if(CL_pdu.op_type() == UssdData::END)
        {
		CG_MoMap.mcfb_removeElement(CL_pdu.receiver_session_id());
	}
	std::string  CL_msg;
	pCG_xmlServer->mcfn_encode(CL_pdu,CL_msg);
	CG_EventLog.mcfn_logU8Message((u8*)CL_msg.c_str(),CL_msg.length());
	if(pmeC_serverSock->mcfn_send((const char*)CL_msg.c_str(),CL_msg.length()))
		return true;

	mcfn_close();
	return false;
}

bool CEsmeClient::mcfn_sendMsgToServer(u8 *,int,UssdData& CL_pdu)
{
  return mcfn_sendUssdMessage(CL_pdu);
}
