/*
Pace University CS610
Dr.Lixin Tao
Project8 @Kai Wang
*/

/* Program Spatial filtering of a visual image */
/* edited from textbook Figure 10.4 */
ARCHITECTURE HYPERCUBE(7); /*128 processors*/
#include <mpi.h>
#include <math.h>
#include <stdlib.h>
/* Can't do n=1280 & m=10, which will cause STORAGE OVERFLOW */
#define n 640 /* dimension of image */
#define m 5   /* number of rows in each partition */
int inrows[m + 2][n + 2], outrows[m][n];
int filter[3][3] = {1, 1, 1, 1, 1, 1, 1, 1, 1};
MPI_Status status;
int myrank, totalproc, blocksize;
int dest, tag, source;
int i, j, k;

void input_image(){
    int pixel;
    /*input and partition the image*/

    int image[n + 2][n + 2];
    /* input pixel values for image */
    for(i=0; i<=n+1; i++){
        for (j=0; j<=n+1; j++){
            pixel = rand()%(255-0 + 1) + 0;
            image[i][j] = pixel;            
        }
    }
    /* send partition to each process */
    for (k = 0; k < totalproc; k++){
        dest = k;
        tag = 1;
        MPI_Send(&image[k * m][0], blocksize, MPI_INT, dest, tag, MPI_COMM_WORLD);
    }
}

void print(int image[n + 2][n + 2]){
    cout << "Original image data (randomly generated):" << endl;
    for(i=0; i<=n+1; i++){
       for (j=0; j<=n+1; j++){
           cout << image[i][j] << " ";          
       }
        cout << endl;
    }
    cout << endl;
}

main(){
    MPI_Init();
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);
    MPI_Comm_size(MPI_COMM_WORLD, &totalproc);
    blocksize = (m + 2) * (n + 2);

    if (myrank == 0){
        input_image();        
    }
        
    /* receive my block of image*/
    source = 0;
    tag = 1;
    MPI_Recv(&inrows[0][0], blocksize, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
    /* apply filter mask to my partition*/
    for (i = 1; i <= m; i++){
        for (j = 1; j <= n; j++){
            outrows[i - 1, j - 1] = (int)((
            filter[0, 0] * inrows[i - 1, j - 1] + 
            filter[0, 1] * inrows[i - 1, j] + 
            filter[0, 2] * inrows[i - 1, j + 1] + 
            filter[1, 0] * inrows[i, j - 1] + 
            filter[1, 1] * inrows[i, j] + 
            filter[1, 2] * inrows[i, j + 1] + 
            filter[2, 0] * inrows[i + 1, j - 1] + 
            filter[2, 1] * inrows[i + 1, j] + 
            filter[2, 2] * inrows[i + 1, j + 1]
            ) / 9);
        }
    }
        
           
    /* send filtered partition back to process 0 */
    blocksize = m * n;
    dest = 0;
    tag = 2;
    MPI_Send(&outrows[0][0], blocksize, MPI_INT, dest, tag, MPI_COMM_WORLD);
    /*assemble partitions into filtered image as output*/
    if (myrank == 0){
        for (k = 0; k < totalproc; k++){
            source = k;
            tag = 2;
            MPI_Recv(&outrows[0][0], blocksize, MPI_INT, source, tag, MPI_COMM_WORLD, &status);
        
            cout << "Filtered image data of Parition" << source << ": " << endl;
            for (i = 0; i < m; i++){
                cout << "Row" << i << ": " << endl;
                int counter = 0;
                for (j = 0; j < n; j++){
                    cout << outrows[i][j];
                    counter ++;
                    if( counter%10==0){
                        cout << endl;
                    }
                }                    
                cout << endl;
            }
        }
    }
    MPI_Finalize();
}