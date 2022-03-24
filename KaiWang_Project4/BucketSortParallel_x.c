/*
Pace University CS610
Dr.Lixin Tao
Project4 @Kai Wang
*/

/* !! This test program distribute number by their first ONE digit,
for example, 1999 and 1111 are in the same bucket since the first
ONE digits of both are 1. As the result, there are 9 buckets for
numbers from 1000 to 9999 !! */

/* sort numbers from 1000 to 9999 */
#include <stdlib.h>
#define n 10000 /* n numbers */
spinlock L;
int stream index[10];

int num[n+1];
int workSpace[10][n+1]; 
/* workSpace[i][0] indicates the number of numbers included
   workSpace[0][n] remains 0s*/

int i, j;

void sortProcess(int pid){
  /* sorted arrays, process 0 handle the number begins with 1 */
  int length = workSpace[pid+1][0];
  int beginIndex, endIndex;    
  int key;
  int k, z;
  for(k=2; k<length+1; k++){
    key = workSpace[pid+1][k];
    z = k - 1;
    while(z >= 1 && workSpace[pid+1][z] > key){
      workSpace[pid+1][z+1] = workSpace[pid+1][z];
      z = z - 1;
    }
    workSpace[pid+1][z+1] = key;
  }

  if(pid==0){    
    beginIndex = 1;
    endIndex = beginIndex + length - 1;
    send(index[pid+1], endIndex+1);
  }else{
    recv(index[pid], beginIndex);
    endIndex = beginIndex + length - 1;
    send(index[pid+1], endIndex+1);    
  }
  z = 1;
  for(k=beginIndex; k<=endIndex; k++){
    num[k] =  workSpace[pid+1][z];
    z++;
  }


}




main() {
  
  
  /* initialize numbers randomly between 1000~9999 */
  for(i=1; i<=n; i++){
    num[i] = rand()%(9999-1000 + 1) + 1000;/* number range */
  }

  /* print the unsorted numbers */
  cout << "Unsorted random numbers: " << endl;
  for(i=1; i<=n; i++){
    cout << num[i] << " ";
    if(i%10==0){
      cout << endl;
    }
  }
  cout << endl;

  /* distribute the numbers */
  /* grouping m, m can be change for testing */
  int m; 
  m = n / 500;
  

  forall i=1 to n grouping m do{
    int curNum = num[i];
    int firstNum = curNum / 1000;
    int curIndex;
    Lock(L);
    curIndex = ++workSpace[firstNum][0];
    workSpace[firstNum][curIndex] = curNum;
    Unlock(L);
  }
  
  /* assign the portions to parallel process */
  int pid;
  forall pid=0 to 8 do{
    sortProcess(pid);
  }

  cout << "Final: " << endl;
  for(i=1; i<=n; i++){
    cout << num[i] << " ";
    if(i%10==0){
      cout << endl;
    }
  }


}
