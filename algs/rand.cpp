#include<bits/stdc++.h>
#include <random>
#include <chrono>

using namespace std;

using I = long long int;
using V = vector<I>;
using VV = vector<V>;

using P = pair<I, I>;
using VP = vector<P>;
using VVP = vector<VP>;

// Prints a vector
template<class T>
ostream& operator <<(ostream& out, const vector<T>& a){
	for(const auto& x : a){
		out << x << " " ;
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
	out <<"("<<p.X<<", "<<p.Y<<")";
	return out;
}

void solve(I t)
{

}

struct Rand{

	mt19937_64 gen;
	Rand(): gen(chrono::steady_clock::now().time_since_epoch().count()){}

	void shuffle_v(V& a)
	{
		shuffle(a.begin(), a.end(), gen);
	}
	int rand_range(I a, I b)
	{
		return uniform_int_distribution<I>(a, b)(gen);
	}
	int rand_I()
	{
		return gen();
	}
} rnd;

int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);


	int r = 20;
	while(r--)
	{
		V permutation(10);
		iota(permutation.begin(), permutation.end(), 0);
		rnd.shuffle_v(permutation);
		cout << permutation << endl;
	}


	I t;
	cin >> t;
	for(int i = 0; i < t; i++)
	{
		solve(i+1);
	}


	return 0;
}
