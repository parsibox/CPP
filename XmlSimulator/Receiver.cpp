#include"Receiver.hpp"
#include"EventLog.h" 
extern CEventLog CG_EventLog;
bool CReceiver::mcfn_onAccept(CSTSSocket *pCL_conn){
	pmeC_conn=pCL_conn;
	mcb_Login= mcfn_receiveFromTcp(pCL_conn);
	return mcb_Login;	
}
bool CReceiver::mcfn_sendMsgToServer(u8* buf, int len){
        return pmeC_conn->mcfn_send((const char*)buf,len);
}


bool CReceiver::mcfn_receiveFromTcp(CSTSSocket *pCL_Socket){
        CHeader CL_header;
        Buffer CL_receiveData;
        CL_receiveData.resize(HEADER_LENGTH);
        if(pCL_Socket->mcfn_receive((char*)&CL_receiveData[0],HEADER_LENGTH)){//receiving header
                CL_header.mcfn_setHeader((u8*)&CL_receiveData[0]);
                int u32L_len= CL_header.mcsi_cmdLength;
                if(u32L_len>HEADER_LENGTH){
			CL_receiveData.resize(u32L_len);
			if(pCL_Socket->mcfn_receive((char*)&CL_receiveData[HEADER_LENGTH],u32L_len-HEADER_LENGTH)){
				DBG_INFO((CG_EventLog),("Receiveing the data from SMPP Clinet:%d",u32L_len));
				if(mcb_Login)
					return mcfn_processReceivedMessage(CL_header,&CL_receiveData[HEADER_LENGTH],u32L_len);
				else
					return mcfn_onUssdBind(CL_header,&CL_receiveData[HEADER_LENGTH],u32L_len);
			}
                }
	}
return false;
}

bool CReceiver::mcfn_onUssdBind(CHeader& CL_header,u8* pu8L_param,int iL_len){
	CUssdBind CL_pdu;
	DBG_INFO((CG_EventLog),("Received Request :%s",pu8L_param));	
	if(!CL_pdu.mcfn_decode((char*)pu8L_param))
		DBG_INFO((CG_EventLog),("Decode Failed :%s",pu8L_param));	
		
	DBG_INFO((CG_EventLog),("Sending Bind Responc:%s",CL_pdu.mcfn_ShortDebugString().c_str()));	
	return mcfn_sendUssdBindResp(CL_header.mcsi_cmdSessionId,0,"Success Full ",atoi(CL_pdu.mcC_requestId.c_str()));
}
bool CReceiver::mcfn_onUssdBegin(CHeader& CL_header,CUssdMsg &CL_pdu) {
	char pcL_input[10]={0x00};
	printf("Recevied Data:%s\n",CL_pdu.mcC_userData.c_str());
	scanf("%s",pcL_input);
	CL_pdu.mcC_userData.assign(pcL_input);
	CL_pdu.mcC_msgType.assign("4");
	CL_pdu.mcb_isRequest = false;
	return mcfn_sendUssdContinue(CL_pdu,CL_header.mcsi_cmdSessionId);
}
bool CReceiver::mcfn_onUsddContinue(CHeader& CL_header,CUssdMsg &CL_pdu){
	char pcL_input[10]={0x00};
	printf("Recevied Request Data:%s\n",CL_pdu.mcC_userData.c_str());
	scanf("%s",pcL_input);
	CL_pdu.mcC_userData.assign(pcL_input);
	CL_pdu.mcb_isRequest = false;
	CL_pdu.mcC_msgType.assign("4");
        return mcfn_sendUssdContinue(CL_pdu,CL_header.mcsi_cmdSessionId);
}
bool CReceiver::mcfn_onUssdEnd(CHeader& CL_header,CUssdMsg &CL_pdu){
	printf("Recevied Responce Data:%s\n",CL_pdu.mcC_userData.c_str());
	return true;
}
bool CReceiver::mcfn_onUssdShake(CHeader&){
	return true;
}
bool CReceiver::mcfn_onUssdShakeResp(CHeader&){
	return true;
}

