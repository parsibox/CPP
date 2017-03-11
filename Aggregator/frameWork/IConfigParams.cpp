#include"IConfigParams.hpp"
#include <stdlib.h>
#include <stdio.h>
#include"FrameWorkExterns.hpp"
namespace FrameWork
{

	IConfiguration::IConfiguration( std::string CL_moduleTag)
        {
                pCG_FrameWorkCfgObj=this;
		mdC_moduleTag = CL_moduleTag;
		if(mcfn_loadConfigurations()==false){
			exit(1);
		}
	}
	bool IConfiguration::mcfn_loadConfigurations(){
		try{
			mcfn_setFileName("System.ini");
			mcfn_getInt(LABLE_NAME,"MODULE_ID",mesi_moduleId);
			mcfn_getInt(LABLE_NAME,"LOG_LEVEL",mesi_logLevel);
			mcfn_getInt(LABLE_NAME,"LOG_INTERVEL",mesi_logFileChangeInterval);
			mcfn_getString(LABLE_NAME,"LOG_PATH",pmesc_logPath);
			mcfn_getInt(LABLE_NAME,"NUM_OF_WORKER_THREADS",mesi_numOfWorkerThreads);
			return true;
		}
		catch(STSGenException e){
			printf("%s\n",e.reasonString());	
		}	
		return false;
	}

	IConfiguration *pCG_FrameWorkCfgObj = NULL;
}
