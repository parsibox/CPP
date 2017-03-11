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
		mcfn_getInt("HTTP","TIME_OUT",mesi_ussdTimeout);
		mcfn_getInt("HTTP","HTTP_PORT",mesi_httpPort);
		mcfn_getInt("HTTP","ESME_PORT",mesi_esmePort);
		mcfn_getInt("HTTP","ESME_SEC_PORT",mesi_esmeSecPort);
 
		mcfn_getString("HTTP","SSL_FILE",pmesc_httpSecurFile,false);
		mcfn_getString("HTTP","ESME_IP",pmesc_esmeIp);
		mcfn_getString("HTTP","ESME_SEC_IP",pmesc_esmeSecIp);
		return true;
	}
	 catch(STSGenException e){
		printf("%s\n",e.reasonString());	
	}	
	return false;
}
