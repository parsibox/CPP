#ifndef __STSUTIL_H
#define __STSUTIL_H

#include "STSSystem.h"
#include "IEData.h"
#include<string.h>
class CSTSUtil
{
public:
	static int mcfn_replaceSpecialChar(u8 * deststr, u8 * sourcestr,u8 srclen);
	static int mcfn_restoreSpecialChar(u8 * deststr, u8 * sourcestr,int srclen);
	static void mcfn_char2Hex(unsigned char ch, char* szHex);
	static void mcfn_hex2Char(char const* szHex, unsigned char& rch);
	static void mcfn_hexCharStr2Str(unsigned char const* pucCharStr, char* pszHexStr);
	static void mcfn_charStr2HexStr(unsigned char const* pucCharStr, char* pszHexStr, int iSize);
	static int mcfn_hexStr2CharStr(char const* pszHexStr, unsigned char* pucCharStr, int iSize);
	static void mcfn_encodeBCDValue(char *pscL_Address,CIEData &CL_IEData,u8 u8L_Filler = 0x0f);
	static void mcfn_encodeBCDValue(CIEData &CL_Address,CIEData &CL_IEData,u8 u8L_Filler = 0x0f);
	static void mcfn_encodeBCDValue(u8 *pu8L_Address, u8 u8L_AddrLen, CIEData &CL_IEData,u8 u8L_Filler = 0x0f);
	static int mcfn_encodeBCDValue(u8 *pu8L_Address, u8 u8L_AddrLen, u8 * u8L_IEData,u8 u8L_Filler= 0x0f);
	static int mcfn_decodeBCDValue(CIEData &CL_IEData, u8 *pu8L_Value,u8 u8L_Filler= 0x0f);
	static int mcfn_decodeBCDValue(CIEData &CL_IEData, CIEData &CL_Value,u8 u8L_Filler= 0x0f);
	static int mcfn_decodeBCDValue(u8 *pu8L_Pptr, int siL_DataLen, CIEData &CL_Value,u8 u8L_Filler=0x0f);
	static int mcfn_decodeBCDValue(u8 *pu8L_Pptr, int siL_DataLen, u8 *pu8L_Value,u8 u8L_Filler = 0x0f);
	static u8  mcfn_convert8BitTo7BitUSSD(u8 *pu8L_DstParam,u8 *pu8L_SrcParam,u8 u8len);
	static void mcfn_convert7BitTo8BitUSSD(u8 * u8L_Param, u8 u8L_ParamLen, u8 * pscL_Param, u8 * u8L_Len);
	static u8  mcfn_convert8BitTo7Bit(u8 *pu8L_DstParam,u8 *pu8L_SrcParam,u8 u8len);
	static void mcfn_convert7BitTo8Bit(u8 * u8L_Param, u8 u8L_ParamLen, u8 * pscL_Param, u8 * u8L_Len);
	static bool mcfn_wildMatch(char * pscL_Pattern,char * pscL_String); 
	static bool mcfn_removeStrg(char * pscL_String,char * pscL_ByteStng,int siL_Size); 
	static bool mcfn_getLangString(char* pscL_UD, char* pscL_FinalUD);	
	static int mcfn_UCS2TOUTF8(int, unsigned char*);
	static void mcfn_trim(std::string&);
	static void mcfn_trim(char*);
	static void mcfn_ByteToHex(char* cL_out,u8* cL_source,int iL_length);
};

#endif

