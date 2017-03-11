/*
 * DBPicker.h
 *
 *  Created on: Apr 14, 2014
 *      Author: surya
 */

#ifndef DBPICKER_H_
#define DBPICKER_H_
#include"DBOperations.h"
#include"Thread.h"
#include"Task.h"
#include"STSMsgQue.h"
#include"Publisher.h"
class CDBPicker:public CThread{

	private:
		int mesi_workerThreadCounter;
		CCriticalSection meC_taskListLock;
		CClientConnect *pmeC_Connections;
		std::list <CTask*> meC_TaskList;	
		CMsgQue <CTask*> meC_workerQue;	
	private:
		bool mefn_addTaskToList(CTask* );
		bool mefn_updateTaskStaus(int,int);	
		bool mefn_updateTaskStaus(int,int,int,std::string,std::string);	
	public:
		CDBPicker(int,int);
		~CDBPicker();
		//virtual function
		bool mcfn_run(int iL_threadId,void *vL_arg);
		std::string mcfn_getThreadName(void*);
		
		bool mcfn_taskSelectionThread();
		bool mcfn_taskAssignmentThread();
		bool mcfn_workerThread(int iL_threadId);
		bool mcfn_loadEsmeDetails(int iL_modId);
};
#endif
