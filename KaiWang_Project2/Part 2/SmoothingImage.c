/*
Pace University CS610
Dr.Lixin Tao
Project2 @Kai Wang
*/

#include <stdlib.h>
#define n 23 /* size of the image, n*n pixels */
int image[n+1][n+1], final[n+1][n+1];
int i, j;

void Smooth(int i, int j){
  
  int sum, av;
  sum = image[i-1][j-1] + image[i-1][j] + image[i-1][j+1] 
      + image[i][j-1] + image[i][j] + image[i][j+1]
      + image[i+1][j-1] + image[i+1][j] + image[i+1][j+1];
  av = sum / 9;
  final[i][j] = av;
}

main() {
  int pixel;
  /* initialize pixels randomly between 0~255 */
  for(i=1; i<=n; i++){
    for (j=1; j<=n; j++){
      pixel = rand()%(255-0 + 1) + 0;
      image[i][j] = pixel;
      if(i==1 || j==1 || i==n ||j==n){
        final[i][j] = pixel;
      }
    }
  }

  /* print the initial pixels */
  cout << "Original image data (randomly generated):" << endl;
  for(i=1; i<=n; i++){
    for(j=1; j<=n; j++){
      cout << image[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;

  
  forall i=2 to n-1 do{/* Ignore pixels along the four outer boundaries of the image array. */
    forall j=2 to n-1 do{
      Smooth(i,j);/* caculate and place the smoothed pixels */
    }
  }
    

  /* print the final pixels */
  cout << "Smoothed image data:"<< endl;
  for(i=1; i<=n; i++){
    for(j=1; j<=n; j++){
      cout << final[i][j] << " ";
    }
    cout << endl;
  }
  cout << endl;



}
