/*
 * CdrMain.cpp
 *
 *  Created on: Aug 16, 2012
 *      Author: ramesh
 */
/* Tested cases: 05/10/2012
 * 1. deleted the files while uploading and tested.	
 * 2. sent a single record, 200 and 500 and tested. 	
 * 3. DB was made down several times during loading and tested.	
 * 4. created 1 lakh files in the 'input' directory to check if 'ls' like command in boost has any limitation on the no. of files.	
 * 5. Application was kept idle for over 12 hours to check if it had any impact.	
 * 6. Deleted the directories(input, output & Errorpath) while the application is running	We are exiting the application.
 */
#include <stdio.h>
#include <filesystem/v3/convenience.hpp>
#include <regex.hpp>
#include <program_options.hpp>
#include <list>
#include "DBPoolManager.h"
#include <EventLog.h>
#include "FileUploader.h"
#include "ConfigParams.h"
#include "xmlParser.h"
//#include "IPCConnection.h"
 #include <signal.h>
#include<unistd.h>
#define HEARTBEAT 0x73

const char scG_Delimiter = '_';
CEventLog CG_EventLog;
CConfigParams CG_Config;
CFileUploader *pCG_Inst[50];
//CIPCConnection CG_IPCConnection;
/*
FNVOID fnG_sendHeartBeatGCT(void *p)
{
	STSMSG_SS7 *pSL_Msg;
	DBG_INFO((CG_EventLog),("HeartBeat Thread started GCT"));
	int processId = getpid();
	while(1)
	{
		try
		{
			while((pSL_Msg = CG_IPCConnection.getm(1,1,1,1)) == NULL)
			{
				DBG_ERROR((CG_EventLog),("getm failed"));
				CSTSOSUtil::mcfn_sleep(1);
			}

			pSL_Msg->hdr.type = HEARTBEAT;
			pSL_Msg->hdr.src  = CG_Config.mcfn_ModuleId();
			pSL_Msg->hdr.dst = CG_Config.mcfn_getPMId();
			pSL_Msg->hdr.err_info = processId;
			pSL_Msg->len = 0x00;

			if(CG_IPCConnection.GCT_send(pSL_Msg->hdr.dst,(HDR*&)pSL_Msg) != 0x00)
			{
				CG_IPCConnection.relm((HDR*&)pSL_Msg);
				DBG_CRITICAL((CG_EventLog),("Not able to send HeartBeat"));
			}
			else
			{
				DBG_INFO((CG_EventLog),("Heartbeat Message sent to App. Manager=%02x",CG_Config.mcfn_getPMId()));
			}
			CSTSOSUtil::mcfn_sleep(CG_Config.mcfn_HeartBeatInt());
		}
		catch(...)
		{
			DBG_ERROR((CG_EventLog),("Exception in Heartbeat Thread"));
		}
	}
}
*/
/*Function which catches the signals
 */
#define _MAX_PATH 500
void fnG_catchSignal(int siL_signalNo)
{
	return;
	/*printf("Entering fnG_catchSignal function\n");
	char _path[_MAX_PATH]={0x00};
	char pscL_Data[200]={0x00};
	char *path = NULL;
	path = getcwd(_path,sizeof(_path));
	char _filename[500]={0x00};
        sprintf(_filename,"%s/Signals_CdrUploader.txt",_path);
        FILE *_fp = NULL;
        _fp = fopen(_filename,"a");
        if(_fp == NULL){
                printf("exiting..\n");
		return;
        }
        fseek(_fp,0,SEEK_END);
	struct timeb LS_currentTime;
        ftime(&LS_currentTime);
        long slL_CurTime = LS_currentTime.time;
        struct tm *pSL_TM;
        pSL_TM = localtime(&slL_CurTime);
	sprintf(pscL_Data,"[%02d-%02d-%02d %02d:%02d:%02d:%03d]Signal %d received\n",pSL_TM->tm_mday,pSL_TM->tm_mon + 1,pSL_TM->tm_year + 1900,\
                                        pSL_TM->tm_hour,pSL_TM->tm_min,pSL_TM->tm_sec,LS_currentTime.millitm,siL_signalNo);
        fprintf(_fp,"%s\n",pscL_Data);
	fflush(_fp);
        fclose(_fp);
*/
}


int main(int count,char* args[])
{
	if((count >= 2) && (strcasecmp(args[1],"-v") == 0)){
		printf("CdrUploader.exe 2.0.1.0 -- 02nd Jan 2014\n");
		printf("Copyright (C) SixDee Telecom Solutions Pvt Ltd.\n");
		exit(0);
	}	
	//set the logging properties
	CG_EventLog.mcfn_setFilePath((char*)CG_Config.mcfn_getLogPath());
	CG_EventLog.mcfn_setLogLevel(CG_Config.mcfn_getLogLevel());
	CG_EventLog.mcfn_setNewFileGenerationPeriod(CG_Config.mcfn_getLogDuration());
	CG_EventLog.mcfn_setAPPTag("CDRMaintain");
	//Open a new log file
	try{
		CG_EventLog.mcfn_openNewFile();
	}
	catch(...){
		printf("Unable to open file in path:%s", CG_Config.mcfn_getLogPath());
		exit(1);
	}
	//ignoring the SIGPIPE signal(8th July 2013)
	signal(SIGPIPE,fnG_catchSignal);
	signal(SIGFPE,SIG_IGN);
	signal(SIGABRT,SIG_IGN);
	//Reading the xml configuration file
	unsigned int uiL_Tables = 0;
	char pscL_FileName[] = "CDR.xml";
	XMLResults CL_XMLRes;
	XMLNode xMainNode = XMLNode::parseFile(pscL_FileName,"CdrUploadParams",&CL_XMLRes);
	if(xMainNode.isEmpty())
	{
		fprintf(stdout,"Mandatory Parameter 'CdrUploadParams' is missing\n");
		exit(1);
	}

	XMLNode xNoofTables = xMainNode.getChildNode("numberofUploaders");
	if((!xNoofTables.isEmpty()) && (xNoofTables.getText() != NULL))
	{
		uiL_Tables = atoi(xNoofTables.getText());
	}
	else
	{
		fprintf(stdout,"Mandatory Parameter 'numberofUploaders' is missing\n");
		exit(1);
	}
	DBG_CRITICAL((CG_EventLog),("No of uploader's : %d",uiL_Tables));
	XMLNode xTable;
	int itr;
	char pscL_Table[20];
	char DBIP[20],DBName[50],DBUser[50],DBPass[50],Function[30],FileInPath[500],FileOutPath[500],FileErrorPath[500];
	char RegExpr[50],LineSep[20],FieldSep[20],Columns[2000];
	bool isReplicationRequired;
	for(itr=0;itr<uiL_Tables;++itr)
	{
		memset(Columns,0x00,sizeof(Columns));
		memset(pscL_Table,0x00,sizeof(pscL_Table));
		sprintf(pscL_Table,"Uploader%d",itr+1);
		xTable = xMainNode.getChildNode(pscL_Table);
		if(!xTable.isEmpty())
		{
			//The parameter is present and contains a value.
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter '%s' is missing\n",pscL_Table);
			exit(1);
		}
		XMLNode xNode;
		xNode = xTable.getChildNode("DBIP");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(DBIP,xNode.getText());
		}
		else if(strlen(CG_Config.mcfn_getDBServerIP()))
		{
			strcpy(DBIP,CG_Config.mcfn_getDBServerIP());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'DBIP' is missing in '%s' tag\n",pscL_Table);
			exit(1);
		}

		xNode = xTable.getChildNode("DBName");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(DBName,xNode.getText());
		}
		else if(strlen(CG_Config.mcfn_getDBSid()))
		{
			strcpy(DBName,CG_Config.mcfn_getDBSid());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'DBName' is missing in '%s' tag\n",pscL_Table);
			exit(1);
		}

		xNode = xTable.getChildNode("DBUserName");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(DBUser,xNode.getText());
		}
		else if(strlen(CG_Config.mcfn_getDBUserName()))
		{
			strcpy(DBUser,CG_Config.mcfn_getDBUserName());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'DBUserName' is missing in '%s' tag\n",pscL_Table);
			exit(1);
		}

		xNode = xTable.getChildNode("DBPassword");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(DBPass,xNode.getText());
		}
		else if(strlen(CG_Config.mcfn_getDBPassword()))
		{
			strcpy(DBPass,CG_Config.mcfn_getDBPassword());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'DBPassword' is missing in '%s' tag\n",pscL_Table);
			exit(1);
		}

		xNode = xTable.getChildNode("FunctionName");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(Function,xNode.getText());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'FunctionName' is missing in '%s' tag\n",pscL_Table);
			exit(1);
		}

		xNode = xTable.getChildNode("FilesInpath");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(FileInPath,xNode.getText());
		}
		else if(strlen(CG_Config.mcfn_getsubmitfilespath()))
		{
			strcpy(FileInPath,CG_Config.mcfn_getsubmitfilespath());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'FilesInpath' is missing in '%s' tag\n",pscL_Table);
			exit(1);
		}

		xNode = xTable.getChildNode("FilesOutPath");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(FileOutPath,xNode.getText());
		}
		else if(strlen(CG_Config.mcfn_submitoutputpath()))
		{
			strcpy(FileOutPath,CG_Config.mcfn_submitoutputpath());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'FilesOutPath' is missing in '%s' tag\n",pscL_Table);
			exit(1);
		}

		xNode = xTable.getChildNode("FilesErrorPath");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(FileErrorPath,xNode.getText());
		}
		else if(strlen(CG_Config.mcfn_getErrorPath()))
		{
			strcpy(FileErrorPath,CG_Config.mcfn_getErrorPath());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'FilesErrorPath' is missing in '%s' tag\n",pscL_Table);
			exit(1);
		}

		xNode = xTable.getChildNode("RegularExpr");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(RegExpr,xNode.getText());
		}
		else if(strlen(CG_Config.mcfn_getfileRegExpr()))
		{
			strcpy(RegExpr,CG_Config.mcfn_getfileRegExpr());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'RegularExpr' is missing in '%s' tag\n",pscL_Table);
			exit(1);
		}

		xNode = xTable.getChildNode("Columns");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(Columns,xNode.getText());
		}
		else if(strlen(CG_Config.mcfn_getColumns()))
		{
			strcpy(Columns,CG_Config.mcfn_getColumns());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'Columns' is missing in '%s' tag\n",pscL_Table);
			//exit(1);
		}

		xNode = xTable.getChildNode("FieldSep");
		if((!xNode.isEmpty()) && (xNode.getClear().lpszValue/*xNode.getText()*/ != NULL))
		{
			strcpy(FieldSep,xNode.getClear().lpszValue);
		}
		else if(strlen(CG_Config.mcfn_getFieldSep()))
		{
			strcpy(FieldSep,CG_Config.mcfn_getFieldSep());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'FieldSep' is missing in '%s' tag\n",pscL_Table);
			exit(1);
		}

		xNode = xTable.getChildNode("LineSep");
		if((!xNode.isEmpty()) && (xNode.getClear().lpszValue != NULL))
		{
			strcpy(LineSep,xNode.getClear().lpszValue);
		}
		else if(strlen(CG_Config.mcfn_getLineSep()))
		{
			strcpy(LineSep,CG_Config.mcfn_getLineSep());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'LineSep' is missing in '%s' tag\n",pscL_Table);
			exit(1);
		}
		//The new tag ReplicationRequired is added (17/05/2013)
		xNode = xTable.getChildNode("ReplicationRequired");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
			isReplicationRequired = (atoi(xNode.getText()))?true:false;
		else
			isReplicationRequired = false;
		//Check for ReplicationPath tag only if ReplicationRequired is 'true'
		/*if(isReplicationRequired)
		{
			xNode = xTable.getChildNode("ReplicationPath");
			if((!xNode.isEmpty()) && (xNode.getText() != NULL))
			{
				strcpy(ReplicationPath,xNode.getText());
			}
			else
			{
			fprintf(stdout,"Mandatory Parameter 'ReplicationPath'(since ReplicationRequired is true) is missing in '%s' tag\n",pscL_Table);
			exit(1);
			}
			}*/
		//Creating an object with the above configuration parameters
		DBG_INFO((CG_EventLog),("%s,%s,%s,%s",DBIP,DBName,DBPass,Function));
		DBG_INFO((CG_EventLog),("Field seperator:%s",FieldSep));
		pCG_Inst[itr] = new
			CFileUploader(DBIP,DBName,DBUser,DBPass,Function,Columns,FileInPath,FileOutPath,FileErrorPath,RegExpr,LineSep,FieldSep,isReplicationRequired);
	}

	DBG_INFO((CG_EventLog),("All tables are being uploaded"));
	DBG_INFO((CG_EventLog),("%d,%d",CG_Config.mcfn_IsHeartBeatReq(),CG_Config.mcfn_HeartBeatInt()));
/*
	if(CG_Config.mcfn_IsHeartBeatReq() && CG_Config.mcfn_HeartBeatInt()>0){
		DBG_INFO((CG_EventLog),("Connecting to the IPC router"));
		if(!CG_IPCConnection.mcfn_initialise(CG_Config.mcfn_ModuleId(),CG_Config.mcfn_getIPCRouterId(),(char*)CG_Config.mcfn_getIPCRouterIP(),CG_Config.mcfn_getIPCRouterPort(),&CG_EventLog,CG_Config.mcfn_getIPCMode()))
		{
			DBG_ERROR((CG_EventLog),("Not able to connect to IPC Router in 1st attempt"));
			while(1)
			{
				if(!CG_IPCConnection.STSIPCConnect())
				{
					DBG_ERROR((CG_EventLog),("Failed connecting to STSIPCRouter"));
					CSTSOSUtil::mcfn_sleep(1);
					break;
				}
				else{
					break;
				}
			}
		}
		//Thread sending the heartbeat	
		CSTSOSUtil::mcfn_beginAndDetachThread(fnG_sendHeartBeatGCT,NULL);

		HDR *pSL_Hdr;
		while(1)
		{
			try{
				pSL_Hdr = CG_IPCConnection.GCT_receive(CG_Config.mcfn_ModuleId());
				if(pSL_Hdr){
					DBG_INFO((CG_EventLog),("Received message from %d",pSL_Hdr->src));
					CG_EventLog.mcfn_logSS7Message((STSMSG_SS7*)pSL_Hdr);
					CG_IPCConnection.relm(pSL_Hdr);
				}
			}
			catch(...){
				DBG_CRITICAL((CG_EventLog),("Caught Exception"));
			}
		}
	}	
*/
	while(1){
		CSTSOSUtil::mcfn_sleep(10);
	}
}
