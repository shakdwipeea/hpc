#include<stdio.h>
#include<math.h>
#include<time.h>
#include<omp.h>
#include<iostream>

using namespace std;

void primeFactors(int n){
	
	for(;n%2 == 0;)
	{
		cout << 2 <<endl;
		n = n/2;
	}

	for(int i = 3; i <= sqrt(n); i=i+2)
		for(;n%i == 0;)
		{
			cout << i << endl ;
			n = n/i;
		}
	if(n > 2)
		cout << n << endl ;
}

int main(){
	long long int n;
	cout << "Enter number:";
	cin >> n;
	float t = omp_get_wtime();
	primeFactors(n);
	float endTime =omp_get_wtime() - t;
	cout << "Elapsed time =" << endTime << endl;
	return 0;
}
