#ifndef _STATRAND_
#define _STATRAND_

#include<cstdlib>
#include<cstdio>
#include<cmath>

//structure for getting high fidelity uniform deviates
//taken from 'Numerical Recipes 3rd Edition' page 342.
struct stdRan
{
	unsigned long long u,v,w;
	stdRan(unsigned long long j) : v(4101842887655102017LL), w(1)
	{
		u = j^v; int64();
		v = u; int64();
		w = v; int64();
	}
	inline unsigned long long int64()
	{
		u = u*2862933555777941757LL + 7046029254386353087LL;
		v ^= v >> 17; v ^= v << 31; v ^= v >> 8;
		w = 4294957665U*(w & 0xffffffff) + (w >> 32);
		unsigned long long x = u ^ (u << 21); x ^= x >> 35; x ^= x << 4;
		return (x+v) ^ x;
	}
	inline double doub() {return 5.42101086242752217e-20 * int64();}
	inline unsigned int int32() {return (unsigned int)int64();}
};

#endif
