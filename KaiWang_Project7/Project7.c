/*
Pace University CS610
Dr.Lixin Tao
Project7 @Kai Wang
*/

/* Program Matrixmult */
/* edited from textbook Figure 9.10 */
ARCHITECTURE TORUS(7);

#include <stdlib.h>
#include <math.h>
#define m 7                            /* Torus has m by m processors */
#define p 5                            /* Partition size is p by p */
typedef float partition[p + 1][p + 1]; /*2D array of float type*/
partition A[m][m], B[m][m], C[m][m];   /* master arrays */
partition stream Achan[m][m], stream Bchan[m][m];
int i, j;

partition random(){
    seqoff;
    int i, j;
    partition temp;
    for (i = 0; i <= p; i++){
        for (j = 0; j <= p; j++){
            temp[i][j] = rand() % (99) + 1; /* number range 0~99*/
        }
    }
    return temp;
    seqon;
}

void printPartition(partition X){
    seqoff;
    
    int i, j;    
    for (i = 1; i <= p; i++){
        for (j = 1; j <= p; j++){   
            cout.precision(6);
            cout << X[i][j] << " ";
        }
        cout << endl;
    }
    seqon;
}

void printArr(partition arr[m][m]){
    seqoff;
    int i, j;
    for (i = 0; i < m; i++){
        for (j = 0; j < m; j++){
            cout << "partition[" << i << "][" << j << "] :" << endl;
            printPartition(arr[i][j]);
            cout << endl;
        }
    }
    seqon;
}

void Multiply(int row, int col, value partition myA, value partition myB, partition mainC){
    int i, j, k, iter, above, left;
    partition myC = {0};
    if (row > 0){
        above = row - 1;
    }
    else{
        above = m - 1;
    }
    if (col > 0){
        left = col - 1;
    }
    else{
        left = m - 1;
    }
    for (iter = 1; iter <= m; iter++){
        send(Achan[row][left], myA);
        send(Bchan[above][col], myB);
        for (i = 1; i <= p; i++){
            for (j = 1; j <= p; j++){
                for (k = 1; k <= p; k++){
                    myC[i][j] = myC[i][j] + myA[i][k] * myB[k][j];
                }
            }
        }
        recv(Achan[row][col], myA);
        recv(Bchan[row][col], myB);
    }

    mainC = myC;
}

main()
{
    /* Initialize values for A and B matrices */ 
    for (i = 0; i < m; i++){
        for (j = 0; j < m; j++){
            A[i][j] = random();
            B[i][j] = random();
        }
    }
    
   
    for (i = 0; i < m; i++){
        for (j = 0; j < m; j++){
            fork(@i * m + j)
                Multiply(i, j, A[i][(j + i) % m], B[(i + j ) % m][j], C[i][j]);            
        }
    }

   printArr(C);
    
}