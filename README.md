# CSCI 4061 Project 1
Project Group Number: 27
Partners: Reed Fazenbaker (fazen007) Mikkel Folting (folti002)

Contributions:
  Mikkel Folting: Worked on child creation and incorporating quick and merge sort
  Reed Fazenbaker: Worked on merge, quick sort, and accessing files
  
Leaf Node Sorting Program:  
```
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
```

Assumptions: We made no significant assumptions

To compile the program: make

To run the program: ./master inputFile
