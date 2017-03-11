
/**
 * SixDEE Telecom Solutions Pvt. Ltd.
 * Copyright 2006
 * All Rights Reserved.
 */

#include "IEData.h"
#include "STSException.h"


CIEData::CIEData(u8 u8L_Size)
{
	pmcu8_Data = NULL;
	pmcu8_Data = new u8[u8L_Size];
	if(pmcu8_Data == NULL)
		throw STSGenException(1000,"Memory Allocation Failed");
	mcu8_DataSize = u8L_Size;
	mcfn_initialise();
}


CIEData::CIEData(const CIEData &CL_IEData)
{
	mcu8_DataSize = CL_IEData.mcu8_DataSize;
	pmcu8_Data = new u8[mcu8_DataSize];
	if(pmcu8_Data == NULL)
		throw STSGenException(1000,"Memory Allocation Failed");
	mcu8_Len = CL_IEData.mcu8_Len;
	memcpy(pmcu8_Data,CL_IEData.pmcu8_Data,mcu8_Len);
}

CIEData::~CIEData()
{
	delete []pmcu8_Data;
	pmcu8_Data = NULL;
}


void CIEData::operator = (const CIEData &CL_IEData)
{
	if(mcu8_DataSize == CL_IEData.mcu8_DataSize)
	{
		mcu8_Len = CL_IEData.mcu8_Len;
		memcpy(pmcu8_Data,CL_IEData.pmcu8_Data,mcu8_Len);
		pmcu8_Data[mcu8_Len] = 0x00; //Added just to make life more easier in case of strcpy

	}
	else
	{
		if(pmcu8_Data)
		{	
			delete [] pmcu8_Data;
			pmcu8_Data = NULL;
		}


		mcu8_DataSize = CL_IEData.mcu8_DataSize;
		pmcu8_Data = new u8[mcu8_DataSize];
		if(pmcu8_Data == NULL)
			throw STSGenException(1000,"Memory Allocation Failed");
		mcu8_Len = CL_IEData.mcu8_Len;
		memcpy(pmcu8_Data,CL_IEData.pmcu8_Data,mcu8_Len);
		pmcu8_Data[mcu8_Len] = 0x00; //Added just to make life more easier in case of strcpy
	}
}

void CIEData::mcfn_decode(u8 *pu8L_Ptr,u8 u8L_Len)
{
	if(u8L_Len > mcu8_DataSize)
		u8L_Len = mcu8_DataSize-1;
	mcu8_Len = u8L_Len;
	memcpy(pmcu8_Data,pu8L_Ptr,u8L_Len);
	pmcu8_Data[u8L_Len] = 0x00;
}

void CIEData::mcfn_decodepp(u8 *pu8L_Ptr,u8 u8L_Len,u8 u8L_Val)
{
	u8L_Len++;
	if(u8L_Len > mcu8_DataSize)
		u8L_Len = mcu8_DataSize-1;
	mcu8_Len = u8L_Len;
	pmcu8_Data[0] = u8L_Val;
	memcpy(&pmcu8_Data[1],pu8L_Ptr,u8L_Len-1);
	pmcu8_Data[u8L_Len] = 0x00;
}
void CIEData::mcfn_decodepp(const CIEData &CL_Val,u8 u8L_Val)
{
	u8 u8L_Len = CL_Val.mcu8_Len;
	u8L_Len++;
	if(u8L_Len > mcu8_DataSize)
		u8L_Len = mcu8_DataSize-1;
	mcu8_Len = u8L_Len;
	pmcu8_Data[0] = u8L_Val;
	memcpy(&pmcu8_Data[1],CL_Val.pmcu8_Data,u8L_Len-1);
	pmcu8_Data[u8L_Len] = 0x00;
}
void CIEData::mcfn_decodepd(u8 *pu8L_Ptr,u8 u8L_Len,u8 u8L_Val)
{
	if(u8L_Len > mcu8_DataSize)
		u8L_Len = mcu8_DataSize-1;
	if(pu8L_Ptr[0] == u8L_Val)
	{
		memcpy(pmcu8_Data,&pu8L_Ptr[1],u8L_Len-1);
		pmcu8_Data[u8L_Len-1] = 0x00;
		mcu8_Len = u8L_Len-1;
	}
	else
	{
		memcpy(pmcu8_Data,pu8L_Ptr,u8L_Len);
		pmcu8_Data[u8L_Len] = 0x00;
		mcu8_Len = u8L_Len;
	}
}
void CIEData::mcfn_decodepd(const CIEData &CL_Val,u8 u8L_Val)
{
	u8 u8L_Len = CL_Val.mcu8_Len;
	if(u8L_Len > mcu8_DataSize)
		u8L_Len = mcu8_DataSize-1;
	if(CL_Val.pmcu8_Data[0] == u8L_Val)
	{
		memcpy(pmcu8_Data,&CL_Val.pmcu8_Data[1],u8L_Len-1);
		pmcu8_Data[u8L_Len-1] = 0x00;
		mcu8_Len = u8L_Len-1;
	}
	else
	{	
		memcpy(pmcu8_Data,CL_Val.pmcu8_Data,u8L_Len);
		pmcu8_Data[u8L_Len] = 0x00;
		mcu8_Len = u8L_Len;
	}
}

CIEData CIEData::operator +(CIEData &CL_Val1)
{
	u8 u8L_Len = mcu8_Len + CL_Val1.mcu8_Len;
	CIEData CL_Val(u8L_Len + 1);

	CL_Val.mcu8_Len = u8L_Len;
	memcpy(CL_Val.pmcu8_Data,pmcu8_Data,mcu8_Len);
	memcpy(&CL_Val.pmcu8_Data[mcu8_Len],CL_Val1.pmcu8_Data ,CL_Val1.mcu8_Len); 
	return CL_Val;
}


bool CIEData::operator == (const CIEData & CL_IEData)
{
	if(this->mcu8_Len != CL_IEData.mcu8_Len)
		return false;

	if(memcmp(this->pmcu8_Data,CL_IEData.pmcu8_Data,mcu8_Len) == 0)
		return true;

	return false;
}


bool CIEData::operator != (const CIEData &CL_IEData)
{
	return !(*this == CL_IEData);
}

bool  operator < ( const CIEData &CL_IEData1,  const CIEData &CL_IEData2)
{
	if(CL_IEData1.mcu8_Len > CL_IEData2.mcu8_Len)
		return false;
	else if(CL_IEData1.mcu8_Len == CL_IEData2.mcu8_Len)
	{
		if((memcmp(CL_IEData1.pmcu8_Data,CL_IEData2.pmcu8_Data,CL_IEData1.mcu8_Len))>=0)
			return false;
	}
	return true;
}

