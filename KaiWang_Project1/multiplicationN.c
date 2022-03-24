

int i, n, multiplication;

main() {
	cout << "Input a positive integer";
	cin >> n;
	
	
	multiplication = 1;
	for(i=1; i<=n; i++){
		multiplication = multiplication * i;
	}

	cout <<multiplication;
}

