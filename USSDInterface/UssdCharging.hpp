#ifndef _USSD_CHARGING_H_
#define _USSD_CHARGING_H_
#include"UssdHeader.hpp"
#define CUssdChargingIndResp CHeader

class CUssdChargingInd:public CHeader
{
	public:
		int mcsi_chargeRation;//Its unit is fen (0.01 yuan). The value 0 means free of charge
		int mcsi_chargeType;//:0: free, 1: per message, 2: per monthly flat rate, and 3: per monthly flat rate for downlink
		u8 pmcu8_chargeSource[21];
		u8 mcu8_chargeLocation;//The ChargeLocation field indicates the location where bills are generated. There are the following three cases: 1: Both the USSDC and the USSD service platform generate bills. 2: Only the USSDC generates bills. 3: Only the USSD service platform generates bills
	public:
		CUssdChargingInd(){
			mcfn_init();	
		}

		CUssdChargingInd(CHeader &CL_header):CHeader(CL_header){
			mcfn_init();	
		}


		void mcfn_init(){
			mcsi_chargeRation=0x00;
			mcsi_chargeType=0x00;
			memset(pmcu8_chargeSource,0x00,21);
			mcu8_chargeLocation=0x00;
		}	
		void mcfn_setParam(u8* u8L_param){
			memcpy(&mcsi_chargeRation,&u8L_param[0],4);
			memcpy(&mcsi_chargeType,&u8L_param[4],4);
			memcpy(pmcu8_chargeSource,&u8L_param[8],21);
			mcu8_chargeLocation=u8L_param[29];
		}

		u8* mcfn_getParam(u8* u8L_param){
			memcpy(&u8L_param[0],&mcsi_chargeRation,4);
			memcpy(&u8L_param[4],&mcsi_chargeType,4);
			memcpy(&u8L_param[8],pmcu8_chargeSource,21);
			u8L_param[29]=mcu8_chargeLocation;
			return u8L_param;
		}
		  std::string mcfn_ShortDebugString(){
                        std::stringstream CL_strStr;
                        CL_strStr.str("");
                        CL_strStr<<"Header{"<<mcfn_getHeaderPrintable()<<"},chargeRation:"<<mcsi_chargeRation<<",chargeType:"<<mcsi_chargeType<<",chargeSource:"<<pmcu8_chargeSource<<",chargeLocation:"<<(int)mcu8_chargeLocation;
                        return CL_strStr.str();
                }


};
#endif
