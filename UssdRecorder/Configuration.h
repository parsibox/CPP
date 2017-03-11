#ifndef __CONFIGURATIONS__
#define __CONFIGURATIONS__
#include"Get_pri_profile.h"
class CConfiguration{
	private:
		int mesi_moduleId;
		int mesi_logLevel;
		int mesi_logFileChangeInterval;
		int mesi_cdrServerPort;
		int mesi_maxNumOfLinesInFile;
		int mesi_cdrFileChangeTime;
		int mesi_numOfdayParts;
		char pmesc_logPath[1024];
		char pmesc_cdrFileGenPath[1024];
		char pmesc_fieldSep[8];
		char pmesc_lineSep[8];
	
	public:
		CConfiguration();
		bool mcfn_loadConfigurations();
		int mcfn_getModuleId();
		int mcfn_getLogLevel();
		int mcfn_getLogChangeInterval();
		int mcfn_getCdrServerPort();
		int mcfn_getMaxNumOfLinesinFile();
		int mcfn_getCdrFileChangeTime();
		int mcfn_getPartsOfDay();
		const char* mcfn_getLogPath();
		const char* mcfn_getCdrFileGenPath();
		const char* mcfn_getFieldSep();
		const char* mcfn_getLineSep();
		
};
inline const char* CConfiguration::mcfn_getFieldSep(){
	return pmesc_fieldSep;
}
inline const char* CConfiguration::mcfn_getLineSep(){
	return pmesc_lineSep;
}
inline int CConfiguration::mcfn_getModuleId(){
	return mesi_moduleId;
}
inline int CConfiguration::mcfn_getLogLevel(){
	return mesi_logLevel;
}
inline int CConfiguration::mcfn_getLogChangeInterval(){
	return mesi_logFileChangeInterval;
}
inline int CConfiguration::mcfn_getCdrServerPort(){
	return  mesi_cdrServerPort;
}
inline int CConfiguration::mcfn_getMaxNumOfLinesinFile(){
	return mesi_maxNumOfLinesInFile;
}
inline int CConfiguration::mcfn_getCdrFileChangeTime(){
	return mesi_cdrFileChangeTime;
}
inline int CConfiguration::mcfn_getPartsOfDay(){
	return mesi_numOfdayParts;
}
inline const char* CConfiguration::mcfn_getLogPath(){
	return pmesc_logPath;
}
inline const char* CConfiguration::mcfn_getCdrFileGenPath(){
	return pmesc_cdrFileGenPath;
}
#endif
