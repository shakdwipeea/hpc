#include<omp.h>
#include<stdio.h>
#include<stdlib.h>

int main()
{
	int  i;
	double x, y;
	long int size;
printf("Enter Size:\n");
scanf("%ld",&size);
	
	int *arr = (int *) malloc(size*sizeof(int));
	
	for(i = 0; i < size; i++)
		arr[i] = i;
	
	x = omp_get_wtime(); 
	long long int sum = 0;
#pragma omp parallel num_threads(4)
#pragma omp parallel reduction(+:sum)
#pragma omp for
	for(i = 0; i < size;i++)
		sum +=arr[i];
	
	#pragma omp for
	for(i = 1; i < size; i++)
//  #pragma omp critical
		if(arr[i] > arr[0])
			arr[0] = arr[i]; 

	printf("Sum : %lld\n", sum);
	
	printf("Max : %d\n", arr[0]);

	y = omp_get_wtime(); 
	

	printf("Time taken : %lf\n", y-x);
	return 0;
}
