/*
Pace University CS610
Dr.Lixin Tao
Project8 @Kai Wang
*/


/* PROGRAM NumericalIntegration */
ARCHITECTURE MESH2(10); /* 10*10 */
#include <stdlib.h>
#include <math.h>


#define numproc 100 /* number of processes */
#define numpoints 100 /* number of points per process */
float a, b, w, globalsum, answer;
int i;

float stream inputdata[numproc+1];
float answerArr[numproc];

float f(float t){
  return (sqrt(4 - t*t));
}

void Intergrate(int myindex, float *localsum){
  float internal = 0;
  float t;
  int j;
  float aa, bb, ww;

  /* receive values from main/left process and send it to right */
  recv(inputdata[myindex], aa);
  send(inputdata[myindex+1], aa);
  recv(inputdata[myindex], bb);
  send(inputdata[myindex+1], bb);
  recv(inputdata[myindex], ww);
  send(inputdata[myindex+1], ww);
  
  t = aa + myindex*(bb-aa) / numproc; /* My start position */ 

  for(j=1; j<=numpoints; j++){
    internal = internal + f(t); /* Add next point */
    t = t + ww;
  }

  internal = ww * internal;
  *localsum = internal;
}



main() { 
  /* Initialize values of end points "a" and "b" */
  a = 0;
  b = 2;
  w = (b-a) / (numproc*numpoints); /* spacing of points */

  send(inputdata[0], a);
  send(inputdata[0], b);
  send(inputdata[0], w);
  
  forall i=0 to numproc-1 do{
    Intergrate(i, &answerArr[i]);    
  }

  for(i=0; i<=numproc-1; i++){
    globalsum = globalsum + answerArr[i];
  }
  answer = globalsum + w/2 * (f(b) - f(a)); /* end points */
  
  cout.precision(20);
  cout << answer;
  
  

  



}
