#include<Server.hpp>
#include<vector>
#include<EventLog.h>
extern CEventLog CG_EventLog;

CServer :: CServer (int iL_port, CMsgQue<IPCMsg>* pCL_recQue, std::string CL_ip ): CTcpServer( iL_port, CL_ip )
{
	pmeC_RecQue=pCL_recQue;
	mcfn_setAppName("IPCServer");
}
CServer:: ~CServer()
{
}
//virtual function from CTcpServer class
bool CServer::mcfn_onAccept(CSTSSocket *pCL_client)
{
	bool bL_return = false;
	char pcL_ipAndPort[40]={0x00}; 
	CHeader hdr;
	CSTSSocket *pCL_temp;
	pCL_client->mbfn_getIPAndPort(pcL_ipAndPort,40);
        DBG_INFO((CG_EventLog),("New Connction from :%s,Receiving Header:%d",pcL_ipAndPort,HEADER_LENGTH));
	
	if(pCL_client->mcfn_receive((char*)&hdr,HEADER_LENGTH))
	{
		short siL_tmpSrc=hdr.src;
		hdr.src=hdr.dst;
		hdr.dst=siL_tmpSrc;
		hdr.status = IPC_ALREADY_BINDED;
		if(false == meC_srcMap.mcfb_findElement(siL_tmpSrc,pCL_temp))
		{
			if(meC_srcMap.mcfb_insert(siL_tmpSrc,pCL_client))
			{
				meC_connMap.mcfb_insert(pCL_client,siL_tmpSrc);
				hdr.status=0;
				bL_return = true;
        		DBG_ERROR((CG_EventLog),("New Connection Added:%d,%p",siL_tmpSrc,pCL_temp));
			}
		}
		else{
			pCL_temp->mbfn_getIPAndPort(pcL_ipAndPort,40);
        		DBG_ERROR((CG_EventLog),("Connection Already Present From:%s ,so disconnecting:%d",pcL_ipAndPort,siL_tmpSrc));
		}
		pCL_client->mcfn_send((char*)&hdr,HEADER_LENGTH);
	}
	return bL_return;
}

bool CServer::mcfn_receiveFromTcp(CSTSSocket* pCL_client)
{
	bool bL_return=false;
	IPCMsg CL_msg;
	if(pCL_client->mcfn_receive((char*)&CL_msg.hdr,HEADER_LENGTH))
	{
		if(CL_msg.hdr.len<0) return false;
	
		unsigned char  *pcL_data = new unsigned char [CL_msg.hdr.len];
		if(pCL_client->mcfn_receive((char*)pcL_data, CL_msg.hdr.len))
		{
			 DBG_ERROR((CG_EventLog),("Received data: %s,Len :%d ",pcL_data,CL_msg.hdr.len));
			CL_msg.data.assign((char*)pcL_data,CL_msg.hdr.len);
			pmeC_RecQue->mcfb_insertIntoQue(CL_msg);
			bL_return= true;
		}
		delete[] pcL_data;

	}

	return bL_return;
}

bool CServer::mcfn_onDisconnect(CSTSSocket* pCL_conn)
{
	short siL_src=0;
	char pcL_ipAndPort[40]={0x00};
	if(meC_connMap.mcfb_getElement(pCL_conn,siL_src))
		meC_srcMap.mcfb_removeElement(siL_src);
	
	pCL_conn->mbfn_getIPAndPort(pcL_ipAndPort,40);
	DBG_ERROR((CG_EventLog),("Client %s  Disconnected,so removed mapping data for :%d ",pcL_ipAndPort,siL_src));
	return true;
}

bool CServer::mcfn_sendMessage(IPCMsg &CL_msg)
{
	CSTSSocket* pCL_client =NULL;
	if(meC_srcMap.mcfb_findElement(CL_msg.hdr.dst,pCL_client))
	{
		if(pCL_client->mcfn_send((char*) &CL_msg.hdr, HEADER_LENGTH ) && 
				pCL_client->mcfn_send((char*) CL_msg.data.c_str(), CL_msg.data.length() ) ) 
			return true;
	}
	return false;
}
