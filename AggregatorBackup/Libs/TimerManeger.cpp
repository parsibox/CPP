/*
 * TimerManeger.cpp
 *
 *  Created on: Dec 18, 2013
 *      Author: surya
 */
#include<typeinfo>
#include"TimerManeger.h"
#include "EventLog.h"
extern CEventLog CG_EventLog;

CTimerManeger* CTimerManeger::pmeC_Instance=NULL;
static void* fnG_Wrapper_TimerCheckManeger(void *pvL_This) {
        try {
                (static_cast<CTimerManeger*> (pvL_This))->mcfn_checkTimerThread();
                DBG_CRITICAL((CG_EventLog),("Thread Execution stoped which is checking all file Descriptors"));
        } catch (std::bad_cast x) {
                DBG_ERROR((CG_EventLog), ("Unable to start mcfn_checkTimerThread"));
        }
        return NULL;
}
CTimerManeger* CTimerManeger::mcfnS_GetInstance(){
	 if(pmeC_Instance == NULL){
                DBG_INFO((CG_EventLog),("Creating PoolManager object..."));
                pmeC_Instance = new CTimerManeger();
        }	
	return pmeC_Instance;
}
CTimerManeger::CTimerManeger(){
	 CSTSOSUtil::mcfn_beginAndDetachThread(meC_ThreadHandle,fnG_Wrapper_TimerCheckManeger, this);
	 usleep(1000);
}

CTimerManeger::~CTimerManeger(){
	if(meC_ThreadHandle)
		pthread_cancel(meC_ThreadHandle);
}

time_t CTimerManeger::mcfn_registerTimer(ITimer *pcL_timerObj,int iL_timeOut,void *pvL_event){
	CAutoLocker CL_Lock(meC_timerLoker);
	CMsgMap<ITimer *,void *> *pCL_timerEventMap;
	time_t SL_timer=time(NULL)+iL_timeOut;	
	if(meC_TimerMap.mcfb_findElement(SL_timer,pCL_timerEventMap)){
		DBG_DEBUG((CG_EventLog),("Timer already there so adding to that:%ld",SL_timer));
		pCL_timerEventMap->mcfb_insert(pcL_timerObj,pvL_event);
	}
	else{
		pCL_timerEventMap=new CMsgMap<ITimer*,void *>();
		pCL_timerEventMap->mcfb_insert(pcL_timerObj,pvL_event);
		meC_TimerMap.mcfb_insert(SL_timer,pCL_timerEventMap);
		DBG_DEBUG((CG_EventLog),("Created and  adding to that:%ld",SL_timer));
	}
	pcL_timerObj->mcsi_timerTime=SL_timer;
	return SL_timer;
}

bool CTimerManeger::mcfn_unregisterTimer(ITimer *pCL_timerObj,time_t SL_timer){
	CAutoLocker CL_Lock(meC_timerLoker);
	CMsgMap<ITimer *,void *> *pCL_timerEventMap;
	if(SL_timer==0)SL_timer=pCL_timerObj->mcsi_timerTime;
	DBG_DEBUG((CG_EventLog),("Unregister Call came with :%ld,%p",pCL_timerObj->mcsi_timerTime,pCL_timerObj));
	if(meC_TimerMap.mcfb_findElement(SL_timer,pCL_timerEventMap)){
		pCL_timerEventMap->mcfb_removeElement(pCL_timerObj);
		if(pCL_timerEventMap->mcfb_isEmpty())
			meC_TimerMap.mcfb_removeElement(SL_timer);
		DBG_DEBUG((CG_EventLog),("Created and  adding to that:%ld,%p",SL_timer,pCL_timerObj));
	}
	return true;
}

void CTimerManeger::mcfn_checkTimerThread(){
	DBG_VERBOSE((CG_EventLog),("Started Timer Check Thread"));
	time_t siL_CurrentTime=0;
	time_t siL_ConfiguredTime=0;
	CMsgMap<ITimer *,void *> *pCL_timerEventMap=NULL;
	ITimer *pCL_timerObj=NULL;
	void *pvL_Events;
	int siL_TimeDiff=0;
	while(1){
		try{	
			meC_timerLoker.Lock();;
			time(&siL_CurrentTime);
			meC_TimerMap.mcf_startGet();
			if(meC_TimerMap.mcfb_getNextElement(siL_ConfiguredTime,pCL_timerEventMap)){

				meC_TimerMap.mcf_resetItr();
				siL_TimeDiff=siL_ConfiguredTime-siL_CurrentTime;
				DBG_DEBUG((CG_EventLog),("Time to get Evetn:%ld,%d",siL_TimeDiff,pCL_timerEventMap->mcfi_getCount()));	
				if(siL_TimeDiff>0)
				{
					meC_timerLoker.Unlock();
					sleep(1);
					continue;
				}
				meC_TimerMap.mcfb_removeElement(siL_ConfiguredTime);
				pCL_timerEventMap->mcf_startGet();
				while(pCL_timerEventMap->mcfb_getNextElement(pCL_timerObj,pvL_Events)){
					pCL_timerObj->mcfn_onTimerExpiry(pvL_Events);
					DBG_DEBUG((CG_EventLog),("Calling on Timer expiry:%p",pCL_timerObj));
				}
				delete pCL_timerEventMap;
				pCL_timerEventMap=NULL;
				meC_timerLoker.Unlock();
			}
			else{
				meC_timerLoker.Unlock();
				DBG_DEBUG((CG_EventLog),("No Timer Event Are Registed With TimerManeger"));
				sleep(1);
			}
		}
		catch(...)
		{
			DBG_CRITICAL((CG_EventLog),("Unknown Exception came"));
		}
	}
}
