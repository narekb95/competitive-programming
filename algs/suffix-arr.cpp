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

class SuffixArray
{
	V cls, ord; // class, order
	I n;
	string s_cyc;
public:
	SuffixArray(){}
	void Build(const string& s)
	{
		n = s.size();
		s_cyc = s + s;
		ord.resize(n);
		cls.resize(n);
		iota(ord.begin(), ord.end(), 0);	
		sort(ord.begin(), ord.end(), [&](I i, I j){
			return s[i] < s[j];
		});
		for(I i = 0; i < n; i++)
		{
			if(i == 0)
			{
				cls[ord[0]] = 0;
				continue;
			}
			if(s[ord[i]] == s[ord[i-1]])
			{
				cls[ord[i]] = cls[ord[i-1]];
			}
			else
			{
				cls[ord[i]] = cls[ord[i-1]] + 1;
			}
		}
		auto cmp = [&](I a, I b, I l){
			if(cls[a] != cls[b])
			{
				return (cls[a] - cls[b] > 0) * 2 - 1;
			}
			I a2 = a + (1<<l);
			if(a2 >= n) a2 -= n;
			I b2 = b + (1<<l);
			if(b2 >= n) b2 -= n;
			if(cls[a2] != cls[b2])
			{
				return (cls[a2] - cls[b2] > 0) * 2 - 1;
			}
			return 0;
		};
		for(I i = 1; (1<<i) <= n; i++)
		{
			sort(ord.begin(), ord.end(), [&](I a, I b){
				return cmp(a, b, i - 1) < 0;
			});
			V cls_new(n);
			for(I j = 0; j < n; j++)
			{
				if(j == 0)
				{
					cls_new[ord[0]] = 0;
					continue;
				}
				if(cmp(ord[j], ord[j-1], i - 1) == 0)
				{
					cls_new[ord[j]] = cls_new[ord[j-1]];
				}
				else
				{
					cls_new[ord[j]] = cls_new[ord[j-1]] + 1;
				}
			}
			cls = move(cls_new);
		}
	}
} suff;

void solve(I t)
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
