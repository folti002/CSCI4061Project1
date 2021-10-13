/* test machine: csel-kh1250-01.cselabs.umn.edu 
 * group number: G[27] 
 * name: Reed Fazenbaker, Mikkel Folting
 * x500: fazen007, folti002
*/

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

// Takes in an array and a start and end position and sorts values
void quickSort(int arr[], int low, int high) {
   int a, b, split, temp;
   
   if(low<high){
      split=low;
      a=low;
      b=high;

      while(a<b){
         while(arr[a] <= arr[split] && a < high)
            a++;
         while(arr[b] > arr[split])
            b--;
         if(a < b){
            temp = arr[a];
            arr[a] = arr[b];
            arr[b] = temp;
         }
      }

      temp = arr[split];
      arr[split] = arr[b];
      arr[b] = temp;
      quickSort(arr, low, b - 1);
      quickSort(arr, b + 1, high);
   }
}

// Takes in two arrays to be merged and one to store the merged sorted data from the two
void merge(int totalArr[], int arr1[], int arr2[], int len1, int len2){
   int i, j, k;
   j = k = 0;

   for(i = 0; i < len1 + len2;){
      if(j < len1 && k < len2){
         if(arr1[j] < arr2[k]){
            totalArr[i] = arr1[j];
            j++;
         } else {
            totalArr[i] = arr2[k];
            k++;
         }
         i++;
      } else if(j == len1){
         for(; i < len1 + len2;){
            totalArr[i] = arr2[k];
            k++;
            i++;
         }
      } else {
         for(; i < len1 + len2;){
            totalArr[i] = arr1[j];
            j++;
            i++;
         }
      }
   }
}
