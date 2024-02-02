#include "BTree.c"
#define MAX_FILENAME 100

int main(int argc, char *argv[]){
    int choice;
    char filename[MAX_FILENAME], outputFilename[MAX_FILENAME], compareFilename[MAX_FILENAME], deletionCompareFilename[MAX_FILENAME];
    FILE *file, *outputFile, *CompareFile, *deletionCompareFile;
    BNodePtr root = NULL;

    while (1){
        printf("1. Insert\n2. Delete\n3. Quit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);

        switch (choice){
        case 1:
            printf("Enter the filename for insertion: ");
            scanf("%s", filename);
            file = fopen(filename, "r");

            if (file == NULL){
                printf("Could not open file %s\n", filename);
                break;
            }
            snprintf(outputFilename, MAX_FILENAME, "%s_insertion.csv", filename);
            outputFile = fopen(outputFilename, "w");
            if (outputFile == NULL){
                printf("Could not open file %s\n", outputFilename);
                fclose(file);
                break;
            }

            printf("Inserting keys from %s...\n", filename);
            InsertFromFile(file, root, outputFile);

            fclose(file);
            fclose(outputFile);

            // input파일과 output파일 비교
            file = fopen(outputFilename, "r");
            printf("Enter the filename for comparison: ");
            scanf("%s", compareFilename);
            CompareFile = fopen(compareFilename, "r");

            if (file == NULL || CompareFile == NULL){
                printf("Error opening files for comparison.\n");
                break;
            }
            CompareCSV(file, CompareFile, outputFilename, filename);
            break;

        case 2:
            printf("Enter the filename for deletion: ");
            scanf("%s", filename);

            if (file == NULL){
                printf("Could not open file %s\n", filename);
                break;
            }

            snprintf(outputFilename, MAX_FILENAME, "%s_deletion.csv", filename);
            outputFile = fopen(outputFilename, "w");
            if (outputFile == NULL){
                printf("Could not open file %s\n", outputFilename);
                break;
            }
            printf("Enter the filename for comparison: ");
            scanf("%s", compareFilename);

            printf("Deleting keys from %s...\n", filename);
            MarkValueAsDeletedInInputFile(outputFile ,filename, compareFilename);

            fclose(file);
            fclose(outputFile);

            // 정답이랑 비교
            printf("Enter the filename for comparing answer: ");
            scanf("%s", deletionCompareFilename);
            outputFile = fopen(outputFilename, "r");
            deletionCompareFile = fopen(deletionCompareFilename, "r");

            if (outputFile == NULL || deletionCompareFile == NULL){
                printf("Error opening files for comparison.\n");
                break;
            }
            CompareCSV(outputFile, deletionCompareFile, outputFilename, deletionCompareFilename);
            break;

        case 3:
            printf("Exiting the program.\n");
            return 0;

        default:
            printf("Invalid choice. Please enter again.\n");
        }
    }

    return 0;
}