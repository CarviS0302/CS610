/*
Pace University CS610
Dr.Lixin Tao
Project4 @Kai Wang
*/



/* sort numbers from 0 to 9999 */
#include <stdlib.h>
#define n 10000 /* n numbers */
spinlock L;
int stream index[101];

int num[n+1];
int workSpace[100][n+1]; 
/* workSpace[i][0] indicates the number of numbers included
   workSpace[0][n] remains 0s*/
int i;

void sortProcess(int bucket){
  /* sorted arrays, process 0 handle the number begins with 1 */
  int length = workSpace[bucket][0];
  if(length != 0){
    int beginIndex, endIndex;    
    int key;
    int k, z;
    for(k=2; k<length+1; k++){
      key = workSpace[bucket][k];
      z = k - 1;
      while(z >= 1 && workSpace[bucket][z] > key){
        workSpace[bucket][z+1] = workSpace[bucket][z];
        z = z - 1;
      }
      workSpace[bucket][z+1] = key;
    }

    if(bucket==0){    
      beginIndex = 1;
      endIndex = beginIndex + length - 1;
      send(index[bucket+1], endIndex+1);
    }else{
      recv(index[bucket], beginIndex);
      endIndex = beginIndex + length - 1;
      send(index[bucket+1], endIndex+1);    
    }
    z = 1;
    for(k=beginIndex; k<=endIndex; k++){
      num[k] =  workSpace[bucket][z];
      z++;
    }
  }
}


void printArr(){
  seqoff;
  int k;
  for(k=1; k<=n; k++){
    cout << num[k] << " ";
    if(k%10==0){
      cout << endl;
    }
  }
  cout << endl;
  seqon;
}

main() {
  /* grouping size m, can be changed for testing */
  int m; 
  m = 50;
  
  /* initialize numbers randomly between 0~9999 */  
  forall i=1 to n grouping m do{
    num[i] = rand()%(10000);/* number range 0~9999*/
  }

  /* print the unsorted numbers */  
  cout << "Unsorted array: " << endl;
  seqoff;
  printArr();
  seqon;
 
  
  /* distribute the numbers according to their first TWO digits */
  forall i=1 to n grouping m do{
    int curNum = num[i];
    int firstNum = curNum / 100;
    int curIndex;
    
    curIndex = ++workSpace[firstNum][0];
    workSpace[firstNum][curIndex] = curNum;    
  }
  
  /* assign the portions to parallel process */
  int bucket;
  forall bucket = 0 to 99 do{
    sortProcess(bucket);
  }


  /* print the sorted numbers */
  cout << "Sorted array: " << endl;
  seqoff;
  printArr();
  seqon;
}
