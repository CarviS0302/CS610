/*
Pace University CS610
Dr.Lixin Tao
Project5: parallel Jacobi relaxation
@Kai Wang
*/

#include <stdlib.h>
#include <math.h>
#define n 64
#define tolerance .01
float A[n+2][n+2], B[n+2][n+2];
int i, j;
float change, maxchange;
int stream higher[n+1], stream lower[n+1];
int count;
spinlock Arrival, Departure;
boolean globaldone;

void printArr(){
    cout.precision(4);
    seqoff;
    int k, z;    
    for(k=0; k<=n+1; k++){
        cout << "Row " << k <<": " << endl;
            for (z=0; z<=n+1; z++){
                cout << A[k][z];
                if(z%7==0){
                    cout << endl;
                }                
            }
            cout << endl;
        }
    cout << endl;
    seqon;
}

void  LocalBarrier(int i){
    int dummy;

    if(i>1){
        send(higher[i-1], 1);
    }
    if(i<n){
        send (lower[i+1], 1);
        recv(higher[i], dummy);
    }
    if(i>1){
        recv(lower[i], dummy);
    }
}

boolean Aggregate(boolean mydone){
    boolean result;
    /* Arrival Phase */
    Lock(Arrival);
        count = count + 1;
        globaldone = globaldone && mydone; /* aggregation */
    if(count<n){
        Unlock(Arrival); /* continue Arrival Phase */
    }else{
        Unlock(Departure);/* end Arrival Phase */
    }

    /* Departure Phase */
    Lock(Departure);
        count = count - 1;
        result = globaldone; /* return "done" flag*/
    if(count>0){
        Unlock(Departure); /* continue Departure Phase */
    }else{
        Unlock(Arrival); /* terminate Departure Phase */
        globaldone = true; /* reset for new Aggregation */
    }

    return (result);
}

main(){
    
    /* read in initial values for array A */
    for(i=0; i<=n+1; i++){
        for (j=0; j<=n+1; j++){
            A[i][j] =  rand()%(100);
        }
    }

    cout << "random array: " << endl;
    printArr();


    count = 0;
    Unlock(Arrival);
    Lock(Departure);
    globaldone = true;

    B = A;
    B = A;
    forall i=1 to n do { /*Create the processes*/
        int j;
        float change, maxchange;
        boolean done;
        do {
            maxchange = 0;
            for (j=1; j<=n;j++) {
            /*compute new value for each point in my row*/
                B[i][j] = (A[i-1][j] + A[i+1][j] + A[i][j-1] + A[i][j+1]) / 4;
                change = fabs( B[i][j] - A[i][j] );
                if (change > maxchange){
                    maxchange = change;
                } 
            }
            LocalBarrier(i);
            A[i] = B[i];
            LocalBarrier(i);
            done = Aggregate(maxchange < tolerance);
        }while (!done); /*iterate until global termination*/
    }

    cout << "completed array: " << endl;
    printArr();
    
}