#ifndef _CONFIGPARAMS_H
#define _CONFIGPARAMS_H

#include <stdio.h> 
#include <string.h>

typedef unsigned char u8;
typedef unsigned short u16;

class CConfigParams {
private:
	char mesc_DBServerIP[50];
	char mesc_DBSid[50];
	char mesc_DBUserName[50];
	char mesc_DBPassword[50];
	char mesc_LogDir[100];
	int  mesl_LogLevel;
	char mesc_LineSep[20];
	char mesc_FieldSep[20];
	char mesc_ErrorPath[200];
	char mesc_OutPath[200];
	char mesc_submitfilesPath[200];
	char mesc_RegularExpr[50];
	char mesc_Columns[1000];
	char mesc_TableName[100];
	int mesl_NoOfTables;
	int mesl_LogDuration;
	int mesl_ModuleId;
	int mesl_IpcMode;
	int mesl_IpcId;
	int mesl_IpcPort;
	int mesl_PMId;
	char pmesc_IpcIp[20];
	int mesl_HTBTInt;
	bool meb_IsHeartBeatReq;


private:
public:
	CConfigParams();
	
	bool mcfn_ReadConfigurations(bool);

	const char* mcfn_getDBServerIP() const;
	const char* mcfn_getDBUserName() const;
	const char* mcfn_getDBPassword() const;
	const char* mcfn_getDBSid() const;
	const char* mcfn_getLogPath() const;
	int mcfn_getLogLevel() const;
	int mcfn_getLogDuration() const;
	const char* mcfn_getColumns() const;
	const char* mcfn_getfileRegExpr() const;
	const char* mcfn_getsubmitfilespath() const;
	const char* mcfn_submitoutputpath() const;
	const char* mcfn_getErrorPath() const;
	const char* mcfn_getFieldSep() const;
	const char* mcfn_getLineSep() const;
	const char* mcfn_getTableName() const;
	int mcfn_ModuleId() const;
	int mcfn_getIPCRouterId() const;
	const char* mcfn_getIPCRouterIP() const;
	int mcfn_getIPCRouterPort() const;
	int mcfn_getIPCMode() const;
	int mcfn_getPMId() const;
	bool mcfn_IsHeartBeatReq() const{
		return meb_IsHeartBeatReq;
	}
	int mcfn_HeartBeatInt() const{
		return mesl_HTBTInt;
	}
};

inline int CConfigParams::mcfn_ModuleId() const
{
	return mesl_ModuleId;
}

inline int CConfigParams::mcfn_getIPCRouterId() const
{
	return mesl_IpcId;
}

inline int CConfigParams::mcfn_getPMId() const
{
	return mesl_PMId;
}

inline const char* CConfigParams::mcfn_getIPCRouterIP() const
{
	return pmesc_IpcIp;
}

inline int CConfigParams::mcfn_getIPCRouterPort() const
{
	return mesl_IpcPort;
}

inline int CConfigParams::mcfn_getIPCMode() const
{
	return mesl_IpcMode;
}

inline const char* CConfigParams::mcfn_getErrorPath() const
{
	return mesc_ErrorPath;
}

inline const char* CConfigParams::mcfn_getColumns() const
{
	return mesc_Columns;
}

inline const char* CConfigParams::mcfn_getfileRegExpr() const
{
	return mesc_RegularExpr;
}

inline const char* CConfigParams::mcfn_getsubmitfilespath() const
{
	return mesc_submitfilesPath;
}

inline const char* CConfigParams::mcfn_submitoutputpath() const
{
	return mesc_OutPath;
}

inline const char* CConfigParams::mcfn_getFieldSep() const
{
	return mesc_FieldSep;
}

inline const char* CConfigParams::mcfn_getLineSep() const
{
	return mesc_LineSep;
}

inline const char* CConfigParams::mcfn_getLogPath() const
{
	return mesc_LogDir;
}

inline int CConfigParams::mcfn_getLogLevel() const
{
	return mesl_LogLevel;
}
inline const char *CConfigParams::mcfn_getDBServerIP() const {
	return mesc_DBServerIP;
}

inline const char *CConfigParams::mcfn_getDBUserName() const {
	return mesc_DBUserName;
}

inline const char *CConfigParams::mcfn_getDBPassword() const {
	return mesc_DBPassword;
}

inline const char *CConfigParams::mcfn_getDBSid() const {
	return mesc_DBSid;
}

inline const char* CConfigParams::mcfn_getTableName() const {
	return mesc_TableName;
}

inline int CConfigParams::mcfn_getLogDuration() const {
	return mesl_LogDuration;
}
#endif
