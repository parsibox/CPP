#ifndef __CONFIGURATIONS__H
#define __CONFIGURATIONS__H
#include<IConfigParams.hpp>
class CConfiguration:public FrameWork::IConfiguration
{
	private:
		int mesi_serverPort;
		char pmesc_dbIp[40];
		char pmesc_dbUserName[40];
		char pmesc_dbPassword[40];
		char pmesc_dbName[40];

	public:
		CConfiguration();
		int mcfn_getServerPort();
		bool mcfn_readConfigurations();

		const char* mcfn_getDbIp();
		const char* mcfn_getDbUserName();
		const char* mcfn_getDbPassword();
		const char* mcfn_getDbName();
	
};
inline int CConfiguration::mcfn_getServerPort()
{
	return mesi_serverPort;
}

inline const char* CConfiguration::mcfn_getDbIp(){
	return pmesc_dbIp;
}
inline const char* CConfiguration::mcfn_getDbUserName(){
	return pmesc_dbUserName;
}
inline const char* CConfiguration::mcfn_getDbPassword(){
	return pmesc_dbPassword;
}
inline const char* CConfiguration::mcfn_getDbName(){
	return pmesc_dbName;
}

#endif
