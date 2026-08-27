#ifndef HASH_TABLE_CHAINING_H
#define HASH_TABLE_CHAINING_H

struct chainingHashNode
{
  char key[30];
  int value;
  struct chainingHashNode *nextNode;
};

struct chainingHashTable
{
  struct chainingHashNode **table;
  int size;
  int capacity;
};

struct chainingHashTable *chainingCreateTable(int capacity);

void chainingInsertNode(struct chainingHashTable *hashTable, char *key, int value);
void chainingDeleteNode(struct chainingHashTable *hashTable, char *key);

void chainingFindNode(struct chainingHashTable *hashTable, char *key);
void chainingFreeTable(struct chainingHashTable *hashTable);

#endif
