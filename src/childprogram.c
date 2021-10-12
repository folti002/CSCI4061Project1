#include "myutils.h"

int main(int argc, char *argv[]) {

    // TODO: CHANGE AMOUNT OF ARGS NECESSARY
    if (argc < 8) {
        printf("Less number of arguments.\n");
        printf("./childProgram myDepth parentID startIdx endIdx myDataLen InputFileName depth\n");
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

    printf("Inside child program, ID: %s, Depth: %d\n", strMyID, myDepth);


    // Have to write to file the sorted contents of the array passed into this function
    // writeSortedResultToFile(strMyID, arr, myDataLen);

    return EXIT_SUCCESS;
}