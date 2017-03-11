#include"Configuration.h"

CConfiguration::CConfiguration():FrameWork::IConfiguration("AGGRIGATOR")
{ 
	mcfn_readConfigurations();
}

bool CConfiguration::mcfn_readConfigurations()
{
	try
	{
		mcfn_getInt(LABLE_NAME,"SERVER_PORT",mesi_serverPort);
		mcfn_getString(LABLE_NAME,"DB_IP",pmesc_dbIp);
		mcfn_getString(LABLE_NAME,"DB_USERNAME",pmesc_dbUserName);
		mcfn_getString(LABLE_NAME,"DB_PASSWORD",pmesc_dbPassword);
		mcfn_getString(LABLE_NAME,"DB_NAME",pmesc_dbName);
	
		return true;
	}
	catch(STSGenException e){
		printf("%s\n",e.reasonString());
	}
	return false;

}
