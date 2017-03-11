#include"Configuration.h"
 #include <stdlib.h>
 #include <stdio.h>

CConfiguration::CConfiguration(){
	if(mcfn_loadConfigurations()==false){
		exit(1);
	}
}
bool CConfiguration::mcfn_loadConfigurations(){
	try{
		mcfn_setFileName("System.ini");
		mcfn_getInt("PUBLISHER","MODULE_ID",mesi_moduleId);
		mcfn_getInt("PUBLISHER","LOG_LEVEL",mesi_logLevel);
		mcfn_getInt("PUBLISHER","LOG_INTERVEL",mesi_logFileChangeInterval);
		mcfn_getInt("PUBLISHER","NUM_OF_WORKER_THREADS",mesi_numOfWorkerThreads);
		mcfn_getString("PUBLISHER","LOG_PATH",pmesc_logPath);
		mcfn_getString("PUBLISHER","DB_IP",pmesc_dbIp);
		mcfn_getString("PUBLISHER","DB_USERNAME",pmesc_dbUserName);
		mcfn_getString("PUBLISHER","DB_PASSWORD",pmesc_dbPassword);
		mcfn_getString("PUBLISHER","DB_NAME",pmesc_dbName);
		mcfn_getInt("PUBLISHER","CHANNEL_ID",mesi_channelId);
		mcfn_getInt("PUBLISHER","TASKPOOL_INTERVAL",mesi_taskPoolInterval);
		mcfn_getInt("PUBLISHER","TASK_ASSIGNMENT_INTERVAL",mesi_taskAssignmentInterval);
		mcfn_getInt("PUBLISHER","SELECT_MSG_LIMIT",mesi_bunchSize);
		mcfn_getChar("PUBLISHER","ISONLINE",mesc_isOnline);
		
		return true;
	}
	 catch(STSGenException e){
		printf("%s\n",e.reasonString());	
	}	
	return false;
}
