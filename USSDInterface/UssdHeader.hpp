
#ifndef _USSD_HEADER_H
#define _USSD_HEADER_H
#include<string>
#include <sstream>

typedef unsigned char u8;
class COMMAND_ID{
	public:
		enum _COMMAND_ID{
			USSD_NONE=0x00,
			USSD_BIND=0x65,
			USSD_UNBIND=0x66,
			USSD_BINDRESP=0x67,
			USSD_UNBINDRESP=0x68,
			USSD_BEGIN=0x6F,
			USSD_CONTINUE=0x70,
			USSD_END=0x71,
			USSD_ABORT=0x72,
			USSD_SWITCH=0x74,
			USSD_CHARGINGIND=0x75,
			USSD_CHARGINGRINDESP=0x76,
			USSD_HANDSHAKE=0x83,
			USSD_HANDSHAKERESP=0x84
		};
		enum _COMMAND_LENGTH{
			USSD_BIND_LEN=57,
			USSD_BINDRESP_LEN=31,
			USSD_UNBIND_LEN=20,
			USSD_UNBINDRESP_LEN=20,
			USSD_BEGIN_LEN=230,
			USSD_CONTINUE_LEN=230,
			USSD_END_LEN=230,
			USSD_ABORT_LEN=20,
			USSD_SWITCH_LEN=228,
			USSD_CHARGINGIND_LEN=49,
			USSD_CHARGINGRINDESP_LEN=20,
			USSD_HANDSHAKE_LEN=20,
			USSD_HANDSHAKERESP_LEN=20
		};
};
class USSD_VERSION{
	public:	
	enum _USSD_VERSION{
		USSD_PHASE_1=0x10,
		USSD_PHASE_2=0x20,
		USSD_PHASE_2_PLUS=0x25
	};
};
class USSD_OPTYPE{
	public:
		enum _USSD_OPTYPE{
			USSD_NONE=0x00,
			USSD_REQUEST=0x01,
			USSD_NOTIFY=0x02,
			USSD_RESPONCE=0x03,
			USSD_RELEASE=0x04
		};
};

class CHeader{
	public:	
		int mcsi_cmdLength;
		int mcsi_cmdId;
		int mcsi_cmdStatus;
		int mcsi_senderCB;
		int mcsi_receiverCB;
		CHeader(int iL_cmdLen=0,int iL_cmdId=COMMAND_ID::USSD_NONE,int iL_cmdStatus=0x00,int iL_senderCB=0xFFFFFFFF,int iL_receiverCB=0xFFFFFFFF){
			mcsi_cmdLength=iL_cmdLen;
			mcsi_cmdId=iL_cmdId;
			mcsi_cmdStatus=iL_cmdStatus;
			mcsi_senderCB=iL_senderCB;
			mcsi_receiverCB=iL_receiverCB;
		}
		CHeader(const CHeader &CL_copy){
			mcsi_cmdLength=CL_copy.mcsi_cmdLength;
			mcsi_cmdId=CL_copy.mcsi_cmdId;
			mcsi_cmdStatus=CL_copy.mcsi_cmdStatus;
			mcsi_senderCB=CL_copy.mcsi_senderCB;
			mcsi_receiverCB=CL_copy.mcsi_receiverCB;
		}
		~CHeader(){};
	        void mcfn_setHeader(u8* u8L_header){
			memcpy(&mcsi_cmdLength,&u8L_header[0],4);
			memcpy(&mcsi_cmdId,&u8L_header[4],4);
			memcpy(&mcsi_cmdStatus,&u8L_header[8],4);
			memcpy(&mcsi_senderCB,&u8L_header[12],4);
			memcpy(&mcsi_receiverCB,&u8L_header[16],4);
		}
		u8* mcfn_getHeader(u8* u8L_header){
			memcpy(&u8L_header[0],&mcsi_cmdLength,4);
			memcpy(&u8L_header[4],&mcsi_cmdId,4);
			memcpy(&u8L_header[8],&mcsi_cmdStatus,4);
			memcpy(&u8L_header[12],&mcsi_senderCB,4);
			memcpy(&u8L_header[16],&mcsi_receiverCB,4);
			return u8L_header;
		}
		std::string mcfn_ShortDebugString(){
			std::stringstream CL_bufStr;
			CL_bufStr.str("");
			CL_bufStr<<"cmdLength:"<<mcsi_cmdLength<<",cmdId:"<<mcsi_cmdId<<",cmdStatus:"<<mcsi_cmdStatus<<",senderCB:"<<mcsi_senderCB<<",receiverCB:"<<mcsi_receiverCB;
			return  CL_bufStr.str();
		}
		std::string mcfn_getHeaderPrintable(){
			return mcfn_ShortDebugString();
		}	
};

class CUssdSwitch:public CHeader{
	public:
		u8 mcu8_switchMode;//The SwitchMode field indicates the modes of service switching. It comprises two switching modes:0: global switching and 1: local switching. Global switching means that the service switching is implemented by the USSD central office where the service application resides. Local switching means that the service switching is implemented by the service platform (or the USSDC) directly connecting the service application
		u8 pmcu8_msisdn[21];
		u8 pmcu8_serviceCode[4];
		u8 pmcu8_ussdString[182];
	public:
		CUssdSwitch(){
			mcfn_init();
		}
		CUssdSwitch(CHeader &CL_header):CHeader(CL_header){
			mcfn_init();
		}
		void mcfn_init(){
			mcu8_switchMode=0x00;
			memset(pmcu8_msisdn,0x00,21);
			memset(pmcu8_serviceCode,0x00,4);
			memset(pmcu8_ussdString,0x00,182);

		}
		void mcfn_setParam(u8* u8L_param){
			mcu8_switchMode=u8L_param[0];
			memcpy(pmcu8_msisdn,&u8L_param[1],21);
			memcpy(pmcu8_serviceCode,&u8L_param[22],4);
			memcpy(pmcu8_ussdString,&u8L_param[26],182);
		}
		u8* mcfn_getParam(u8* u8L_param){
			u8L_param[0]=mcu8_switchMode;
			memcpy(&u8L_param[1],pmcu8_msisdn,21);
			memcpy(&u8L_param[22],pmcu8_serviceCode,4);
			memcpy(&u8L_param[26],pmcu8_ussdString,182);
			return u8L_param;
		}
		std::string mcfn_ShortDebugString(){
			std::stringstream CL_strStr;
			CL_strStr.str("");
			CL_strStr<<"Header{"<<mcfn_getHeaderPrintable()<<"},switchMode:"<<(int)mcu8_switchMode<<",msisdn:"<<pmcu8_msisdn<<",serviceCode:"<<pmcu8_serviceCode<<",ussdString"<<pmcu8_ussdString;
			return CL_strStr.str();
		}
};

#endif

