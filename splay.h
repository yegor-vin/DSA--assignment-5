#ifndef SPLAY_TREE_H
#define SPLAY_TREE_H

struct splayTreeNode
{
  int key;
  struct splayTreeNode *leftNode;
  struct splayTreeNode *rightNode;
  struct splayTreeNode *parentNode;
};


struct splayTreeNode *splayInsertNode(struct splayTreeNode *root, int key);

struct splayTreeNode *splayDeleteNode(struct splayTreeNode *root, int key);


struct splayTreeNode *splayFindNode(struct splayTreeNode *root, int key);


void splayFreeTree(struct splayTreeNode *root);
struct splayTreeNode *splayInorderSearch(struct splayTreeNode *root, int key);
struct splayTreeNode *splayPreorderSearch(struct splayTreeNode *root, int key);
struct splayTreeNode *splayPostorderSearch(struct splayTreeNode *root, int key);


#endif
