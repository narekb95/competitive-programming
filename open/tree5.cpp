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
	const I& root;

	vector<Node_data> dec_data;
	vector<vector<Ancestor_data>> ancestors; // maps root of subtree to its data
	

public:
	Centroid(const VV& G, const V& par, const V& sz, const I& root):G(G), par(par), root(root), dec_data(G.size()), ancestors(G.size())
	{
		I n = G.size();
		V del(n, 0);
		V dyn_sz = sz;
		vector<Ancestor_data> ancestor_arr(n);
		decompose(root, 0, ancestor_arr, 0, dyn_sz, del);
		cout << "Decomposition done\n";
		// compute ancestor distances
		for(I i =0 ; i < n; i++)
		{
			for(auto& anc : ancestors[i])
			{
				anc.distance = get_cost(i, anc.neighbor) + 1;
			}
		}
		cout << "Ancestor distances computed\n";
		// I ind = 0;
		// for(const auto& data : dec_data)
		// {
		// 	cout << ind + 1<< ": " << data << endl;
		// 	ind ++;
		// }
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

		I anc = x;
		I anc_ind = 0;
		I dst = 0;
		while(anc != -1)
		{
			auto& anc_data = dec_data[anc];
			if(state == 0)
			{
				auto it = anc_data.white_descendants.find(dst);
				anc_data.white_descendants.erase(it);
			}
			else
			{
				anc_data.white_descendants.insert(dst);
			}
			anc = anc_data.parent;
			anc_ind++;
			dst = ancestors[x][anc_ind].distance;
		}
	}

	I query(I x)
	{
		if(dec_data[x].state == 1)
		{
			return 0;
		}
		I opt = INFTY;
		I anc = x;
		I anc_ind = 0;
		while(anc != -1){
			auto& anc_data = dec_data[anc];
			I d = INFTY;
			d = anc_data.white_descendants.empty() ? INFTY : *anc_data.white_descendants.begin();
			if(d != INFTY)
			{
				opt = min(opt, d + ancestors[x][anc_ind].distance);
			}
			anc = anc_data.parent;
			anc_ind++;
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

	void decompose(I u, I d, vector<Ancestor_data>& anc, I ind, V& dyn_sz, V& del)
	{
		I center = find_centroid(u, dyn_sz, del);
		
		dec_data[center].depth = d;
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
			decompose(neighbor, d+1, anc, ind + 1, dyn_sz, del);
		}
	}

	I get_cost(I u, I v)
	{
		if(dec_data[u].depth < dec_data[v].depth)
		{
			swap(u, v);
		}
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
