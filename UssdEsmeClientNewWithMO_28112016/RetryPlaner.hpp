#ifndef __RETRYPLANNER__
#define __RETRYPLANNER__

#include<MsgMap.h>
#include<vector>
#include<string>
#include"TimerManeger.h"
#include"DeliverToEsme.pb.h"
#include"STSMsgQue.h"
#include"Thread.h"

class CRetryProfile
{

	private:
		
		CMsgMap<int,std::vector<int> > meC_planProfile;

	public:
		CRetryProfile();
		~CRetryProfile();
		bool mcfn_loadPlansConfiguration();	
		bool mcfn_getRetryInterval(std::string CL_intervalRation,int iL_interval,std::vector<int>& CL_rationVec);	
		bool mcfn_getPlanTimer(int iL_planId,int iL_retryCount,int &iL_nextScheduleTime);

};

typedef struct _planData 
{
	public:
		std::string mcC_username;
		int 	    mcsi_errorCode;
		int 	    mcsi_msgType;
		int 	    mcsi_planId;
		int 	    mcsi_retryValue;
		int         mcsi_setCount;
}planData;

class CRetryMsgTypes: public ITimer
{
	public:
		DeliverToEsme *pmcC_EsmeMsg;
		int 	mesi_nextTimeSchedule;	
	public:
		CRetryMsgTypes(int iL_nextSchedule){
			pmcC_EsmeMsg=NULL;
			mesi_nextTimeSchedule = iL_nextSchedule;
		}
		~CRetryMsgTypes(){
		}
		void mcfn_onTimerExpiry(void *p);

} ;

class CRetryPlanner : public CThread
{
	private:
		CCriticalSection meC_retryLocker;
		CRetryProfile 		*pmeC_retryProfile;
		std::vector< planData > meC_retryPlanner;
		CMsgQue<CRetryMsgTypes*> meC_retryMsgQue;	
	public:
		CRetryPlanner();
		~CRetryPlanner();
		bool mcfn_run(int,void *);
		bool mcfn_loadConfiguration();	
		bool mcfn_loadPlannerConfigurations();	
		bool mcfn_getCheckForRetry(DeliverToEsme *pcL_data);
		bool mcfn_gerPlanId(DeliverToEsme *pcL_data,int &iL_planId);
		std::string mcfn_getThreadName(void*)
		{
			return "RetryThread";
		}

};
#endif
