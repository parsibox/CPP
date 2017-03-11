#ifndef EXTERNS_H
#define EXTERNS_H


#include"DeliverToEsme.pb.h"
#include"STSMsgQue.h"
#include"EventLog.h"
#include"TimerManeger.h"
#include"EsmeClient.hpp"
#include"MenuTree.h"
#include"Configuration.h"
#include"CdrClient.hpp"
#include"Types.hpp"
class MsgTypes;
class CSmpp;

extern CEventLog CG_EventLog;
extern CConfiguration CG_Cfg;
extern CCdrClient CG_CdrClient;
extern CMsgQue<DeliverToEsme*> CG_ReceiverQ;
extern  CMsgMap<std::string,MsgTypes*> CG_seqMap;
extern CSmpp *pCG_Smpp;

//username ,<service_code,MenuTree>
extern  CMsgMap<std::string,CMsgMap<std::string,CMenuTree*>*> *pCG_menuMap;

#endif
