#include<stdio.h>
#include<stdlib.h>
#include<omp.h>
#include<math.h>
#define k 4
#define POINTS_SIZE 100000
#define PRINT_POINTS 0

double cur_distance;
double cluster_mean[k];
long long cluster_count[k];
int points[POINTS_SIZE][2];

void populate_points()
{
	long long i;
	for(i=0;i<POINTS_SIZE;i++)
	{
		srand(i);
		points[i][0]=rand()%100;
		points[i][1]=-1;
    }

	for(i=0;i<k;i++)
	{
		cluster_mean[i]=points[i][0];
        cluster_count[i]=1;
		points[i][1]=i;
    }
}

double get_distance(double x,double y){
        double diff=x-y;
        if(diff<0)
			diff*=-1;
        
        return diff;
}

void main()
{
	int nt,cluster_index,j;
	double t,min_dist,cur_dist;
	long long i;
	
	printf("\en enter the number of threads : ");
	scanf("%d",&nt);
	printf("\n");

	populate_points();
	
	t=omp_get_wtime();
	
	#pragma omp parallel for private(i,j,min_dist,cur_dist,cluster_index) num_threads(nt)
	for(i=0;i<POINTS_SIZE;i++)
	{
		min_dist=1000,cur_dist=-1;
	    cluster_index=-1;
	    for(j=0;j<k;j++)
		{
			cur_dist=get_distance((double)points[i][0],cluster_mean[j]);
			if(cur_dist<min_dist)
			{
				min_dist=cur_dist;
				cluster_index=j;
			}
		}
	    
		if(PRINT_POINTS!=0)
			printf("\n%d belongs to cluster %d",points[i][0],cluster_index+1);
	
		points[i][1]=cluster_index;
        
		#pragma omp critical
        {
			cluster_mean[cluster_index]=((cluster_mean[cluster_index]*cluster_count[cluster_index])
										+points[i][0])/(cluster_count[cluster_index]+1);
            cluster_count[cluster_index]++;
        }
	}

	t=omp_get_wtime()-t;
	
	printf("\n\nTime taken :%lf\n",t);
}
