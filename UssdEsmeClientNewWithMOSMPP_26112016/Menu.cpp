#include"Menu.hpp"
#include"DBOperations.h"
#include"File_Interpretor.h"

std::string CMenu::mcfnS_getRootMenuData(std::string& CL_userName,std::string& CL_serviceCode,std::string CL_msg,Session *&pCL_session){
        try
	{
		
		bool bL_IsLeaf=false;//not in use
		bool bL_IsValidInput=false;//not in use
		std::string CL_tempUrl;
		pCL_session->mcsi_activeMenu =0; //root node
                std::vector<std::string> CL_ussdStrings;
               if(CL_userName.empty())
               {
                    MenuDetails *pCL_menuDetails;
		    if(false ==CG_moMenuMap.mcfb_findElement(CL_serviceCode,pCL_menuDetails)) 
		    {
			    DBG_CRITICAL((CG_EventLog),("service code is not configured:%s",CL_serviceCode.c_str()));
			    return "";
		    }
                    CL_userName = pCL_menuDetails->mcC_userName;
                    CL_serviceCode = pCL_menuDetails->mcC_menuServiceCode;
	            DBG_INFO((CG_EventLog),("MO serice configured is %s:%s",CL_userName.c_str(),CL_serviceCode.c_str()));
               }
	       else
	       {
		       CFileInterpretor::mcfn_GetFields(CL_serviceCode, CL_ussdStrings,"*",true);
		       if(CL_ussdStrings.empty()) 
		       {
			       DBG_CRITICAL((CG_EventLog),("service code vector is empty:%s",CL_serviceCode.c_str()));
			       return "";
		       }
	       }

	       std::string CL_out = mcfnS_getLeafData(pCL_session,"-1",CL_userName,CL_serviceCode,bL_IsLeaf,CL_tempUrl,bL_IsValidInput);
               for(int i=1;i<CL_ussdStrings.size();i++)
               {
               CL_out = mcfnS_getLeafData(pCL_session,CL_ussdStrings[i].c_str(),CL_userName,CL_serviceCode,bL_IsLeaf,CL_tempUrl,bL_IsValidInput);
               }
               return CL_out;
	}
	catch(const mysqlpp::Exception&  e)
        {
			DBG_CRITICAL((CG_EventLog),("Exception came  :%s",e.what()));
                         return "";
	}
}

int CMenu::mcfnS_charToLong(const char* pscL_UD, int slL_UDLength)
{
        int     slL_Number      =       0;
	if( slL_UDLength > 12 )
		slL_Number = 0x01;
	else
	{
		for(int siL_Iterator=0;siL_Iterator<slL_UDLength;siL_Iterator++)
			if(!isdigit(pscL_UD[siL_Iterator]))
				return 0;
		sscanf(pscL_UD, "%d", &slL_Number);
	}
	return slL_Number;
}

std::string CMenu::mcfnS_getLeafData(Session *&pCL_session,std::string CL_input,std::string CL_userName,std::string CL_serviceCode,bool &bL_IsLeaf,std::string &CL_url,bool &bL_isValidInput ){
	NodeTree* pCL_menuTree=NULL;
        bL_isValidInput = true;
	int iL_activeMenu = pCL_session->mcsi_activeMenu;
        if(iL_activeMenu == -1 ||
	   mcfnS_getMenuNode(CL_userName,CL_serviceCode,pCL_menuTree)==false) return "";
   
        CMsgMap<std::string,NodeData*>* pCL_nodeMenu = NULL;
	if(pCL_menuTree->nodeTree.mcfb_findElement(iL_activeMenu,pCL_nodeMenu) == false) 
	{
		DBG_CRITICAL((CG_EventLog),("Node tree not found :menu Id:%d,username:%s,serviceCode:%s",iL_activeMenu,CL_userName.c_str(),CL_serviceCode.c_str() ));
        }
        NodeData* pCL_nodeData = NULL;
	if(!(iL_activeMenu == 0 && CL_input.compare("-1")==0)) //not root menu
	{ 
		if(pCL_nodeMenu->mcfb_findElement(CL_input,pCL_nodeData) &&  pCL_nodeData->nextMenuId.compare("") == 0)
		{
			bL_IsLeaf = true;
			DBG_DEBUG((CG_EventLog),("Node Found:%s",pCL_nodeData->mcfn_ShortDebugString().c_str()));
			CL_url = pCL_nodeData->url.c_str();
                        pCL_session->mcsi_activeMenu = -1;
			return pCL_nodeData->endMsg.empty() ? "Thank You For using our services!": pCL_nodeData->endMsg;
		}
		bL_IsLeaf = false;
		if(pCL_nodeData != NULL)
		{ 
			iL_activeMenu = atoi(pCL_nodeData->nextMenuId.c_str());
		}
		else
                {
		 DBG_CRITICAL((CG_EventLog),("UserInput is invalid {input:%s,menu Id:%d,username:%s,serviceCode:%s}",CL_input.c_str(),iL_activeMenu,CL_userName.c_str(),CL_serviceCode.c_str() ));
                 bL_isValidInput = false;
                }
		if(false == pCL_menuTree->nodeTree.mcfb_findElement(iL_activeMenu,pCL_nodeMenu))
		{
		 DBG_CRITICAL((CG_EventLog),("Node tree not found :menu Id:%d,username:%s,serviceCode:%s",iL_activeMenu,CL_userName.c_str(),CL_serviceCode.c_str() ));
		}
	}
        std::string dtmf;
        std::stringstream CL_outStr;
        CL_outStr.str("");
        pCL_nodeMenu->mcf_startGet();
        while(pCL_nodeMenu->mcfb_getNextElement(dtmf,pCL_nodeData))
        {
    	 CL_outStr<<pCL_nodeData->nodeMesssage;
         CL_outStr<<dtmf<<".";
         CL_outStr<<pCL_nodeData->nodeData;
        }
        pCL_session->mcsi_activeMenu = iL_activeMenu; 
       return CL_outStr.str();
}

bool CMenu::mcfnS_getMenuNode(std::string CL_userName,std::string CL_serviceCode,NodeTree*& pCL_menuTree){
	CMsgMap<std::string,NodeTree*>* pCL_serviceMenuMap=NULL;
	if(pCG_menuMap->mcfb_findElement(CL_userName,pCL_serviceMenuMap)==false){
		//TODO::need to load menu here for username
		if(mcfnS_loadMenuServiceMap(CL_userName,pCL_serviceMenuMap,pCL_menuTree)==false){
			DBG_CRITICAL((CG_EventLog),("No menu created for username:%s",CL_userName.c_str()));
			return false;
		}
	}

	if(pCL_serviceMenuMap->mcfb_findElement(CL_serviceCode,pCL_menuTree)==false){
		if(mcfnS_loadMenuMap(CL_userName,CL_serviceCode,pCL_serviceMenuMap,pCL_menuTree)==false){
			DBG_CRITICAL((CG_EventLog),("No menu created for username:%s,serviceCode",CL_userName.c_str(),CL_serviceCode.c_str()));
			return false;
		}
	}
	return true;
}
bool CMenu::mcfnS_loadMenuServiceMap(std::string CL_username,CMsgMap<std::string,NodeTree*> *&pCL_serviceMenuMap,NodeTree*& pCL_nodeTree){
	 DBG_INFO((CG_EventLog),("Loading Menus for username:%s",CL_username.c_str()));
	 mysqlpp::StoreQueryResult CL_ResultSet;
	 CDBOperations CL_dbObj;
	 std::stringstream CL_QueryStream;
	 CL_QueryStream.str("");
	 CL_QueryStream<<"SELECT DISTINCT(menu_service_code) from menu_details WHERE USER_NAME='"<<CL_username<<"' AND STATUS='A'";
	 if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		 if(CL_ResultSet.num_rows()>0){
			 pCL_serviceMenuMap=new CMsgMap<std::string,NodeTree*>();
			 for(int i=0;i<CL_ResultSet.num_rows();i++){
				 mcfnS_loadMenuMap(CL_username,CL_ResultSet[0]["menu_service_code"].c_str(),pCL_serviceMenuMap,pCL_nodeTree);
			 }
			 pCG_menuMap->mcfb_insert(CL_username,pCL_serviceMenuMap);
			 return true;
		 }
		else{
			 DBG_CRITICAL((CG_EventLog),("No menu configured for username:%s",CL_username.c_str()));
		}
	 }
	 return false;
}
bool CMenu::mcfnS_loadMenuMap(std::string CL_username,std::string CL_seriveCode,CMsgMap<std::string,NodeTree*> *&pCL_serviceMenuMap,NodeTree*& pCL_nodeTree){
	DBG_INFO((CG_EventLog),("Loading Menu for username:%s,Service Code:%s",CL_username.c_str(),CL_seriveCode.c_str()));
	mysqlpp::StoreQueryResult CL_ResultSet;
	CDBOperations CL_dbObj;
	std::stringstream CL_QueryStream;
	CL_QueryStream.str("");
	CL_QueryStream<<"SELECT * from menu_details WHERE USER_NAME='"<<CL_username<<"' and MENU_SERVICE_CODE='"<<CL_seriveCode<<"' AND STATUS='A' order by menu_id";
	if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		if(CL_ResultSet.num_rows()>0){
			pCL_nodeTree = new NodeTree(); 
			for(int i=0;i<CL_ResultSet.num_rows();i++){
                                  
				CMsgMap<std::string,NodeData*> *pCL_treeMenu  = NULL;
                                int menuId = atoi(CL_ResultSet[i]["menu_id"].c_str());
                                if( false == pCL_nodeTree->nodeTree.mcfb_findElement(menuId,pCL_treeMenu))
                                {
                                  pCL_treeMenu  = new CMsgMap<std::string,NodeData*>();
                                  pCL_nodeTree->nodeTree.mcfb_insert(menuId,pCL_treeMenu);
                                }
				NodeData *pCL_nodeData = new NodeData();
				pCL_nodeData->nodeData.assign(CL_ResultSet[i]["message"].c_str());
				pCL_nodeData->nodeMesssage.assign(CL_ResultSet[i]["info_msg"].c_str());
				pCL_nodeData->url.assign(CL_ResultSet[i]["url"].c_str());
				pCL_nodeData->endMsg.assign(CL_ResultSet[i]["end_msg"].c_str());
				pCL_nodeData->nextMenuId.assign(CL_ResultSet[i]["next_menu_id"].c_str());
				pCL_treeMenu->mcfb_insert(CL_ResultSet[i]["dtmf_id"].c_str(),pCL_nodeData); 
				DBG_INFO((CG_EventLog),("Menu Id:%d,Dtmf:%s,Node{%s}",menuId,CL_ResultSet[i]["dtmf_id"].c_str(),pCL_nodeData->mcfn_ShortDebugString().c_str()));
			}
                        pCL_serviceMenuMap->mcfb_insert(CL_seriveCode,pCL_nodeTree);
			return true;
		}
		else{
			DBG_CRITICAL((CG_EventLog),("No menu configured for username:%s,Service Code:%s",CL_username.c_str(),CL_seriveCode.c_str()));
		}
	}
	return false;
}

