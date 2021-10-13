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
        printf("There is no input data.\n");
        exit(EXIT_FAILURE);
    }

    // Read the number of data and depth
    int nData;
    int depth = 0;
    if((nread = getLineFromFile(fp, line, len)) != -1) {            // Read next line and write it to line buffer
        sscanf(line, "%d %d\n", &nData, &depth);
    }

    if(depth > 9){
        printf("Depth is greater than 9.\n");
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
                // If this child process is the last child process created by its parent
                childDataSize = curDataSize - (degree - 1) * floor(curDataSize / degree);
            } else {
                // NORMAL FLOOR CALCULATIONS FOR CHILD PROCESSES THAT AREN'T THE LAST ONES
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
        execl("childProgram", "childProgram", strCurDepth, newID, strStartIndex, strEndIndex, strDataLen, inputFileName, strDepth, strNumChildren, NULL);
        printf("Error executing child program, strID: %s\n", strID);
        exit(EXIT_FAILURE);
    }

    // ONLY MASTER PROCESS GETS TO THIS POINT
    // Wait for master's children to complete
    numberOfChildren = degreesPerLevel[0];
    for(int i = 0; i < numberOfChildren; i++){
        terminated_pid = wait(NULL);
    }


    // IF WE HAVE NO CHILDREN, CALL QUICK SORT DIRECTLY
    if(numberOfChildren == 0){
        quickSort(input, 0, nData);
        writeSortedResultToFile("master", input, nData);
        printf("Process [master] - Quick Sort - Done\n");
        return EXIT_SUCCESS;
    }







    // CALL MERGE SORT FROM INTERMEDIATE FILES

    // Prepare file I/O variables
    //char* line = (char*) malloc(sizeof(char) * LineBufferSize);
    //size_t len = LineBufferSize;
    //ssize_t nread;
    char* path = "output/";

    // This will store the ID of the file we are getting our new array from
    char* filename = (char*) malloc(sizeof(char) * 128);
    sprintf(filename, "%s1.out", path);

    // Open file with filename
    fp = fopen(filename, "r");
    if(fp == NULL){
        printf("No file titled \"%s\" could be opened\n", filename);
    }

    // Get amount of data that this file has - store in dataAmount
    int dataAmount;
    if((nread = getLineFromFile(fp, line, len)) != 1){
        sscanf(line, "%d\n", &dataAmount);
        //printf("Data Amount read in: %d\n", dataAmount); // THIS WORKS CORRECTLY
    }

    // Allocate memory for the first array to be read in
    // It will have enough space to store all data for this process
    int* prevArr = (int*) malloc(sizeof(int) * nData);

    // Read data into new array
    idxInput = 0;
    while((nread = getLineFromFile(fp, line, len)) != -1) {
        sscanf(line, "%d\n", &aNumber);
        prevArr[idxInput++] = aNumber;
    }

    for(int curChild = 2; curChild <= numberOfChildren; curChild++){
        // File we will read from to get current quick sorted array
        sprintf(filename, "%s%d.out", path, curChild);

        // printf("Current filename: %s\n", filename);

        // Open file with filename
        fp = fopen(filename, "r");
        if(fp == NULL){
            printf("No file titled \"%s\" could be opened\n", filename);
        }

        int newDataAmount;
        // Get amount of data that this file has - store in dataAmount
        if((nread = getLineFromFile(fp, line, len)) != 1){
            sscanf(line, "%d\n", &newDataAmount);
            //printf("Data Amount read in: %d\n", newDataAmount); // THIS WORKS CORRECTLY
        }

        // printf("File Name: %s, Data Amount: %d\n", filename, newDataAmount);

        // Malloc new array for this file
        int* arr = (int*) malloc(sizeof(int) * newDataAmount);

        // Read data into new array
        idxInput = 0;
        while((nread = getLineFromFile(fp, line, len)) != -1) {
            sscanf(line, "%d\n", &aNumber);
            arr[idxInput++] = aNumber;
        }

        // printf("prevArr: ");
        // printArray(prevArr, dataAmount); // testing
        // printf("arr: ");
        // printArray(arr, newDataAmount); // testing

        // Allocate space for a new array to hold the merged arrays
        int* newData = (int*) malloc(sizeof(int) * (dataAmount + newDataAmount));

        // Merge the new array into the previous array and store in another array
        merge(newData, prevArr, arr, dataAmount, newDataAmount);

        //printf("MERGED\n");

        // Update dataAmount variable
        dataAmount += newDataAmount;
        // printf("Sorted data: ");
        // printArray(newData, dataAmount); // testing

        // Copy contents of newData array into prevArr to use for next run through for-loop
        for(int i = 0; i < dataAmount; i++){
            prevArr[i] = newData[i];
        }

        // Free newly allocated array
        free(arr);
        free(newData);
    }

    writeSortedResultToFile("master", prevArr, dataAmount);
    printf("Process [master] - Merge Sort - Done\n");

    // Free items used for merge
    free(filename);
    free(prevArr);
    free(line);
    fclose(fp);

    // Free malloc'd arrays
    free(input);
    free(degreesPerLevel);

    return EXIT_SUCCESS;
}