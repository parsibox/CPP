#include"SMPP.hpp"
#include"Externs.h"
CMsgQue<MsgTypes*> CG_SmppQue;

void MsgTypes::mcfn_onTimerExpiry(void *p){
	//we may need to retry here
	Smpp::SubmitSm CL_pdu;	
	CL_pdu.destination_addr(
			Smpp::SmeAddress(Smpp::Ton(Smpp::Ton::International),
				Smpp::Npi(Smpp::Npi::E164),
				Smpp::Address(mcC_daStr)));
	std::string msgtext="Internal Timeout No responce from subscriber";
	CL_pdu.short_message(
                reinterpret_cast<const Smpp::Uint8*>(msgtext.data()),
                msgtext.length());
         CL_pdu.command_status(1034);
	pCG_Smpp->mcC_SmppClient.mcfn_onSubmitSmResp((u8*)CL_pdu.encode(),CL_pdu.command_length());
}

CSmpp::CSmpp(int iL_modId){	
	if(mcfn_loadConfigurations(iL_modId)){
		DBG_INFO((CG_EventLog), ("Loaded initial configuration sucessfully:%d",iL_modId));	
	}
	else
	{
		DBG_ERROR((CG_EventLog), ("Loaded initial configuration Failed:%d",iL_modId));	
		exit(1);
	}	
	int iL_threadId[3]={1,2,3};
	mcfn_start(&iL_threadId[0]);
	mcfn_start(&iL_threadId[1]);

}

bool CSmpp::mcfn_loadConfigurations(int iL_modId){
	return mcfn_loadSmscDetails(mcfn_loadEsmeDetails(iL_modId));
}

bool CSmpp::mcfn_run(int iL_threadId,void *vL_arg){
	  switch(*(int*)vL_arg){
                case 1:
                        return mcfn_processReceivedMsgs(NULL);
                case 2:
                        return mcfn_sendSmppMsgsToSmsc(NULL);
        }

}

std::string CSmpp::mcfn_getThreadName(void* vL_arg)
{
	  switch(*(int*)vL_arg){
                case 1:
                        return "ProcessReceivedMsgsThread";
                case 2:
                        return "SendMsgToSmscThread";
        }
}

int CSmpp::mcfn_loadEsmeDetails(int iL_modId){
	mysqlpp::StoreQueryResult CL_ResultSet;
	CDBOperations CL_dbObj;
	 std::stringstream CL_QueryStream;
        CL_QueryStream.str("");
        CL_QueryStream<<"SELECT * from esme_details WHERE ESME_ID="<<iL_modId <<" limit 1";
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		if(CL_ResultSet.num_rows()>0){
			//start the receiver to receive messages from publisher
			pmcC_Receiver=new CReceiver(atoi(CL_ResultSet[0]["esme_port"].c_str()),atoi(CL_ResultSet[0]["throwttel_qsize"].c_str()));
			return atoi(CL_ResultSet[0]["smsc_id"].c_str());
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
		CL_QueryStream<<"SELECT * from smsc_details WHERE SMSC_ID="<<iL_smscId <<" limit 1";
		if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
			if(CL_ResultSet.num_rows()>0){
				mcC_SmppClient.mcfn_initilise(CL_ResultSet[0]["user_name"].c_str(),CL_ResultSet[0]["password"].c_str(),CL_ResultSet[0]["sys_type"].c_str(),atoi(CL_ResultSet[0]["interface_version"].c_str()));
				mcsi_MsgPerSec=atoi(CL_ResultSet[0]["msg_per_sec"].c_str());
				 return mcC_SmppClient.mcfn_addTcpDetails(CL_ResultSet[0]["smsc_ip"].c_str(),atoi(CL_ResultSet[0]["smsc_port"].c_str()),CL_ResultSet[0]["smsc_ip"].c_str(),atoi(CL_ResultSet[0]["smsc_port"].c_str()));
				
			}
		}
	}
	return false;
}

bool CSmpp::mcfn_processReceivedMsgs(void *p){
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
	return true;
}
bool  CSmpp::mcfn_sendSmppMsgsToSmsc(void *p){
	DBG_INFO((CG_EventLog), ("Started sendSmppMsgsToSmsc Thread"));
	int iL_MsgSubmitCounter=0;
	int iL_heartBeatcounter=0;
	timeval SL_StartTime;
	timeval SL_PresentTime;
	while(1){
		try{
			if(iL_MsgSubmitCounter==0)
				gettimeofday(&SL_StartTime,NULL);
			MsgTypes *pcL_Msg=NULL;
			if(!CG_SmppQue.mcfb_getNextMessage(pcL_Msg,false)){
				sleep(1);
				if(mcC_SmppClient.mcfn_getConnectionStatus()==CONNECTED)
				{
					++iL_heartBeatcounter;
					if(iL_heartBeatcounter > CG_Cfg.mcfn_getHeartBeatInterval())
					{
						if(!mcC_SmppClient.mcfn_sendShake())
						{
							mcC_SmppClient.mcfn_close(); 
						}
						iL_heartBeatcounter=0;	 
					}
				}
			}
			else{
				iL_heartBeatcounter=0;
				u8 u8L_dcs=(u8)pcL_Msg->pmcC_EsmeMsg->dcs();
				std::string CL_msg=pcL_Msg->pmcC_EsmeMsg->msg().c_str();
				pcL_Msg->pmcC_EsmeMsg->set_srv_code(pcL_Msg->pmcC_EsmeMsg->oa());
				int iL_sessionId=mcC_SmppClient.mcfn_getSeqNumber();
				DBG_VERBOSE((CG_EventLog),("Menu Sending to SMSC:%s",CL_msg.c_str()));	
				pcL_Msg->pmcC_EsmeMsg->set_submit_time(time(NULL));
				pcL_Msg->pmcC_EsmeMsg->set_submit_msgid(iL_sessionId);
				pcL_Msg->pmcC_EsmeMsg->set_sender_sessionid(iL_sessionId);
				pcL_Msg->meC_traversolPath.assign(pcL_Msg->pmcC_EsmeMsg->srv_code().c_str());
				if(mcC_SmppClient.mcfn_sendSubmitSm(iL_sessionId,pcL_Msg->pmcC_EsmeMsg->da().c_str(),pcL_Msg->pmcC_EsmeMsg->oa().c_str(),u8L_dcs,CL_msg.c_str(),pcL_Msg->pmcC_EsmeMsg->reg_del())==false){	
					DBG_CRITICAL((CG_EventLog),("Send failed to SMSC"));
					sleep(1);	
					CG_SmppQue.mcfb_insertIntoQue(pcL_Msg);
				}
				else{
					pcL_Msg->mcC_daStr.assign(pcL_Msg->pmcC_EsmeMsg->da());
					CG_seqMap.mcfb_insert(iL_sessionId,pcL_Msg);
					CTimerManeger::mcfnS_GetInstance()->mcfn_registerTimer((ITimer*)pcL_Msg,CG_Cfg.mcfn_getUssdTimeout(),NULL);
					DBG_VERBOSE((CG_EventLog),("Message Sent to SMSC:%d",iL_sessionId));
					++iL_MsgSubmitCounter;
					DBG_INFO((CG_EventLog),("[%d]TPS:%d",mcsi_MsgPerSec,iL_MsgSubmitCounter));	
					if(iL_MsgSubmitCounter>=mcsi_MsgPerSec){
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
	return true;
}
bool CSmpp::mcfn_createSmppMessage(DeliverToEsme *pCL_Msg){
	MsgTypes *pCL_MsgObj=new MsgTypes();
	pCL_MsgObj->pmcC_EsmeMsg=pCL_Msg;
	pCL_MsgObj->pmcC_Session=new Session();
	CG_SmppQue.mcfb_insertIntoQue(pCL_MsgObj);	
	return true;
}
