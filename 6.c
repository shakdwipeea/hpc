#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>
#include<time.h>
#define pf printf
#define sf scanf

int nt;

int* prime_table(int prime_num) 
{
	int i=2,j,p=0,prime,*primes;
	primes = (int *) malloc(prime_num * sizeof(int));
	while(p<prime_num)
	{
		prime = 1;
		for(j=2;j<i;j++)
			if( (i%j) == 0) 
			{
                prime = 0;
                break;
            }
		if(prime) 
		{
			primes[p] = i;
			p++;
        }
        i++;
    }
    return primes;
}

double* sine_table(int sine_num) 
{
	double a,pi=3.141592653589793,*sines;
	int i,j;
	sines = (double *) malloc(sine_num * sizeof(double));

	#pragma omp parallel for private(j,a) num_threads(nt)
    for(i=0;i<sine_num;i++)
	{
		sines[i] = 0.0;
		for(j=0;j<=i;j++)
		{
            a = j * pi / (sine_num-1);
            sines[i] += sin(a);
		}
	}
    return sines;
}

void timestamp() 
{
	#define TIME_SIZE 40
	static char time_buffer[TIME_SIZE];
	const struct tm *times;
	size_t len;
	time_t now;
	now = time(NULL);
	times = localtime(&now);
	len = strftime(time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p",times);
	pf("%s\n",time_buffer);
	#undef TIME_SIZE
	return;
}

int main() 
{
	int prime_num, *primes, sine_num;
    double *sines, wtime,wtime1,wtime2;
    pf("Enter the no of threads:\t");
    sf("%d",&nt);
    
	pf("Enter the upper bound:\t");
    sf("%d",&prime_num);
    
	sine_num = prime_num;
    timestamp();
    
	pf("\nMULTITASK_OPENMP:\nC version\n");
	wtime = omp_get_wtime();
    #pragma omp parallel sections num_threads(nt)
    {
        wtime1 = omp_get_wtime();    
        #pragma omp section
        {primes = prime_table(prime_num);
        wtime1 = omp_get_wtime() - wtime1;
        wtime2 = omp_get_wtime();}
        #pragma omp section
        {sines = sine_table(sine_num);
        wtime2 = omp_get_wtime() - wtime2;}
    }
    
	wtime = omp_get_wtime() - wtime;

    pf("\nNumber of primes computed: %d\nLast prime: %d\nNumber of sines computed:	%d\nLast sine computed: %g\n\nElapsed time = %g\n
		Task 1 time = %g\nTask 2 time = %g\n",prime_num,primes[prime_num-1],sine_num,sines[sine_num-1],wtime,wtime1,wtime2);
    
	free(primes);
    free(sines);
    
	pf("MULTITASK:\nNormal end of execution\n\n");
    timestamp();
    return 0;
}
