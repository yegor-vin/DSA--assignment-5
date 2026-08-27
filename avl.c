
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "avl.h"

struct avlTreeNode *createNode(int key, struct avlTreeNode *parent)
{
  struct avlTreeNode *newNode = (struct avlTreeNode *)malloc(sizeof(struct avlTreeNode));
  newNode->key = key;
  newNode->height = 1;
  newNode->leftNode = NULL;
  newNode->rightNode = NULL;
  newNode->parentNode = parent;
  return newNode;
}

int getHeight(struct avlTreeNode *node)
{
  return (node == NULL) ? 0 : node->height;
}

struct avlTreeNode *rotateRight(struct avlTreeNode *root)
{
  struct avlTreeNode *newRoot = root->leftNode;
  struct avlTreeNode *tempRight = newRoot->rightNode;
  newRoot->rightNode = root;
  root->leftNode = tempRight;

  newRoot->parentNode = root->parentNode;
  root->parentNode = newRoot;
  if (tempRight != NULL)
    tempRight->parentNode = root;

  root->height = 1 + ((getHeight(root->leftNode) > getHeight(root->rightNode)) ? getHeight(root->leftNode) : getHeight(root->rightNode));
  newRoot->height = 1 + ((getHeight(newRoot->leftNode) > getHeight(newRoot->rightNode)) ? getHeight(newRoot->leftNode) : getHeight(newRoot->rightNode));
  return newRoot;
}

struct avlTreeNode *rotateLeft(struct avlTreeNode *root)
{
  struct avlTreeNode *newRoot = root->rightNode;
  struct avlTreeNode *tempLeft = newRoot->leftNode;
  newRoot->leftNode = root;
  root->rightNode = tempLeft;

  newRoot->parentNode = root->parentNode;
  root->parentNode = newRoot;
  if (tempLeft != NULL)
    tempLeft->parentNode = root;

  root->height = 1 + ((getHeight(root->leftNode) > getHeight(root->rightNode)) ? getHeight(root->leftNode) : getHeight(root->rightNode));
  newRoot->height = 1 + ((getHeight(newRoot->leftNode) > getHeight(newRoot->rightNode)) ? getHeight(newRoot->leftNode) : getHeight(newRoot->rightNode));

  return newRoot;
}

struct avlTreeNode *rebalance(struct avlTreeNode *root)
{
  root->height = 1 + ((getHeight(root->leftNode) > getHeight(root->rightNode)) ? getHeight(root->leftNode) : getHeight(root->rightNode));
  int bf = getHeight(root->leftNode) - getHeight(root->rightNode);

  if (bf > 1)
  {
    if (getHeight(root->leftNode->leftNode) - getHeight(root->leftNode->rightNode) < 0)
    {
      root->leftNode = rotateLeft(root->leftNode);
    }

    return rotateRight(root);
  }

  else if (bf < -1)
  {
    if (getHeight(root->rightNode->leftNode) - getHeight(root->rightNode->rightNode) > 0)
    {
      root->rightNode = rotateRight(root->rightNode);
    }
    return rotateLeft(root);
  }

  return root;
}

struct avlTreeNode *avlInsertNode(struct avlTreeNode *root, int key)
{

  if (root == NULL)
    return createNode(key, NULL);

  struct avlTreeNode *curr = root;
  struct avlTreeNode *parent = NULL;

  while (curr != NULL)
  {
    parent = curr;
    if (key < curr->key)
      curr = curr->leftNode;
    else if (key > curr->key)
      curr = curr->rightNode;
    else
      return root;
  }

  struct avlTreeNode *newNode = createNode(key, parent);
  if (key < parent->key)
    parent->leftNode = newNode;
  else
    parent->rightNode = newNode;

  struct avlTreeNode *current = parent;

  while (current != NULL)
  {
    struct avlTreeNode *upperParent = current->parentNode;
    struct avlTreeNode *subtreeRoot = rebalance(current);

    if (upperParent == NULL)
    {
      root = subtreeRoot;
    }
    else if (upperParent->leftNode == current)
    {
      upperParent->leftNode = subtreeRoot;
    }
    else
    {
      upperParent->rightNode = subtreeRoot;
    }

    current = upperParent;
  }

  return root;
}

struct avlTreeNode *avlDeleteNode(struct avlTreeNode *root, int key)
{
  if (root == NULL)
  {
    return NULL;
  }

  struct avlTreeNode *current = root;
  while (current != NULL && current->key != key)
  {
    if (key < current->key)
    {
      current = current->leftNode;
    }
    else if (key > current->key)
    {
      current = current->rightNode;
    }
  }

  if (current == NULL)
  {

    return root;
  }

  struct avlTreeNode *parentOfNodeToDelete = NULL;
  struct avlTreeNode *nodeToDelete = current;

  if (nodeToDelete->leftNode != NULL && nodeToDelete->rightNode != NULL)
  {
    struct avlTreeNode *successor = nodeToDelete->rightNode;
    while (successor->leftNode != NULL)
    {
      successor = successor->leftNode;
    }

    nodeToDelete->key = successor->key;
    nodeToDelete = successor;
  }

  struct avlTreeNode *childNode = (nodeToDelete->leftNode != NULL) ? nodeToDelete->leftNode : nodeToDelete->rightNode;

  parentOfNodeToDelete = nodeToDelete->parentNode;

  if (parentOfNodeToDelete == NULL)
  {
    root = childNode;
  }
  else if (parentOfNodeToDelete->leftNode == nodeToDelete)
  {
    parentOfNodeToDelete->leftNode = childNode;
  }
  else
  {
    parentOfNodeToDelete->rightNode = childNode;
  }

  if (childNode != NULL)
  {

    childNode->parentNode = parentOfNodeToDelete;
  }

  free(nodeToDelete);

  struct avlTreeNode *currentParent = parentOfNodeToDelete;

  while (currentParent != NULL)
  {
    struct avlTreeNode *grandParent = currentParent->parentNode;
    struct avlTreeNode *subtreeRoot = rebalance(currentParent);

    if (grandParent == NULL)
    {
      root = subtreeRoot;
    }
    else if (grandParent->leftNode == currentParent)
    {
      grandParent->leftNode = subtreeRoot;
    }
    else
    {
      grandParent->rightNode = subtreeRoot;
    }
    subtreeRoot->parentNode = grandParent;
    currentParent = grandParent;
  }

  return root;
}

void avlFindNode(struct avlTreeNode *root, int key)
{
  if (root == NULL)
  {
    return;
  }

  while (root != NULL)
  {
    if (key < root->key)
    {
      root = root->leftNode;
    }
    else if (key > root->key)
    {
      root = root->rightNode;
    }
    else
    {
      return;
    }
  }
}

struct avlTreeNode *avlInorderSearch(struct avlTreeNode *root, int key)
{
  if (root == NULL)
    return NULL;

  struct avlTreeNode *leftNode = avlInorderSearch(root->leftNode, key);
  if (leftNode != NULL)
    return leftNode;

  if (root->key == key)
  {
    return root;
  }

  return avlInorderSearch(root->rightNode, key);
}

struct avlTreeNode *avlPreorderSearch(struct avlTreeNode *root, int key)
{
  if (root == NULL)
    return NULL;

  if (root->key == key)
  {
    return root;
  }
  struct avlTreeNode *leftNode = avlPreorderSearch(root->leftNode, key);
  if (leftNode != NULL)
    return leftNode;

  return avlPreorderSearch(root->rightNode, key);
}

struct avlTreeNode *avlPostorderSearch(struct avlTreeNode *root, int key)
{
  if (root == NULL)
    return NULL;

  struct avlTreeNode *found = avlPreorderSearch(root->leftNode, key);
  if (found != NULL)
    return found;

  found = avlPreorderSearch(root->rightNode, key);
  if (found != NULL)
    return found;

  if (root->key == key)
  {
    return root;
  }

  return NULL;
}

void avlFreeTree(struct avlTreeNode *root)
{
  if (root == NULL)
  {
    return;
  }

  avlFreeTree(root->leftNode);
  avlFreeTree(root->rightNode);

  free(root);
}
