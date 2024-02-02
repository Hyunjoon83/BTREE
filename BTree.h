#ifndef BTREE_H
#define BTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

#define M 2000000
#define MAX_CHILD M
#define MIN_DEGREE 2
#define MAX_KEY MAX_CHILD - 1
#define MIN_KEY (int)(ceil(M/2.0))-1

struct BNode{
    int key_count;
    int child_count;
    struct BNode *child[MAX_CHILD + 1];
    int key[MAX_KEY+1];
    int value[MAX_KEY+1];
    bool is_leaf;
};

typedef struct BNode* BNodePtr;

/* BTree */
BNodePtr CreateNode(int key, int value);                                                                         // B Tree 생성
void Insert(int key, int value);                                                                                 // Key를 B Tree에 삽입
BNodePtr InsertNode(BNodePtr node, BNodePtr parent, int pos, int key, int value);                                // Key를 B Tree에 삽입
BNodePtr SplitNode(BNodePtr parent, BNodePtr node, int idx);                                                     // Node가 가득 찼을 때 노드를 분할
void DeleteNode(BNodePtr node, int pos);                                                                         // 노드에서 값을 삭제
int DeleteVal(BNodePtr node, int val);                                                                           // 노드에서 value를 삭제하는 함수
void Delete(BNodePtr node, int val);                                                                             // Key를 삭제
void ShiftRight(BNodePtr node, int pos);                                                                         // Node의 Key를 오른쪽으로 이동
void ShiftLeft(BNodePtr node, int pos);                                                                          // Node의 Key를 왼쪽으로 이동
void MergeNodes(BNodePtr node, int pos1, int pos2);                                                              // Split 했던 Node들을 병합
void BalanceNode(BNodePtr node, int pos);                                                                        // Shift + Merge
int MergeChildNode(BNodePtr node, int pos);                                                                      // Node의 Child를 병합
int findPredecessor(BNodePtr node);                                                                              // Node의 Predecessor를 찾는 함수
int findSuccessor(BNodePtr node);                                                                                // Node의 Successor를 찾는 함수
int OverridePredecessor(BNodePtr node, int pos);                                                                 // Node의 Predecessor를 Override
int OverrideSuccessor(BNodePtr node, int pos);                                                                   // Node의 Successor를 Override

/* csvfile */
BNodePtr PrintTreeToFile(BNodePtr node, FILE *f);                                                                // CSV파일에 B Tree의 Value를 출력
void InsertFromFile(FILE* inputfile, BNodePtr node, FILE* outputFile);                                           // input.csv파일에서 값을 읽어와 insert할 함수
void DeleteFromFile(FILE* file, BNodePtr node);                                                                  // delete.csv파일에서 값을 읽어와 delete할 함수
void CompareCSV(FILE *file, FILE *CompareFile, char *outputFilename, char* compareFilename);                     // input.csv파일과 output.csv파일을 비교하는 함수
void MarkValueAsDeletedInInputFile(FILE *outputFile, char *inputFilename, char *compareFilename);                // input.csv파일에서 delete.csv파일의 값을 삭제

#endif