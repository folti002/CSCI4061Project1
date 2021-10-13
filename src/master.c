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

    if(depth > 9 || depth < 0){
        printf("Depth is greater than 9 or less than 0.\n");
        exit(EXIT_FAILURE);
    }

    // TODO: Read degrees of each level - DONE
    int* degreesPerLevel = malloc(sizeof(int) * depth);
    if(depth != 0){
        char* token;
        char* delimiter = " ";
        int i = 0;
        if((nread = getLineFromFile(fp, line, len)) != -1){
            token = strtok(line, delimiter);
            if(token == NULL){
                printf("No degrees arguments found despite a number greater than zero being specified.\n");
                exit(EXIT_FAILURE);
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

    printArray(degreesPerLevel, depth);

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

    printArray(input, nData);
    
    // TODO: Spawn child processes and launch childProgram if necessary
    char strID[128];
    char newID[128];

    int startIndex = 0;
    int endIndex = 0;
    int curDataSize = nData;
    int childDataSize = 0;
    int childStartIndex = endIndex;
    int newStartIndex = 0;

    int isChild = 0;
    int pid;
    
    // SPAWN CHILDREN
    for(int curDepth = 1; curDepth <= depth; curDepth++){
        int degree = degreesPerLevel[curDepth - 1];
        for(int curID = 1; curID <= degree; curID++){
            strcpy(strID, newID);
            pid = fork();

            // Data Partitions
            if(curID == degree){
                // THIS IS THE SPECIAL CASE FOR DATA PARTITIONING
                childDataSize = curDataSize - (degree - 1) * floor(curDataSize / degree);
            } else {
                // NORMAL FLOOR CALCULATIONS
                childDataSize = floor(curDataSize / degree);
            }

            if(pid > 0){
                newStartIndex += childDataSize;
            } else if(pid < 0){
                // Child process not created properly
                printf("Error creating child process.\n");
                exit(0);
            } else if(pid == 0){
                // Logic for updating indexes for data partitioning
                if(curID != 1){
                    startIndex = newStartIndex;
                }
                endIndex = startIndex + childDataSize - 1;
                childStartIndex = startIndex;
                curDataSize = endIndex - startIndex + 1;

                // Set up ID for new child that's just been created
                isChild = 1;
                sprintf(newID, "%s%d", strID, curID);
                if(curDepth == 1){
                    strcpy(strID, "master");
                }
                //printf("String ID: %s\n", strID);
                printf("Parent [%s] - Spawn Child [Level: %d, ID: %s, Start Index: %d, End Index: %d, Data Size: %d]\n", strID, curDepth, newID, startIndex, endIndex, curDataSize);
                //printf("Parent [%s] - Spawn Child [Level: %d, ID: %s, Start Index: %d, End Index: %d, Data Size: %d]\n", strID, myDepth + 1, childID, startIndex, endIndex, dataSize);
                break;
            }
        }
        // Stop creating children if we are a parent process
        if(pid > 0){
            break;
        }
    }

    // TODO: Wait all child processes to terminate if necessary
    int numberOfChildren;
    pid_t terminated_pid;
    if(isChild){
        // WAIT FOR NODES AT A LOWER DEPTH BEFORE CONTINUING
        int depthOfCurrentProcess = strlen(newID);
        numberOfChildren = degreesPerLevel[depthOfCurrentProcess];
        for(int i = 0; i < numberOfChildren; i++){
            terminated_pid = wait(NULL);\
        }

        // CONVERT ALL DATA VARIABLES TO STRINGS
        char* strCurDepth = (char*) malloc(sizeof(char) * 128);
        char* strStartIndex = (char*) malloc(sizeof(char) * 128);
        char* strEndIndex = (char*) malloc(sizeof(char) * 128);
        char* strDataLen = (char*) malloc(sizeof(char) * 128);
        char* strDepth = (char*) malloc(sizeof(char) * 128);
        char* strNumChildren = (char*) malloc(sizeof(char) * 128);
        sprintf(strCurDepth, "%d", depthOfCurrentProcess);
        sprintf(strStartIndex, "%d", startIndex);
        sprintf(strEndIndex, "%d", endIndex);
        sprintf(strDataLen, "%d", curDataSize);
        sprintf(strDepth, "%d", depth);
        sprintf(strNumChildren, "%d", numberOfChildren);

        // CALL CHILD PROGRAM
        execl("childProgram", "childProgram", strCurDepth, newID, strStartIndex, strEndIndex, strDataLen, inputFileName, strDepth, NULL);
        printf("Error executing child program, strID: %s\n", strID);
        exit(EXIT_FAILURE);
    }

    // ONLY MASTER GETS TO THIS POINT
    // Wait for master's children to complete
    numberOfChildren = degreesPerLevel[0];
    for(int i = 0; i < numberOfChildren; i++){
        terminated_pid = wait(NULL);
    }
    printf("I AM THE MASTER\n");

    // CALL MERGE SORT FROM INTERMEDIATE FILES



//     for(int i = 0; i < degree; i++){
//         childrenMade++;
//         char childID[MaxDepth];
//         sprintf(childID, "%d", childrenMade);

//         // CALCULATE START AND END INDEX
//         int m = nData;
//         int k = degree;

//         if(i == degree - 1){
//             int temp = endIndex;
//             startIndex = endIndex;
//             endIndex = nData;
//         } else {
//             int temp = startIndex;
//             startIndex += i * floor(m / k);
//             endIndex = temp + floor(m / k);
//         }

//         char strStartIndex[100];
//         sprintf(strStartIndex, "%d", startIndex);

//         char strEndIndex[100];
//         sprintf(strEndIndex, "%d", endIndex);

//         int dataSize = endIndex - startIndex;
//         char strDataSize[100];
//         sprintf(strDataSize, "%d", dataSize);

//         pid = fork();

//         if(pid > 0){
//             // Print to console information about the child that was just created
//             printf("Parent [%s] - Spawn Child [Level: %d, ID: %s, Start Index: %d, End Index: %d, Data Size: %d]\n", strID, myDepth + 1, childID, startIndex, endIndex, dataSize);
//         } else if(pid < 0){
//             // Child process not created properly
//             printf("Error creating child process.\n");
//             exit(0);
//         } else {
//             // CALL CHILD PROGRAM
//             char strChildDepth[MaxDepth];
//             sprintf(strChildDepth, "%d", myDepth + 1);

//             char totalDepth[MaxDepth];
//             sprintf(totalDepth, "%d", depth);
            
//             execl("childProgram", "childProgram", strChildDepth, childID, strStartIndex, strEndIndex, strDataSize, inputFileName, totalDepth, strDegreesArr, NULL);
//             printf("Error executing childProgram.\n");
//             exit(0);
//         }
//     }


    // myID as a string
    // char asStrID[MaxDepth];
    // if(myID != 0){
    //     char* args[] = {"childProgram", depth, myID, itoa(myID, strID, MaxDepth), 0, 0, 0, inputFileName};
    //     execv("childProgram", args);
    //     printf("Error executing childProgram\n");
    //     exit(0);
    // }


    // pid_t terminated_pid;
    // for(int i = 0; i < degreesPerLevel[0]; i++){
    //     terminated_pid = wait(NULL);
    //     printf("Child process (%d) terminated.\n", terminated_pid);
    // }

    // TODO: Merge sort or Quick sort (or other leaf node sorting algorithm)
    

    // CALL MERGE SORT
    // printf("%s Merge Sort\n", strID);
    // printf("Process [%s] - Merge Sort - Done", myID);
    // printf("Process [%s] - Quick Sort - Done", myID);

    // Free malloc'd arrays
    free(input);
    free(degreesPerLevel);

    return EXIT_SUCCESS;
}