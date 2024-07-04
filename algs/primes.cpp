#include<bits/stdc++.h>

using namespace std;

using I = int;
using V = vector<I>;
using VV = vector<V>;

using P = pair<I, I>;
using VP = vector<P>;
using VVP = vector<VP>;

// Prints a vector
template<class T>
ostream& operator <<(ostream& out, const vector<T>& a){
	for(const auto& x : a){
		if(&x != &a[0]) out << " x ";
		out << x;
	}
	return out;
}

// Reads a vector
template<class T>
istream& operator >>(istream& in, vector<T> & a)
{
    for(auto& x : a)
    {
        in >> x;
    }
    return in;
}

template<class S, class T>
ostream& operator <<(ostream& out, const pair<S,T>& p){
	out <<"("<<p.first<<", "<<p.second<<")";
	return out;
}

struct Sieve{

	V factor;
	Sieve(I end) : factor(end)
	{
		for(I i = 3; i < end; i+=2)
		{
			if(factor[i])
			{
				continue;
			}
			for(I j = i; j < end; j+=i)
			{
				if(!factor[j])
					factor[j] = i;
			}
		}
	}

	bool is_prime(I n)
	{
		if(n == 2)
		{
			return true;
		}
		if(n & 1)
		{
			return false;
		}
		return factor[n] == n;
	}

	void factorize(I n, V& out)
	{
		out.clear();
		while(!(n&1) && (n > 1))
		{
			out.push_back(2);
			n >>= 1;
		}
		while(factor[n])
		{
			out.push_back(factor[n]);
			n /= factor[n];
		}
		assert(n == 1);
	}
};
void solve(I t)
{
	static Sieve sieve(10000007);
	static V factors;
	sieve.factorize(t, factors);
	printf("1");
	for(auto f : factors)
	{
		printf(" x %d", f);
	}
	printf("\n");

}

int main()
{
	I t;
	while(scanf("%d", &t) != EOF)
	{
		solve(t);
	}

	// solve(0);

	return 0;
}
