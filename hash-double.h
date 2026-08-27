#ifndef HASH_TABLE_DOUBLE_H
#define HASH_TABLE_DOUBLE_H
struct doubleHashNode
{
  char key[30];
  int value;
  int status;
};

struct doubleHashTable
{
  struct doubleHashNode *table;
  int size;
  int capacity;
};

struct doubleHashTable *doubleCreateTable(int capacity);

void doubleInsertNode(struct doubleHashTable *hashTable, char *key, int value);

void doubleDeleteNode(struct doubleHashTable *hashTable, char *key);
void doubleFindNode(struct doubleHashTable *hashTable, char *key);

void doubleFreeTable(struct doubleHashTable *hashTable);
#endif
