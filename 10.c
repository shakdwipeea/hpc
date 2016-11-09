#include<stdio.h>
#include<omp.h>
#include<math.h>

int main()
{
	double x, y, sqt;
	int threadNum, i;

	printf("Enter number of threads:");
	scanf("%d", &threadNum);
	omp_set_num_threads(threadNum);
	
	x = omp_get_wtime();
	
	#pragma omp parallel for private(i)//parallel takes longer
	for(i = 0; i <= 1000; i++)
	{
		sqt = sqrt(i);
		printf("\nRoot of %d is %lf", i, sqt);
	}
	
	y = omp_get_wtime();
	
	printf("\nTime taken: %lf\n", y-x);
	return 0;
}
