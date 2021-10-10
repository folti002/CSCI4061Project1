#include "myutils.h"

int main(int argc, char *argv[]) {

    if (argc < 2) {
        printf("Less number of arguments.\n");
        printf("./master InputFileName\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    // Please keep this function call in the main.
    // This function initializes "output" folder
    // by deleting the folder and all its files,
    // and then creating a clean "output" folder.
    bookeepingCode();

    // Parse input file
    FILE * fp;                                                      // File pointer
    char *line = (char *)malloc(sizeof(char) * LineBufferSize);     // Line buffer where a new line is stored
    size_t len = LineBufferSize;                                    // The size of the line buffer
    ssize_t nread;                                                  // Length of a new line read

    char inputFileName[MaxFileNameLength];
    memset(inputFileName, '\0', MaxFileNameLength);
    sprintf(inputFileName, "input/%s", argv[1]);

    if ((fp = getFilePointer(inputFileName)) == NULL) {             // Open a file and return file pointer to the file
        exit(EXIT_FAILURE);
    }

    // Read the number of data and depth
    int nData;
    int depth = 0;
    if((nread = getLineFromFile(fp, line, len)) != -1) {            // Read next line and write it to line buffer
        sscanf(line, "%d %d\n", &nData, &depth);
    }

    // TODO: Read degrees of each level - DONE
    int* degreesPerLevel = malloc(sizeof(int) * depth);
    char* strDegreesArr = (char*) malloc(sizeof(char) * LineBufferSize);
    if(depth != 0){
        char* token;
        char* delimiter = " ";
        int i = 0;
        if((nread = getLineFromFile(fp, line, len)) != -1){
            strcpy(strDegreesArr, line);
            token = strtok(line, delimiter);
            if(token == NULL){
                printf("No degrees arguments found despite a number greater than zero being specified.\n");
                exit(0);
            }
            while(token != NULL){
                int curDegree = atoi(token);
                if(curDegree >= 0 && curDegree <= MaxDegree){
                    degreesPerLevel[i++] = curDegree;
                } else {
                    printf("Degree greater than 10 or less than 0 found.\n");
                    exit(0);
                }
                token = strtok(NULL, delimiter);
            }
        }
    }

    // Read input data
    int * input = (int *)malloc(sizeof(int) * nData);
    int aNumber;
    int idxInput = 0;
    while((nread = getLineFromFile(fp, line, len)) != -1) {
        sscanf(line, "%d\n", &aNumber);
        input[idxInput++] = aNumber;
    }

    free(line);
    fclose(fp);

    
    // TODO: Spawn child processes and launch childProgram if necessary
    char* strID = "master";

    int startIndex = 0;
    char strStartIndex[100];
    sprintf(strStartIndex, "%d", startIndex);

    int endIndex = nData;
    char strEndIndex[100];
    sprintf(strEndIndex, "%d", endIndex);

    int dataSize = endIndex - startIndex;
    char strDataSize[100];
    sprintf(strDataSize, "%d", dataSize);
    
    int myDepth = 0;
    int childrenMade = 0;

    pid_t pid;
    if(depth < 0){
        printf("Depth of less than 0 found.\n");
        exit(0);
    } else if(depth > 0) {
        //int degree = degreesPerLevel[0];

        char* token;
        char* delimiter = " ";
        printf("strDegreesArr: %s", strDegreesArr);
        token = strtok(strDegreesArr, delimiter);
        int degree = atoi(token);
        printf("Degree in master: %d\n", degree);

        for(int i = 0; i < degree; i++){
            childrenMade++;
            char childID[MaxDepth];
            sprintf(childID, "%d", childrenMade);
            pid = fork();

            if(pid > 0){
                // Print to console information about the child that was just created
                printf("Parent [%s] - Spawn Child [Level: %d, ID: %s, Start Index: %d, End Index: %d, Data Size: %d]\n", strID, myDepth + 1, childID, startIndex, endIndex, dataSize);
            } else if(pid < 0){
                // Child process not created properly
                printf("Error creating child process.\n");
                exit(0);
            } else {
                // CALL CHILD PROGRAM
                char strChildDepth[MaxDepth];
                sprintf(strChildDepth, "%d", myDepth + 1);

                char totalDepth[MaxDepth];
                sprintf(totalDepth, "%d", depth);
                
                execl("childProgram", "childProgram", strChildDepth, childID, strStartIndex, strEndIndex, strDataSize, inputFileName, totalDepth, strDegreesArr, NULL);
                printf("Error executing childProgram.\n");
                exit(0);
            }
        }
    }

    // myID as a string
    // char asStrID[MaxDepth];
    // if(myID != 0){
    //     char* args[] = {"childProgram", depth, myID, itoa(myID, strID, MaxDepth), 0, 0, 0, inputFileName};
    //     execv("childProgram", args);
    //     printf("Error executing childProgram\n");
    //     exit(0);
    // }

    // TODO: Wait all child processes to terminate if necessary
    pid_t terminated_pid;
    for(int i = 0; i < degreesPerLevel[0]; i++){
        terminated_pid = wait(NULL);
        printf("Child process (%d) terminated.\n", terminated_pid);
    }

    // TODO: Merge sort or Quick sort (or other leaf node sorting algorithm)
    

    // CALL MERGE SORT
    // printf("%s Merge Sort\n", strID);
    // printf("Process [%s] - Merge Sort - Done", myID);
    // printf("Process [%s] - Quick Sort - Done", myID);

    free(input);
    free(degreesPerLevel);
    free(strDegreesArr);

    return EXIT_SUCCESS;
}