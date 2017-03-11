#include"Thread.h"
#include<EventLog.h>
#include<typeinfo>
#include <signal.h>
#include <mysql++.h>

extern CEventLog CG_EventLog;
typedef struct _SPARAM{
	CThread* pmcC_obj;
	void *pmcv_arg;
	_SPARAM(CThread* pCL_obj,void *pvL_arg){
		pmcC_obj=pCL_obj;
		pmcv_arg=pvL_arg;
	}
}SPARAM;

static FNVOID fnG_Wrapper_ThreadStarter(void *pvL_Param){
	try{
		SPARAM SL_param=*(static_cast<SPARAM*>(pvL_Param));
		SL_param.pmcC_obj->mcfn_threadFunction(SL_param.pmcv_arg);
	}catch(std::bad_cast x){
		DBG_CRITICAL((CG_EventLog), ("Bad cast while starting fnG_Wrapper_ThreadStarter"));
	}
	RETURN;
}

CThread::CThread(){
	meC_threadHandles.clear();
	mesi_numberOfThreadsStarted=0;
}

CThread::~CThread(){
	mcfn_stop();
}

int CThread::mcfn_start(void *p){
	
	//startd thread function here
	THREAD_HANDLE iL_threadHandler;
	SPARAM SL_param(this,p);
	CSTSOSUtil::mcfn_beginAndDetachThread(iL_threadHandler,fnG_Wrapper_ThreadStarter,&SL_param); 
	sleep(1);
	meC_threadHandles.push_back(iL_threadHandler);
	return mesi_numberOfThreadsStarted;
}

bool  CThread::mcfn_stop(int iL_threadId){
	//TODO::need to kill the trheads runned from this,pthread_cancle  logic is not working need to see other way 
	printf("killing Thread:%d\n",iL_threadId);
	if(iL_threadId<=0){
		for(int i=0;i<meC_threadHandles.size();i++){
			if(meC_threadHandles[i]){
				pthread_cancel(meC_threadHandles[i]);
			}
		}	
	}
	else if(iL_threadId<mesi_numberOfThreadsStarted){
		if(meC_threadHandles[iL_threadId-1]){
			pthread_cancel(meC_threadHandles[iL_threadId-1]);
		}
	}
	else{
		return false;
	}
	return true;
}

void CThread::mcfn_threadFunction(void *p){
	int iL_threadId=mesi_numberOfThreadsStarted++;
        std::string threadName = mcfn_getThreadName(p);
	DBG_CRITICAL((CG_EventLog),("Started %s Thread:%d",threadName.c_str(),iL_threadId));
	while(1){
		try{
			if(mcfn_run(iL_threadId,p)==false)
				break;
		}
		catch(const CSignalException& e){
			DBG_CRITICAL((CG_EventLog),("[%d]Exception came in %s Thread:%s",iL_threadId,threadName.c_str(),e.what()));
			sleep(1);
		}
		catch(const mysqlpp::Exception&  e){
			DBG_CRITICAL((CG_EventLog),("[%d]Exception came in %s Thread:%s",iL_threadId,threadName.c_str(),e.what()));
			sleep(1);
		}
		catch(...){
			//DBG_CRITICAL((CG_EventLog),("Unkown Exception came in %s Thread:%d" ,threadName.c_str(),iL_threadId));
 			break;
		}
	}
//	DBG_CRITICAL((CG_EventLog),("Exiting from %s Thread:%d",iL_threadId));
}
