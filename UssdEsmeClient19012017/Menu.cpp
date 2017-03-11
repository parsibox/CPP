#include"Menu.hpp"
#include"DBOperations.h"
std::string CMenu::mcfnS_getRootMenuData(std::string CL_userName,std::string CL_serviceCode,std::string CL_msg,Session *&pCL_session){
	CMenuTree* pCL_menuTree=NULL;	
	bool bL_IsLeaf=false;//not in use
	char pscL_tempOutBuffer[400];
	char pscL_OutBuffer[400];
	
	memset(pscL_tempOutBuffer,0,400);
	memset(pscL_OutBuffer,0,400);
	if(pCL_session->pmcv_ActiveNode==NULL){
		if(mcfnS_getMenuNode(CL_userName,CL_serviceCode,pCL_menuTree)==false) return "";
		pCL_session->pmcv_ActiveNode=pCL_menuTree->mcfn_getMenuTreeRoot();
		pCL_menuTree->mcfn_getMenuList(pCL_session->pmcv_ActiveNode,pscL_tempOutBuffer,&bL_IsLeaf);
		sprintf(pscL_OutBuffer,"%s\n%s",CL_msg.c_str(),pscL_tempOutBuffer);
		return pscL_OutBuffer;
	}	
	return "";	
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

std::string CMenu::mcfnS_getLeafData(Session *&pCL_session,std::string CL_input,std::string CL_userName,std::string CL_serviceCode,bool &bL_IsLeaf,std::string &CL_url ){
	CMenuTree* pCL_menuTree=NULL;
	MenuTreeNode *pSL_MenuNode=(MenuTreeNode*)pCL_session->pmcv_ActiveNode;
	char pscL_OutBuffer[400]={0x00};
	bL_IsLeaf=true;
	if(mcfnS_getMenuNode(CL_userName,CL_serviceCode,pCL_menuTree)==false) return "";

		if(!pCL_menuTree->mcfn_checkForLastNode(pCL_session->pmcv_ActiveNode))
		{
			int siL_MenuLink= mcfnS_charToLong(CL_input.c_str(),CL_input.length());
			DBG_INFO((CG_EventLog),("MenuLink=%d",siL_MenuLink));
			if (!((0==siL_MenuLink) ||(siL_MenuLink >=MAX_SUB_MENU))){
				pSL_MenuNode=pSL_MenuNode->link[siL_MenuLink];
				
			}
			else
				pSL_MenuNode=NULL;
			DBG_INFO((CG_EventLog),("MenuLink=%x",pSL_MenuNode));
			if(NULL!=pSL_MenuNode)
			{
				pCL_session->pmcv_ActiveNode=(void*)pSL_MenuNode;
				/*if(strcmp(pSL_MenuNode->description,"Back")==0)
				{
					MenuTreeNode *pSL_ActiveNode=(MenuTreeNode*)pCL_session->pmcv_ActiveNode;
					pSL_ActiveNode =pSL_ActiveNode->previousNode->previousNode;
					pCL_session->pmcv_ActiveNode=(void*)pSL_ActiveNode;
				}*/
				 bL_IsLeaf=false;
				CL_url=pCL_menuTree->mcfn_getMenuList(pCL_session->pmcv_ActiveNode,pscL_OutBuffer,&bL_IsLeaf);
				DBG_INFO((CG_EventLog),("Returing :%s",pscL_OutBuffer));
				return pscL_OutBuffer;
			}
			if ((0==siL_MenuLink)||(siL_MenuLink >=MAX_SUB_MENU))
			{
				DBG_INFO((CG_EventLog),("UD =Invalid id received;Thank You For using our services!"));
				return "Invalid id received;Thank You For using our services!";
			}
			else
			{
				DBG_INFO((CG_EventLog),("UD =Thank You For using our services! -- also Hard coded 1"));
				return "Thank You For using our services!";
			}
		}
		else
		{
			DBG_INFO((CG_EventLog),("UD = Thank You For using our services! -- also Hard coded 2"));
			return "Thank You For using our services!";
		}
	return "";
}
bool CMenu::mcfnS_getMenuNode(std::string CL_userName,std::string CL_serviceCode,CMenuTree*& pCL_menuTree){
	CMsgMap<std::string,CMenuTree*>* pCL_sericeMenuMap=NULL;
	if(pCG_menuMap->mcfb_findElement(CL_userName,pCL_sericeMenuMap)==false){
		//TODO::need to load menu here for username
		if(mcfnS_loadMenuServiceMap(CL_userName,pCL_sericeMenuMap)==false){
			DBG_CRITICAL((CG_EventLog),("No menu created for username:%s",CL_userName.c_str()));
			return false;
		}
	}

	if(pCL_sericeMenuMap->mcfb_findElement(CL_serviceCode,pCL_menuTree)==false){
		if(mcfnS_loadMenuMap(CL_userName,CL_serviceCode,pCL_sericeMenuMap,pCL_menuTree)==false){
			DBG_CRITICAL((CG_EventLog),("No menu created for username:%s,serviceCode",CL_userName.c_str(),CL_serviceCode.c_str()));
			return false;
		}
	}
	return true;
}
bool CMenu::mcfnS_loadMenuServiceMap(std::string CL_username,CMsgMap<std::string,CMenuTree*> *&pCL_sericeMenuMap){
	 DBG_INFO((CG_EventLog),("Loading Menus for username:%s",CL_username.c_str()));
	 mysqlpp::StoreQueryResult CL_ResultSet;
	 CDBOperations CL_dbObj;
	 std::stringstream CL_QueryStream;
	 CL_QueryStream.str("");
	 CL_QueryStream<<"SELECT DISTINCT(menu_service_code) from menu_details WHERE USER_NAME='"<<CL_username<<"' AND STATUS='A'";
	 if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
		 if(CL_ResultSet.num_rows()>0){
			 pCL_sericeMenuMap=new CMsgMap<std::string,CMenuTree*>();
			 for(int i=0;i<CL_ResultSet.num_rows();i++){
				 CMenuTree* pCL_menuTree=NULL;
				 mcfnS_loadMenuMap(CL_username,CL_ResultSet[0]["menu_service_code"].c_str(),pCL_sericeMenuMap,pCL_menuTree);
			 }
			 pCG_menuMap->mcfb_insert(CL_username,pCL_sericeMenuMap);
			 return true;
		 }
		else{
			 DBG_CRITICAL((CG_EventLog),("No menu configured for username:%s",CL_username.c_str()));
		}
	 }
	 return false;
}
bool CMenu::mcfnS_loadMenuMap(std::string CL_username,std::string CL_seriveCode,CMsgMap<std::string,CMenuTree*> *&pCL_sericeMenuMap,CMenuTree *&pCL_menuTree){
	 DBG_INFO((CG_EventLog),("Loading Menu for username:%s,Service Code:%s",CL_username.c_str(),CL_seriveCode.c_str()));
         mysqlpp::StoreQueryResult CL_ResultSet;
         CDBOperations CL_dbObj;
         std::stringstream CL_QueryStream;
         CL_QueryStream.str("");
         CL_QueryStream<<"SELECT * from menu_details WHERE USER_NAME='"<<CL_username<<"' and MENU_SERVICE_CODE='"<<CL_seriveCode<<"' AND STATUS='A' order by parent_id";
         if(CL_dbObj.mcfn_ExecuteQuery(CL_QueryStream.str(),CL_ResultSet)==0){
                 if(CL_ResultSet.num_rows()>0){
			 pCL_menuTree=new CMenuTree();
			 MenuData        *pSL_Pptr       =       NULL;
			 MenuData        *pSL_Temp       =       NULL;
			 pSL_Temp=new MenuData[CL_ResultSet.num_rows()];
                         for(int i=0;i<CL_ResultSet.num_rows();i++){
				 pSL_Pptr=pSL_Temp+i;
				 if(pSL_Pptr!=NULL){
					 pSL_Pptr->mcsl_MenuId           =       atoi(CL_ResultSet[i]["menu_id"].c_str());
					 pSL_Pptr->mcsl_ParentId         =       atoi(CL_ResultSet[i]["parent_id"].c_str());
					 pSL_Pptr->mcsl_Order            =       atoi(CL_ResultSet[i]["dtmf_id"].c_str());
					 pSL_Pptr->mcsc_NodeNature       =       'A';
					 memcpy(pSL_Pptr->mcsc_Description,CL_ResultSet[i]["message"].c_str(),MAX_DESC_LEN);
					 memcpy(pSL_Pptr->mcsc_Keyword,"NULL"/*CL_ResultSet[i]["keyword"].c_str()*/,MAX_KEYWORD_LEN);
					 memcpy(pSL_Pptr->mcsc_Msg,CL_ResultSet[i]["info_msg"].c_str(),MAX_UD_LEN);
					 pSL_Pptr->mcC_url.assign(CL_ResultSet[i]["url"].c_str());
					 pSL_Pptr->mcC_endMessage.assign(CL_ResultSet[i]["end_msg"].c_str());
					 memset(pSL_Pptr->mcsc_DestinationAddress,0x00,100);
					 DBG_CRITICAL((CG_EventLog),("MenuId:%d,ParentId:%d,Order:%d,NodeNature:%c,Description:%s,Keyword:%s,DestinationAddress:%s,Message:%s,Url:%s,EndMsg:%s",pSL_Pptr->mcsl_MenuId,pSL_Pptr->mcsl_ParentId  ,pSL_Pptr->mcsl_Order,pSL_Pptr->mcsc_NodeNature,pSL_Pptr->mcsc_Description,pSL_Pptr->mcsc_Keyword,pSL_Pptr->mcsc_DestinationAddress,pSL_Pptr->mcsc_Msg,pSL_Pptr->mcC_url.c_str(),pSL_Pptr->mcsc_Msg,pSL_Pptr->mcC_endMessage.c_str()));
				 }
                         }
		
			 pCL_menuTree->mcfn_createMenuTree(pSL_Temp,CL_ResultSet.num_rows());
			 pCL_sericeMenuMap->mcfb_insert(CL_seriveCode,pCL_menuTree);
			 delete []pSL_Temp;
                         return true;
                 }
                else{
                         DBG_CRITICAL((CG_EventLog),("No menu configured for username:%s,Service Code:%s",CL_username.c_str(),CL_seriveCode.c_str()));
                }
         }
         return false;
}

