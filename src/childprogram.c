#include "myutils.h"

int main(int argc, char *argv[]) {

    // TODO: CHANGE AMOUNT OF ARGS NECESSARY
    if (argc < 9) {
        printf("Less number of arguments.\n");
        printf("./childProgram myDepth parentID startIdx endIdx myDataLen InputFileName depth degreesArr\n");
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
    char* degreesArr = argv[8]; // The degrees array in string format
    printf("Degrees array in child: %s\n", degreesArr);

    // TODO: Write child program code here
    if(myDepth == depth){
        // CALL QUICK SORT, WRITE TO INTERMEDIATE FILE, AND RETURN
        printf("Calling quick sort algorithm.\n");
        exit(0);
    } else {
        // EXEC childProgram degree NUMBER OF TIMES
        int childrenMade = 0;
        // WAIT FOR CHILDREN
        // READ FROM INTERMEDIATE FILE OF CHILDREN
        // MERGE THE FILES
        // RETURN
    }

    // Read degree of current depth to know how many children to make
    int degree = 0;
    char* delimiter = " ";
    char* token = strtok(degreesArr, delimiter);
    printf("Token in child %s: %s\n", strMyID, token);
    if(token == NULL){
        printf("Degrees amount not found for current depth: %d\n", myDepth);
        exit(0);
    }
    for(int i = 1; i < depth; i++){
        token = strtok(NULL, delimiter);
    }
    degree = atoi(token);
    printf("Inside child: %s with degree %d\n", strMyID, degree);

    // int arr[] = NULL;

    // Have to write to file the sorted contents of the array passed into this function
    // writeSortedResultToFile(strMyID, arr, myDataLen);

    return EXIT_SUCCESS;
}