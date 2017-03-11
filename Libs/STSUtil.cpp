
/**
 * SixDEE Telecom Solutions Pvt. Ltd.
 * Copyright 2006
 * All Rights Reserved.
 */

#include "STSUtil.h"
#include "STSOSUtil.h"
#include <stdio.h>

#define MAXSPECIALCHAR 117
#define MAXSPECIALCHAR 117
unsigned char Char_Mapping[][2]=
{
	{64,0},
	{163,1},
	{36,2},
	{165,3},
	{232,4},
	{233,5},
	{249,6}, 
	{236,7},
	{242,8},
	{199,9},
	{10,10},
	{216,11},
	{248,12},
	{13,13},
	{197,14},
	{229,15},
	{95,17},
	{198,28},
	{230,29},
	{223,30},
	{201,31},
	{32,32},
	{33,33},
	{34,34},
	{35,35},
	{164,36},
	{37,37},
	{38,38},
	{39,39},
	{40,40},
	{41,41},
	{42,42},
	{43,43},
	{44,44},
	{45,45},
	{46,46},
	{47,47},
	{48,48},
	{49,49},
	{50,50},
	{51,51},
	{52,52},
	{53,53},
	{54,54},
	{55,55},
	{56,56},
	{57,57},
	{58,58},
	{59,59},
	{60,60},
	{61,61},
	{62,62},
	{63,63},
	{161,64},
	{65,65},
	{66,66},
	{67,67},
	{68,68},
	{69,69},
	{70,70},
	{71,71},
	{72,72},
	{73,73},
	{74,74},
	{75,75},
	{76,76},
	{77,77},
	{78,78},
	{79,79},
	{80,80},
	{81,81},
	{82,82},
	{83,83},
	{84,84},
	{85,85},
	{86,86},
	{87,87},
	{88,88},	
	{89,89},
	{90,90},
	{196,91},
	{214,92},
	{209,93},
	{220,94},
	{167,95},
	{191,96},
	{97,97},
	{98,98},
	{27,27},//added for `
	{96,27},//added for `
	{99,99},
	{100,100},
	{101,101},
	{102,102},
	{103,103},
	{104,104},
	{105,105},
	{106,106},
	{107,107},
	{108,108},
	{109,109},
	{110,110},
	{111,111},
	{112,112},
	{113,113},
	{114,114},
	{115,115},
	{116,116},
	{117,117},
	{118,118},
	{119,119},
	{120,120},
	{121,121},
	{122,122},
	{228,123},
	{246,124},
	{241,125},
	{252,126},
	{224,127},
};
#define MAXEXTCHAR 10
unsigned char Ext_Char_Mapping[][2]=
{
	{12,10},
	{94,20},
	{123,40},
	{125,41},
	{92,47},
	{91,60},
	{126,61},
	{93,62},
	{124,64},
	{164,101}
};


int CSTSUtil::mcfn_replaceSpecialChar(u8 * deststr, u8 * sourcestr,u8 srclen)
{
	int len = srclen;
	int dest_index = 0;
	bool Ischaged;
	int i=0,j=0;
	for(i = 0; i<len; i++)
	{
		Ischaged = false;
		//replacing extended characters
		for(j = 0;j < MAXEXTCHAR ;j++)
		{
			if(sourcestr[i] == Ext_Char_Mapping[j][0])
			{
				deststr[dest_index++] = 27;
				deststr[dest_index++] = Ext_Char_Mapping[j][1];					
				Ischaged = true;
			}
		}
		//replacing special characters
		if(!Ischaged)
		{
			for(j = 0;j < MAXSPECIALCHAR ;j++)
			{
				if(sourcestr[i] == Char_Mapping[j][0])
				{
					deststr[dest_index++] = Char_Mapping[j][1];
					Ischaged = true;
					break;			
				}
			}
		}
		//copying normal characters as it is
		if(!Ischaged) //copy the actual data here
			deststr[dest_index++] = sourcestr[i]; 
	}
	
	return dest_index;
}
int CSTSUtil::mcfn_restoreSpecialChar(u8 * deststr, u8 * sourcestr,int srclen)
{                                                                                                          
	int len = srclen;                                                               
	int dest_index = 0;                                                                                
	bool Ischaged;                                                                                     
	int i=0,j=0;                                                                                       
	for(i = 0; i<len; i++)                                                                             
	{                                                                                                  
		Ischaged = false;
		//restoring extended characters
		for(j = 0;j < MAXEXTCHAR ;j++)                                                             
		{                                                                                          
			if(sourcestr[i] == 27)
			{
				if(sourcestr[i+1] == Ext_Char_Mapping[j][1])                                         
				{                                                                                  
					deststr[dest_index++] = Ext_Char_Mapping[j][0];                                 
					i++;
					Ischaged = true;                                                           
				}
			}                                                                                  
		}                                                                                          
		if(!Ischaged)                                                                              
		{   //restoring special characters                                                                                       
			for(j = 0;j < MAXSPECIALCHAR ;j++)                                                 
			{                                                                                  
				if(sourcestr[i] == Char_Mapping[j][1])                                     
				{                                                                          
					deststr[dest_index++] = Char_Mapping[j][0];                        
					Ischaged = true;                                                   
					break;                                                             
				}                                                                          
			}                                                                                  
		}                                                                                          
		//copying normal characters as it is
		if(!Ischaged)
			deststr[dest_index++] = sourcestr[i];                                              
	}
	return dest_index;                                                                                 
}

void CSTSUtil::mcfn_char2Hex(unsigned char ch, char* szHex)
{
	unsigned char byte[2];
	byte[0] = ch/16;
	byte[1] = ch%16;
	for(int i=0; i<2; i++)
	{
		if((byte[i] <= 9))
			szHex[i] = '0' + byte[i];
		else 
			szHex[i] = 'A' + byte[i] - 10;
	}
	szHex[2] = 0;
}

//Function to convert string of length 2 to unsigned char
void CSTSUtil::mcfn_hex2Char(char const* szHex, unsigned char& rch)
{
	rch = 0;
	for(int i=0; i<2; i++)
	{
		if(*(szHex + i) >='0' && *(szHex + i) <= '9')
			rch = (rch << 4) + (*(szHex + i) - '0');
		else if(*(szHex + i) >='A' && *(szHex + i) <= 'F')
			rch = (rch << 4) + (*(szHex + i) - 'A' + 10);
		else if(*(szHex + i) >='a' && *(szHex + i) <= 'f')
			rch = (rch << 4) + (*(szHex + i) - 'a' + 10);
		else
			break;
	}
}

//Function to convert string of unsigned chars to string of chars
void CSTSUtil::mcfn_hexCharStr2Str(unsigned char const* pucCharStr, char* pszHexStr)
{
	int i,j;
	int iSize = strlen((char*)pucCharStr);
	pszHexStr[0] = 0;
	for(i=0,j=0; i<iSize;j++)
	{
		sscanf((char*)&pucCharStr[i],"%02x",&pszHexStr[j]);
		i = i+2;
	}
}   

//Function to convert string of unsigned chars to string of chars
void CSTSUtil::mcfn_charStr2HexStr(unsigned char const* pucCharStr, char* pszHexStr, int iSize)
{
	int i;
	char szHex[3];
	pszHexStr[0] = 0;
	for(i=0; i<iSize; i++)
	{
		CSTSUtil::mcfn_char2Hex(pucCharStr[i], szHex);
		strcat(pszHexStr, szHex);
	}
}

//Function to convert string of chars to string of unsigned chars
int CSTSUtil::mcfn_hexStr2CharStr(char const* pszHexStr, unsigned char* pucCharStr, int iSize)
{
	int i,j;
	unsigned char ch;
	for(i=0,j=0; i<iSize;j++)
	{
		CSTSUtil::mcfn_hex2Char(pszHexStr+ i, ch);
		pucCharStr[j] = ch;
		i = i+2;
	}
	return j;
}


void CSTSUtil::mcfn_encodeBCDValue(char *pscL_Address,CIEData &CL_IEData,u8 u8L_Filler)
{
	u8 *pu8L_Pptr = CL_IEData.pmcu8_Data;
	int i = 0;
	
	u8 u8L_Len = strlen(pscL_Address);
	bool bL_IsOdd = false;
	
	if(u8L_Len % 2)
	{
		bL_IsOdd = true;
		u8L_Len--;
	}
	for(i = 0; i < u8L_Len ; i++)
	{
		*pu8L_Pptr = (pscL_Address[i + 1] - '0') << 4;
		*pu8L_Pptr++ |= (pscL_Address[i++] - '0');
	}
	
	if(bL_IsOdd)
	{
		*pu8L_Pptr = (u8L_Filler << 4);
		*pu8L_Pptr |= (pscL_Address[i] - '0');
		u8L_Len += 2;
	}
	CL_IEData.mcu8_Len = u8L_Len / 2;
}

void CSTSUtil::mcfn_encodeBCDValue(CIEData &CL_Address,CIEData &CL_IEData,u8 u8L_Filler)
{
	u8 *pu8L_Pptr = CL_IEData.pmcu8_Data;
	int i = 0;
	
	u8 u8L_Len = CL_Address.mcu8_Len;
	bool bL_IsOdd = false;

	char *pscL_Address = (char*)CL_Address.pmcu8_Data;

	if(u8L_Len % 2)
	{
		bL_IsOdd = true;
		u8L_Len--;
	}
	for(i = 0; i < u8L_Len ; i++)
	{
		*pu8L_Pptr = (pscL_Address[i + 1] - '0') << 4;
		*pu8L_Pptr++ |= (pscL_Address[i++] - '0');
	}
	
	if(bL_IsOdd)
	{
		*pu8L_Pptr = (u8L_Filler << 4);
		*pu8L_Pptr |= (pscL_Address[i] - '0');
		u8L_Len += 2;
	}
	CL_IEData.mcu8_Len = u8L_Len / 2;
}

void CSTSUtil::mcfn_encodeBCDValue(u8 *pu8L_Address, u8 u8L_AddrLen, CIEData &CL_IEData,u8 u8L_Filler)
{
	u8 *pu8L_Pptr = CL_IEData.pmcu8_Data;
	
	u8 u8L_Len = u8L_AddrLen;
	bool bL_IsOdd = false;
        int i;

	if(u8L_Len % 2)
	{
		bL_IsOdd = true;
		u8L_Len--;
	}
	for( i = 0; i < u8L_Len ; i++)
	{
		*pu8L_Pptr = (pu8L_Address[i + 1] - '0') << 4;
		*pu8L_Pptr++ |= (pu8L_Address[i++] - '0');
	}

	if(bL_IsOdd)
	{
		*pu8L_Pptr = (u8L_Filler << 4);
		*pu8L_Pptr |= (pu8L_Address[i] - '0');
		u8L_Len += 2;
	}
	CL_IEData.mcu8_Len = u8L_Len / 2;
}
int CSTSUtil::mcfn_encodeBCDValue(u8 *pu8L_Address, u8 u8L_AddrLen, u8 * u8L_IEData,u8 u8L_Filler)
{
	u8 *pu8L_Pptr = u8L_IEData;
	
	u8 u8L_Len = u8L_AddrLen;
	bool bL_IsOdd = false;
        int i;

	if(u8L_Len % 2)
	{
		bL_IsOdd = true;
		u8L_Len--;
	}
	for( i = 0; i < u8L_Len ; i++)
	{
		*pu8L_Pptr = (pu8L_Address[i + 1] - '0') << 4;
		*pu8L_Pptr++ |= (pu8L_Address[i++] - '0');
	}

	if(bL_IsOdd)
	{
		*pu8L_Pptr = (u8L_Filler << 4);
		*pu8L_Pptr |= (pu8L_Address[i] - '0');
		u8L_Len += 2;
	}
	return(u8L_Len / 2);
}

int CSTSUtil::mcfn_decodeBCDValue(CIEData &CL_IEData, u8 *pu8L_Value,u8 u8L_Filler)
{
	int i = 0, j = 0;
	u8 *pu8L_Pptr = CL_IEData.pmcu8_Data;
	u8 u8L_Len    = CL_IEData.mcu8_Len; 
	u8L_Len--;
 	for( i = 0, j = 0; i < u8L_Len ; i++)
	{
		pu8L_Value[j++] = (*pu8L_Pptr & 0x0f)+ '0';
		pu8L_Value[j++] = (*pu8L_Pptr++ >> 4) + '0';
	}
	if((*pu8L_Pptr >> 4) == u8L_Filler)
	{
		pu8L_Value[j++] = (*pu8L_Pptr & 0x0f) + '0';
	}
	else
	{
		pu8L_Value[j++] = (*pu8L_Pptr & 0x0f)+ '0';
		pu8L_Value[j++] = (*pu8L_Pptr++ >> 4) + '0';
	}
	pu8L_Value[j] = '\0';
	return j;
}

int CSTSUtil::mcfn_decodeBCDValue(CIEData &CL_IEData, CIEData &CL_Value,u8 u8L_Filler)
{
	int i = 0, j = 0;
	u8 *pu8L_Pptr = CL_IEData.pmcu8_Data;
	u8 u8L_Len    = CL_IEData.mcu8_Len; 
	u8L_Len--;
	
	u8 *pu8L_Value = CL_Value.pmcu8_Data;

 	for( i = 0, j = 0; i < u8L_Len ; i++)
	{
		pu8L_Value[j++] = (*pu8L_Pptr & 0x0f)+ '0';
		pu8L_Value[j++] = (*pu8L_Pptr++ >> 4) + '0';
		if(j+2 > CL_Value.mcu8_DataSize)
			break;
	}
	if((*pu8L_Pptr >> 4) == u8L_Filler)
	{
		pu8L_Value[j++] = (*pu8L_Pptr & 0x0f) + '0';
	}
	else
	{
		pu8L_Value[j++] = (*pu8L_Pptr & 0x0f)+ '0';
		pu8L_Value[j++] = (*pu8L_Pptr++ >> 4) + '0';
	}
	pu8L_Value[j] = '\0';
	CL_Value.mcu8_Len = j;
	return j;
}

int CSTSUtil::mcfn_decodeBCDValue(u8 *pu8L_Pptr, int siL_DataLen, CIEData &CL_Value,u8 u8L_Filler)
{
	int i = 0, j = 0;
	u8 u8L_Len    = siL_DataLen; 
	u8L_Len--;
	
	u8 *pu8L_Value = CL_Value.pmcu8_Data;

 	for( i = 0, j = 0; i < u8L_Len ; i++)
	{
		pu8L_Value[j++] = (*pu8L_Pptr & 0x0f)+ '0';
		pu8L_Value[j++] = (*pu8L_Pptr++ >> 4) + '0';
		if(j+2 > CL_Value.mcu8_DataSize)
			break;
	}
	if((*pu8L_Pptr >> 4) == u8L_Filler)
	{
		pu8L_Value[j++] = (*pu8L_Pptr & 0x0f) + '0';
	}
	else
	{
		pu8L_Value[j++] = (*pu8L_Pptr & 0x0f)+ '0';
		pu8L_Value[j++] = (*pu8L_Pptr++ >> 4) + '0';
	}
	pu8L_Value[j] = '\0';
	CL_Value.mcu8_Len = j;
	return j;
}

int CSTSUtil::mcfn_decodeBCDValue(u8 *pu8L_Pptr, int siL_DataLen, u8 *pu8L_Value,u8 u8L_Filler)
{
	u8 u8L_Len    = siL_DataLen; 
	u8L_Len--;
	int i, j;

	for( i = 0, j = 0; i < u8L_Len ; i++)
	{
		pu8L_Value[j++] = (*pu8L_Pptr & 0x0f)+ '0';
		pu8L_Value[j++] = (*pu8L_Pptr++ >> 4) + '0';
	}
	if((*pu8L_Pptr >> 4) == u8L_Filler)
	{
		pu8L_Value[j++] = (*pu8L_Pptr & 0x0f) + '0';
	}
	else
	{
		pu8L_Value[j++] = (*pu8L_Pptr & 0x0f)+ '0';
		pu8L_Value[j++] = (*pu8L_Pptr++ >> 4) + '0';
	}
	pu8L_Value[j] = '\0';
	return j;
}


void CSTSUtil::mcfn_ByteToHex(char* cL_out,u8* cL_source,int iL_length)
{
        //char cL_outData[400];
        memset(cL_out,0x00,sizeof(cL_out));
        //DBG_VERBOSE((CG_EventLog),("Entering into fnG_ByteToHex"));
        //cL_out.clear();
        for(int cL_i=0;cL_i < iL_length;cL_i++)
        {
                sprintf(cL_out,"%s%02x",cL_out,*cL_source++);
        }
        //cL_out.assign(cL_outData);
        //DBG_VERBOSE((CG_EventLog),("ByteToHex:::%s",cL_out));
}

u8 CSTSUtil::mcfn_convert8BitTo7BitUSSD(u8 *pu8L_DstParam,u8 *pu8L_SrcParam,u8 u8len)
{
	if(u8len == 0x00)
		return 0x00;
	unsigned char andbits[] = { 0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f};
	//u8 u8len = strlen(pu8L_SrcParam);
	unsigned char pscL_GSMBit[1024];
	memset(pscL_GSMBit,0x00,sizeof(pscL_GSMBit));
	if((pu8L_DstParam == NULL)|| (pu8L_SrcParam == NULL))
		return 0;
	if(u8len <=0)
		return 0;
	if(pu8L_SrcParam[u8len-1] == 0x00)
		u8len--;
	//replacing special characters with their GSM 7bit value Refer GSM 03.8 Doc
	u8len = mcfn_replaceSpecialChar(pscL_GSMBit,(u8*)pu8L_SrcParam,u8len);
	u8 value = 0x00;
	u8 * param = pu8L_DstParam;
	
	u8 i = 0x00 , j = 0x00 , n = 0x00;
	u8 k;
	while(i < u8len)
	{
		param[j] = pscL_GSMBit[i] >> n;
		i++;
		k = pscL_GSMBit[i] & andbits[n];
		++n;
		k = k << (8 - n);
		param[j] |= k;
		j++;
		//param[j++] |= ((pscL_param[i] & andbits[n]) << ( 8 - ++n));
		if(n > 6)
		{
			i++;
			n  = 0x00;
		}
	}
	/*	If <CR> is intended to be the last character and 
	*	the message (including the wanted <CR>) ends on an octet boundary,
	*	then another <CR> must be added together with a padding bit 0.
	*	detail in 3.38 pg.no.15
	*/
	if(((u8len%8) == 0)&&((param[j-1] & 0xfe) == 0x1a))
        {
                param[j] |= 0x0d;
		j++;
        }
        //correcting error of (8n-1) as defined in 3.38 pg.no.15
	if((((u8len+1)%8) == 0)&&((param[j-1] & 0xfe) == 0x00))
        {
                unsigned char orbits = 0x1a; //0001 1010                                                   
                param[j-1] |= orbits;                                                                        
        }
	return j;
}






void CSTSUtil::mcfn_convert7BitTo8BitUSSD(u8 * u8L_Param, u8 u8L_ParamLen, u8 * pscL_Param, u8 * u8L_Len)
{
	unsigned char andbits[] = { 0x7f, 0x3f, 0x1f,0x0f,0x07,0x03,0x01};
    	int k = 0x00 , n = 0x00 , i = 0, j = 0;
	unsigned char pscL_GSMBit[1024];
	memset(pscL_GSMBit,0x00,sizeof(pscL_GSMBit));
	
	if((u8L_Param == NULL)||(pscL_Param == NULL) ||(u8L_Len == NULL))
		return;
	if(u8L_ParamLen == 0x00)
		return;
	
	//removing 1 CR in case 2 CR are coming
	if(((u8L_ParamLen)%8==0) && (u8L_Param[u8L_ParamLen-1] == 0x0d) && ((u8L_Param[u8L_ParamLen-2]& 0xfe)== 0x1a))
	{
		u8L_ParamLen--;
	}
		
	while(i < u8L_ParamLen)
	{
		pscL_GSMBit[j++] = ((u8L_Param[i++] & andbits[n]) << n++ ) | k;
		k = u8L_Param[i-1] >> (8 - n);
		if(n > 6)
		{
			if( (i * 7) <= (u8L_ParamLen * 7))
				pscL_GSMBit[j++] = (u8L_Param[i - 1] >> 1) & 0x7f;
			k = 0x00;
			n = 0x00;
		}
	}
	pscL_GSMBit[j] = '\0';
	if(pscL_GSMBit[j-1] == 0x0d)
	{
		pscL_GSMBit[j-1] = '\0';
		j--;
	}
	//restoring special characters from their GSM 7bit value to Ascii value Refer GSM 03.8 Doc
	*u8L_Len = mcfn_restoreSpecialChar(pscL_Param,pscL_GSMBit,j);
}

u8 CSTSUtil::mcfn_convert8BitTo7Bit(u8 *pu8L_DstParam,u8 *pu8L_SrcParam,u8 u8len)
{
	unsigned char andbits[] = { 0x01,0x03,0x07,0x0f,0x1f,0x3f,0x7f};
	//u8 u8len = strlen(pu8L_SrcParam);
	unsigned char pscL_GSMBit[1024];
	memset(pscL_GSMBit,0x00,sizeof(pscL_GSMBit));
	//replacing special characters with their GSM 7bit value Refer GSM 03.8 Doc
	u8len = mcfn_replaceSpecialChar(pscL_GSMBit,(u8*)pu8L_SrcParam,u8len);
	u8 value = 0x00;
	u8 * param = pu8L_DstParam;
	
	u8 i = 0x00 , j = 0x00 , n = 0x00;
	u8 k;
	while(i < u8len)
	{
		param[j] = pscL_GSMBit[i] >> n;
		i++;
		k = pscL_GSMBit[i] & andbits[n];
		++n;
		k = k << (8 - n);
		param[j] |= k;
		j++;
		//param[j++] |= ((pscL_param[i] & andbits[n]) << ( 8 - ++n));
		if(n > 6)
		{
			i++;
			n = 0x00;
		}
	}
	return j;
}


void CSTSUtil::mcfn_convert7BitTo8Bit(u8 * u8L_Param, u8 u8L_ParamLen, u8 * pscL_Param, u8 * u8L_Len)
{
	unsigned char andbits[] = { 0x7f, 0x3f, 0x1f,0x0f,0x07,0x03,0x01};
    	int k = 0x00 , n = 0x00 , i = 0, j = 0;
	unsigned char pscL_GSMBit[1024];
	memset(pscL_GSMBit,0x00,sizeof(pscL_GSMBit));
	
	while(i < u8L_ParamLen)
	{
		pscL_GSMBit[j++] = ((u8L_Param[i++] & andbits[n]) << n++ ) | k;
		k = u8L_Param[i-1] >> (8 - n);
		if(n > 6)
		{
			if( (i * 7) <= (u8L_ParamLen * 7))
				pscL_GSMBit[j++] = (u8L_Param[i - 1] >> 1) & 0x7f;
			k = 0x00;
			n = 0x00;
		}
	}
	pscL_GSMBit[j] = '\0';
	//restoring special characters from their GSM 7bit value to Ascii value Refer GSM 03.8 Doc
	*u8L_Len = mcfn_restoreSpecialChar(pscL_Param,pscL_GSMBit,j);
}

/*

 *	A.The code described below is focused on the design of an algorithm to compare a pattern
 *	string containing wildcards (also known as metacharacters, global characters or
 *	joker characters), with a text string (typically a file name).

 *	B.RULES :-
 
 *	1.Character matching is case sensitive.

 *	2.The metacharacter '*' matches any sequence of zero or more characters.
 *	For instance "*.ZIP" matches ".zip", "A*.bbb" matches "A.bbb" or "Axyz.BBB". 
 
 *	3.The metacharacter '?' matches exactly one character unless that character
 *	is a period ('.'). 
 
 *	File name matching doesn't  follow the stupid rules used by the OS/2 WPS for 
 *	associations based on file extension, i.e. it does not consider as file extension
 *	what follows the first period, but what follows the last period in the file name.
 *	So "*.zip" matches both "aaa.zip" and "aaa.1.0.0.4.zip".

*/

bool CSTSUtil::mcfn_wildMatch(char * pscL_Pattern,char * pscL_String)
{
	char * pscL_StrIdx = NULL;
	char * pscL_StrTemp= NULL;
	bool bL_star = false;
	
LOOPSTART:
	for (pscL_StrIdx = pscL_String, pscL_StrTemp = pscL_Pattern; *pscL_StrIdx; ++pscL_StrIdx, ++pscL_StrTemp) 
	{
		switch (*pscL_StrTemp) 
		{
		case '?':
			if (*pscL_StrIdx == '.') 
				goto STARCHECK;
			break;
		case '*':
			bL_star = true;
            		pscL_String = pscL_StrIdx, pscL_Pattern = pscL_StrTemp;
			//removing extra *
			do { ++pscL_Pattern; } while (*pscL_Pattern == '*');
            
			if (!*pscL_Pattern) 
				return true;
            		goto LOOPSTART;
		default:
            		/*if (mapCaseTable[*pscL_StrIdx] != mapCaseTable[*pscL_StrTemp])*/
			if(CSTSOSUtil::mcfn_strncasecmp(pscL_StrIdx,pscL_StrTemp,1) != 0)
				goto STARCHECK;
            		break;
		} /* endswitch */
	} /* endfor */
	while (*pscL_StrTemp == '*') ++pscL_StrTemp;
		return (!*pscL_StrTemp);

	
STARCHECK:
	if (!bL_star) 
		return false;
	pscL_String++;
	goto LOOPSTART;
	
}


//Function which changes the userdata in other language(non english) to correct format:
bool CSTSUtil::mcfn_getLangString(char* pscL_UD, char* pscL_FinalUD)
{
        char pscL_Dup[1000];
        memset(pscL_Dup,0x00,sizeof(pscL_Dup));

	//validating length
	if(strlen(pscL_UD) > 999)
		return false;
	
	//taking copy
        strcpy(pscL_Dup,pscL_UD);
	//removing extra Bytes
	if(!mcfn_removeStrg("&#;",pscL_Dup,strlen(pscL_Dup)))
		return false;	

        int i = 0;
        int j = 0;
        int p = 0;
        while(i < strlen(pscL_Dup))
        {
                p = 0;
                if((pscL_Dup[i]>=0x30 && pscL_Dup[i]<=0x39) &&\
                                (pscL_Dup[i+1]>=0x30 && pscL_Dup[i+1]<=0x39) &&\
                                (pscL_Dup[i+2]>=0x30 && pscL_Dup[i+2]<=0x39) &&\
                                (pscL_Dup[i+3]>=0x30 && pscL_Dup[i+3]<=0x39))
                {
                        sscanf(&pscL_Dup[i],"%04d",&p);
                        sprintf(&pscL_FinalUD[j],"%04x",p);
                        i+=4;
                }
                else
                {
                        sscanf(&pscL_Dup[i],"%c",&p);
                        sprintf(&pscL_FinalUD[j],"%04x",p);
                        i+=1;

                }
                j+=4;
        }
	return true;
}

//Function which removes extra characters
bool CSTSUtil::mcfn_removeStrg(char * pscL_Delim,char * pscL_UD,int siL_Size)
{
	char pscL_Dup[1000];
        memset (pscL_Dup,0x00,sizeof(pscL_Dup));
	if(siL_Size >999)
		return false;	

        strcpy(pscL_Dup,pscL_UD);

        memset(pscL_UD,0x00,siL_Size);

        char * pscL_Token=strtok(pscL_Dup,pscL_Delim);
        if(strlen(pscL_Token))
                strcat(pscL_UD,pscL_Token);
        while (pscL_Token=strtok(NULL,pscL_Delim))
                strcat(pscL_UD,pscL_Token);
	return true;
}
/* Returns no. of bytes written or -1 if error. Adds a zero byte to
   the end of the string. */

int CSTSUtil::mcfn_UCS2TOUTF8 (int ucs2, unsigned char * utf8)
{
    if (ucs2 < 0x80) {
        utf8[0] = ucs2;
        utf8[1] = '\0';
        return 1;
    }
    if (ucs2 >= 0x80  && ucs2 < 0x800) {
        utf8[0] = (ucs2 >> 6)   | 0xC0;
        utf8[1] = (ucs2 & 0x3F) | 0x80;
        utf8[2] = '\0';
        return 2;
    }
    if (ucs2 >= 0x800 && ucs2 < 0xFFFF) {
        utf8[0] = ((ucs2 >> 12)       ) | 0xE0;
        utf8[1] = ((ucs2 >> 6 ) & 0x3F) | 0x80;
        utf8[2] = ((ucs2      ) & 0x3F) | 0x80;
        utf8[3] = '\0';
        return 3;
    }
    return -1;
}

void CSTSUtil::mcfn_trim(string& str)
{
	string::size_type pos = str.find_last_not_of(' ');
	if(pos != string::npos) {
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if(pos != string::npos) str.erase(0, pos);
	}
	else str.erase(str.begin(), str.end());
}
void CSTSUtil::mcfn_trim(char * pscL_Str)
{
	string str = pscL_Str;
	string::size_type pos = str.find_last_not_of(' ');
	if(pos != string::npos) {
		str.erase(pos + 1);
		pos = str.find_first_not_of(' ');
		if(pos != string::npos) str.erase(0, pos);
	}
	else str.erase(str.begin(), str.end());

	strcpy(pscL_Str,str.c_str());
}
