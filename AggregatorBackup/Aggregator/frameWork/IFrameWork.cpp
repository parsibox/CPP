#include<IFrameWork.hpp>
#include"FrameWorkExterns.hpp"
#include"signalHandler.h"
#include<EventLog.h>
extern CEventLog CG_EventLog;

namespace FrameWork
{
IFrameWork::IFrameWork(std::string moduleName,std::string version,std::string relDate)
{
	meC_moduleName = moduleName;
	meC_version = version;
        meC_releaseDate = relDate;   
        meb_appicationStatus = true; 
	pCG_FrameWorkObj = this;
}

void IFrameWork::mcfn_handleApplication()
{
	//CSignalHandler CL_SigHandler;
	while(meb_appicationStatus)
	{
		try{ 
			sleep(10);
		}
		catch(const CSignalException& e){
			DBG_ERROR((CG_EventLog),("Exception:%s",e.what()));
		}
		catch(...){
			DBG_ERROR((CG_EventLog),("Some unknown Exception came in main"));
		}
	}
	mcfn_clearApplication();
}

void IFrameWork::mcfn_printBanner(bool isEventLogObjCreated)
{
	printf("******************************************************************\n");
	printf("NOBSUR- (All rights reserved)\n");
	printf(" %s--Version: %s\n", meC_moduleName.c_str(),meC_version.c_str());
	printf("Release date :%s                                        \n",meC_releaseDate.c_str());
	printf("******************************************************************\n");
	if(isEventLogObjCreated)
	{
		DBG_INFO((CG_EventLog),("Version:%s",meC_version.c_str()));
		DBG_INFO((CG_EventLog),("Release Date:%s",meC_releaseDate.c_str())); 
	}
}

    IFrameWork* pCG_FrameWorkObj = NULL ;
}
