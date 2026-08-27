#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "splay.h"

struct splayTreeNode *createSplayNode(int key, struct splayTreeNode *parent)
{
  struct splayTreeNode *newNode = (struct splayTreeNode *)malloc(sizeof(struct splayTreeNode));
  newNode->key = key;
  newNode->leftNode = NULL;
  newNode->rightNode = NULL;
  newNode->parentNode = parent;
  return newNode;
}

struct splayTreeNode *splayRotateRight(struct splayTreeNode *root)
{
  struct splayTreeNode *newRoot = root->leftNode;
  struct splayTreeNode *tempRight = newRoot->rightNode;
  newRoot->rightNode = root;
  root->leftNode = tempRight;

  newRoot->parentNode = root->parentNode;
  root->parentNode = newRoot;
  if (tempRight != NULL)
    tempRight->parentNode = root;

  return newRoot;
}

struct splayTreeNode *splayRotateLeft(struct splayTreeNode *root)
{
  struct splayTreeNode *newRoot = root->rightNode;
  struct splayTreeNode *tempLeft = newRoot->leftNode;
  newRoot->leftNode = root;
  root->rightNode = tempLeft;

  newRoot->parentNode = root->parentNode;
  root->parentNode = newRoot;
  if (tempLeft != NULL)
    tempLeft->parentNode = root;

  return newRoot;
}

struct splayTreeNode *toSplay(struct splayTreeNode *newNode)
{
  while (newNode->parentNode != NULL)
  {
    struct splayTreeNode *parent = newNode->parentNode;
    struct splayTreeNode *grandParent = parent->parentNode;
    struct splayTreeNode *greatGrandParent = (grandParent != NULL) ? grandParent->parentNode : NULL;

    if (grandParent == NULL)
    {
      if (newNode == parent->leftNode)
      {
        splayRotateRight(parent);
      }
      else
      {
        splayRotateLeft(parent);
      }
    }
    else if (newNode == parent->leftNode && parent == grandParent->leftNode)
    {
      splayRotateRight(grandParent);
      splayRotateRight(parent);
    }
    else if (newNode == parent->rightNode && parent == grandParent->rightNode)
    {
      splayRotateLeft(grandParent);
      splayRotateLeft(parent);
    }

    else
    {
      if (newNode == parent->rightNode)
      {
        splayRotateLeft(parent);
        splayRotateRight(grandParent);
      }
      else
      {
        splayRotateRight(parent);
        splayRotateLeft(grandParent);
      }
    }

    if (greatGrandParent != NULL)
    {
      if (greatGrandParent->leftNode == (grandParent ? grandParent : parent))
      {
        greatGrandParent->leftNode = newNode;
      }
      else
      {
        greatGrandParent->rightNode = newNode;
      }
    }
    newNode->parentNode = greatGrandParent;
  }
  return newNode;
}

struct splayTreeNode *splayInsertNode(struct splayTreeNode *root, int key)
{
  if (root == NULL)
  {
    return createSplayNode(key, NULL);
  }
  struct splayTreeNode *current = root;
  struct splayTreeNode *parent = NULL;

  while (current != NULL)
  {
    parent = current;
    if (key < current->key)
    {
      current = current->leftNode;
    }
    else if (key > current->key)
    {
      current = current->rightNode;
    }
    else
    {
      return root;
    }
  }

  struct splayTreeNode *newNode = createSplayNode(key, parent);

  if (key < parent->key)
  {
    parent->leftNode = newNode;
  }
  else
    parent->rightNode = newNode;

  return toSplay(newNode);
}

struct splayTreeNode *splayDeleteNode(struct splayTreeNode *root, int key)
{
  if (root == NULL)
  {
    return NULL;
  }

  struct splayTreeNode *current = root;
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

  struct splayTreeNode *newRoot = toSplay(current);

  struct splayTreeNode *leftSubtree = newRoot->leftNode;
  struct splayTreeNode *rightSubtree = newRoot->rightNode;
  free(newRoot);

  if (leftSubtree != NULL)
  {
    leftSubtree->parentNode = NULL;
  }

  if (rightSubtree != NULL)
  {
    rightSubtree->parentNode = NULL;
  }

  if (leftSubtree == NULL)
  {
    return rightSubtree;
  }

  struct splayTreeNode *maxLeft = leftSubtree;

  while (maxLeft->rightNode != NULL)
  {
    maxLeft = maxLeft->rightNode;
  }

  leftSubtree = toSplay(maxLeft);
  leftSubtree->rightNode = rightSubtree;

  if (rightSubtree != NULL)
  {

    rightSubtree->parentNode = leftSubtree;
  }

  return leftSubtree;
}

struct splayTreeNode *splayFindNode(struct splayTreeNode *root, int key)
{
  if (root == NULL)
  {
    return NULL;
  }

  struct splayTreeNode *current = root;

  while (current != NULL)
  {
    if (key < current->key)
    {
      current = current->leftNode;
    }
    else if (key > current->key)
    {
      current = current->rightNode;
    }
    else
    {
      return toSplay(current);
    }
  }

  return NULL;
}

struct splayTreeNode *splayInorderSearch(struct splayTreeNode *root, int key)
{
  if (root == NULL)
    return NULL;

  struct splayTreeNode *leftNode = splayInorderSearch(root->leftNode, key);
  if (leftNode != NULL)
    return leftNode;

  if (root->key == key)
  {
    return root;
  }

  return splayInorderSearch(root->rightNode, key);
}

struct splayTreeNode *splayPreorderSearch(struct splayTreeNode *root, int key)
{
  if (root == NULL)
    return NULL;

  if (root->key == key)
  {
    return root;
  }
  struct splayTreeNode *leftNode = splayPreorderSearch(root->leftNode, key);
  if (leftNode != NULL)
    return leftNode;

  return splayPreorderSearch(root->rightNode, key);
}

struct splayTreeNode *splayPostorderSearch(struct splayTreeNode *root, int key)
{
  if (root == NULL)
    return NULL;

  struct splayTreeNode *found = splayPreorderSearch(root->leftNode, key);
  if (found != NULL)
    return found;

  found = splayPreorderSearch(root->rightNode, key);
  if (found != NULL)
    return found;

  if (root->key == key)
  {
    return root;
  }

  return NULL;
}

void splayFreeTree(struct splayTreeNode *root)
{
  if (root == NULL)
  {
    return;
  }

  splayFreeTree(root->leftNode);
  splayFreeTree(root->rightNode);
  free(root);
}
