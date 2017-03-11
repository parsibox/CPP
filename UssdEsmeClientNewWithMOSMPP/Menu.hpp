#ifndef __MENU_H__
#define __MENU_H__
#include"Externs.h"
class CMenu{
	public:
		static bool mcfnS_validateUserInput(std::string CL_input,int inputType);
		static std::string mcfnS_getRootMenuData(std::string& CL_userName,std::string& CL_serviceCode,std::string CL_msg,Session *&pCL_session);
		static int mcfnS_charToLong(const char* pscL_UD, int slL_UDLength);
		static std::string mcfnS_getLeafData(Session *&pCL_session,std::string CL_input,std::string CL_userName,std::string CL_serviceCode,bool &bL_IsLeaf,std::string &,bool&);
		static bool mcfnS_getMenuNode(std::string CL_userName,std::string CL_serviceCode,NodeTree*& pCL_menuTree);
		static bool mcfnS_loadMenuServiceMap(std::string CL_username,CMsgMap<std::string,NodeTree *>*& pCL_sericeMenuMap,NodeTree*&);
		static bool mcfnS_loadMenuMap(std::string CL_username,std::string CL_seriveCode,CMsgMap<std::string,NodeTree*> *&pCL_sericeMenuMap,NodeTree *&pCL_menuTree);	
};
#endif
