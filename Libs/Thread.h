#ifndef __THREAD_H_
#define __THREAD_H_
#include<vector>
#include<STSOSUtil.h>
#include"signalHandler.h"
class CThread{
	private:
		std::vector<THREAD_HANDLE> meC_threadHandles;
		int mesi_numberOfThreadsStarted;
	public:
		CThread();
		~CThread();
		int mcfn_start(void *p=NULL);
		bool mcfn_stop(int =0);
		
		void mcfn_threadFunction(void *);
		
		virtual bool  mcfn_run(int,void*)=0;
		virtual std::string mcfn_getThreadName(void*)=0;
};
#endif
