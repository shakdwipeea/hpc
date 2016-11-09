#include<math.h>
#include<string.h>
#include<omp.h>
#include<iostream>

using namespace std;

double t = 0.0;


inline long Strike (bool composite[], long i, long stride, long limit)
{
	for (; i <= limit; i += stride)
		composite[i] = true;
	
	return i;
}


long CacheUnfriendlySieve (long n)
{
	long count = 0;
	long m = (long) sqrt ((double) n);
	bool *composite = new bool[n + 1];
	memset (composite, 0, n + 1);
	t = omp_get_wtime ();
	for (long i = 2; i <= m; ++i)
		if (!composite[i])
		{
			++count;
			Strike (composite, 2 * i, i, n);
		}
	for (long i = m + 1; i <= n; ++i)
		if (!composite[i])
			++count;
    
  t = omp_get_wtime () - t;
  
  delete[]composite;
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

int main ()
{
	long count = CacheUnfriendlySieve (1000000000);
	cout << count;
	cout << "Time :" << t << endl;
	long count1 = CacheFriendlySieve (1000000000);
	cout << count1;
	cout << "Time :" << t << endl;

