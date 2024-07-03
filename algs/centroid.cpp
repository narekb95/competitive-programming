#include<bits/stdc++.h>

#include<queue>

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

I dfs(I u, I p, V& sz, V& par, V& depth, VVP& G)
{
	static V vis(G.size());
	vis[u] = true;
	par[u] = p;
	sz[u] = 1;
	depth[u] = (p == -1 ? 0 : depth[p] + 1);
	for(auto p : G[u])
	{
		I v = p.first;
		// I c = p.second;
		if(vis[v]) continue;
		sz[u] += dfs(v, u, sz, par, depth, G);
	}
	return sz[u];
}

class Centroid{

	struct Node_data{
		I state = 0;
		I parent;
		I depth;
		V ancestor_distance;
		V descendants;
		// priority_queue<P, VP, greater<P>> white_descendants;
		multiset<I> white_descendants;


		friend ostream& operator<<(ostream& out, const Node_data& data)
		{
			out << "Parent: " << data.parent + 1 << ".  Depth: " << data.depth <<  ". Descendants: " << data.descendants << ". Distances: " << data.ancestor_distance << endl;
			return out;
		}
	};

	struct Subtree_data{
		I neighbor;
		I center;
		I distance_to_white;
		Subtree_data(){}
		Subtree_data(I neighbor, I center, I distance_to_white = -1):neighbor(neighbor), center(center), distance_to_white(distance_to_white){};
	};

	const VVP& G;
	const V& par;
	const I& root;

	vector<Node_data> dec_data;
	vector<vector<Subtree_data>> subtrees; // maps root of subtree to its data
	

public:
	Centroid(const VVP& G, const V& par, const V& sz, const I& root):G(G), par(par), root(root), dec_data(G.size()), subtrees(G.size())
	{
		V del(G.size(), 0);
		V dyn_sz = sz;
		decompose(root, -1, 0, dyn_sz, del);
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
			dst = dec_data[x].ancestor_distance[anc_ind];
		}
	}

	I query(I x)
	{
		if(dec_data[x].state == 1)
		{
			return 0;
		}
		const auto& data = dec_data[x];	
		I opt = INFTY;
		I anc = x;
		I anc_ind = 0;
		while(anc != -1){
			auto& anc_data = dec_data[anc];
			I d = INFTY;
			if(anc_data.state == 1)
			{
				d = 0;
			}
			else
			{
				d = anc_data.white_descendants.empty() ? INFTY : *anc_data.white_descendants.begin();
			}
			if(d != INFTY)
			{
				opt = min(opt, d + data.ancestor_distance[anc_ind]);
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
				I v = p.first;
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

	I decompose(I u, I dec_p, I d, V& dyn_sz, V& del)
	{
		I center = find_centroid(u, dyn_sz, del);
		auto& data = dec_data[center];
		auto& subtree = subtrees[center];
		subtree.assign(G[center].size(), Subtree_data());
		// cout << center << endl;
		
		data.descendants.push_back(center);
		data.ancestor_distance.push_back(0);
		data.parent = dec_p;
		data.depth = d;
		del[center] = 1;

		I p = par[center];
		while(p != -1)
		{
			dyn_sz[p] -= dyn_sz[center];
			p = par[p];
		}
		
		for(const auto& p : G[center]) // possibly parent as well
		{
			const auto& neighbor = p.first;
			if(del[neighbor]) continue;
			const auto& edge_weight = p.second;
			I neighbor_ind = distance(&G[center][0], &p);
			
			I sub_root = decompose(neighbor, center, d+1, dyn_sz, del);
			subtree[neighbor_ind] = Subtree_data(neighbor, sub_root);
			for(const auto& w : dec_data[sub_root].descendants)
			{
				data.descendants.push_back(w);
				I dst = get_cost(w, neighbor) + edge_weight;
				dec_data[w].ancestor_distance.push_back(dst);
			}
		}
		return center;
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
		I dst_u = dec_data[u].ancestor_distance[dec_data[u].depth - dec_data[pu].depth];
		I dst_v = dec_data[v].ancestor_distance[dec_data[v].depth - dec_data[pv].depth];
		return dst_u + dst_v;
	}
};

void solve(I t)
{
	I n;
	cin >> n;
	VVP G(n);
	for(I i = 0; i < n - 1; i++)
	{
		I u,v;
		I c;
		cin >> u >> v;
		c = 1;
		u--;v--;
		G[u].push_back(P{v, c});
		G[v].push_back(P{u, c});
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
	cin >> tt;
	while(tt--)
	{
		I q, x;
		cin >> q >> x;
		x--;
		I v;
		if(q == 0)
		{
			centroid.flip_state(x);
		}
		else
		{
			cout << ((v = centroid.query(x)) == INFTY?-1: v) << endl;
		}
	}
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
