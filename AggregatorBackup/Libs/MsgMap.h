/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  File name									:: MsgMap.h
//  Purpose				      					:: Ineterface for CMsgMap class.
//
//  Classes included							:: 
//           Name									 Purpose
//	        -------									-----------
//			CMsgMap								To store the Messages in (key,Value) pair
//
//  Development History						     ::
// 
//   Date          Author								Description
//   -----------   -----------------------			--------------------------------------------------------------     
//   08/08/2003        Sahoo TM						Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#if !defined(AFX_MSGMAP_H__966EAF0A_4F62_4EEE_B30E_6D6348638594__INCLUDED_)
#define AFX_MSGMAP_H__966EAF0A_4F62_4EEE_B30E_6D6348638594__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "CriticalSection.h"
#include <map>
#include <algorithm>



template <class pt_Key, class pt_Val>
class CMsgMap  
{

public:
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A Constructor 
///	For initializing the message Map
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CMsgMap();

	
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// A Destructor for message Map
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	virtual ~CMsgMap();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Inserts messages into the Map
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool mcfb_insert (const pt_Key&, const pt_Val& );

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Get the Number of messages in the Map
/// @return int, the result (success/Failure)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	int mcfi_getCount();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Search the element(key) in the Map
/// @return bool, the result (found/not found)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool mcfb_findElement(const pt_Key& p_element, pt_Val& p_val);
	//int mfb_GetCount();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Get the element(key) in the Map remove it from the Map
/// @return bool, the result (found/not found)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool mcfb_getElement(const pt_Key& p_element, pt_Val& p_val);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Remove the element(key) in the Map remove it from the Map
/// @return bool, the result (found/not found)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool mcfb_removeElement(const pt_Key& p_element);
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Get the next element(key) in the Map don't remove it from the Map
/// @return bool, the result (found/not found)
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool mcfb_getNextElement( pt_Key& p_element, pt_Val& p_val);
	void mcf_startGet();
	void mcf_resetItr();
	bool mcfb_isEmpty(){
		return meC_Map.empty();
	}
	void mcf_clear()
	{
		meC_Map.clear();
	}
private:
	CCriticalSection meC_LockObj;	/// Variable for Critical Section
	
	std::map<pt_Key, pt_Val> meC_Map;			/// List for storing SS7 messages 
	typename std::map<pt_Key,pt_Val>::iterator mC_Iterator;
};
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgMap:CMsgMap()
//
//  Purpose      	:: Constructor
//
//  Input          	:: NONE
//
//  Output			:: NONE
//
//  Return Value	:: NONE
//
//	Notes			::
//
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class pt_Key, class pt_Val>
CMsgMap<pt_Key,pt_Val>::CMsgMap()
{
	//mC_Iterator = NULL;
}



//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgMap::~CMsgMap()
//
//  Purpose      	:: Destructor. 
//
//  Input          	:: NONE
//
//  Output			:: NONE
//
//  Return Value	:: NONE
//
//	Notes			::
//
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class pt_Key, class pt_Val>
CMsgMap<pt_Key,pt_Val>::~CMsgMap()
{

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgMap:: mcfb_insertIntoList
//
//  Purpose      	:: Addds an element (T) to the list
//
//  Input          	:: 1. T object Reference
//
//  Output			:: NONE
//
//  Return Value	:: Bool (success/failure) of the Operation
//
//	Notes			::
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

template <class pt_Key, class pt_Val>
bool CMsgMap<pt_Key,pt_Val>::mcfb_insert(const pt_Key& p_k, const pt_Val& p_element)
{
	
	//std::pair<pt_Key, pt_Val> lC_val(p_k,p_element);
	typename std::map<pt_Key, pt_Val>::value_type lC_Val(p_k,p_element);
	//lC_val.first = p_k;
	//lC_val.second = p_element;

	meC_LockObj.Lock();

	meC_Map.insert(lC_Val);
	
	meC_LockObj.Unlock();
	return true;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgMap:: mfb_GetCount
//
//  Purpose      	:: Counts the number of messages in the list
//
//  Input          	:: None
//
//  Output			:: Size of the message list
//
//  Return Value	:: Integer
//
//	Notes			::
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class pt_Key, class pt_Val>
int CMsgMap<pt_Key,pt_Val>::mcfi_getCount()
{
	meC_LockObj.Lock();
	int li_size = meC_Map.size();
	meC_LockObj.Unlock();

	return li_size;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgMap:: mcfb_findElement
//
//  Purpose      	:: Finds  an element (T) from the list
//						Doesn't remove the element from Map
//
//  Input          	:: 1. pt_Key object Reference
//					   2. pt_Val object Reference
//
//  Output			:: NONE
//
//  Return Value	:: Bool (success/failure) of the Operation
//
//	Notes			::
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class pt_Key, class pt_Val>
bool CMsgMap<pt_Key,pt_Val>::mcfb_findElement(const pt_Key& p_element, pt_Val& p_val)
{
	meC_LockObj.Lock();
	typename std::map<pt_Key,pt_Val>::iterator l_Itr= meC_Map.find(p_element);
	if(l_Itr != meC_Map.end())
	{
		p_val = meC_Map[p_element];
		meC_LockObj.Unlock();
		return true;
	}
	else
	{
		meC_LockObj.Unlock();
		return false;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgMap:: mcfb_getElement
//
//  Purpose      	:: Finds  an element (T) from the list
//					   Remove the element from Map
//
//  Input          	:: 1. pt_Key object Reference
//					   2. pt_Val object Reference
//
//  Output			:: NONE
//
//  Return Value	:: Bool (success/failure) of the Operation
//
//	Notes			::
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class pt_Key, class pt_Val>
bool CMsgMap<pt_Key,pt_Val>::mcfb_getElement(const pt_Key& p_element, pt_Val& p_val)
{
	meC_LockObj.Lock();
	typename std::map<pt_Key,pt_Val>::iterator l_Itr= meC_Map.find(p_element);
	if(l_Itr != meC_Map.end())
	{
		p_val = meC_Map[p_element];
		meC_Map.erase(p_element);
		meC_LockObj.Unlock();
		return true;
	}
	else
	{
		meC_LockObj.Unlock();
		return false;
	}
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgMap:: mcfb_removeElement
//
//  Purpose      	:: Finds  an element (T) from the list
//					   Remove the element from Map
//
//  Input          	:: 1. pt_Key object Reference
//
//
//  Output			:: NONE
//
//  Return Value	:: Bool (success/failure) of the Operation
//
//	Notes			::
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


template <class pt_Key, class pt_Val>
bool CMsgMap<pt_Key,pt_Val>::mcfb_removeElement(const pt_Key& p_element)
{
	meC_LockObj.Lock();
	typename std::map<pt_Key,pt_Val>::iterator l_Itr = meC_Map.find(p_element);
	if(l_Itr != meC_Map.end())
	{
		meC_Map.erase(p_element);
		meC_LockObj.Unlock();
		return true;
	}
	else
	{
		meC_LockObj.Unlock();
		return false;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgMap:: mcf_startGet
//
//  Purpose      	:: Prepares the the map to getNext
//					  
//
//  Input          	::NONE
//
//  Output			:: NONE
//
//  Return Value	:: NONE
//
//	Notes			::
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class pt_Key, class pt_Val>
void CMsgMap<pt_Key,pt_Val>::mcf_startGet()
{
	meC_LockObj.Lock();
	mC_Iterator= meC_Map.begin();
	meC_LockObj.Unlock();
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgMap:: mcf_resetItr
//
//  Purpose      	:: Prepares the the map to getNext
//					  
//
//  Input          	::NONE
//
//  Output			:: NONE
//
//  Return Value	:: NONE
//
//	Notes			::
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class pt_Key, class pt_Val>
void CMsgMap<pt_Key,pt_Val>::mcf_resetItr()
{
	meC_LockObj.Lock();
	mC_Iterator= meC_Map.begin();
	meC_LockObj.Unlock();
}
////
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//  Name 			:: CMsgMap:: mcfb_getNextElement
//
//  Purpose      	:: Finds  an element (T) from the list
//					   Remove the element from Map
//
//  Input          	:: 1. pt_Key object Reference
//					   2. pt_Val object Reference
//
//  Output			:: NONE
//
//  Return Value	:: Bool (success/failure) of the Operation
//
//	Notes			::
//	Modifications	::	Date					Author				Description
//						------					-------------		----------------
//						08/08/2003 12:00:00		Sahoo TM			Created.
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
template <class pt_Key, class pt_Val>
bool CMsgMap<pt_Key,pt_Val>::mcfb_getNextElement(pt_Key& p_element, pt_Val& p_val)
{
	//std::map<pt_Key,pt_Val>::iterator l_it= meC_Map.begin();
	meC_LockObj.Lock();
	if(mC_Iterator != meC_Map.end())
	{
		p_element = mC_Iterator->first;
		p_val = mC_Iterator->second;
		mC_Iterator++;
		//meC_Map.erase(p_element);
		meC_LockObj.Unlock();
		return true;
	}
	else
	{
		meC_LockObj.Unlock();
		return false;
	}
}


#endif // !defined(AFX_MSGLIST_H__966EAF0A_4F62_4EEE_B30E_6D6348638594__INCLUDED_)
