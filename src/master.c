#include "myutils.h"

int main(int argc, char *argv[]) {
    // ID of master process set
    int myID = 0;
    char* strID = "master";

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

    // TODO: Read degrees of each level
    int* degreesPerLevel = malloc(sizeof(int) * depth);
    char temp;
    int i = 0;
    if((nread = getLineFromFile(fp, line, len)) != -1){
        char* curChar = line;
        while(*curChar != '\n'){
            if(*curChar == ' '){
                curChar++;
            } else {
                degreesPerLevel[i] = atoi(curChar);
                i++;
                curChar++;
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
    pid_t pid;
    pid_t terminated_pid;
    if(depth == 0){

    }

    // TODO: Wait all child processes to terminate if necessary
    if(pid != 0){
        terminated_pid = wait(NULL);
    }

    // TODO: Merge sort or Quick sort (or other leaf node sorting algorithm)
    
    // myID as a string
    char asStrID[MaxDepth];
    // if(myID != 0){
    //     char* args[] = {"childProgram", depth, myID, itoa(myID, strID, MaxDepth), 0, 0, 0, inputFileName};
    //     execv("childProgram", args);
    //     printf("Error executing childProgram\n");
    //     exit(0);
    //}

    // CALL MERGE SORT
    printf("%s Merge Sort\n", strID);

    free(input);

    return EXIT_SUCCESS;
}