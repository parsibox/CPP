#ifndef __STSUSERTYPES_H
#define __STSUSERTYPES_H
#include <EventLog.h>
#include <STSMsgQue.h>
typedef unsigned char 	u8;
typedef unsigned short 	u16;
typedef unsigned int	u32;

#ifdef __64BIT__
typedef unsigned long u64;
#endif 


//Standard ByteStream type
struct SByteStream{
	u8 *pmcu8_Ptr;
	u32 mcu32_Len;
	SByteStream(u8* pu8L_Ptr, u32 u32L_Len){
		pmcu8_Ptr = pu8L_Ptr;
		mcu32_Len  = u32L_Len;
	}

	void mcfn_Release(){
		delete[] pmcu8_Ptr;
		mcu32_Len = 0x00;
		puts("Releasing");
	}
};

typedef CMsgQue<SByteStream> ByteStreamQ;


#endif //__STSUSERTYPES_H
