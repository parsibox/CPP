#ifndef _USSD_BIND_H
#define _USSD_BIND_H
#include "UssdHeader.hpp"
#define CUssdUnbind CHeader
#define CUssdUnbindResp CHeader
#define CUssdShake CHeader
#define CUssdShakeResp CHeader


class CUssdBind:public CHeader{
	public:
		u8 pmcu8_AccountName[11];
		u8 pmcu8_password[9];
		u8 pmcu8_systemType[13];
		int mcsi_interfaceVersion;
	public:
		CUssdBind(CHeader &CL_header):CHeader(CL_header){
			memset(pmcu8_AccountName,0x00,11);
			memset(pmcu8_password,0x00,9);
			memset(pmcu8_systemType,0x00,13);
			mcsi_interfaceVersion=0x10;
		}
		CUssdBind(){
			memset(pmcu8_AccountName,0x00,11);
			memset(pmcu8_password,0x00,9);
			memset(pmcu8_systemType,0x00,13);
			mcsi_interfaceVersion=0x10;
		}
		void mcfn_setParam(u8* u8L_param){
			memcpy(pmcu8_AccountName,&u8L_param[0],11);
			memcpy(pmcu8_password,&u8L_param[11],9);
			memcpy(pmcu8_systemType,&u8L_param[20],13);
			memcpy(&mcsi_interfaceVersion,&u8L_param[33],4);
		}
		u8* mcfn_getParam(u8* u8L_param){
			memcpy(&u8L_param[0],pmcu8_AccountName,11);
			memcpy(&u8L_param[11],pmcu8_password,9);
			memcpy(&u8L_param[20],pmcu8_systemType,13);
			memcpy(&u8L_param[33],&mcsi_interfaceVersion,4);
			return u8L_param;
		}
		   std::string mcfn_ShortDebugString(){
                        std::stringstream CL_strStr;
                        CL_strStr.str("");
                        CL_strStr<<"Header{"<<mcfn_getHeaderPrintable()<<"},AccountName:"<<pmcu8_AccountName<<",password:"<<pmcu8_password<<",systemType:"<<pmcu8_systemType<<"interfaceVersion:"<<mcsi_interfaceVersion;
                        return CL_strStr.str();
                }

};

class CUssdBindResp:public CHeader{
	public:
		u8 pmcu8_AccountName[11];
	public:
		CUssdBindResp(CHeader &CL_header):CHeader(CL_header){
			memset(pmcu8_AccountName,0x00,11);
		}
		void mcfn_setParam(u8* u8L_param){
			memcpy(pmcu8_AccountName,&u8L_param[0],11);
		}	
		u8* mcfn_getParam(u8* u8L_param){
			memcpy(u8L_param,pmcu8_AccountName,11);
			return u8L_param;
		}
		  std::string mcfn_ShortDebugString(){
                        std::stringstream CL_strStr;
                        CL_strStr.str("");
                        CL_strStr<<"Header{"<<mcfn_getHeaderPrintable()<<"},AccountName:"<<pmcu8_AccountName;
                        return CL_strStr.str();
                }

};


#endif
