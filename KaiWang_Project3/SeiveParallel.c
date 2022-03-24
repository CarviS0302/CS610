#include <stdlib.h>
#include <math.h>
#define n 10000
#define endmarker -1

boolean Prime[n+1];
int stream pipeInt[n];/* too much length*/



void PipeProcess(int pid, int capacity){
	int i, num;
	int loc;
	int begin = 1 + pid*capacity;
	int end = begin + capacity - 1;
	int firstPrime;

	if (end > n+1){
		end = n;
	}
	for(i=begin; i<=end; i++){
		Prime[i] = TRUE;
	}
	/*process 0 search for prime numbers and send to the others, 
	other process receive from process one and mark the multiples of the number*/
	if(pid==0){
		Prime[1] = False;		
		for(num=2; num<=end; num++){
			if(Prime[num]){
				send(pipeInt[pid+1], num);
				loc = num + num;				
			}
			while (loc <= end){
				Prime[loc] = False;
				loc = loc + num;				
			}	
		}
		send(pipeInt[pid+1], endmarker);
		Prime[2]=true; 
		/*for unkown reason, when num=3, it changes Prime[2] to false*/
	}else{
		do {
			recv(pipeInt[pid], num);
			send(pipeInt[pid+1], num);
			if(num == endmarker){
				break;
			}
			
			for(i=begin; i<=end; i++){				
				if(i%num==0){
					Prime[i] = False;
					firstPrime = i;
					loc = firstPrime + num;
					break;
				}
			}
			while (loc <= n){
				Prime[loc] = False;
				loc = loc + num;
			}

		}while (num != endmarker);					
	}
}


main() {
	int i;
	int numOfProcess, numOfPortion;
	int capacity = (int)(Sqrt(n)+2);
	int k = 0;
	int j  = 0;

	numOfPortion = n/capacity + 1;
		
	forall i = 0 to numOfPortion do{
		PipeProcess(i, capacity);
	}

	for (i=1; i<=n; i++){
		if(Prime[i]){
			cout << i << " ";
			k = k + 1;
			j = j + 1;
		}
		if(k==10){
			cout << endl;
			k = 0;
		}
		
	}
	cout << endl;
	cout << "total: " << j << " prime numbers";

}