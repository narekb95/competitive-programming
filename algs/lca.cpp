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

I dfs(I u, I p, V& sz, V& par, V& depth, VV& G)
{
	static V vis(G.size());
	vis[u] = true;
	par[u] = p;
	sz[u] = 1;
	depth[u] = (p == -1 ? 0 : depth[p] + 1);
	for(auto v : G[u])
	{
		if(vis[v]) continue;
		sz[u] += dfs(v, u, sz, par, depth, G);
	}
	return sz[u];
}

class LCA {
	const VV& G;
	const V& par;
	const V& dep;
	const I& root;
	VV anc;

	I get_anc(I u, I k)
	{
		I d = 0; 
		while(k)
		{
			if(k&1)
			{
				u = anc[u][d];
			}
			k>>=1;
			d++;
		}
		return u;
	}
	I get_anc_at_dep(I u, I d)
	{
		return get_anc(u, dep[u] - d);
	}
public:
	LCA(const VV& G, const V& par, const V& dep, const I& root):G(G), par(par), dep(dep), root(root), anc(G.size())
	{
		I n = G.size();
		for(I u = 0; u < n; u++)
		{
			if(u == root)
			{
				continue;
			}
			anc[u].push_back(par[u]);
		}
		for(I d = 1; (1<<d) < n; d++)
		{
			for(I u = 0; u < n; u++)
			{
				if(dep[u] < (1<<d))
				{
					continue;
				}
				I v = anc[u][d-1];
				anc[u].push_back(anc[v][d-1]);

			}

		}
	}
	I lca(I u, I v)
	{
		if(dep[u] < dep[v])
		{
			swap(u, v);
		}
		u = get_anc(u, dep[u]-dep[v]);
		I s = 0;
		I e = dep[u] + 1;
		while(s+1 < e)
		{
			I m = (s+e)/2;
			I a_u = get_anc_at_dep(u, m);
			I a_v = get_anc_at_dep(v, m);
			if(a_u == a_v)
			{
				s = m;
			}
			else
			{
				e = m;
			}
		}
		return get_anc_at_dep(v, s);
	}
};

void solve(I t)
{
	I n;
	cin >> n;
	VV G(n);
	for(I i = 0; i < n - 1; i++)
	{
		I u,v;
		cin >> u >> v;
		u--;v--;
		G[u].push_back(v);
		G[v].push_back(u);
	}
	I root = 0;
	V sz(n, 0);
	V par(n, -1);
	V depth(n, 0);
	dfs(root, -1, sz, par, depth, G);

	// cout << "Parents: " << par << endl;
	// cout << "depth:   " << depth << endl;
	// cout << "subtree: " << sz << endl;

	LCA lca(G, par, depth, root);
	I u, v;
	while(cin >> u >> v)
	{
		u--;v--;
		cout << lca.lca(u, v) + 1<< endl;
	}
	//V del(n, 0);
	//I centroid_root = decompose(root, sz, del, par, G);
}

int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);

//	I t;
//	cin >> t;
//	for(int i = 0; i < t; i++)
//	{
//		solve(i+1);
//	}

	solve(0);

	return 0;
}
