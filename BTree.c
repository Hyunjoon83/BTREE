#include "BTree.h"

struct BNode *root = NULL;

BNodePtr CreateNode(int key, int value){
    BNodePtr newNode = (BNodePtr)malloc(sizeof(struct BNode));
    newNode->key_count = 1;
    newNode->child_count = 0;
    newNode->is_leaf = false;
    newNode->key[0] = key;
    newNode->value[0] = value;
    return newNode;
}

void Insert(int key, int value){
    if(!root){ // root가 없는 경우
        root = CreateNode(key, value); 
        root->is_leaf = true; // root == leaf
        return;
    }else{ // root가 있는 경우
        root = InsertNode(root, root, 0, key, value); // 처음에는 root==parent
    }
    return;
}

BNodePtr InsertNode(BNodePtr node, BNodePtr parent, int pos, int key, int value){
    int idx;
    for(idx = 0; idx<node->key_count; idx++){
        if(value==node->value[idx]){
            return node;
        }else if(value<node->value[idx]){
            break;
        }
    }

    if(!node->is_leaf){ // node가 leaf node가 아닌 경우
        node->child[idx] = InsertNode(node->child[idx], node, idx, key, value);
        if(node->key_count == MAX_KEY){
            node = SplitNode(node, parent, idx);
        }
    }else{ // node가 leaf node인 경우
        for(int i=node->key_count; i>idx; i--){ // key와 value를 한 칸씩 뒤로 밀기
            node->key[i] = node->key[i-1];
            node->value[i] = node->value[i-1];
            node->child[i+1] = node->child[i];
        }

        node->key[idx] = key;
        node->value[idx] = value;
        node->key_count++;

        if(node->key_count == MAX_KEY){ // MAX_KEY 개수에 도달하면 node를 split
            node = SplitNode(node, parent, idx);
        }
    }
    return node;
}

BNodePtr SplitNode(BNodePtr parent, BNodePtr node, int idx){
    int mid = (node->key_count) / 2;
    BNodePtr rightchild = (BNodePtr)malloc(sizeof(struct BNode));
    rightchild->is_leaf = node->is_leaf;
    rightchild->key_count=0;
    rightchild->child_count=0;

    for (int i = mid + 1; i < node->key_count; i++){ // 오른쪽 자식 노드에 key와 value를 복사
        rightchild->key[i - mid - 1] = node->key[i];
        rightchild->value[i - mid - 1] = node->value[i];
        rightchild->key_count++;
        node->key_count--;
    }

    if (!node->is_leaf){
        for (int i = mid + 1; i < node->child_count; i++){ // 오른쪽 자식 노드에 child를 복사
            rightchild->child[i - mid] = node->child[i];
            rightchild->child_count++;
            node->child_count--;
        }
    }

    if (node == root){
        BNodePtr newRoot = CreateNode(node->key[mid], node->value[mid]);
        root->key_count--;
        newRoot->child[0] = node;
        newRoot->child[1] = rightchild;

        newRoot->child_count = 2;
        return newRoot;
    }else{
        for (int i = parent->key_count; i > idx; i--){ // 부모 노드에 key와 value를 복사
            parent->key[i] = parent->key[i - 1];
            parent->child[i + 1] = parent->child[i];
        }

        parent->key[idx] = node->key[mid];
        parent->value[idx] = node->value[mid];
        parent->key_count++;
        parent->child[idx + 1] = rightchild;
        parent->child_count++;
    }
    return node;
}

void DeleteNode(BNodePtr node, int pos){
    // 왼쪽 자식과 오른쪽 자식의 개수 비교
    if(node->child[pos]->key_count >= node->child[pos+1]->key_count){
        if(node->child[pos]->key_count > MIN_KEY){
            DeleteVal(node->child[pos] , OverridePredecessor(node, pos)); // 왼쪽 자식이 더 많은 경우
        }else{
            DeleteVal(node->child[pos] , MergeChildNode(node, pos)); // 왼쪽 자식이 더 적은 경우
        }
    }else{
        if(node->child[pos+1]->key_count > MIN_KEY){
            DeleteVal(node->child[pos + 1] , OverrideSuccessor(node, pos)); // 오른쪽 자식이 더 많은 경우
        }else{
            DeleteVal(node->child[pos] , MergeChildNode(node, pos)); // 오른쪽 자식이 더 적은 경우
        }
    }

    // 노드의 key 개수가 MIN_KEY보다 작으면 balance
    if (node->child[pos]->key_count < MIN_KEY){
        BalanceNode(node, pos);
    }
}

int DeleteVal(BNodePtr node, int val){
    int pos;
    int flag = false;
    for (pos = 0; pos<node->key_count; pos++){ // key를 찾아서 삭제
        if(val == node->key[pos]){
            flag = true;
            break;
        }else if(val < node->key[pos]){
            break;
        }
    }

    if (node->is_leaf){
        for (int i = pos; i < node->key_count; i++){ // key와 value를 한 칸씩 앞으로 당기기
            node->key[i] = node->key[i + 1];
        }
        node->key_count--;
    }else{
        DeleteNode(node, pos);
    }
    return flag;
}

void Delete(BNodePtr node, int val){
    if(!node){
        return;
    }
    int flag = DeleteVal(node, val);
    if(!flag){ // key를 찾지 못한 경우
        return;
    }
    if(node->key_count==0){ 
        if (node->is_leaf){
            free(node);
            return;
        }else{
            BNodePtr newRoot = node->child[0];
            free(node);
            return;
        }
    }
    return;
}

void ShiftRight(BNodePtr node, int pos){
    int idx=0;

    // 빌리는 노드의 자리를 위해 남은 key값들을 한 칸씩 뒤로 밀음
    for(int i=0;i<node->child[pos]->key_count; i++){
        node->child[pos]->key[i+1] = node->child[pos]->key[i];
    }
    node->child[pos]->key[idx] = node->key[pos-1];
    node->child[pos]->key_count++;

    // 부모 노드를 내리고 형제 노드를 올리는 경우
    int idx_from_parent = (node->child[pos-1]->key_count)-1;
    node->key[pos-1] = node->child[pos-1]->key[idx_from_parent];
    node->child[pos-1]->key_count--;

    // 자식의 위치 정리
    if(node->child[pos-1]->child_count > 0){
        int child_idx = (node->child[pos-1]->child_count)-1;
        for(int i=node->child[pos]->child_count; i>0; i--){
            node->child[pos]->child[i] = node->child[pos]->child[i-1];
        }

        node->child[pos]->child[0] = node->child[pos-1]->child[child_idx];
        node->child[pos]->child_count++;
        node->child[pos-1]->child_count--;
    }
}

void ShiftLeft(BNodePtr node, int pos){
    int idx = node->child[pos]->key_count;
    node->child[pos]->key[idx] = node->key[pos];
    node->child[pos]->key_count++;

    int idx_to_parent = 0;
    node->key[pos] = node->child[pos+1]->key[idx_to_parent];

    // 빌리는 노드의 자리를 위해 남은 key값들을 한 칸씩 앞으로 당기기
    for (int i = 0; i < (node->child[pos+1]->key_count)-1; i++){
        node->child[pos+1]->key[i] = node->child[pos+1]->key[i+1];
    }
    node->child[pos+1]->key_count--;

    // 부모 노드를 내리고 형제 노드를 올리는 경우
    int idx_from_sibling = 0;

    // 자식의 위치 정리
    if (node->child[pos + 1]->child_count > 0){
        node->child[pos]->child[0] = node->child[pos + 1]->child[idx_from_sibling]; // Corrected this line

        for (int i = 0; i < node->child[pos + 1]->child_count - 1; i++){
            node->child[pos + 1]->child[i] = node->child[pos + 1]->child[i + 1];
        }
        node->child[pos + 1]->child_count--;
    }
}

void MergeNodes(BNodePtr node, int pos1, int pos2){
    int idx = node->child[pos2]->key_count;

    // 부모 노드를 내리고 형제 노드를 올리는 경우
    for(int i=0; i<node->child[pos1]->key_count; i++){
        node->child[pos2]->key[idx+1+i] = node->child[pos1]->key[i];
        node->child[pos2]->key_count++;
    }
    node->child[pos2]->key[idx] = node->key[pos2];
    node->child[pos2]->key_count++;

    int child_idx = node->child[pos2]->child_count;
    // 자식의 위치 정리
    for(int i=0; i<node->child[pos1]->child_count; i++){
        node->child[pos2]->child[child_idx+i] = node->child[pos1]->child[i];
        node->child[pos2]->child_count++;
    }

    free(node->child[pos1]);

    // 부모 노드의 key와 child 정리
    for(int i=pos2; i<(node->key_count)-1;i++){
        node->key[i] = node->key[i+1];
    }
    node->key_count--;

    // 부모 노드의 child 정리
    for(int i=pos2+1; i<(node->child_count)-1; i++){
        node->child[i] = node->child[i+1];
    }
    node->child_count--;
}

void BalanceNode(BNodePtr node, int pos){
    if (pos == 0){ // 왼쪽 자식이 없는 경우
        if (node->child[pos + 1]->key_count > MIN_KEY)
            ShiftLeft(node, pos);
        else
            MergeNodes(node, pos + 1, pos);
        return;
    }else if (pos == (node->key_count)){ // 오른쪽 자식이 없는 경우
        if (node->child[pos - 1]->key_count > MIN_KEY)
            ShiftRight(node, pos);
        else
            MergeNodes(node, pos, pos - 1);
        return;
    }else{ // 양쪽 자식이 있는 경우
        if (node->child[pos - 1]->key_count > MIN_KEY)
            ShiftRight(node, pos);
        else if (node->child[pos + 1]->key_count > MIN_KEY)
            ShiftLeft(node, pos);
        else
            MergeNodes(node, pos, pos - 1);
        return;
    }
}

int MergeChildNode(BNodePtr node, int pos){
    int idx = node->child[pos]->key_count;
    node->child[pos]->key[idx] = node->key[pos];
    node->child[pos]->key_count++;

    // 부모 노드를 내리고 형제 노드를 올리는 경우
    for(int i=0; i<node->child[pos+1]->key_count; i++){
        node->child[pos]->key[idx+1+i] = node->child[pos+1]->key[i];
        node->child[pos]->key_count++;
    }

    // 자식의 위치 정리
    for(int i=0; i<node->child[pos+1]->child_count; i++){
        node->child[pos]->child[idx+1+i] = node->child[pos+1]->child[i];
        node->child[pos]->child_count++;
    }

    // 부모 노드의 key와 child 정리
    for(int i=pos; i<node->key_count; i++){
        node->key[i] = node->key[i+1];
        node->key_count--;
    }

    // 부모 노드의 child 정리
    for(int i=pos+1; i<node->child_count; i++){
        node->child[i] = node->child[i+1];
        node->child_count--;
    }
    return node->key[pos];
}

int findPredecessor(BNodePtr node){
    if(node->is_leaf){
        return node->key[node->key_count-1];
    }
    return findPredecessor(node->child[(node->child_count)-1]); // 가장 오른쪽 자식으로 이동
}

int findSuccessor(BNodePtr node){
    if(node->is_leaf){
        return node->key[0];
    }
    return findSuccessor(node->child[0]); // 가장 왼쪽 자식으로 이동
}

int OverridePredecessor(BNodePtr node, int pos){
    int predecessor = findPredecessor(node->child[pos]);
    node->key[pos] = predecessor;
    return predecessor;
}

int OverrideSuccessor(BNodePtr node, int pos){
    int successor = findSuccessor(node->child[pos+1]);
    node->key[pos] = successor;
    return successor;
}

BNodePtr PrintTreeToFile(BNodePtr node, FILE *f){
    if (node == NULL){
        return node;
    }
    for (int i = 0; i < node->key_count; i++){
        PrintTreeToFile(node->child[i], f);
    }
    return node;
}

void InsertFromFile(FILE* inputfile, BNodePtr node, FILE* outputfile){
    int key, value;

    if (inputfile==NULL || outputfile==NULL){
        return;
    }

    while (fscanf(inputfile, "%d %d", &key, &value) != EOF){
        Insert(key, value);
        PrintTreeToFile(node, outputfile);
        fprintf(outputfile, "%d %d\n", key, value);
    } // file을 열어서 InsertNode를 통해 B Tree에 삽입한 뒤 파일에 key, value를 출력
    return;
}

void DeleteFromFile(FILE* file, BNodePtr node){
    int key;

    if (file == NULL){
        return;
    }
    while (fscanf(file, "%d", &key) != EOF){
        Delete(node, key);
    }

    fclose(file);
    return;
}

void CompareCSV(FILE* file, FILE* CompareFile, char* outputFilename, char* compareFilename){
    int fileValue1, fileValue2, inputValue1, inputValue2;
    int equal = 1;

    while (fscanf(file, "%d %d", &fileValue1, &fileValue2) == 2 && fscanf(CompareFile, "%d %d", &inputValue1, &inputValue2) == 2){
        if (fileValue1 != inputValue1 || fileValue2 != inputValue2){
            equal = 0;
            break;
        }
    }

    fclose(file);
    fclose(CompareFile);

    if (equal){
        printf("The file %s and %s are the same.\n", outputFilename, compareFilename);
    }else{
        printf("The file %s and %s are not the same.\n", outputFilename, compareFilename);
    }
}

void MarkValueAsDeletedInInputFile(FILE* outputFile, char *inputFilename, char *compareFilename){
    FILE *inputFile = fopen(inputFilename, "r");
    FILE *compareFile = fopen(compareFilename, "r");

    if (inputFile == NULL || compareFile == NULL || outputFile == NULL){
        printf("Error opening files.\n");
        return;
    }

    int col1, col2;
    int key, value;

    while (fscanf(inputFile, "%d %d", &col1, &col2) == 2){
        int found = 0;

        rewind(compareFile); // compareFile을 처음으로 돌림
        while (fscanf(compareFile, "%d %d", &key, &value) == 2){
            if (col1 == key){
                found = 1;
                break;
            }
        }

        if (found){
            fprintf(outputFile, "%d N/A\n", col1);
            Delete(root, col1);
        }else{
            fprintf(outputFile, "%d %d\n", col1, col2);
        }
    }

    fclose(inputFile);
    fclose(compareFile);
    fclose(outputFile);
}