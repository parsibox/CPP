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
		mcfn_getInt("USSDCLIENT","MODULE_ID",mesi_moduleId);
		mcfn_getInt("USSDCLIENT","LOG_LEVEL",mesi_logLevel);
		mcfn_getInt("USSDCLIENT","LOG_INTERVEL",mesi_logFileChangeInterval);
		mcfn_getInt("USSDCLIENT","NUM_OF_WORKER_THREADS",mesi_numOfWorkerThreads);
		mcfn_getString("USSDCLIENT","LOG_PATH",pmesc_logPath);
		mcfn_getString("USSDCLIENT","DB_IP",pmesc_dbIp);
		mcfn_getString("USSDCLIENT","DB_USERNAME",pmesc_dbUserName);
		mcfn_getString("USSDCLIENT","DB_PASSWORD",pmesc_dbPassword);
		mcfn_getString("USSDCLIENT","DB_NAME",pmesc_dbName);
		mcfn_getInt("USSDCLIENT","TIME_OUT",mesi_ussdTimeout);
		mcfn_getString("USSDCLIENT","CDR_IP",pmesc_CdrIp);
		mcfn_getInt("USSDCLIENT","CDR_PORT",mesi_cdrPort);
		 mcfn_getString("USSDCLIENT","CDR_SEC_IP",pmesc_CdrSecIp);
                mcfn_getInt("USSDCLIENT","CDR_SEC_PORT",mesi_cdrSecPort);

		
		return true;
	}
	 catch(STSGenException e){
		printf("%s\n",e.reasonString());	
	}	
	return false;
}
