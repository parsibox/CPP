#ifndef EXTERNS_H
#define EXTERNS_H


#include"DeliverToEsme.pb.h"
#include"STSMsgQue.h"
#include"EventLog.h"
#include"TimerManeger.h"
#include"smpp.hpp"


class MsgTypes;
class CSmpp;

extern CEventLog CG_EventLog;
extern CMsgQue<DeliverToEsme*> CG_ReceiverQ;
extern  CMsgMap<int,MsgTypes*> CG_seqMap;
extern CSmpp *pCG_Smpp;

class MsgTypes:ITimer{
	public:
	int mcsi_seqNum;
	DeliverToEsme *pmcC_EsmeMsg;	
	Smpp::SubmitSm *pmcC_SmppMsg;
	public:
	MsgTypes(){
		mcsi_seqNum=0;
		pmcC_EsmeMsg=NULL;
		pmcC_SmppMsg=NULL;
	}
	~MsgTypes(){
		delete pmcC_EsmeMsg;
		delete pmcC_SmppMsg;
	}
	void mcfn_onTimerExpiry(void *p){
		CG_seqMap.mcfb_removeElement(mcsi_seqNum);	
		//todo::need to insert into CDR que here
	}

} ;

#endif
