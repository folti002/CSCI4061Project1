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
    int nChild = strtol(argv[8], NULL, 10); // Holds number of children this process has

    if(myDepth == depth){
        // THIS IS A LEAF NODE - CALL QUICK SORT
        // printf("This is a leaf node, ID: %s, Start Index: %d, End Index: %d, Data Size: %d\n", strMyID, myStartIdx, myEndIdx, myDataLen);

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
            arr[index++] = input[i];
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
        // printf("NOT a leaf node, ID: %s, Start Index: %d, End Index: %d, Data Size: %d\n", strMyID, myStartIdx, myEndIdx, myDataLen)

        // FIRST WE READ IN THE ARRAY FROM THE FIRST CHILD SO WE HAVE AN ARRAY TO MERGE WITH IN THE FOR-LOOP

        // Prepare file I/O variables
        FILE* fp;
        char* line = (char*) malloc(sizeof(char) * LineBufferSize);
        size_t len = LineBufferSize;
        ssize_t nread;
        char* path = "output/";

        // This will store the ID of the file we are getting our new array from
        char* filename = (char*) malloc(sizeof(char) * 128);
        sprintf(filename, "%s%s1.out", path, strMyID);
        // printf("Current filename: %s\n", filename); // testing

        // Open file with filename
        fp = fopen(filename, "r");
        if(fp == NULL){
            printf("No file titled \"%s\" could be opened\n", filename);
        }

        // Get amount of data that this file has - store in dataAmount
        int dataAmount;
        if((nread = getLineFromFile(fp, line, len)) != 1){
            sscanf(line, "%d\n", &dataAmount);
        }
        // printf("File Name: %s, Data Amount: %d\n", filename, dataAmount);

        // Allocate memory for the first array to be read in
        // It will have enough space to store all data for this process
        int* prevArr = (int*) malloc(sizeof(int) * myDataLen);

        // Read data into new array
        int aNumber;
        int idxInput = 0;
        while((nread = getLineFromFile(fp, line, len)) != -1) {
            sscanf(line, "%d\n", &aNumber);
            prevArr[idxInput++] = aNumber;
        }
        
        // printArray(prevArr, dataAmount);
        
        // Now we loop over the rest of the children and keep merging with the array made in the previous run through
        for(int curChild = 2; curChild <= nChild; curChild++){
            // File we will read from to get current quick sorted array
            sprintf(filename, "%s%s%d.out", path, strMyID, curChild);

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

            // printf("%s prevArr: ", strMyID);
            // printArray(prevArr, dataAmount); // testing
            // printf("%s arr: ", strMyID);
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

        writeSortedResultToFile(strMyID, prevArr, myDataLen);
        printf("Process [%s] - Merge Sort - Done\n", strMyID);

        free(filename);
        free(prevArr);
        free(line);
        fclose(fp);
    }

    return EXIT_SUCCESS;
}