/*
 * DBPicker.h
 *
 *  Created on: Apr 14, 2014
 *      Author: surya
 */

#ifndef DBPICKER_H_
#define DBPICKER_H_
#include"DBOperations.h"
#include"STSMsgQue.h"
#include"Publisher.h"
class CDBPicker{

	private:
		int mesi_workerThreadCounter;
		CClientConnect *pmeC_Connections;
	private:
		bool mefn_updateTaskStaus(int,int,int,std::string,std::string);	
	public:
		CDBPicker(int,int);
		~CDBPicker();
		bool mcfn_loadEsmeDetails(int iL_modId);
                bool mcfn_setTaskByServiceName(std::string CL_serviceName,DeliverToEsme *pCL_packet);
};
#endif
