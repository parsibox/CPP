
/**
 * SixDEE Telecom Solutions Pvt. Ltd.
 * Copyright 2006
 * All Rights Reserved.
 */


#ifndef _IEDATA_H
#define _IEDATA_H

#include "STSSystem.h"
#include <string.h>
#include <string>
using namespace std;

class CIEData
{
public:
	u8 mcu8_Len;
	u8 *pmcu8_Data;
	u8 mcu8_DataSize;

public:
	CIEData(u8 u8L_Size);
	CIEData(const CIEData &);
	~CIEData();

	void operator = (const CIEData &);
	void operator = (u8);
	void operator = (const char*);
	void operator = (string &);
	void operator = (const u8*);
	bool operator != (u8);
	bool operator == (u8);
	bool operator == (const CIEData &);
	bool operator != (const CIEData &);
	friend bool operator < (const CIEData &,const CIEData &);
	CIEData operator + (CIEData &);

	void mcfn_decode(u8 *,u8);
	void mcfn_decodepp(u8 *,u8,u8);
	void mcfn_decodepp(const CIEData &,u8);
	void mcfn_decodepd(u8 *,u8,u8);
	void mcfn_decodepd(const CIEData &,u8);
	void mcfn_encode(u8 *&);
	void mcfn_encodeChar(char *,int);
	void mcfn_encodeString(std::string&) const;
	void mcfn_initialise();
	bool mcfn_isEmpty();
	u8 mcfn_getLength() const;
};

inline
bool CIEData::mcfn_isEmpty()
{
	return mcu8_Len == 0 ? true : false;
}

inline
void CIEData::operator = (u8 u8L_Value)
{
	pmcu8_Data[0] = u8L_Value;
	mcu8_Len  = 0x01;
}
inline
void CIEData::operator = (const char* pscL_Data)
{
	mcu8_Len  = strlen(pscL_Data);
	if(mcu8_Len > mcu8_DataSize)
		mcu8_Len = mcu8_DataSize-1;
	memcpy(pmcu8_Data,pscL_Data,mcu8_Len);
	pmcu8_Data[mcu8_Len] = 0x00;
}
inline
void CIEData::operator = (string & SL_Data)
{
	mcu8_Len  = SL_Data.length();
	if(mcu8_Len > mcu8_DataSize)
		mcu8_Len = mcu8_DataSize-1;
	memcpy(pmcu8_Data,SL_Data.c_str(),mcu8_Len);
	pmcu8_Data[mcu8_Len] = 0x00;
}
inline
void CIEData::operator = (const u8* pu8L_Data)
{
	mcu8_Len  = strlen((char*)pu8L_Data);
	if(mcu8_Len > mcu8_DataSize)
                mcu8_Len = mcu8_DataSize-1;
	memcpy(pmcu8_Data,pu8L_Data,mcu8_Len);
	pmcu8_Data[mcu8_Len] = 0x00;
}
inline
bool CIEData::operator == (u8 u8L_Value)
{
	return (pmcu8_Data[0] == u8L_Value);
}

inline
bool CIEData::operator != (u8 u8L_Value)
{
	return (pmcu8_Data[0] != u8L_Value);
}

inline
void CIEData::mcfn_initialise()
{
	memset(pmcu8_Data,0,mcu8_DataSize);
	mcu8_Len = 0;
}

inline
void CIEData::mcfn_encode(u8 *&pu8L_Ptr)
{
	*pu8L_Ptr++ = mcu8_Len;
	if(mcu8_Len)
	{
		memcpy(pu8L_Ptr,pmcu8_Data,mcu8_Len);
		pu8L_Ptr += mcu8_Len;
	}
}
inline
void CIEData::mcfn_encodeChar(char *pscL_Char,int siL_Size)
{
	if((siL_Size-1) >= mcu8_Len)
	{
		memcpy(pscL_Char,(char*)pmcu8_Data,mcu8_Len);
		pscL_Char[mcu8_Len] = '\0';
	}
	else
	{
		memcpy(pscL_Char,(char*)pmcu8_Data,siL_Size-1);
		pscL_Char[siL_Size] = '\0';
		
	}
}

inline
void CIEData::mcfn_encodeString(std::string &strL_Temp) const
{
	strL_Temp.assign((const char*)pmcu8_Data,mcu8_Len);
}

inline
u8 CIEData::mcfn_getLength() const
{
	return mcu8_Len;
}




#endif

