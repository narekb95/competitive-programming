#include<bits/stdc++.h>

using namespace std;

using I = int;
using V = vector<I>;
using VV = vector<V>;

using P = pair<I, I>;
using VP = vector<P>;
using VVP = vector<VP>;

using VM = vector<map<I, I>>;

const I INFTY = 1<< 30;

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
	out <<"("<<p.first<<", "<<p.second<<")";
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

class Centroid{

	struct Ancestor_data
	{
		I ancestor;
		I neighbor;
		I distance;
		Ancestor_data(){}
		Ancestor_data(I ancestor, I neighbor, I distance = -1):ancestor(ancestor), neighbor(neighbor), distance(distance){}
	};

	struct Node_data{
		I state = 0;
		I parent;
		I depth;
		// priority_queue<P, VP, greater<P>> white_descendants;
		multiset<I> white_descendants;


		friend ostream& operator<<(ostream& out, const Node_data& data)
		{
			out << "Parent: " << data.parent + 1 << ".  Depth: " << data.depth << endl;
			return out;
		}
	};

	// struct Subtree_data{
	// 	I neighbor;
	// 	I center;
	// 	I distance_to_white;
	// 	Subtree_data(){}
	// 	Subtree_data(I neighbor, I center, I distance_to_white = -1):neighbor(neighbor), center(center), distance_to_white(distance_to_white){};
	// };

	const VV& G;
	const V& par;
	I root;

	I max_depth = 0;
	I dec_root;
	vector<Node_data> dec_data;
	vector<vector<Ancestor_data>> ancestors; // maps root of subtree to its data
	

public:
	Centroid(const VV& G, const V& par, const V& sz, const I& root):G(G), par(par), root(root), dec_data(G.size()), ancestors(G.size())
	{
		I n = G.size();
		V del(n, 0);
		V dyn_sz = sz;
		vector<Ancestor_data> ancestor_arr(n);
		decompose(root, -1, 0, ancestor_arr, 0, dyn_sz, del);
		// cout << "Decomposition done\n";
		// for(I i = 0; i < n; i++)
		// {
		// 	const auto& data = dec_data[i];
		// 	cout << i+1 << ": depth:" << data.depth << ", parent: " << data.parent + 1 << endl;
		// }
		// compute ancestor distances
		// neighbor of any anc is in the subtree so can compute by depth of anc
		for(I d = max_depth -1, i; d >= 0; d--)
		{
			for(i = 0; i < n; i++)
			{
				// if equal depth => same element => already computed
				if(dec_data[i].depth <= d)
				{
					continue;
				}
				I d_diff = dec_data[i].depth - d;
				auto &anc = ancestors[i][d_diff];
				anc.distance = get_cost(i, anc.neighbor) + 1;
				// cout << i+1 << " " << anc.ancestor + 1 << " " << anc.neighbor + 1 << " " << anc.distance << endl;
			}
		}
		// for(I i = 0; i < n; i++)
		// {
		// 	cout << i+1 << ": ";
		// 	for(auto& anc : ancestors[i])
		// 	{
		// 		cout << "("<<anc.ancestor + 1 << ", " << anc.distance << "), ";
		// 	}
		// 	cout << endl;
		// }
		// cout << "Ancestor distances computed\n";
	}

	void flip_state(I x)
	{
		I& state = dec_data[x].state;
		state = 1 - state;

		// if(state == 0)
		// {
		// 	return;
		// }
		// I anc = dec_data[x].parent;
		// I anc_ind = 1;
		// while(anc != -1)
		// {
		// 	dec_data[anc].white_descendants.push(P{dec_data[x].ancestor_distance[anc_ind], x});
		// 	anc = dec_data[anc].parent;
		// 	anc_ind++;
		// }
		for(const auto& anc_o : ancestors[x])
		{
			auto& anc_data = dec_data[anc_o.ancestor];
			I dst = anc_o.distance;
			if(state == 0)
			{
				auto it = anc_data.white_descendants.find(dst);
				anc_data.white_descendants.erase(it);
			}
			else
			{
				anc_data.white_descendants.insert(dst);
			}
		}
	}

	I query(I x)
	{
		if(dec_data[x].state == 1)
		{
			return 0;
		}
		I opt = INFTY;
		for(const auto& anc_o : ancestors[x])
		{
			const auto& anc_data = dec_data[anc_o.ancestor];
			I d = anc_data.white_descendants.empty() ? INFTY : *anc_data.white_descendants.begin();
			if(d==INFTY)
			{
				continue;
			}
			opt = min(opt, d + anc_o.distance);
		}
		return opt;
	}
private:	
	// Always start the search from the highest point in the subtree
	I find_centroid(I u, const V& dyn_sz, const V& del)
	{
		while(par[u] != -1 && !del[par[u]])
		{
			u = par[u];
		}
		I subtree_size = dyn_sz[u];
		// cout <<	 u << " " << dyn_sz[u] << " " << subtree_size << endl;

		bool done = false;
		while(!done)
		{
			done = true;
			for(auto p : G[u])
			{
				I v = p;
				if(del[v] || v == par[u])
				{
					continue;
				}
				if(dyn_sz[v] > subtree_size/2)
				{
					u = v;
					done = false;
					break;
				}
			}
		}
		return u;
	}

	void decompose(I u, I d_p, I d, vector<Ancestor_data>& anc, I ind, V& dyn_sz, V& del)
	{
		I center = find_centroid(u, dyn_sz, del);
		if(dyn_sz[u] == (int)G.size())
		{
			dec_root = center;
		}
		dec_data[center].depth = d;
		dec_data[center].parent = d_p;
		max_depth = max(d, max_depth);
		del[center] = 1;

		anc[ind++] = {center, -1, 0};
		ancestors[center].resize(ind);
		copy(anc.begin(), anc.begin()+ind, ancestors[center].rbegin());

		I p = par[center];
		while(p != -1)
		{
			dyn_sz[p] -= dyn_sz[center];
			p = par[p];
		}
		
		for(I neighbor : G[center]) // possibly parent as well
		{
			if(del[neighbor]) continue;
			anc[ind-1].neighbor = neighbor;
			anc[ind-1].distance = -1;
			decompose(neighbor, center, d+1, anc, ind, dyn_sz, del);
		}
	}

	I get_cost(I u, I v)
	{
		I pu = u;
		I pv = v;
		while(pu != pv)
		{
			if(dec_data[pu].depth > dec_data[pv].depth)
			{
				pu = dec_data[pu].parent;
			}
			else
			{
				pv = dec_data[pv].parent;
			}
		}
		I dst_u = ancestors[u][dec_data[u].depth - dec_data[pu].depth].distance;
		I dst_v = ancestors[v][dec_data[v].depth - dec_data[pv].depth].distance;
		return dst_u + dst_v;
	}
};

void solve()
{
	I r;
	I n;
	r = scanf("%d", &n);
	VV G(n);
	for(I i = 0; i < n - 1; i++)
	{
		I u,v;
		r = scanf("%d %d", &u, &v);
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

	Centroid centroid(G, par, sz, root);
	
	I tt;
	r = scanf("%d", &tt);
	while(tt--)
	{
		I q, x;
		r = scanf("%d %d", &q, &x);
		x--;
		I v;
		if(q == 0)
		{
			centroid.flip_state(x);
		}
		else
		{
			printf("%d\n", ((v = centroid.query(x)) == INFTY?-1: v));
			// cout << ((v = centroid.query(x)) == INFTY?-1: v) << endl;
		}
	}
	assert(r);
}



int main()
{
	solve();
	return 0;
}
