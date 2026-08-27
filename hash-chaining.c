#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "hash-chaining.h"

struct chainingHashTable *chainingCreateTable(int capacity)
{
  struct chainingHashTable *newTable = (struct chainingHashTable *)malloc(sizeof(struct chainingHashTable));
  newTable->size = 0;
  newTable->capacity = capacity;

  newTable->table = (struct chainingHashNode **)malloc(capacity * sizeof(struct chainingHashNode *));

  for (int i = 0; i < capacity; i++)
  {
    *(newTable->table + i) = NULL;
  }

  return newTable;
}

int hashFunction(char *key, int size)
{
  unsigned long hash = 5381;

  while (*key)
  {
    hash = ((hash << 5) + hash) + *key;
    key++;
  }

  return hash % size;
}

void resize(struct chainingHashTable *hashTable)
{

  int oldCapacity = hashTable->capacity;
  struct chainingHashNode **oldTable = hashTable->table;

  hashTable->capacity = oldCapacity * 2;
  hashTable->table = (struct chainingHashNode **)malloc(hashTable->capacity * sizeof(struct chainingHashNode *));

  for (int i = 0; i < hashTable->capacity; i++)
  {
    *(hashTable->table + i) = NULL;
  }

  hashTable->size = 0;

  for (int i = 0; i < oldCapacity; i++)
  {
    struct chainingHashNode *current = *(oldTable + i);
    while (current != NULL)
    {
      struct chainingHashNode *nextNode = current->nextNode;

      int index = hashFunction(current->key, hashTable->capacity);

      current->nextNode = hashTable->table[index];
      *(hashTable->table + index) = current;

      hashTable->size++;

      current = nextNode;
    }
  }

  free(oldTable);
}

struct chainingHashNode *chainingCreateNode(char *key, int value)
{
  struct chainingHashNode *newNode = (struct chainingHashNode *)malloc(sizeof(struct chainingHashNode));
  sprintf(newNode->key, "%s", key);
  newNode->value = value;
  newNode->nextNode = NULL;
  return newNode;
}

void chainingInsertNode(struct chainingHashTable *hashTable, char *key, int value)
{

  float loadFactor = (float)hashTable->size / hashTable->capacity;

  if (loadFactor > 0.75)
  {
    resize(hashTable);
  }

  int hashIndex = hashFunction(key, hashTable->capacity);

  struct chainingHashNode *newNode = chainingCreateNode(key, value);

  newNode->nextNode = *(hashTable->table + hashIndex);

  *(hashTable->table + hashIndex) = newNode;

  hashTable->size++;
}

void chainingDeleteNode(struct chainingHashTable *hashTable, char *key)
{

  if (hashTable->size == 0)
  {
    return;
  }

  int hashIndex = hashFunction(key, hashTable->capacity);

  struct chainingHashNode *current = *(hashTable->table + hashIndex);
  struct chainingHashNode *prev = NULL;

  while (current != NULL)
  {
    if (strcmp(current->key, key) == 0)
    {
      if (prev == NULL)
      {
        *(hashTable->table + hashIndex) = current->nextNode;
      }
      else
      {
        prev->nextNode = current->nextNode;
      }

      free(current);
      hashTable->size--;
      return;
    }

    prev = current;
    current = current->nextNode;
  }
}

void chainingFindNode(struct chainingHashTable *hashTable, char *key)
{

  if (hashTable->size == 0)
  {
    return;
  }

  int hashIndex = hashFunction(key, hashTable->capacity);

  struct chainingHashNode *current = *(hashTable->table + hashIndex);

  while (current != NULL)
  {
    if (strcmp(current->key, key) == 0)
    {
      return;
    }

    current = current->nextNode;
  }
}

void chainingFreeTable(struct chainingHashTable *hashTable)
{
  for (int i = 0; i < hashTable->capacity; i++)
  {
    struct chainingHashNode *current = *(hashTable->table + i);
    struct chainingHashNode *temp = NULL;

    while (current != NULL)
    {
      temp = current;
      current = current->nextNode;
      free(temp);
    }
  }
  free(hashTable->table);
  free(hashTable);
}


