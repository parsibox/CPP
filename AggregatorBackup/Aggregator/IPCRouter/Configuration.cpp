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
		mcfn_getInt("IPCROUTER","MODULE_ID",mesi_moduleId);
		mcfn_getInt("IPCROUTER","LOG_LEVEL",mesi_logLevel);
		mcfn_getInt("IPCROUTER","LOG_INTERVEL",mesi_logFileChangeInterval);
		mcfn_getInt("IPCROUTER","NUM_OF_WORKER_THREADS",mesi_numOfWorkerThreads);
		mcfn_getString("IPCROUTER","LOG_PATH",pmesc_logPath);
		mcfn_getInt("IPCROUTER","SERVER_PORT",mesi_serverPort);
		
		return true;
	}
	 catch(STSGenException e){
		printf("%s\n",e.reasonString());	
	}	
	return false;
}
