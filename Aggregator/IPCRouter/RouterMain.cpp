#include<EventLog.h>
#include<STSOSUtil.h>

#include<Server.hpp>

#include"Configuration.h"
#define REL_VER "V1.0"
#define RELEASE_DATE "14-Apr-2014"

CEventLog CG_EventLog;
CConfiguration CG_Cfg;

CServer *pCG_Server=NULL;
CMsgQue<IPCMsg> CG_MsgQue;

void* fnG_processReceivedMessage(void* p)
{
	IPCMsg CL_msg;
	while(1)
	{	
		try{
			CL_msg.clear();
			if(CG_MsgQue.mcfb_getNextMessage(CL_msg,true))
			{
				if(false==pCG_Server->mcfn_sendMessage(CL_msg))
				{
					short siL_tmpSrc=CL_msg.hdr.src;
					CL_msg.hdr.src = CL_msg.hdr.dst;
					CL_msg.hdr.dst = siL_tmpSrc;
					CL_msg.hdr.status = 1001;

					pCG_Server->mcfn_sendMessage(CL_msg);
					DBG_ERROR((CG_EventLog),("Sent Failed so sending back to sender with error"));
				}
				else{
        				DBG_INFO((CG_EventLog),("Message Sent Successfully:%s",CL_msg.data.c_str()));
				}
			}

		}
		catch(...)
		{
		}
	}
}
static void fnG_PrintCompanyBanner(char* pscL_ModuleName,char* pscL_Version)
{
        printf("******************************************************************\n");
        printf("SmppClient- (All rights reserved)\n");
        printf(" %s--Version: %s\n", pscL_ModuleName,REL_VER);
        printf("Release date :%s                                        \n",RELEASE_DATE);
        printf("******************************************************************\n");
}


int main(int argc, char* argv[]) {
        if((argc == 2) && (strcasecmp(argv[1],"-v")==0))
        {
                fnG_PrintCompanyBanner("Publisher", REL_VER);
                return 0;
        }
        char pscL_Temp[40]={0x00};

        sprintf(pscL_Temp, "PUBLISHER%d",CG_Cfg.mcfn_getModuleId());

        CG_EventLog.mcfn_setAPPTag(pscL_Temp);
        CG_EventLog.mcfn_setFilePath((char*)CG_Cfg.mcfn_getLogPath());
        CG_EventLog.mcfn_setLogLevel(CG_Cfg.mcfn_getLogLevel());
        CG_EventLog.mcfn_setNewFileGenerationPeriod(CG_Cfg.mcfn_getLogChangeInterval());
        try {
                CG_EventLog.mcfn_openNewFile();
        } catch (STSGenException e) {
                printf("Unable to open file in path:%s",e.reasonString());
                exit(1);
        }

        DBG_INFO((CG_EventLog),("Version:%s",REL_VER));
        DBG_INFO((CG_EventLog),("Release Date:%s",RELEASE_DATE));

	pCG_Server = new CServer(CG_Cfg.mcfn_getServerPort(),&CG_MsgQue);
	CSTSOSUtil::mcfn_beginAndDetachThread(fnG_processReceivedMessage,NULL);	
	while(1)
	{
		sleep(60);
	}
}
