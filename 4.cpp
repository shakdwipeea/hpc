#include <math.h>
#include<string.h>
#include<omp.h>
#include<iostream>

using namespace std;

double t;

inline long Strike (bool composite[], long i, long stride, long limit)
{
	for (; i <= limit; i += stride)
		composite[i] = true;
	
	return i;
}

long ParallelSieve( long n ) 
{
	long count = 0;
	long m = (long)sqrt((double)n);
	long n_factor = 0;
	long* factor = new long[m];
	
	t= omp_get_wtime();
	
	#pragma omp parallel
	{
		bool* composite = new bool[m+1];
		long* striker = new long[m];
		#pragma omp single
		{
			memset( composite, 0, m );
			for( long i=2; i<=m; ++i )
			if( !composite[i] )
			{
				++count;
				Strike( composite, 2*i, i, m );
				factor[n_factor++] = i;
			}
		}
		long base = -1;
		
		#pragma omp for reduction (+:count)
			for( long window=m+1; window<=n; window+=m )
			{
				memset( composite, 0, m );
				if( base!=window )
				{
					// Must compute striker from scratch.
					base = window;
					for( long k=0; k<n_factor; ++k )
						striker[k] = (base+factor[k]-1)/factor[k] * factor[k] - base;
				
				}	long limit = min(window+m-1,n) - base;
				
				for( long k=0; k<n_factor; ++k )
					striker[k] = Strike( composite, striker[k],factor[k], limit ) - m;
				
				for( long i=0; i<=limit; ++i )
					if( !composite[i] )
						++count;
				base += m;
			}	
		delete[] striker;
		delete[] composite;
	}
	
	t= omp_get_wtime()-t;
	
	delete[] factor;
	return count;
}

long CacheFriendlySieve (long n)
{
	long count = 0;
	long m = (long) sqrt ((double) n);
	bool *composite = new bool[n + 1];
	memset (composite, 0, n + 1);
	long *factor = new long[m];
	long *striker = new long[m];
	long n_factor = 0;
 
	t = omp_get_wtime ();
	for (long i = 2; i <= m; ++i)
    {
		if (!composite[i])
		{
			++count;
			striker[n_factor] = Strike (composite, 2 * i, i, m);
			factor[n_factor++] = i;
		}
    }
	for (long window = m + 1; window <= n; window += m)
    {
		long limit = min (window + m - 1, n);
		for (long k = 0; k < n_factor; ++k)
			striker[k] = Strike (composite, striker[k], factor[k], limit);
		
		for (long i = window; i <= limit; i++)
			if (!composite[i])
				++count;
    }
	t = omp_get_wtime () - t;
	delete[]striker;
	delete[]factor;
	delete[]composite;
	return count;
}

int main()
{
    long count = ParallelSieve(10000);
    cout << count << " Time : " << t <<endl;
	
	long count1 = CacheFriendlySieve (10000);
	cout << count1<< "Time :" << t << endl;
}
