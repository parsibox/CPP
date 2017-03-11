#include"Receiver.hpp"
#include"EventLog.h"
CEventLog CG_EventLog;

int main(){
	char pscL_Temp[40];
	sprintf(pscL_Temp, "SMPPSERVER%d",10);
        CG_EventLog.mcfn_setAPPTag(pscL_Temp);
        CG_EventLog.mcfn_setFilePath((char*)"./Logs");
        CG_EventLog.mcfn_setLogLevel(127);
        CG_EventLog.mcfn_setNewFileGenerationPeriod(3600);        try {
                CG_EventLog.mcfn_openNewFile();
        } catch (...) {                 
                printf("Unable to open file in path:./Logs");
                exit(1);        
        }                       
                        
	CReceiver CL_Rece(3014,"");	
	while(1){
		sleep(10);
	}
}
