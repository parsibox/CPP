#ifndef EXTERNS_H
#define EXTERNS_H


#include"STSMsgQue.h"
#include"MsgMap.h"
#include"EventLog.h"
#include"Configuration.h"
#include<UssdData.pb.h>

extern CEventLog CG_EventLog;
extern CConfiguration CG_Cfg;
extern CMsgQue<UssdData*> CG_ReceiverQ;
extern CMsgMap<int,short> CG_RouterMap;


#endif
