#ifndef _MENU_TREE_H
#define _MENU_TREE_H

#include <string.h>
#include <string>
#include "Defines.h"

typedef struct MenuData_Tag
{
	int		mcsl_MenuId;
	int		mcsl_ParentId;
	char	mcsc_Description[MAX_DESC_LEN];
	char	mcsc_Keyword[MAX_KEYWORD_LEN];
	char	mcsc_Msg[MAX_UD_LEN];
	char	mcsc_NodeNature;
	char	mcsc_DestinationAddress[100];
	int		mcsl_Order;
	std::string mcC_url;
} MenuData;


struct MenuTreeNode
{
	char			description[50];
	char			destinationAddress[100];
	MenuTreeNode	*link[MAX_SUB_MENU];
	MenuTreeNode	*previousNode;
	char			keyword[255];
	char 			message[255];
	char			nodeNature;
	std::string 		url;
	MenuTreeNode()
	{
		strcpy(keyword,"NULL");
		strcpy(message,"");
		previousNode	=	NULL;
		memset(description, 0x00, 50);
		memset(destinationAddress, 0x00, 100);
		url.clear();
		for( int i = 0; i < MAX_SUB_MENU; i++ )
			link[i] = NULL;
	};
};

class CMenuTree
{
private:

	MenuTreeNode		*pmeS_startNode;
	MenuTreeNode		*pmeS_currentNode;

private:
	void			mefn_loadTree(MenuData*, int, MenuTreeNode**, int);
	void			mefn_deleteMenuTree(MenuTreeNode**);
	void			mefn_getMenuDescription(MenuTreeNode*, char*);

public:

					CMenuTree();
					~CMenuTree();
	void*			mcfn_getMenuTreeRoot();
	bool			mcfn_createMenuTree(MenuData*, int);
	bool			mcfn_checkForLastNode(void*);
	std::string		mcfn_getMenuList(void*, char*, bool*);
};

inline
void* CMenuTree::mcfn_getMenuTreeRoot()
{
	return pmeS_startNode;
}


#endif


