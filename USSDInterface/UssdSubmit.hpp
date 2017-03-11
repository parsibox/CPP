#ifndef _USSD_SUBMIT_H_
#define _USSD_SUBMIT_H_
#include"UssdHeader.hpp"
#define CUssdContinue CUssdBegin
#define CUssdEnd CUssdBegin
#define CUssdAbort CHeader
class CUssdBegin:public CHeader{
	public:
		u8 mcu8_ussdVersion;
		u8 mcu8_ussdOptype;
		u8 pmcu8_msisdn[21];
		u8 pmcu8_serviceCode[21];//not in use
		u8 mcu8_codeSchema;//0x0F for 7 bit,0x44 for 8 bit,0x48 for 16 bit
		u8 pmcu8_ussdString[183];//182 chars of 7 bit,160 chars of 8 bit ,80 chars of 16 bit
		int mcsi_paramLen;
		CUssdBegin(){
			mcfn_init();
		}
		CUssdBegin(CHeader &CL_copy):CHeader(CL_copy){
			mcfn_init();
		}
		void mcfn_init(){
			mcsi_paramLen=0;
			mcu8_ussdVersion=USSD_VERSION::USSD_PHASE_1;
			mcu8_ussdOptype=USSD_OPTYPE::USSD_NONE;
			memset(pmcu8_msisdn,0x00,21);
			memset(pmcu8_serviceCode,0x00,21);
			mcu8_codeSchema=0x0F;
			memset(pmcu8_ussdString,0x00,183);

		}
		void mcfn_setParam(u8* u8L_param){
			mcu8_ussdVersion=u8L_param[mcsi_paramLen++];
			mcu8_ussdOptype=u8L_param[mcsi_paramLen++];
			memcpy(pmcu8_msisdn,&u8L_param[mcsi_paramLen],21);
			mcsi_paramLen+=21;
			memcpy(pmcu8_serviceCode,&u8L_param[mcsi_paramLen],21);
			mcsi_paramLen+=21;
			mcu8_codeSchema=u8L_param[mcsi_paramLen++];
			memcpy(pmcu8_ussdString,&u8L_param[mcsi_paramLen],mcsi_cmdLength-mcsi_paramLen);
                        pmcu8_ussdString[mcsi_cmdLength-mcsi_paramLen] =0x00;
			mcsi_paramLen+=strlen((const char*)pmcu8_ussdString);
		}
		u8* mcfn_getParam(u8* u8L_param){
                        u8L_param[mcsi_paramLen++]=mcu8_ussdVersion;
                        u8L_param[mcsi_paramLen++]=mcu8_ussdOptype;
                        memcpy(&u8L_param[mcsi_paramLen],pmcu8_msisdn,21);
			mcsi_paramLen+=21;
                        memcpy(&u8L_param[mcsi_paramLen],pmcu8_serviceCode,21);
			mcsi_paramLen+=21;
                        u8L_param[mcsi_paramLen++]=mcu8_codeSchema;
                        memcpy(&u8L_param[mcsi_paramLen],pmcu8_ussdString,strlen((const char*)pmcu8_ussdString));
			mcsi_paramLen+=strlen((const char*)pmcu8_ussdString);
			mcsi_cmdLength=mcsi_paramLen+20;//HDR
			return u8L_param;
                }
		 std::string mcfn_ShortDebugString(){
                        std::stringstream CL_strStr;
                        CL_strStr.str("");
                        CL_strStr<<"Header{"<<mcfn_getHeaderPrintable()<<"},ussdVersion:"<<(int)mcu8_ussdVersion<<",ussdOptype:"<<(int)mcu8_ussdOptype<<",msisdn:"<<pmcu8_msisdn<<",serviceCode:"<<pmcu8_serviceCode<<",codeSchema:"<<(int)mcu8_codeSchema<<",ussdString:"<<pmcu8_ussdString<<",ParamLength:"<<mcsi_paramLen;
                        return CL_strStr.str();
                }




};
#endif

