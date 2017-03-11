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
		mcfn_getInt("CDR","MODULE_ID",mesi_moduleId);
		mcfn_getInt("CDR","LOG_LEVEL",mesi_logLevel);
		mcfn_getInt("CDR","LOG_INTERVEL",mesi_logFileChangeInterval);
		mcfn_getInt("CDR","CDR_SERVERPORT",mesi_cdrServerPort);
		mcfn_getInt("CDR","NUMOF_LINESIN_FILE",mesi_maxNumOfLinesInFile);
		mcfn_getInt("CDR","CDR_FILECHANGE_TIME",mesi_cdrFileChangeTime);
		mcfn_getInt("CDR","PARTS_OFDAY",mesi_numOfdayParts);
		mcfn_getString("CDR","LOG_PATH",pmesc_logPath);
		mcfn_getString("CDR","CDR_FILEGEN_PATH",pmesc_cdrFileGenPath);
		mcfn_getString("CDR","LINE_SEPARATOR",pmesc_lineSep);
		mcfn_getString("CDR","FIELD_SEPARATOR",pmesc_fieldSep);
		return true;
	}
	 catch(STSGenException e){
		printf("%s\n",e.reasonString());	
	}	
	return false;
}
