#include"Receiver.hpp"
#include"EventLog.h" 
extern CEventLog CG_EventLog;
int count =0;
bool CReceiver::mcfn_onAccept(CSTSSocket *pCL_conn){
	pmeC_conn=pCL_conn;
        count =0;
	bool is_login=mcfn_receiveFromTcp(pCL_conn); 
	sleep(1);
	return is_login;
}
bool CReceiver::mcfn_sendMsgToServer(u8* buf, int len){
	return pmeC_conn->mcfn_send((const char*)buf,len);
}


bool CReceiver::mcfn_receiveFromTcp(CSTSSocket *pCL_Socket){
	CHeader CL_header;
	if(count ==1)
	{
		sleep(1);
		CL_header.mcsi_senderCB++;
		mcfn_sendUssdBegin(CL_header,(char*)"919493024705","*667#",0x0f,"",USSD_OPTYPE::USSD_REQUEST);
	}
	DBG_INFO((CG_EventLog),("Receiveing the data"));
	Buffer CL_receiveData;
	CL_receiveData.resize(20);
	if(pCL_Socket->mcfn_receive((char*)&CL_receiveData[0],20)){//receiving header
		CL_header.mcfn_setHeader((u8*)&CL_receiveData[0]);
		if(CL_header.mcsi_cmdLength>20){
			CL_receiveData.resize(CL_header.mcsi_cmdLength);
			if(pCL_Socket->mcfn_receive((char*)&CL_receiveData[20],CL_header.mcsi_cmdLength-20)== false){
				return false;	
			}
		}
		else if( CL_header.mcsi_cmdLength < 20)
			return false;
	}
	else{
		DBG_ERROR((CG_EventLog),("Clinet Disconnection happend"));
		return false;
	}
	DBG_INFO((CG_EventLog),("Receiveing the data from SMPP Clinet:%d",CL_header.mcsi_cmdLength));
	count++;
	return mcfn_processReceivedMessage(CL_header,&CL_receiveData[20],CL_header.mcsi_cmdLength);

}

bool CReceiver::mcfn_onUssdBind(CHeader& CL_header,u8* pu8L_param,int iL_len){
	CUssdBind CL_pdu(CL_header);
	CL_pdu.mcfn_setParam(pu8L_param);
	 DBG_INFO((CG_EventLog),("Sending Bind Responc:%s",CL_pdu.pmcu8_AccountName));	
	mcfn_sendUssdBindResp(CL_header,0,(char*)CL_pdu.pmcu8_AccountName);
        return true;
}
bool CReceiver::mcfn_onUssdUnbindResp(CHeader&){
}
bool CReceiver::mcfn_onUssdBegin(CHeader& CL_header,u8* pu8L_param,int iL_len) {
	CUssdBegin CL_begin(CL_header);
	int swaper= CL_header.mcsi_senderCB;
	CL_header.mcsi_senderCB=CL_header.mcsi_receiverCB;
	CL_header.mcsi_receiverCB=swaper;

	CL_begin.mcfn_setParam(pu8L_param);
	char pcL_input[10]={0x00};
	switch(CL_begin.mcu8_ussdOptype){
		case USSD_OPTYPE::USSD_REQUEST:
			printf("Recevied Data:%s\n",CL_begin.pmcu8_ussdString);
			scanf("%s",pcL_input);
			mcfn_sendUssdContinue(CL_header,(char*)CL_begin.pmcu8_msisdn,"",CL_begin.mcu8_codeSchema,pcL_input,USSD_OPTYPE::USSD_RESPONCE);
			return true;
			break;
		case USSD_OPTYPE::USSD_NOTIFY:
			printf("Recevied Notify Data:%s\n",CL_begin.pmcu8_ussdString);
			//mcfn_sendUssdEnd(CL_header,(char*)CL_begin.pmcu8_msisdn,"",CL_begin.mcu8_codeSchema,pcL_input,USSD_OPTYPE::USSD_RELEASE);
			mcfn_sendUssdContinue(CL_header,(char*)CL_begin.pmcu8_msisdn,"",CL_begin.mcu8_codeSchema,pcL_input,USSD_OPTYPE::USSD_RESPONCE);

			break;
                default:
                        break;
        }
        return true;
}
bool CReceiver::mcfn_onUsddContinue(CHeader& CL_header,u8* pu8L_param,int iL_len){
	 CUssdContinue CL_continue(CL_header);
        CL_continue.mcfn_setParam(pu8L_param);
	int swaper= CL_header.mcsi_senderCB;
	CL_header.mcsi_senderCB=CL_header.mcsi_receiverCB;
	CL_header.mcsi_receiverCB=swaper;

	char pcL_input[10]={0x00};
	switch(CL_continue.mcu8_ussdOptype){
		case USSD_OPTYPE::USSD_REQUEST:
			printf("Recevied Request Data:%s\n",CL_continue.pmcu8_ussdString);
			scanf("%s",pcL_input);
			mcfn_sendUssdContinue(CL_header,(char*)CL_continue.pmcu8_msisdn,"",CL_continue.mcu8_codeSchema,pcL_input,USSD_OPTYPE::USSD_RESPONCE);
			break;
		case USSD_OPTYPE::USSD_NOTIFY:
			//TODO::This will never come to CMS.
			printf("Recevied Notify Data:%s\n",CL_continue.pmcu8_ussdString);
			mcfn_sendUssdContinue(CL_header,(char*)CL_continue.pmcu8_msisdn,"",CL_continue.mcu8_codeSchema,pcL_input,USSD_OPTYPE::USSD_RESPONCE);
			//mcfn_sendUssdEnd(CL_header,(char*)CL_continue.pmcu8_msisdn,"",CL_continue.mcu8_codeSchema,pcL_input,USSD_OPTYPE::USSD_RELEASE);
			break;
		case  USSD_OPTYPE::USSD_RESPONCE:
			//TODO::need to find the next menu according to user input
			printf("Recevied Responce Data:%s\n",CL_continue.pmcu8_ussdString);
			scanf("%s",pcL_input);
			mcfn_sendUssdContinue(CL_header,(char*)CL_continue.pmcu8_msisdn,"",CL_continue.mcu8_codeSchema,pcL_input,USSD_OPTYPE::USSD_REQUEST);

			break;
		case USSD_OPTYPE::USSD_RELEASE:
			printf("Recevied Relese Data:%s\n",CL_continue.pmcu8_ussdString);
		default:
			break;
	}
	return true;
}
bool CReceiver::mcfn_onUssdEnd(CHeader& CL_header,u8* pu8L_param,int iL_len){
	 CUssdEnd CL_end(CL_header);
        CL_end.mcfn_setParam(pu8L_param);
        switch(CL_end.mcu8_ussdOptype){
                case  USSD_OPTYPE::USSD_RESPONCE:
			printf("Recevied Responce Data:%s\n",CL_end.pmcu8_ussdString);

                        break;
                case  USSD_OPTYPE::USSD_RELEASE:
			printf("Recevied Relese Data:%s\n",CL_end.pmcu8_ussdString);
                        break;
                default:
                        break;
        }
	return true;
}
bool CReceiver::mcfn_onUssdAbort(CHeader&){
}
bool CReceiver::mcfn_onUssdSwitch(CHeader&,u8*,int){
}
bool CReceiver::mcfn_onUssdChargingInd(CHeader&,u8*,int){
}
bool CReceiver::mcfn_onUssdChargingIndRes(CHeader&){
}

bool CReceiver::mcfn_onUssdShake(CHeader& CL_header){
	int swaper= CL_header.mcsi_senderCB;
	CL_header.mcsi_senderCB=CL_header.mcsi_receiverCB;
	CL_header.mcsi_receiverCB=swaper;
	return mcfn_sendUssdShakeResp(CL_header);
}
bool CReceiver::mcfn_onUssdShakeResp(CHeader&){
	return true;
}

