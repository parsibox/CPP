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
		mcfn_getInt("HTTP","MODULE_ID",mesi_moduleId);
		mcfn_getInt("HTTP","LOG_LEVEL",mesi_logLevel);
		mcfn_getInt("HTTP","LOG_INTERVEL",mesi_logFileChangeInterval);
		mcfn_getInt("HTTP","NUM_OF_WORKER_THREADS",mesi_numOfWorkerThreads);
		mcfn_getString("HTTP","LOG_PATH",pmesc_logPath);
		mcfn_getString("HTTP","DB_IP",pmesc_dbIp);
		mcfn_getString("HTTP","DB_USERNAME",pmesc_dbUserName);
		mcfn_getString("HTTP","DB_PASSWORD",pmesc_dbPassword);
		mcfn_getString("HTTP","DB_NAME",pmesc_dbName);
		mcfn_getInt("HTTP","HTTP_PORT",mesi_httpPort);
		mcfn_getString("HTTP","SSL_FILE_NAME",pmesc_sslFileName,false);
		
		return true;
	}
	 catch(STSGenException e){
		printf("%s\n",e.reasonString());	
	}	
	return false;
}
