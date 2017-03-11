#ifndef _MENU_TREE_H
#define _MENU_TREE_H

#include <string>
#include "Defines.h"
#include <MsgMap.h>

class NodeData
{
 public:
	std::string nodeData;
	std::string nodeMesssage;
	std::string url;
	std::string endMsg;
	std::string nextMenuId;
	NodeData()
	{
		nodeMesssage.clear();
		nodeData.clear();
		url.clear();
		endMsg.clear();
		nextMenuId.clear();
	}

        std::string mcfn_ShortDebugString()
        {
           std::string msg = "Node Data:"+nodeData+",Node Msg:"+nodeMesssage+",URL:"+url+",End Message:"+endMsg+",nextMenuId:"+nextMenuId;
           return msg;
        }
} ;

class NodeTree
{
	public:
		CMsgMap <int,CMsgMap<std::string,NodeData*>* > nodeTree;
		int currentNode;
	public:
		NodeTree();
		~NodeTree();

};
#endif


