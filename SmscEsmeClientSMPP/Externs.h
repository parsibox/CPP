#ifndef EXTERNS_H
#define EXTERNS_H


#include"DeliverToEsme.pb.h"
#include"STSMsgQue.h"
#include"EventLog.h"
#include"TimerManeger.h"
#include"Configuration.h"
#include"CdrClient.hpp"
#include"Types.hpp"
class MsgTypes;
class CSmpp;

extern CEventLog CG_EventLog;
extern CConfiguration CG_Cfg;
extern CCdrClient *pCG_CdrClient;
extern CMsgQue<DeliverToEsme*> CG_ReceiverQ;
extern  CMsgMap<int,MsgTypes*> CG_seqMap;
extern CSmpp *pCG_Smpp;

#endif
