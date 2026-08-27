#ifndef AVL_TREE_H
#define AVL_TREE_H

struct avlTreeNode
{
  int key;
  int height;
  struct avlTreeNode *leftNode;
  struct avlTreeNode *rightNode;
  struct avlTreeNode *parentNode;
};

struct avlTreeNode *avlInsertNode(struct avlTreeNode *root, int key);
struct avlTreeNode *avlDeleteNode(struct avlTreeNode *root, int key);
void avlFindNode(struct avlTreeNode *root, int key);
void avlFreeTree(struct avlTreeNode *root);
struct avlTreeNode *avlInorderSearch(struct avlTreeNode *root, int key);
struct avlTreeNode *avlPreorderSearch(struct avlTreeNode *root, int key);
struct avlTreeNode *avlPostorderSearch(struct avlTreeNode *root, int key);


#endif
