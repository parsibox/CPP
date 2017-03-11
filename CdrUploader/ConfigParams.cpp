#include "ConfigParams.h"
#include "stdio.h"
#include <stdlib.h>
#include "xmlParser.h"

CConfigParams::CConfigParams()
{
	if(!mcfn_ReadConfigurations(false))
	{
		exit(1);
	}
}
bool CConfigParams::mcfn_ReadConfigurations(bool IsReload)
{
	try
	{

		//char pscL_FileName[] = "CDR.xml";
		XMLResults CL_XMLRes;
		XMLNode xMainNode = XMLNode::parseFile("CDR.xml","CdrUploadParams",&CL_XMLRes);
		if(xMainNode.isEmpty())
		{
			//CdrUploadParams
			fprintf(stdout,"Mandatory Parameter 'CdrUploadParams' is missing\n");
			exit(1);
		}

		XMLNode xLogDuration = xMainNode.getChildNode("LogDuration");
		if((!xLogDuration.isEmpty()) && (xLogDuration.getText() != NULL))
		{
			mesl_LogDuration = atoi(xLogDuration.getText());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'LogDuration' is missing\n");
			exit(1);
		}

		XMLNode xLogLevel = xMainNode.getChildNode("LogLevel");
		if((!xLogLevel.isEmpty()) && (xLogLevel.getText() != NULL))
		{
			mesl_LogLevel = atoi(xLogLevel.getText());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'LogLevel' is missing\n");
			exit(1);
		}
		XMLNode xLogPath = xMainNode.getChildNode("LogPath");
		if((!xLogPath.isEmpty()) && (xLogPath.getText() != NULL))
		{
			strcpy(mesc_LogDir,xLogPath.getText());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'LogPath' is missing\n");
			exit(1);
		}

		XMLNode xNode;
		xNode = xMainNode.getChildNode("DBIP");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(mesc_DBServerIP,xNode.getText());
		}

		xNode = xMainNode.getChildNode("DBName");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(mesc_DBSid,xNode.getText());
		}

		xNode = xMainNode.getChildNode("DBUserName");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(mesc_DBUserName,xNode.getText());
		}

		xNode = xMainNode.getChildNode("DBPassword");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(mesc_DBPassword,xNode.getText());
		}

		xNode = xMainNode.getChildNode("TableName");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(mesc_TableName,xNode.getText());
		}

		xNode = xMainNode.getChildNode("FilesInpath");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(mesc_submitfilesPath,xNode.getText());
		}

		xNode = xMainNode.getChildNode("FilesOutPath");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(mesc_OutPath,xNode.getText());
		}

		xNode = xMainNode.getChildNode("FilesErrorPath");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(mesc_ErrorPath,xNode.getText());
		}

		xNode = xMainNode.getChildNode("RegularExpr");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(mesc_RegularExpr,xNode.getText());
		}

		xNode = xMainNode.getChildNode("Columns");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(mesc_Columns,xNode.getText());
		}

		xNode = xMainNode.getChildNode("FieldSep");
		if((!xNode.isEmpty()) && (xNode.getClear().lpszValue != NULL))
		{
			strcpy(mesc_FieldSep,xNode.getClear().lpszValue);
		}

		xNode = xMainNode.getChildNode("LineSep");
		if((!xNode.isEmpty()) && (xNode.getClear().lpszValue != NULL))
		{
			strcpy(mesc_LineSep,xNode.getClear().lpszValue);
		}

		xNode = xMainNode.getChildNode("IPC_ROUTER_ID");
		if(!(xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			mesl_IpcId = atoi(xNode.getText());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'IPC_ROUTER_ID' is missing\n");
			exit(1);
		}

		xNode = xMainNode.getChildNode("IPC_ROUTER_PORT");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			mesl_IpcPort = atoi(xNode.getText());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'IPC_ROUTER_PORT' is missing\n");
			exit(1);
		}

		xNode = xMainNode.getChildNode("IPC_ROUTER_IP");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			strcpy(pmesc_IpcIp,xNode.getText());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'IPC_ROUTER_IP' is missing\n");
			exit(1);
		}

		xNode = xMainNode.getChildNode("IPCMODE");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			mesl_IpcMode = atoi(xNode.getText());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'IPCMODE' is missing\n");
			exit(1);
		}

		xNode = xMainNode.getChildNode("MODULEID");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			mesl_ModuleId = atoi(xNode.getText());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'MODULEID' is missing\n");
			exit(1);
		}

		xNode = xMainNode.getChildNode("PM_ID");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			mesl_PMId = atoi(xNode.getText());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'PM_ID' is missing\n");
			exit(1);
		}

		xNode = xMainNode.getChildNode("HeartBeatInterval");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			mesl_HTBTInt = atoi(xNode.getText());
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'HeartBeatInterval' is missing\n");
			exit(1);
		}

		xNode = xMainNode.getChildNode("HeartBeatReq");
		if((!xNode.isEmpty()) && (xNode.getText() != NULL))
		{
			if(strcasecmp(xNode.getText(),"Y") == 0)
				meb_IsHeartBeatReq = true;
			else
				meb_IsHeartBeatReq = false;
		}
		else
		{
			fprintf(stdout,"Mandatory Parameter 'HeartBeatReq' is missing\n");
			exit(1);
		}
		return true;

	}
	catch(...)
	{
		fprintf(stdout,"Exception Occurred\n");
		return false;
	}
	return false;
}
