# How to Test Program
### 1. Compile
- gcc -o main main.c
- ./main

### 2. After Compile
    [ 원하는 동작 선택 (1: Insertion,  2: Deletion,  3: Quit) ]
    1) Insertion
    - "Enter the filename for insertion :  "이라는 문구가 뜨고, input 파일명을 입력한다.
    - "Inserting keys from [input_file]..."이라는 문구가 뜨고, 새로운 파일에 insertion의 결과가 출력된다.
    - CompareCSV 함수를 통해 새로운 파일과 input파일의 비교 결과가 출력되고 다시 선택창으로 넘어간다.
    
    2) Deletion
    - "Enter the filename for deletion :  "이라는 문구가 뜨고, input 파일명을 입력한다.
    - "Enter the filename for comparison : "이라는 문구가 뜨고, 삭제할 값을 갖고 있는 파일명을 입력한다.
    - "Deleting keys from [input_file]..."이라는 문구가 뜨고, 새로운 파일에 deletion의 결과가 출력된다.
    - "Enter the filename for comparing answer : "이라는 문구가 뜨고, 기존에 주어진 정답 파일명을 입력한다.
    - CompareCSV 함수를 통해 새로운 파일과 input파일의 비교 결과가 출력되고 다시 선택창으로 넘어간다.

    3) Quit
    - "Exiting the program."이 출력되고 프로그램이 종료된다.
