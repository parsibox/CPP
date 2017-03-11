#include"Receiver.hpp"
#include"EventLog.h" 
#include<iostream>
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
	CG_EventLog.mcfn_logU8Message(buf,len);
	return pmeC_conn->mcfn_send((const char*)buf,len);
}


bool CReceiver::mcfn_receiveFromTcp(CSTSSocket *pCL_Socket){
	CHeader CL_header;
	if(count ==1)
	{
	DBG_INFO((CG_EventLog),("sending begin"));
		sleep(5);
		mcfn_sendUssdBegin(2,(char*)"919493024705","*667",0x0f,"*667#",USSD_OPTYPE::USSD_REQUEST);
		DBG_INFO((CG_EventLog),("after sending begin"));
	}
	DBG_INFO((CG_EventLog),("Receiveing the data"));
	Buffer CL_receiveData;
	static int hdrLen = sizeof(CL_header);
	CL_receiveData.resize(hdrLen);
	if(pCL_Socket->mcfn_receive((char*)&CL_receiveData[0],hdrLen)){//receiving header
		CL_header.mcfn_setHeader((u8*)&CL_receiveData[0]);
		DBG_INFO((CG_EventLog),("Receiveing the data :%s",CL_header.mcfn_ShortDebugString().c_str()));
		DBG_INFO((CG_EventLog),("Receiveing the data from SMPP Clinet:%d",CL_header.mcsi_cmdLength));
		if(CL_header.mcsi_cmdLength > hdrLen){
			CL_receiveData.resize(CL_header.mcsi_cmdLength);
			if(pCL_Socket->mcfn_receive((char*)&CL_receiveData[hdrLen],CL_header.mcsi_cmdLength-hdrLen)== false){
				return false;	
			}
		}
		else if( CL_header.mcsi_cmdLength < hdrLen)
			return false;
	}
	else{
		DBG_ERROR((CG_EventLog),("Clinet Disconnection happend"));
		return false;
	}
	DBG_INFO((CG_EventLog),("Receiveing the data from SMPP Clinet:%d",CL_header.mcsi_cmdLength));
	count++;
	return mcfn_processReceivedMessage(CL_header,&CL_receiveData[0],CL_header.mcsi_cmdLength);

}

bool CReceiver::mcfn_onUssdBind(CHeader& CL_header,u8* pu8L_param,int iL_len){
	 DBG_INFO((CG_EventLog),("Sending Bind Responc:%s","testAcount"));	
	mcfn_sendUssdBindResp(0,(char*)"testAcount","TR");
        return true;
}
bool CReceiver::mcfn_onUssdUnbindResp(CHeader&){
}
bool CReceiver::mcfn_onUssdBegin(Smpp::SubmitSm* pdu) {
        return true;
}
bool CReceiver::mcfn_onUssdContinue(Smpp::SubmitSm* pdu){
	  char messageString[2000]={0x00};
        std::copy(pdu->short_message().begin(), pdu->short_message().end(),messageString);
	std::cout<<"Recevied Conitue message :"<<messageString<<std::endl;
	std::cout<<"Enter Your option:";
	std::string input;
	std::cin>>input;
	mcfn_sendUssdContinue(pdu->destination_addr().address(),pdu->source_addr().address(),0x00,input,USSD_OPTYPE::USSD_RESPONCE);
	return true;
}
bool CReceiver::mcfn_onUssdEnd(Smpp::SubmitSm* pdu){
	  char messageString[2000]={0x00};
        std::copy(pdu->short_message().begin(), pdu->short_message().end(),messageString);
	std::cout<<"Recevied End message :"<<messageString<<std::endl;
	
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
	//return mcfn_sendUssdShakeResp(CL_header);
	return true;
}
bool CReceiver::mcfn_onUssdShakeResp(CHeader&){
	return true;
}

