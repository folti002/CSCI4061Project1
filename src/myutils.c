#include "myutils.h"

void printArray(int arr[], int size) {
    int i;
    for (i = 0; i < size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void writeSortedResultToFile(char* myID, int arr[], int size) {
    char line[WriteBufferSize];
    char filepath[MaxFileNameLength];
    memset(line, '\0', WriteBufferSize);
    memset(filepath, '\0', MaxFileNameLength);
    sprintf(filepath, "output/%s.out", myID);

    int i;
    int idx = 0;
    idx += sprintf(&line[idx], "%d\n", size);
    for (i = 0; i < size; i++) {
        idx += sprintf(&line[idx], "%d\n", arr[i]);
    }
    writeLineToFile(filepath, line);                // Write data in the write buffer to the file
}

// TODO: Quick Sort or another leaf node sorting algorithm
void quickSort(int arr[], int low, int high) {
   int a, b, split, temp;
   
   if(low<high){
      split=low;
      a=low;
      b=high;

      while(a<b){
         while(arr[a]<=arr[split]&&a<high)
            a++;
         while(arr[b]>arr[split])
            b--;
         if(a<b){
            temp=arr[a];
            arr[a]=arr[b];
            arr[b]=temp;
         }
      }

      temp=arr[split];
      arr[split]=arr[b];
      arr[b]=temp;
      quickSort(arr,low,b-1);
      quickSort(arr,b+1,high);
   }
}


// TODO: Multiway Merge Sort with multiple data streams from intermediate files
void merge(int arrOne[], int arrTwo[], int lenOne, int lentwo) {
   int i;
   int j;
   int x;
   int merged[] = malloc(sizeof(int)*(lenOne+lenTwo));

   for (x = 0; x < lenOne+lenTwo; x++){
      for (i = 0; i<lenOne; i++){
         for (j = 0; j < lenTwo; j++){
            if (arrOne[i] > arrTwo[j]){
               merged[x] = arrTwo[j];
            }else if (arrOne[i] < arrTwo[j]){
               merged[x] = arrOne[i];
            }else{
               merged[x] = arrOne[i];
            }
         }
         
      }
   }
}
