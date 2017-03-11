/*
 * TimerManeger.h
 *
 *  Created on: Dec 18, 2013
 *      Author: surya
 */

#ifndef TIMERMANEGER_H_
#define TIMERMANEGER_H_
#include<MsgMap.h>
#include<STSOSUtil.h>
#include"ITimer.h"
#include"AutoLocker.h"

class CTimerManeger
{
	private:
		THREAD_HANDLE meC_ThreadHandle;
		CCriticalSection meC_timerLoker;
		CMsgMap <time_t,CMsgMap<ITimer*,void*>*> meC_TimerMap;
		static CTimerManeger *pmeC_Instance;
		CTimerManeger();
		~CTimerManeger();

	public:
		static CTimerManeger* mcfnS_GetInstance();
		time_t mcfn_registerTimer(ITimer *,int,void*);
		bool mcfn_unregisterTimer(ITimer *,time_t=0);
		void mcfn_checkTimerThread();
};
#endif
