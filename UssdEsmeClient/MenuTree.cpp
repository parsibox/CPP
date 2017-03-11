#include "MenuTree.h"
//#include "UserTypes.h"
#include <stdlib.h>
#include <stdio.h>
#include "Defines.h"
#include<math.h>
#ifdef __LINUX__
char* itoa(int iL_value, char* pcL_str, int iL_len){
	memset(pcL_str,0x00,iL_len);
	sprintf(pcL_str,"%d",iL_value);
	return pcL_str;
}
#endif
typedef struct NodeData_tag
{
        char            Description[50];
        char            DestinationAddr[100];
        char            Keyword[MAX_UD_LEN];
        //char            Msg[MAX_UD_LEN];
        char            NodeNature;
} NodeData;

CMenuTree::CMenuTree()
{
	pmeS_startNode		=	new MenuTreeNode();
	memset(pmeS_startNode->description, 0x00, 50);
	memcpy(pmeS_startNode->description, (const void*)"main", strlen((const char*)"main"));
	pmeS_currentNode	=	pmeS_startNode;

}

CMenuTree::~CMenuTree()
{
	mefn_deleteMenuTree(&pmeS_startNode);
}


bool CMenuTree::mcfn_createMenuTree(MenuData* pSL_Root, int slL_ListLen)
{
	MenuTreeNode	*pSL_Temp = NULL;
	mefn_loadTree(pSL_Root, slL_ListLen, &pmeS_startNode, 0);

	int	i = 1;
	for(; i < MAX_SUB_MENU; i++)
	{
		if( pmeS_startNode->link[i] == NULL )
		{
		/*	pSL_Temp	= new MenuTreeNode;
			if( pSL_Temp == NULL )
			{
				return false;
			}

			pmeS_startNode->link[i] = pSL_Temp;
			
			strcpy(pSL_Temp->keyword, "NULL");
			strcpy(pSL_Temp->description, "Exit");
			strcpy(pSL_Temp->keyword, "NULL"); 
			pSL_Temp->nodeNature = 'A';
			strcpy(pSL_Temp->destinationAddress, "NULL");*/
			break;
		}
	}
	return true;
}



void CMenuTree::mefn_loadTree(MenuData *pSL_Root,int slL_ListLen, MenuTreeNode** pRoot,int slL_ParentId)
{
	int i = 0x00;
	for(; i < slL_ListLen; i++)
	{
		printf("load treee parentId:%d,Root ParentId:%d,.mcsl_Order:%d\n",slL_ParentId,pSL_Root[i].mcsl_ParentId,pSL_Root[i].mcsl_Order);
		if(slL_ParentId == pSL_Root[i].mcsl_ParentId)
		{
			(*pRoot)->link[pSL_Root[i].mcsl_Order]  = new MenuTreeNode;

			if((*pRoot)->link[pSL_Root[i].mcsl_Order] == NULL)
			{
				return;
			}

			strcpy((*pRoot)->link[pSL_Root[i].mcsl_Order]->description, pSL_Root[i].mcsc_Description);
			strcpy((*pRoot)->link[pSL_Root[i].mcsl_Order]->keyword, pSL_Root[i].mcsc_Keyword); 
			strcpy((*pRoot)->link[pSL_Root[i].mcsl_Order]->message, pSL_Root[i].mcsc_Msg); 
			(*pRoot)->link[pSL_Root[i].mcsl_Order]->nodeNature = pSL_Root[i].mcsc_NodeNature;
			strcpy((*pRoot)->link[pSL_Root[i].mcsl_Order]->destinationAddress, pSL_Root[i].mcsc_DestinationAddress);
			(*pRoot)->link[pSL_Root[i].mcsl_Order]->previousNode	= (*pRoot);
			(*pRoot)->link[pSL_Root[i].mcsl_Order]->url.assign(pSL_Root[i].mcC_url.c_str()); 
			(*pRoot)->link[pSL_Root[i].mcsl_Order]->endMsg.assign(pSL_Root[i].mcC_endMessage.c_str()); 
	
			/*if( slL_ParentId != 0 )
			{
				(*pRoot)->link[pSL_Root[i].mcsl_Order+1]  = new MenuTreeNode;	
				strcpy((*pRoot)->link[pSL_Root[i].mcsl_Order+1]->description, "Back");
				strcpy((*pRoot)->link[pSL_Root[i].mcsl_Order+1]->keyword, "NULL"); 
				(*pRoot)->link[pSL_Root[i].mcsl_Order+1]->nodeNature = 'A';
				strcpy((*pRoot)->link[pSL_Root[i].mcsl_Order+1]->destinationAddress, "NULL");
				(*pRoot)->link[pSL_Root[i].mcsl_Order+1]->previousNode	= (*pRoot);
				
				(*pRoot)->link[pSL_Root[i].mcsl_Order+2]  = new MenuTreeNode;	
				strcpy((*pRoot)->link[pSL_Root[i].mcsl_Order+2]->description, "Exit");
				strcpy((*pRoot)->link[pSL_Root[i].mcsl_Order+2]->keyword, "NULL"); 
				(*pRoot)->link[pSL_Root[i].mcsl_Order+2]->nodeNature = 'A';
				strcpy((*pRoot)->link[pSL_Root[i].mcsl_Order+2]->destinationAddress, "NULL");
				(*pRoot)->link[pSL_Root[i].mcsl_Order+2]->previousNode	= (*pRoot);
			}*/

			if(strcmp(pSL_Root[i].mcsc_Keyword,"NULL") == 0)
			{
				printf("Keyword is NULL so again calling load tree\n");
				
				mefn_loadTree(pSL_Root, slL_ListLen, &(*pRoot)->link[pSL_Root[i].mcsl_Order], pSL_Root[i].mcsl_MenuId);
			}
		}
	}
}


void CMenuTree::mefn_deleteMenuTree(MenuTreeNode** pSL_Start)
{
	MenuTreeNode*	pSL_Temp	=	NULL;
	int i = 0;
	
	pSL_Temp	=	*pSL_Start;
	
	if( pSL_Temp == NULL )
	{
		printf("Partial Tree allocation..\n");
		return;
	}
	
	for( i = 0; i < 10; i++ )
	{
		if( pSL_Temp->link[i] != NULL )
		{
			mefn_deleteMenuTree(&pSL_Temp->link[i]);
		}
	}
	
	delete pSL_Temp;
	pSL_Temp	=	NULL;
	return;
}

bool CMenuTree::mcfn_checkForLastNode(void* nodePtr)
{
	int	 i;
	MenuTreeNode*	pSL_Node	=	(MenuTreeNode*)nodePtr;

	for( i = 0; i < MAX_SUB_MENU; i++ )
	{
		if( pSL_Node->link[i] != NULL )
			return false;
	}
	return true;
}


std::string  CMenuTree::mcfn_getMenuList(void* pSL_ActiveNode, char* pscL_Buffer, bool* pbL_IsLeaf)
{
	MenuTreeNode	*pSL_ActiveMenuNode	=	(MenuTreeNode*)pSL_ActiveNode;
	NodeData SL_DataNode;	
	memset(pscL_Buffer, 0x00, 400);
	memset(&SL_DataNode,0x00,sizeof(SL_DataNode));
	if( mcfn_checkForLastNode((void*)pSL_ActiveMenuNode) )
	{
		strcpy(pscL_Buffer, pSL_ActiveMenuNode->endMsg.c_str());
		*pbL_IsLeaf	=	true;
		return pSL_ActiveMenuNode->url.c_str();
	}
	else
	{
		memset(pscL_Buffer, 0x00, 160);
		mefn_getMenuDescription(pSL_ActiveMenuNode, pscL_Buffer);
	}
	return "";
}


void CMenuTree::mefn_getMenuDescription(MenuTreeNode* pSL_Node, char* pscL_Buffer)
{
	MenuTreeNode	*pSL_Tmp = NULL;
	MenuTreeNode	*pSL_Des = NULL;
	int				i		 = 0x00;
	char			pscL_Tmp[3];

	pSL_Tmp	=	pSL_Node;
	for( i = 0; i < MAX_SUB_MENU; i++ )
	{
		if( pSL_Tmp->link[i] != NULL )
		{
			memset(pscL_Tmp, 0x00, 3);
			itoa(i, pscL_Tmp, 10);
			try
			{	pSL_Des	=	pSL_Tmp->link[i];
				strcat(pscL_Buffer, pSL_Des->message);
				strcat(pscL_Buffer, pscL_Tmp);
				strcat(pscL_Buffer, ".");
				strcat(pscL_Buffer, pSL_Des->description);
				strcat(pscL_Buffer, "\n");
			}
			catch(...)
			{
				fprintf(stdout, "\t[MenuTree] \t\t -> mefn_getMenuDescription, Exception\n");
			}
		}
	}

}


