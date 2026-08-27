#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <windows.h>
#include "hash-double.h"

struct doubleHashTable *doubleCreateTable(int capacity)
{
  struct doubleHashTable *newTable = (struct doubleHashTable *)malloc(sizeof(struct doubleHashTable));
  newTable->size = 0;
  newTable->capacity = capacity;

  newTable->table = (struct doubleHashNode *)calloc(capacity, sizeof(struct doubleHashNode));

  return newTable;
}

int doubleHashFunction(char *key, int size)
{
  unsigned long hash = 5381;

  while (*key)
  {
    hash = ((hash << 5) + hash) + *key;
    key++;
  }

  return hash % size;
}

int doubleHashFunction1(char *key, int capacity)
{
  unsigned long hash = 0;
  while (*key)
  {
    hash = (hash * 31) + *key;
    key++;
  }

  return 1 + (hash % (capacity - 1));
}

void doubleResize(struct doubleHashTable *hashTable)
{

  int oldCapacity = hashTable->capacity;
  struct doubleHashNode *oldTable = hashTable->table;

  hashTable->capacity = oldCapacity * 2 + 1;
  hashTable->table = (struct doubleHashNode *)calloc(hashTable->capacity, sizeof(struct doubleHashNode));
  hashTable->size = 0;

  for (int i = 0; i < oldCapacity; i++)
  {
    if ((*(oldTable + i)).status == 1)
    {
      int h1 = doubleHashFunction((*(oldTable + i)).key, hashTable->capacity);
      int h2 = doubleHashFunction1((*(oldTable + i)).key, hashTable->capacity);
      int index = h1;
      int startIndex = index;

      while ((*(hashTable->table + index)).status == 1)
      {
        index = (index + h2) % hashTable->capacity;

        if (index == startIndex)
        {
          break;
        }
      }
      strcpy((*(hashTable->table + index)).key, (*(oldTable + i)).key);
      (*(hashTable->table + index)).value = (*(oldTable + i)).value;
      (*(hashTable->table + index)).status = 1;
      hashTable->size++;
    }
  }

  free(oldTable);
}

void doubleInsertNode(struct doubleHashTable *hashTable, char *key, int value)
{

  float loadFactor = (float)hashTable->size / hashTable->capacity;

  if (loadFactor > 0.75)
  {
    doubleResize(hashTable);
  }

  int h1 = doubleHashFunction(key, hashTable->capacity);
  int h2 = doubleHashFunction1(key, hashTable->capacity);
  int index = h1;
  int startIndex = index;
  int firstDeleted = -1;

  while ((*(hashTable->table + index)).status != 0)
  {

    if ((*(hashTable->table + index)).status == 2 && firstDeleted == -1)
    {
      firstDeleted = index;
    }

    if (strcmp((*(hashTable->table + index)).key, key) == 0 && (*(hashTable->table + index)).status == 1)
    {
      (*(hashTable->table + index)).value = value;
      return;
    }

    index = (index + h2) % hashTable->capacity;

    if (index == startIndex)
    {
      break;
    }
  }

  if (firstDeleted != -1)
  {
    index = firstDeleted;
  }
  strcpy((*(hashTable->table + index)).key, key);
  (*(hashTable->table + index)).value = value;
  (*(hashTable->table + index)).status = 1;

  hashTable->size++;
}

void doubleDeleteNode(struct doubleHashTable *hashTable, char *key)
{

  if (hashTable->size == 0)
  {
    return;
  }
  int h1 = doubleHashFunction(key, hashTable->capacity);
  int h2 = doubleHashFunction1(key, hashTable->capacity);
  int index = h1;
  int startIndex = index;

  while ((*(hashTable->table + index)).status != 0)
  {
    if ((*(hashTable->table + index)).status == 1 && strcmp((*(hashTable->table + index)).key, key) == 0)
    {
      (*(hashTable->table + index)).status = 2;
      hashTable->size--;
      return;
    }
    index = (index + h2) % hashTable->capacity;
    if (index == startIndex)
      break;
  }
}

void doubleFindNode(struct doubleHashTable *hashTable, char *key)
{
  if (hashTable->size == 0)
  {
    return;
  }

  int h1 = doubleHashFunction(key, hashTable->capacity);
  int h2 = doubleHashFunction1(key, hashTable->capacity);
  int index = h1;
  int startIndex = index;

  while ((*(hashTable->table + index)).status != 0)
  {
    if ((*(hashTable->table + index)).status == 1 && strcmp((*(hashTable->table + index)).key, key) == 0)
    {
      return;
    }
    index = (index + h2) % hashTable->capacity;
    if (index == startIndex)
      break;
  }
}

void doubleFreeTable(struct doubleHashTable *hashTable)
{

  if (hashTable == NULL)
  {
    return;
  }
  free(hashTable->table);

  free(hashTable);
}
