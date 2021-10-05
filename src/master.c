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

    // FOR INTERIM SUBMISSION
    depth = 1;

    // TODO: Read degrees of each level

    // FOR INTERIM SUBMISSION
    int degree = 3;

    // THIS WON'T WORK FOR CHECKING IF DEPTHS ARE GREATER THAN 9 OR LESS THAN 0
    // int* degreesPerLevel = malloc(sizeof(int) * depth);
    // char temp;
    // if((nread = getLineFromFile(fp, line, len)) != -1){
    //     char* curChar = line;
    //     int i = 0;
    //     while(*curChar != '\n'){
    //         if(*curChar != ' '){
    //             degreesPerLevel[i] = atoi(curChar);
    //             i++;
    //         }
    //         curChar++;
    //     }
    // }

    // for(int j = 0; j < depth; j++){
    //     printf("%d ", degreesPerLevel[j]);
    // }
    // printf("\n");

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

    // ID of master process set
    char* strID = "master";
    int myID = 0;
    int level = 0;
    int startIndex = 0;
    int endIndex = 0;
    int dataSize = endIndex - startIndex;
    int createdChildren = 0;
    int degreesTableIndex = 0;

    pid_t pid;
    pid_t terminated_pid;
    if(depth < 0){
        // ERROR
    } else if(depth == 0) {
        // NO NEED TO MAKE CHILD PROCESSES
    } else {
        // int curDegree = degreesPerLevel[degreesTableIndex];

        // FOR INTERIM SUBMISSION
        for(int i = 0; i < degree; i++){
            pid = fork();
            if(pid == 0){
                break;
            }
        }

        // for(int i = 0; i < curDegree; i++){
        //     pid = fork();
        //     createdChildren++;
        //     if(pid != 0){
        //         printf("Parent [%s] - Spawn Child [%d, %d, %d, %d, %d]\n", strID, level + 1, myID + i, startIndex, endIndex, dataSize);
        //     } else {
        //         break;
        //     }
        // }
    }
    // myID as a string
    // char asStrID[MaxDepth];
    // if(myID != 0){
    //     char* args[] = {"childProgram", depth, myID, itoa(myID, strID, MaxDepth), 0, 0, 0, inputFileName};
    //     execv("childProgram", args);
    //     printf("Error executing childProgram\n");
    //     exit(0);
    // }


    if(pid == 0){
        execl("childProgram", "childProgram", "temp", "temp", "temp", "temp", "temp", "temp", NULL);
        printf("Failure executing childProgram\n");
        exit(0);
    }

    // TODO: Wait all child processes to terminate if necessary
    for(int i = 0; i < degree; i++){
        if(pid != 0){
            terminated_pid = wait(NULL);
            //printf("Process %d has terminated.\n", terminated_pid);
        }
    }   

    if(pid != 0){
        printf("[Master] Merge Sort\n");
    }

    // TODO: Merge sort or Quick sort (or other leaf node sorting algorithm)
    

    // CALL MERGE SORT
    // printf("%s Merge Sort\n", strID);
    // printf("Process [%s] - Merge Sort - Done", myID);
    // printf("Process [%s] - Quick Sort - Done", myID);

    free(input);
    //free(degreesPerLevel);

    return EXIT_SUCCESS;
}