#define MAX(A,B) (((A) > (B)) ? (A) : (B))
#include<stdio.h>
#include<math.h>
#include<float.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

int main()
{
	double *T,*Tnew,*Tmp;
	double to1,var=DBL_MAX,top=100.0;
	unsigned n,n2,maxIter,i,j,iter=0;
	int itemsread;
	FILE *fout;
	printf("Enter mesh size,max iterations and tolerance: ");
	itemsread=scanf("%u, %u,%lf",&n,&maxIter,&to1);
	printf("itemsread is = %d",itemsread);        
	if(itemsread!=3)
	{
		fprintf(stderr,"Input Error");
		exit(1);
	}
                       
	time_t startTime =clock();
	n2=n+2;
	T=(double *) calloc (n2*n2,sizeof(*T));
	Tnew=(double *) calloc (n2*n2,sizeof(*T));
	if(T==NULL || Tnew ==NULL)
	{
		fprintf(stderr,"Not enough memory!");                                                          
                exit(1);                          
	}

	for(i=1;i<=n;i++)
	{
		T[(n+1)*n2+i] = Tnew[(n+1)*n2+i] = i*top/(n+1);
		T[i*n2+n+1] = Tnew[i*n2+n+1] =i*top/(n+1);
	}

	for(;var>to1 && iter<=maxIter;)
	{
		++iter;
		var=0.0;
		for(i=1;i<=n;++i)
		{
			for(j=1;j<=n;j++)
			{
				Tnew[i*n2+j] = 0.25 * (T[(i-1) *  n2 + j] + T[(i+1) * n2 + j] + T[i*n2+(j-1)] + T[i*n2+(j+1)]);
				var = MAX(var,fabs(Tnew[i*n2+j] - T[i*n2+j]));
			}
		}
		Tmp = T; T= Tnew; Tnew=Tmp;
		if(iter %100 == 0)
			printf("iter: %8u, variation=%12.4lE\n",iter,var);
	}

	double endTime = (clock() - startTime) / (double) CLOCKS_PER_SEC;
	printf("Elapsed time (s) = %lf\n", endTime);
	printf("Mesh size: %u\n",n);
	printf("Maximum error: %lE\n",var);
	
	fout = fopen("results","w");
	if(fout == NULL)
	{
		perror("results");
		exit(1);
	}
	for(i=1;i<=n;++i)
		for(j=1;j<=n;++j)
			fprintf(fout,"%8u %8u %18.9lE\n",i,j,T[i*n2+j]);
	fclose(fout);
	free(T);
	free(Tnew);
	return 0;
}            
