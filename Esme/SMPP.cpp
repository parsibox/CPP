#include"SMPP.hpp"
#include"Externs.h"

CMsgQue<MsgTypes*> CG_SmppQue;
CSmpp::CSmpp(int iL_modId){	
	if(mcfn_loadConfigurations(iL_modId)){
		 DBG_INFO((CG_EventLog), ("Loaded initial configuration sucessfully:%d",iL_modId));	
	}
	else
	{
		 DBG_ERROR((CG_EventLog), ("Loaded initial configuration Failed:%d",iL_modId));	
			exit(1);
	}	
}
bool CSmpp::mcfn_loadConfigurations(int iL_modId){
	return mcfn_loadSmscDetails(mcfn_loadEsmeDetails(iL_modId));
}
int CSmpp::mcfn_loadEsmeDetails(int iL_modId){
	mysqlpp::StoreQueryResult CL_ResultSet;
	CDBOperations CL_dbObj;
	 std::stringstream CL_QueryStream;
        CL_QueryStream.str("");
        CL_QueryStream<<"SELECT * from ESME_DETAILS WHERE ESME_ID="<<iL_modId <<" limit 1";
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		if(CL_ResultSet.num_rows()>0){
			//start the receiver to receive messages from publisher
			pmcC_Receiver=new CReceiver(atoi(CL_ResultSet[0]["ESME_PORT"].c_str()),atoi(CL_ResultSet[0]["THROWTTEL_QSIZE"].c_str()));
			return atoi(CL_ResultSet[0]["SMSC_ID"].c_str());
		}
        }
                return 0;
	
	
}
bool  CSmpp::mcfn_loadSmscDetails(int iL_smscId){
	if(iL_smscId>0){
		mysqlpp::StoreQueryResult CL_ResultSet;
		CDBOperations CL_dbObj;
		std::stringstream CL_QueryStream;
		CL_QueryStream.str("");
		CL_QueryStream<<"SELECT * from SMSC_DETAILS WHERE SMSC_ID="<<iL_smscId <<" limit 1";
		if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
			if(CL_ResultSet.num_rows()>0){
				mcC_SmppClient.mcfn_initialise(CL_ResultSet[0]["USER_NAME"].c_str(),CL_ResultSet[0]["PASSWORD"].c_str(),CL_ResultSet[0]["SYS_TYPE"].c_str(),CL_ResultSet[0]["CONN_TYPE"].c_str(),atoi(CL_ResultSet[0]["TON"].c_str()),atoi(CL_ResultSet[0]["NPI"].c_str()),CL_ResultSet[0]["ADDR_RANGE"].c_str());
				mcsi_MsgPerSec=atoi(CL_ResultSet[0]["MSG_PER_SEC"].c_str());
				 return mcC_SmppClient.mcfn_addTcpDetails(CL_ResultSet[0]["SMSC_IP"].c_str(),atoi(CL_ResultSet[0]["SMSC_PORT"].c_str()),CL_ResultSet[0]["SMSC_IP"].c_str(),atoi(CL_ResultSet[0]["SMSC_PORT"].c_str()));
				
			}
		}
	}
	return false;
}

void CSmpp::mcfnS_processReceivedMsgs(void *p){
	DBG_INFO((CG_EventLog), ("Started processReceivedMsgs Thread"));
	CSmpp *pCL_Obj=(CSmpp*)p;
	while(1){
		try{
			DeliverToEsme *pcL_Msg=NULL;
			if(!CG_ReceiverQ.mcfb_getNextMessage(pcL_Msg,true)){
				sleep(1);

			}
			else{
				if(pCL_Obj->mcfn_createSmppMessage(pcL_Msg)==false){
					CG_ReceiverQ.mcfb_insertIntoQue(pcL_Msg);
				}
				else{
					//delete pcL_Msg;
				}	
			}
		}
		catch(...){
		}
	}
	DBG_ERROR((CG_EventLog), ("Exiting From  processReceivedMsgs Thread"));
}
void CSmpp::mcfnS_sendSmppMsgsToSmsc(void *p){
	DBG_INFO((CG_EventLog), ("Started sendSmppMsgsToSmsc Thread"));
	CSmpp *pCL_Obj=(CSmpp*)p;
	int iL_MsgSubmitCounter=0;
	timeval SL_StartTime;
	timeval SL_PresentTime;
	while(1){
		try{
			if(iL_MsgSubmitCounter==0)
				gettimeofday(&SL_StartTime,NULL);
			MsgTypes *pcL_Msg=NULL;
			if(!CG_SmppQue.mcfb_getNextMessage(pcL_Msg,true)){
				sleep(1);

			}
			else{
				int iL_seqNum=0;
				if(pCL_Obj->mcC_SmppClient.mcfn_sendSubmitSm(*pcL_Msg->pmcC_SmppMsg,iL_seqNum)==false){
					CG_SmppQue.mcfb_insertIntoQue(pcL_Msg);
				}
				else{
					//TODO ::need to insert seq Number and message object to get submit-sm-resp
					pcL_Msg->mcsi_seqNum=iL_seqNum;
					CG_seqMap.mcfb_insert(iL_seqNum,pcL_Msg);
					CTimerManeger::mcfnS_GetInstance()->mcfn_registerTimer((ITimer*)pcL_Msg,60,NULL);//TODO::need to take time  from configurations
					DBG_VERBOSE((CG_EventLog),("Message Sent to SMSC:%d",iL_seqNum));
					++iL_MsgSubmitCounter;
					DBG_INFO((CG_EventLog),("[%d]TPS:%d",pCL_Obj->mcsi_MsgPerSec,iL_MsgSubmitCounter));	
					if(iL_MsgSubmitCounter>=pCL_Obj->mcsi_MsgPerSec){
						gettimeofday(&SL_PresentTime, NULL);
						int slL_startTime=SL_StartTime.tv_sec*1000000+SL_StartTime.tv_usec;
						int slL_presentTime=SL_PresentTime.tv_sec*1000000+SL_PresentTime.tv_usec;
						int siL_usleepTime = slL_presentTime-slL_startTime;
						if(siL_usleepTime>=1000000){
							DBG_VERBOSE((CG_EventLog), ("TPS crossed a second :%d",siL_usleepTime));
							if(siL_usleepTime>2000000){
								sleep(1);
							}
						}else{
							DBG_VERBOSE((CG_EventLog),("[%d]Going To Sleep:%d",iL_MsgSubmitCounter,siL_usleepTime));
							if(iL_MsgSubmitCounter>0 && iL_MsgSubmitCounter<=1000000)
							{
								usleep(iL_MsgSubmitCounter);
							}
						}
						iL_MsgSubmitCounter=0;
					}
				}
			}
		}
		catch(...){
		}
	}
	DBG_ERROR((CG_EventLog), ("Exiting From  sendSmppMsgsToSmsc Thread"));

}
bool CSmpp::mcfn_createSmppMessage(DeliverToEsme *pCL_Msg){
	
	Smpp::SubmitSm *pCL_submitSm=new Smpp::SubmitSm();
	pCL_submitSm->source_addr(Smpp::Address(pCL_Msg->oa()));	
	pCL_submitSm->destination_addr(Smpp::SmeAddress(Smpp::Ton(Smpp::Ton::International),Smpp::Npi(Smpp::Npi::E164),Smpp::Address(pCL_Msg->da())));
	pCL_submitSm->registered_delivery(pCL_Msg->reg_del());
	pCL_submitSm->protocol_id((Smpp::Uint8)pCL_Msg->pid());
	pCL_submitSm->data_coding((Smpp::Uint8)pCL_Msg->dcs());
	//TODO::here need to segment the messages if it is more than 160 chars
	pCL_submitSm->short_message(reinterpret_cast<const Smpp::Uint8*>(pCL_Msg->msg().data()),pCL_Msg->msg().length());
	MsgTypes *pCL_MsgObj=new MsgTypes();
	pCL_MsgObj->pmcC_EsmeMsg=pCL_Msg;
	pCL_MsgObj->pmcC_SmppMsg=pCL_submitSm;
	CG_SmppQue.mcfb_insertIntoQue(pCL_MsgObj);	
	return true;
}
