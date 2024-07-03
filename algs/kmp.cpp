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

P kmp(string s, string t)
{
	I n = s.size();
	I m = t.size();
	I cnt = 0;
	I p0 = -1;

	V rec(m, -1);
	I j = -1;
	for(I i = 1; i < m; i++)
	{
		while(j != -1 && t[j+1] != t[i])
		{
			j = rec[j];
		}
		if(t[j+1] == t[i])
		{
			rec[i] = (++j);
		} else {
			assert(j == -1);
			rec[i] = j;
		}
	}
	j = -1;
	for(I i = 0; i < n; i++)
	{
		while(j != -1 && t[j+1] != s[i])
		{
			j = rec[j];
		}
		if(t[j+1] == s[i])
		{
			j++;
		}
		if(j == m-1)
		{
			
			if(p0 == -1)
			{
				p0 = i-m+1;
			}
			cnt++;
			j = rec[j];
		}
	}
	return {cnt, p0};
}
void solve(I n)
{
	
}

int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);

	I t;
	cin >> t;
	for(int i = 0; i < t; i++)
	{
		solve(i+1);
	}

	// solve(0);

	return 0;
}
