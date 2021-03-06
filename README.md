# CSCI 4061 Project 1 - Parallel Multiway Merge Sort
## Project Group Number: 27  
Reed Fazenbaker (fazen007)  
Mikkel Folting (folti002)

## Contributions:  
Mikkel Folting: Worked on data partitioning, child creation, and incorporating quick and merge sort into master.c 
Reed Fazenbaker: Worked on merge and quick sort algorithms, and reading from files in childprogram.c
  
## Leaf Node Sorting Program:  
Quick Sort was used and is implemented as follows:
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

## Assumptions:  
We made no significant assumptions outside of the code specifications.

## To compile the program:  
```
make
```
Type the above command into the home directory of the project. 

## To run the program:  
```
./master inputFile
```
Type the above command into the home directory of the project after calling make.
