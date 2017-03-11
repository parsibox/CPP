#ifndef __MENU_TREE_H__
#define __MENU_TREE_H__

class Node
{
  public:
    int mcsi_parentId;
    std::map<int,Node*> mcC_childIds;
};

class CMenuTree
{
 private:
    Node *meC_head;
 public:
      CMenu();
      ~CMenu();
    bool mcfn_insertNodeData(int iL_parentId,int iL_childId);

};
#endif
