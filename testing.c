#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "avl.h"
#include "splay.h"
#include "hash-chaining.h"
#include "hash-double.h"
#include "windows.h"

void runBenchmarkTrees(char *name, long long n, void (*testFunction)(long long, char *))
{
  testFunction(n, name);
}

void runBenchmarkTables(char *name, long long n, void (*testFunction)(long long, char **, int, char *), char **arrayOfnames, int index)
{

  testFunction(n, arrayOfnames, index, name);
}

void testAvl(long long n, char *name)
{
  struct avlTreeNode *rootAvl = NULL;
  LARGE_INTEGER frequency;
  LARGE_INTEGER start, end;
  QueryPerformanceFrequency(&frequency);

  QueryPerformanceCounter(&start);

  for (int i = 1; i <= n; i++)
  {
    rootAvl = avlInsertNode(rootAvl, i);
  }
  QueryPerformanceCounter(&end);

  double interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to insert %f seconds \n", name, n, interval);

  QueryPerformanceCounter(&start);

  for (int i = 1; i <= n; i++)
  {
    avlFindNode(rootAvl, i);
  }
  QueryPerformanceCounter(&end);

  interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to search %f seconds \n", name, n, interval);
  
    QueryPerformanceCounter(&start);

    for (int i = 1; i <= n; i++)
    {
      avlInorderSearch(rootAvl, i);
    }
    QueryPerformanceCounter(&end);

    interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    printf("[%s] N = %lld |Time taken to search(inorder) %f seconds \n", name, n, interval); 
  
    QueryPerformanceCounter(&start);

    for (int i = 1; i <= n; i++)
    {
      avlPostorderSearch(rootAvl, i);
    }
    QueryPerformanceCounter(&end);

    interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    printf("[%s] N = %lld |Time taken to search(postorder) %f seconds \n", name, n, interval);  */

    QueryPerformanceCounter(&start);

    for (int i = 1; i <= n; i++)
    {
      avlPreorderSearch(rootAvl, i);
    }
    QueryPerformanceCounter(&end);

    interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

    printf("[%s] N = %lld |Time taken to search(preorder) %f seconds \n", name, n, interval); 


  QueryPerformanceCounter(&start);

  for (int i = 1; i <= n; i++)
  {
    rootAvl = avlDeleteNode(rootAvl, i);
  }
  QueryPerformanceCounter(&end);

  interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to delete %f seconds \n", name, n, interval);
  avlFreeTree(rootAvl);
}

void testSplay(long long n, char *name)
{
  struct splayTreeNode *rootSplay = NULL;
  LARGE_INTEGER frequency;
  LARGE_INTEGER start, end;
  QueryPerformanceFrequency(&frequency);

  QueryPerformanceCounter(&start);

  for (int i = 1; i <= n; i++)
  {
    rootSplay = splayInsertNode(rootSplay, i);
  }
  QueryPerformanceCounter(&end);

  double interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to insert %f seconds \n", name, n, interval);

  QueryPerformanceCounter(&start);

  for (int i = 1; i <= n; i++)
  {
    splayFindNode(rootSplay, i);
  }
  QueryPerformanceCounter(&end);

  interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to search %f seconds \n", name, n, interval);

  QueryPerformanceCounter(&start);

  for (int i = 1; i <= n; i++)
  {
    splayInorderSearch(rootSplay, i);
  }
  QueryPerformanceCounter(&end);

  interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to search(inorder) %f seconds \n", name, n, interval);

  QueryPerformanceCounter(&start);

  for (int i = 1; i <= n; i++)
  {
    splayPostorderSearch(rootSplay, i);
  }
  QueryPerformanceCounter(&end);

  interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to search(postorder) %f seconds \n", name, n, interval);

  QueryPerformanceCounter(&start);

  for (int i = 1; i <= n; i++)
  {
    splayPreorderSearch(rootSplay, i);
  }
  QueryPerformanceCounter(&end);

  interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to search(preorder) %f seconds \n", name, n, interval); 

  QueryPerformanceCounter(&start);

  for (int i = 1; i <= n; i++)
  {
    rootSplay = splayDeleteNode(rootSplay, i);
  }
  QueryPerformanceCounter(&end);

  interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to delete %f seconds \n", name, n, interval);
  splayFreeTree(rootSplay);
}

void testChaining(long long n, char **arrayOfNames, int index, char *name)
{
  struct chainingHashTable *hashTable = chainingCreateTable(n);
  LARGE_INTEGER frequency;
  LARGE_INTEGER start, end;
  QueryPerformanceFrequency(&frequency);

  QueryPerformanceCounter(&start);

  char uniqueKey[70];
  for (int i = 0; i < n; i++)
  {
    sprintf(uniqueKey, "%s%d", *(arrayOfNames + (i % index)), i);
    chainingInsertNode(hashTable, uniqueKey, i);
  }
  QueryPerformanceCounter(&end);
  double interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to insert %f seconds \n", name, n, interval);

  QueryPerformanceCounter(&start);
  char uniqueKey1[70];

  for (int i = 0; i < n; i++)
  {
    sprintf(uniqueKey1, "%s%d", *(arrayOfNames + (i % index)), i);
    chainingFindNode(hashTable, uniqueKey1);
  }
  QueryPerformanceCounter(&end);
  interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to search %f seconds \n", name, n, interval);

  QueryPerformanceCounter(&start);
  char uniqueKey2[70];

  for (int i = 0; i < n; i++)
  {
    sprintf(uniqueKey2, "%s%d", *(arrayOfNames + (i % index)), i);
    chainingDeleteNode(hashTable, uniqueKey2);
  }
  QueryPerformanceCounter(&end);
  interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to delete %f seconds \n", name, n, interval);

  chainingFreeTable(hashTable);
}

void testDouble(long long n, char **arrayOfNames, int index, char *name)
{
  struct doubleHashTable *hashTable = doubleCreateTable(n);
  LARGE_INTEGER frequency;
  LARGE_INTEGER start, end;
  QueryPerformanceFrequency(&frequency);

  QueryPerformanceCounter(&start);

  char uniqueKey[70];
  for (int i = 0; i < n; i++)
  {
    sprintf(uniqueKey, "%s%d", *(arrayOfNames + (i % index)), i);
    doubleInsertNode(hashTable, uniqueKey, i);
  }
  QueryPerformanceCounter(&end);

  double interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to insert %f seconds \n", name, n, interval);

  QueryPerformanceCounter(&start);
  char uniqueKey1[70];

  for (int i = 0; i < n; i++)
  {
    sprintf(uniqueKey1, "%s%d", *(arrayOfNames + (i % index)), i);
    doubleFindNode(hashTable, uniqueKey1);
  }
  QueryPerformanceCounter(&end);
  interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to search %f seconds \n", name, n, interval);

  QueryPerformanceCounter(&start);
  char uniqueKey2[70];

  for (int i = 0; i < n; i++)
  {
    sprintf(uniqueKey2, "%s%d", *(arrayOfNames + (i % index)), i);
    doubleDeleteNode(hashTable, uniqueKey2);
  }
  QueryPerformanceCounter(&end);
  interval = (double)(end.QuadPart - start.QuadPart) / frequency.QuadPart;

  printf("[%s] N = %lld |Time taken to delete %f seconds \n", name, n, interval);

  doubleFreeTable(hashTable);
}

int main()
{
  long long int testingCases[4] = {1000, 10000, 100000, 1000000}; 

  FILE *file = fopen("names.txt", "r");
  char currentName[70];
  int nameCount = 0;

  if (!file)
  {
    printf("Could not open file");
    return 0;
  }

  while (fgets(currentName, sizeof(currentName), file) != NULL)
  {
    nameCount++;
  }
  char **arrayOfNames = (char **)malloc(nameCount * sizeof(char *));

  rewind(file);
  int index = 0;
  while (index < nameCount && fgets(currentName, sizeof(currentName), file) != NULL)
  {
    currentName[strcspn(currentName, "\n")] = 0;
    *(arrayOfNames + index) = (char *)malloc(strlen(currentName) + 1);
    strcpy(*(arrayOfNames + index), currentName);
    index++;
  }

  fclose(file);

  for (int i = 0; i < 4; i++)
  {

    printf("Testing operation for %lld elements\n", testingCases[i]);
    runBenchmarkTrees("AVL tree", testingCases[i], testAvl);
    printf("\n");
    runBenchmarkTrees("Splay tree", testingCases[i], testSplay);
    printf("\n");
     runBenchmarkTables("Hash chaining", testingCases[i], testChaining, arrayOfNames, index);
    printf("\n");
    runBenchmarkTables("Double hashing", testingCases[i], testDouble, arrayOfNames, index);
    printf("\n"); 
  }

  for (int i = 0; i < nameCount; i++)
  {
    free(*(arrayOfNames + i));
  }

  free(arrayOfNames);

  return 1;
}
