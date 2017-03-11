#include"CdrServer.hpp"
#include"signalHandler.h"
#include"EventLog.h"
#include"WriteFile.h"
#include"sstream"
#include"Configuration.h"
#define REL_VER "V1.0"
#define RELEASE_DATE "05-Apr-2014"

CWriteFile CG_SubmitWriter;
CWriteFile CG_UssdSubmitWriter;
CWriteFile CG_DeliverWriter;
std::string CG_columnSep="|";
CEventLog CG_EventLog;
CMsgQue<CdrMsg*> CG_ReceiverQ;
CConfiguration CG_Cfg;

CCdrServer *pCG_CdrServer=NULL;
static void fnG_PrintCompanyBanner(char* pscL_ModuleName,char* pscL_Version)
{
        printf("******************************************************************\n");
        printf("SmppClient- (All rights reserved)\n");
        printf(" %s--Version: %s\n", pscL_ModuleName,REL_VER);
        printf("Release date :%s                                        \n",RELEASE_DATE);
        printf("******************************************************************\n");
}
void fnG_getFormatedDate(long siL_Value,char* pscL_Format, char* pscL_Date, int siL_size)
{
        struct tm tmp;
        localtime_r(&siL_Value,&tmp);
        if (strftime(pscL_Date, siL_size, pscL_Format, &tmp) == 0)
        {
                DBG_ERROR((CG_EventLog), ("Unable to get the time string"));
        }
        else
                DBG_VERBOSE((CG_EventLog), ("Timestamp String: %s", pscL_Date))
}


void* fnG_processCdrMsgsThread(void *p){
	
	DBG_INFO((CG_EventLog),("Started fnG_processCdrMsgsThread "));
	while(1){
		try{	CdrMsg *pCL_Msg=NULL;
			if(!CG_ReceiverQ.mcfb_getNextMessage(pCL_Msg,true)){
				sleep(1);
				continue;
			}
			if(pCL_Msg->msg_type()==CdrMsg::SUBMIT){
				 DeliverToEsme *pCL_SubMsg=pCL_Msg->mutable_submit_msg();
				//TODO::here need to genetate submit cdr
				std::stringstream CL_Msg;
				CL_Msg.str("");
				CL_Msg<<pCL_SubMsg->task_id()<<CG_columnSep<<pCL_SubMsg->msg_id()<<CG_columnSep<<pCL_SubMsg->oa()<<CG_columnSep<<pCL_SubMsg->da()
					<<CG_columnSep<<pCL_SubMsg->msg()<<CG_columnSep<<pCL_SubMsg->interface_id()<<CG_columnSep<<pCL_SubMsg->reg_del()<<CG_columnSep
					<<pCL_SubMsg->circle_id()<<CG_columnSep<<pCL_SubMsg->channel_id()<<CG_columnSep<<pCL_SubMsg->expiry_time()<<CG_columnSep
					<<pCL_SubMsg->pid()<<CG_columnSep<<pCL_SubMsg->dcs()<<CG_columnSep<<pCL_SubMsg->lang_id()<<CG_columnSep<<pCL_SubMsg->status()
					<<CG_columnSep<<pCL_SubMsg->submit_time()<<CG_columnSep<<pCL_SubMsg->submit_msgid()<<'\n';//<<CG_Cfg.mcfn_getLineSep();
				if(!CG_SubmitWriter.mcfn_AddStringToFile(CL_Msg.str(),pCL_SubMsg->submit_time())){
					CG_ReceiverQ.mcfb_insertIntoQue(pCL_Msg);
					continue;
				}
				DBG_VERBOSE((CG_EventLog),("Generated Submit CDR Sucessfully:%s",pCL_Msg->ShortDebugString().c_str()));
			}
			else if(pCL_Msg->msg_type()==CdrMsg::USSD_SUBMIT){
				DeliverToEsme *pCL_SubMsg=pCL_Msg->mutable_submit_msg();
				//TODO::here need to genetate submit cdr
				std::stringstream CL_Msg;
				char submitTime[40],endTime[40];
				fnG_getFormatedDate(pCL_SubMsg->submit_time(),"%Y-%m-%d %H:%M:%S",submitTime,40);
				fnG_getFormatedDate(pCL_SubMsg->session_endtime(),"%Y-%m-%d %H:%M:%S",endTime,40);
				CL_Msg.str("");
				CL_Msg<<pCL_SubMsg->task_id()<<CG_columnSep<<pCL_SubMsg->msg_id()<<CG_columnSep<<pCL_SubMsg->da()
					<<CG_columnSep<<pCL_SubMsg->msg()<<CG_columnSep<<pCL_SubMsg->sender_sessionid()<<CG_columnSep<<pCL_SubMsg->receiver_sessionid()
					<<CG_columnSep<<pCL_SubMsg->status()<<CG_columnSep<<pCL_SubMsg->charging_status()<<CG_columnSep<<submitTime
					<<CG_columnSep<<endTime<<CG_columnSep<<pCL_SubMsg->service_url()<<CG_columnSep
					<<pCL_SubMsg->service_url_status()<<CG_columnSep<<pCL_SubMsg->user_input()
					<<'\n';//<<CG_Cfg.mcfn_getLineSep();
				if(!CG_UssdSubmitWriter.mcfn_AddStringToFile(CL_Msg.str(),pCL_SubMsg->submit_time())){
					CG_ReceiverQ.mcfb_insertIntoQue(pCL_Msg);
					continue;
				}
				DBG_VERBOSE((CG_EventLog),("Generated Ussd Submit CDR Sucessfully:%s",pCL_Msg->ShortDebugString().c_str()));


			}
			else{
				//TODO ::here need to genearate deliver CDR
				std::stringstream CL_Msg;
				CL_Msg.str("");
				DeliverMsg *pCL_DlrMsg=pCL_Msg->mutable_deliver_msg();
				CL_Msg<<pCL_DlrMsg->submit_msgid()<<CG_columnSep<<pCL_DlrMsg->submit_time()<<CG_columnSep<<pCL_DlrMsg->status()<<CG_columnSep<<pCL_DlrMsg->delivery_time()<<CG_columnSep<<pCL_DlrMsg->error_code()<<CG_Cfg.mcfn_getLineSep();
				if(!CG_DeliverWriter.mcfn_AddStringToFile(CL_Msg.str(),pCL_DlrMsg->submit_time())){
					CG_ReceiverQ.mcfb_insertIntoQue(pCL_Msg);
					continue;
				}
				DBG_VERBOSE((CG_EventLog),("Generated Deliver CDR Sucessfully:%s",pCL_Msg->ShortDebugString().c_str()));
			}
			delete pCL_Msg;
		}
		catch(CSignalException e){
                        DBG_ERROR((CG_EventLog),("Exception:%s",e.what()));
                }
                catch(...){
                        DBG_ERROR((CG_EventLog),("Some unknown Exception came in main"));
                }
	}
	DBG_INFO((CG_EventLog),("Exiting From fnG_processCdrMsgsThread"));
}
void *fnG_checkFiletoChange(void *p){
	DBG_INFO((CG_EventLog),("Starting fnG_checkFiletoChange"));
	while(1){
		sleep(CG_Cfg.mcfn_getCdrFileChangeTime());
		CG_SubmitWriter.mcfn_checkCloseFilesByTimer();	
		CG_UssdSubmitWriter.mcfn_checkCloseFilesByTimer();	
		CG_DeliverWriter.mcfn_checkCloseFilesByTimer();	
	}
	DBG_INFO((CG_EventLog),("Exiting From fnG_checkFiletoChange"));
}

int main(int argc, char* argv[]) {
	if((argc == 2) && (strcasecmp(argv[1],"-v")==0))
	{
		fnG_PrintCompanyBanner("SmppClient", REL_VER);
		return 0;
	}
	//CSignalHandler CL_SigHandler;
	char pscL_Temp[40]={0x00};
	
	sprintf(pscL_Temp, "RECORDER%d",CG_Cfg.mcfn_getModuleId());
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
	pCG_CdrServer= new CCdrServer(CG_Cfg.mcfn_getCdrServerPort());
	CG_columnSep.assign(CG_Cfg.mcfn_getFieldSep());
	//TODO::need to init Write  file objects
	CG_SubmitWriter.mcfn_initialise(CG_Cfg.mcfn_getCdrFileGenPath(),"SUBMIT",".TXT",CG_Cfg.mcfn_getMaxNumOfLinesinFile(),CG_Cfg.mcfn_getCdrFileChangeTime(),101,CG_Cfg.mcfn_getPartsOfDay(),"",false);
	CG_UssdSubmitWriter.mcfn_initialise(CG_Cfg.mcfn_getCdrFileGenPath(),"USSDSUBMIT",".TXT",CG_Cfg.mcfn_getMaxNumOfLinesinFile(),CG_Cfg.mcfn_getCdrFileChangeTime(),101,CG_Cfg.mcfn_getPartsOfDay(),"",false);
	CG_DeliverWriter.mcfn_initialise(CG_Cfg.mcfn_getCdrFileGenPath(),"DELIVER",".TXT",CG_Cfg.mcfn_getMaxNumOfLinesinFile(),CG_Cfg.mcfn_getCdrFileChangeTime(),101,CG_Cfg.mcfn_getPartsOfDay(),"",false);
	CSTSOSUtil::mcfn_beginAndDetachThread(fnG_checkFiletoChange,NULL);	
	CSTSOSUtil::mcfn_beginAndDetachThread(fnG_processCdrMsgsThread,NULL);
		
	while(1){
		try{
			sleep(1);
		}
		catch(CSignalException e){
			DBG_ERROR((CG_EventLog),("Exception:%s",e.what()));
		}
		catch(...){
			DBG_ERROR((CG_EventLog),("Some unknown Exception came in main"));
		}

	}
	return 0;
}
