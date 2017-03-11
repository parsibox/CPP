#include"EsmeServer.hpp"
#include "Externs.h"
#include "DBOperations.h"
#include"EsmeClient.hpp"
#include"UssdInterface.hpp"
#include<typeinfo>

CMsgMap<int,CEsmeClient*> CG_MoMap;

CEsmeServer::CEsmeServer(int iL_hdrLen,int iL_port,std::string CL_ip):CTcpServer(iL_port,CL_ip)
{
        mesi_headerLen = iL_hdrLen;
        sleep(1);
	mcfn_start();
}

CEsmeServer::~CEsmeServer()
{

}

//virtual function from CTcpClinet class
bool CEsmeServer::mcfn_onAccept(CSTSSocket *pCL_socket) 
{
	sleep(1);
	bL_isLoginSucess=false;	
	bL_isLoginSucess=mcfn_receiveFromTcp(pCL_socket);
	return bL_isLoginSucess;
}
bool CEsmeServer::mcfn_onDisconnect(CSTSSocket *pCL_sock)
{
	std::list<CEsmeClient*>* pCL_clients = NULL;
	if(meC_socketMap.mcfb_findElement(pCL_sock,pCL_clients))
	{
		std::list<CEsmeClient*> ::iterator pCL_Itr;   
		for(pCL_Itr=pCL_clients->begin();pCL_Itr!=pCL_clients->end();++pCL_Itr)
		{
			try
			{ 
				CEsmeClient *pCL_esmeClient=(*pCL_Itr);
				delete pCL_esmeClient;
				pCL_esmeClient = NULL;
				pCL_Itr = pCL_clients->erase(pCL_Itr);
			}
			catch(const CSignalException& e)
			{
				DBG_ERROR((CG_EventLog),("Exception:%s",e.what()));	
			}
		}
		delete pCL_clients;
               meC_socketMap.mcfb_removeElement(pCL_sock);
	}
}
bool CEsmeServer::mcfn_receiveFromTcp(CSTSSocket *pCL_socket)
{
	if(bL_isLoginSucess == false  && !pCL_socket->mcfn_isDataAvailable(30000000)){
		DBG_ERROR((CG_EventLog),("Login Responce didn't came in 30 sec so reconnecting"));
		return false;	
	}
        int iL_dataLen=0;
        Buffer pu8_data;
	pu8_data.resize(mesi_headerLen);
	if(pCL_socket->mcfn_receive((char*)&pu8_data[0],mesi_headerLen)){//receiving header
		iL_dataLen = mcfn_getDataLen((u8*)&pu8_data[0]);
		if(iL_dataLen >mesi_headerLen){
			pu8_data.resize(iL_dataLen);
			if(false == pCL_socket->mcfn_receive((char*)&pu8_data[mesi_headerLen],iL_dataLen-mesi_headerLen)){
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
	DBG_INFO((CG_EventLog),("Receiveing the data [%s]",CL_recData.substr(mesi_headerLen,iL_dataLen).c_str()));
	if(!bL_isLoginSucess){
		return mcfn_processReceivedMessage(CL_recData,pCL_socket);
	}
	else{
		meC_RecDataQue.mcfb_insertIntoQue(CL_recData);
	}	
	return true;	
}

bool CEsmeServer::mcfn_run(int,void *)
{
	while(1)
	{
		std::string CL_recData;
		if(!meC_RecDataQue.mcfb_getNextMessage(CL_recData,false)){	
			sleep(1);
		}
		else
		{
			//todo::need to check retry errors 
			mcfn_processReceivedMessage(CL_recData);
		}
	}
	return true;
}
std::string CEsmeServer::mcfn_getThreadName(void*){
	return "PrecessReceivedSmppPacketThread";
}
//virtual function from EsmeClinet Class
bool CEsmeServer::mcfn_sendMsgToServer( u8* buf, int len,UssdData &CL_pdu){
	return true;
}

bool CEsmeServer::mcfn_onUssdBind(UssdData &CL_pdu,void *ptr)
{
	DBG_INFO((CG_EventLog),("Receive Huwai Ussd Bind :%s",CL_pdu.ShortDebugString().c_str()));
	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
	CL_QueryStream<<"SELECT * from USER_DETAILS WHERE USERNAME ='"<<CL_pdu.username()<<"'";
	CSTSSocket *pCL_sock = static_cast<CSTSSocket*>	(ptr);
	mysqlpp::StoreQueryResult CL_Result;
	if(CDBOperations::mcfn_ExecuteQuery(CL_QueryStream.str(),CL_Result)==SUCCESS)
	{
		if( CL_Result.num_rows() > 0 && CL_pdu.password().compare(CL_Result[0]["PASSWORD"]) == 0 )
		{ 
			CL_QueryStream.str("");
			CL_QueryStream<<"SELECT * from SERVER_DETAILS WHERE USERNAME ='"<<CL_pdu.username()<<"'";
			if(CDBOperations::mcfn_ExecuteQuery(CL_QueryStream.str(),CL_Result)==SUCCESS && CL_Result.num_rows() > 0)
			{
				std::list<CEsmeClient*>* pCL_clients = NULL;
				for(int i=0;i<CL_Result.num_rows();i++)
				{
                                        if(pCL_clients == NULL ) pCL_clients = new std::list<CEsmeClient*>;
					UssdInterface *pCL_esme = new UssdInterface(pCL_sock);
					pCL_esme->mcfn_setDetails(CL_Result[i]["USERNAME"].c_str(),CL_Result[i]["PASSWORD"].c_str(),CL_Result[i]["APPLICATIO_ID"].c_str(),atoi(CL_Result[i]["VERSION"]));
					pCL_esme->mcfn_addTcpDetails(CL_Result[i]["IP"].c_str(),atoi(CL_Result[i]["PORT"].c_str()),CL_Result[i]["IP"].c_str(),atoi(CL_Result[i]["PORT"].c_str()));
					pCL_clients->push_back(pCL_esme);
					CL_pdu.set_session_status(0) ;
					CL_pdu.set_msg("");
					meC_socketMap.mcfb_insert(pCL_sock,pCL_clients);
				}
			}
			else
			{
				CL_pdu.set_session_status(3) ;
				CL_pdu.set_msg("Server details unavailable");
			}

		}
		else
		{
			CL_pdu.set_session_status(2) ;
			CL_pdu.set_msg("password incorrect");
		}

	}
	else
	{
		CL_pdu.set_session_status(1) ;
		CL_pdu.set_msg("UNKNOWN User");
	}
	int tempSessionId = CL_pdu.sender_session_id();
	CL_pdu.set_sender_session_id(CL_pdu.receiver_session_id());
	CL_pdu.set_receiver_session_id(tempSessionId);
	CL_pdu.set_op_type(UssdData::LOGIN_RESP);
        std::string CL_loginResp;
	mcfn_encode(CL_pdu,CL_loginResp);
	pCL_sock->mcfn_send((char*) CL_loginResp.c_str(),CL_loginResp.length());
        return (CL_pdu.session_status()==0)?true:false;
}

bool CEsmeServer::mcfn_onUssdUnbind(UssdData &CL_pdu,void *ptr)
{
	DBG_INFO((CG_EventLog),("Receive Huwai Ussd Unbind :%s",CL_pdu.ShortDebugString().c_str()));
	CSTSSocket *pCL_sock = static_cast<CSTSSocket*>	(ptr);
	mcfn_onDisconnect(pCL_sock);
	return false;
}

bool CEsmeServer::mcfn_onUssdShake(UssdData &CL_pdu)
{
	DBG_INFO((CG_EventLog),("Receive Huwai Ussd Shake :%s",CL_pdu.ShortDebugString().c_str()));
}

bool CEsmeServer::mcfn_onUssdShakeResp(UssdData& CL_pdu)
{
	DBG_INFO((CG_EventLog),("Receive Huwai Ussd Shake Responce :%s",CL_pdu.ShortDebugString().c_str()));
}
bool CEsmeServer::mcfn_sendUssdMessage(UssdData &CL_pdu){
	DBG_INFO((CG_EventLog),("Receive Sending mesage to clinet:%s",CL_pdu.ShortDebugString().c_str()));
	CEsmeClient* pCL_sock = NULL ;
	if(CG_MoMap.mcfb_findElement(CL_pdu.sender_session_id(),pCL_sock) && pCL_sock!= NULL)
	{
		DBG_INFO((CG_EventLog),("Session id found a clinet:%ld",CL_pdu.sender_session_id()));
		if(CL_pdu.op_type() == UssdData::END)
		{
			CG_MoMap.mcfb_removeElement(CL_pdu.sender_session_id());
		} 
		std::string  CL_msg;
		pCL_sock->mcfn_encode(CL_pdu,CL_msg);
		CG_EventLog.mcfn_logU8Message((u8*)CL_msg.c_str(),CL_msg.length());
		return pCL_sock->mcfn_send((char*)CL_msg.c_str(),CL_msg.length());
	}
	DBG_INFO((CG_EventLog),("Session id  not found a clinet:%ld",CL_pdu.receiver_session_id()));
	//TODO::here need to send abort
	return true;
}
