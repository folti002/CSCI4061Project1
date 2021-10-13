#include "myutils.h"

int main(int argc, char *argv[]) {

    // TODO: CHANGE AMOUNT OF ARGS NECESSARY
    if (argc < 8) {
        printf("Less number of arguments.\n");
        printf("./childProgram myDepth parentID startIdx endIdx myDataLen InputFileName depth nChild\n");
        fflush(stdout);
        exit(EXIT_FAILURE);
    }

    int myDepth = strtol(argv[1], NULL, 10);
    char* strMyID = argv[2];
    int myStartIdx = strtol(argv[3], NULL, 10);
    int myEndIdx = strtol(argv[4], NULL, 10);
    int myDataLen = strtol(argv[5], NULL, 10);
    char* inputFileName = argv[6];
    // Additional args
    int depth = strtol(argv[7], NULL, 10); // Holds the total depth of the sort
    int nChild = strtol(argv[8], NULL, 10);
    // char* degreesArr = argv[8]; // The degrees array in string format
    // printf("Degrees array in child: %s\n", degreesArr);

    // TODO: Write child program code here
    // if(myDepth == depth){
    //     // CALL QUICK SORT, WRITE TO INTERMEDIATE FILE, AND RETURN
    //     printf("Calling quick sort algorithm.\n");
    //     exit(0);
    // } else {
    //     // Read degree of current depth to know how many children to make
    //     int degree = 0;
    //     printf("Inside child: %s with degree %d\n", strMyID, degree);

    //     // EXEC childProgram degree NUMBER OF TIMES
    //     int childrenMade = 0;
    //     // WAIT FOR CHILDREN
    //     pid_t terminated_pid;
    //     for(int i = 0; i < degree; i++){
    //         terminated_pid = wait(NULL);
    //     }
    //     // READ FROM INTERMEDIATE FILE OF CHILDREN
    //     // MERGE THE FILES
    //     // RETURN
    // }

    if(myDepth == depth){
        // THIS IS A LEAF NODE - CALL QUICK SORT
        printf("This is a leaf node, ID: %s, Start Index: %d, End Index: %d, Data Size: %d\n", strMyID, myStartIdx, myEndIdx, myDataLen);

        FILE* fp;
        char *line = (char *)malloc(sizeof(char) * LineBufferSize);
        size_t len = LineBufferSize;
        ssize_t nread;

        char inputFileName[MaxFileNameLength];
        memset(inputFileName, '\0', MaxFileNameLength);
        sprintf(inputFileName, "%s", argv[6]);

        if ((fp = getFilePointer(inputFileName)) == NULL) {             // Open a file and return file pointer to the file
            exit(EXIT_FAILURE);
        }

        // Read the number of data and depth
        int nData;
        int depth = 0;
        if((nread = getLineFromFile(fp, line, len)) != -1) {
            sscanf(line, "%d %d\n", &nData, &depth);
        }

        if((nread = getLineFromFile(fp, line, len)) == -1){
            printf("Error reading line.\n");
        }

        // Read input data
        int* input = (int*) malloc(sizeof(int) * nData);
        int aNumber;
        int idxInput = 0;
        while((nread = getLineFromFile(fp, line, len)) != -1) {
            sscanf(line, "%d\n", &aNumber);
            input[idxInput++] = aNumber;
        }

        int* arr = (int*) malloc(sizeof(int) * myDataLen);
        int index = 0;
        for(int i = myStartIdx; i <= myEndIdx; i++){
            arr[index] = input[i];
            index++;
        }

        //printArray(arr, myDataLen);
        quickSort(arr, 0, index - 1);
        //printArray(arr, myDataLen);
        writeSortedResultToFile(strMyID, arr, myDataLen);

        printf("Process [%s] - Quick Sort - Done\n", strMyID);

        free(input);
        free(line);
        free(arr);
    } else {
        // THIS IS NOT A LEAF NODE
        // CALL MERGE SORT

        // Malloc array for total data - myDataLen holds this information
        int* totalData = (int*) malloc(sizeof(int) * myDataLen);
        
        // Loop over all files that are children of this process
        for(int i = 0; i < nChild; i++){
            // Find new file that is child of this process
            // Get amount of data that this file has
            // Malloc new array for this file
            // 
        }

        // char* curID = malloc(sizeof(char));
        // int* childArray[MaxDataSize] = malloc(sizeof(int) * MaxDataSize * nChild);
        // int curArray[] = malloc(sizeof(int) * MaxDataSize);
        // for (int i = 1; i <= nChild; i++){
        //     sprintf(curID, "%s%d", myID,i);
        //     int curIndex = 0;
        //     //while(// There are more lines in the file){
        //         // Read from file to get array inputs
        //         // curArray[j] = // next int read from file
        //     //}
        // }

        writeSortedResultToFile();
        
        printf("NOT a leaf node, ID: %s, Start Index: %d, End Index: %d, Data Size: %d\n", strMyID, myStartIdx, myEndIdx, myDataLen);
    }

    //printf("Inside child program, ID: %s, Depth: %d, Total Depth: %d\n", strMyID, myDepth, depth);


    // Have to write to file the sorted contents of the array passed into this function
    // writeSortedResultToFile(strMyID, arr, myDataLen);

    return EXIT_SUCCESS;
}