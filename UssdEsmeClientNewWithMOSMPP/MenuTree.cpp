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


NodeTree::NodeTree()
{
  nodeTree.mcf_clear();
}

NodeTree::~NodeTree()
{
 nodeTree.mcf_startGet();
 int menuId=0;
 CMsgMap<std::string,NodeData*> *nodeMenu = NULL;
 while(nodeTree.mcfb_getNextElement(menuId,nodeMenu))
 {
   NodeData* nodeData = NULL;
   std::string menuOption;
   while(nodeMenu->mcfb_getNextElement(menuOption,nodeData))
   {
      delete nodeData;
   }
   nodeMenu->mcf_clear(); 
   delete nodeMenu;
 }
}

