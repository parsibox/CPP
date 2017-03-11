#include<IPCClient.hpp>
#include<EventLog.h>
extern CEventLog CG_EventLog;

CIPCClient::CIPCClient(short siL_moduleId,short siL_ipcRouterId,CMsgQue<IPCMsg> *pCL_recQue) : CTcpClient("IPCServer")
{ 
	pmeC_RecQue=pCL_recQue; 
	mesi_moduleId=siL_moduleId;
	mesi_ipcRouterId=siL_ipcRouterId;
}

bool CIPCClient::mcfn_receiveData()
{
	bool bL_return = false;
	 IPCMsg CL_msg;
         if(mcfn_receive((char*)&CL_msg.hdr,HEADER_LENGTH))
        {
                
		unsigned char *pcL_data =  new unsigned char[CL_msg.hdr.len];
                if(mcfn_receive((char*)pcL_data, CL_msg.hdr.len))
                {
                        CL_msg.data.assign((char*)pcL_data,CL_msg.hdr.len);
                        pmeC_RecQue->mcfb_insertIntoQue(CL_msg);
			bL_return = true;
                }
		delete[] pcL_data;
        }

        return bL_return;
}
bool CIPCClient::mcfn_onConnect()
{
	CHeader hdr;
	hdr.src=mesi_moduleId;
	hdr.dst=mesi_ipcRouterId;
	if(mcfn_send((char*)&hdr,HEADER_LENGTH))
	{
		hdr.clear();
		if(mcfn_receive((char*)&hdr,HEADER_LENGTH))
		{
			if(IPC_SUCCESS == hdr.status )
				return true;
		}	
	}
	sleep(1);
	return false;
}

bool CIPCClient::mcfn_sendMessage(short iL_dst,std::string &CL_data)
{
	CHeader hdr;
	hdr.src=mesi_moduleId;
	hdr.dst=iL_dst;
	hdr.len= CL_data.length();	
	if(mcfn_send((char*) &hdr, HEADER_LENGTH ) && 
			mcfn_send((char*) CL_data.c_str(), CL_data.length() ) ) 
		return true;
	return false;
}

