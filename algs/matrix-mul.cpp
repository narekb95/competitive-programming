#include<bits/stdc++.h>

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
		if(&x != &a[0]) out << " ";
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

const I MOD = 1000000007;
void mul(const VV& a, const VV& b, VV& out)
{
	VV c(a.size(), V(b[0].size(), 0));
	I n = a.size();
	I m = b[0].size();
	for(I i = 0; i < n; i++)
	{
		for(I j = 0; j < m; j++)
		{
			for(I k = 0; k < n; k++)
			{
				c[i][j] += (a[i][k] * b[k][j]) % MOD;
				c[i][j] %= MOD;				
			}
		}
	}
	out = move(c);
}
class MatMul{
	I n;
	VV a;
	VV id;
	vector<VV> po;
public:
	MatMul():po(31){}
	void init(const VV& a){
		this->a = a;
		n = a.size();
		id.assign(n, V(n, 0));
		for(I i = 0; i < n; i++)
		{
			id[i][i] = 1;
		}
		po[0] = a;
		for(I i = 1; i < 31; i++)
		{
			mul(po[i-1], po[i-1], po[i]);
		}
	}
	VV exp(I e)
	{
		VV ans = id;
		for(I i = 0; (1<<i) <= e; i++)
		{
			if((1<<i) & e)
			{
				mul(ans, po[i], ans);
			}
		}
		return ans;
	}
} matmul;

I sum_fib(I n)
{
	static V a1 = {1, 1, 0};
	if(n == 0)
	{
		return 0;
	}
	VV out = matmul.exp(n - 1);
	return out[0][0] + out[0][1]; // mutiply by a1 and take first row

}
void solve(I t)
{
	VV M = {{1,1,1}, {0,1,1}, {0,1,0}};
	matmul.init(M);
	I n;
	cin >> n;
	for(I i = 0; i < n; i++)
	{
		I x, y;
		cin >> x >> y;
		cout << (sum_fib(y) - (x == 0 ? 0 : sum_fib(x-1)) + MOD) % MOD << endl;
	}

}

int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);

	// I t;
	// cin >> t;
	// for(int i = 0; i < t; i++)
	// {
	// 	solve(i+1);
	// }

	solve(0);

	return 0;
}
