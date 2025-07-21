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

class LCA {
	const VV& G;
	const V& par;
	const V& dep;
	const I& root;
	VV anc;

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

	I get_anc(I u, I k) const
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
	I get_anc_at_dep(I u, I d) const
	{
        if(dep[u] < d){
            throw invalid_argument("Invalid depth");
        }
		return get_anc(u, dep[u] - d);
	}

    bool is_anc_of(I u, I w) const
    {
        return (dep[u] <= dep[w] && get_anc_at_dep(w, dep[u]) == u);
    }

};

void dfs(I v, I p, I t, V& tin, V& desc, V& parent, V& depth, const VV& G)
{
    parent[v] = p;
    desc[v] = tin[v] = t++;
    depth[v] = (p == -1 ? 0 : depth[p] + 1);
    for(auto w : G[v])
    {
        // cout << w << endl;
        if(w == p) continue;
        if(tin[w] == -1)
        {
            dfs(w, v, t, tin, desc, parent, depth, G);
            desc[v] = min(desc[v], desc[w]);
        }
        else
        {
            desc[v] = min(desc[v], tin[w]);
        }
    }
}


void solve()
{
	I n, m;
	scanf("%d %d", &n, &m);
	VV G(n);
	for(I i = 0; i < m; i++)
	{
		I u,v;
		scanf("%d %d", &u, &v);
		u--;v--;
		G[u].push_back(v);
		G[v].push_back(u);
	}
	I root = 0;
	// V sz(n, 0);
    V tin(n, -1);
    V desc(n, -1);
	V par(n, -1);
	V depth(n, 0);

    depth[root] = 0;
    dfs(root, -1, 0, tin, desc, par, depth, G);

    // for(I i = 0; i < n; i++)
    // {
    //     cout << par[i] + 1 << " ";
    // }
    // cout << endl;
	// cout << "Parents: " << par << endl;
	// cout << "depth:   " << depth << endl;
	// cout << "subtree: " << sz << endl;

	LCA lca(G, par, depth, root);

    I q;
    scanf("%d", &q);
    // "da": can get from u to v, "ne": otherwise.

    while(q--)
    {
        I qtype;
        scanf("%d", &qtype);
        if(qtype == 1)
        {
            // Edge query
            // A and B will be different. G1 and G2 will represent an existing road.
            I u, v, ex, ey;
            scanf("%d %d %d %d", &u, &v, &ex, &ey);
            u--; v--; ex--; ey--;
            if(depth[ex] > depth[ey]) {
                swap(ex, ey);
            }
            if(par[ey] != ex) {
                printf("da\n");
                continue;
            }
            bool y_anc_u = lca.is_anc_of(ey, u);
            bool y_anc_v = lca.is_anc_of(ey, v);
            if(y_anc_u && y_anc_v){
                printf("da\n");
                continue;
            }
            if(!y_anc_u && !y_anc_v) {
                printf("da\n");
                continue;
            }
            if(y_anc_v){
                swap(u,v);
                swap(y_anc_u, y_anc_v);
            }
            // assert(y_anc_u && !y_anc_v);

            // It is an edge in the tree.
            // y ancestor of u only
            
            // assert(desc[ey] <= desc[u]);
            if(desc[ey] <= tin[ex]) {
                printf("da\n");
            } else {
                printf("ne\n");
            }
        } else {
            // Vertex query
            // A, B and C will be distinct integers
            I u, v, sep;
            scanf("%d %d %d", &u, &v, &sep);
            u--; v--; sep--;

            bool is_anc_of_u = lca.is_anc_of(sep, u);
            bool is_anc_of_v = lca.is_anc_of(sep, v);

            // [todo] handle that sep is one of the two vertices.

            if(!is_anc_of_u && !is_anc_of_v) {
                printf("da\n");
                continue;
            }
            if(is_anc_of_v){
                swap(u,v);
                swap(is_anc_of_u, is_anc_of_v);
            }
            // Now is_anc_of_u is true.
            // assert(is_anc_of_u);
            if(!is_anc_of_v)
            {
                I sep_child = lca.get_anc_at_dep(u, depth[sep] + 1);
                if(desc[sep_child] < tin[sep]) {
                    printf("da\n");
                } else {
                    printf("ne\n");
                }
                continue;
            }
            I anc = lca.lca(u, v);
            if(anc != sep)
            {
                printf("da\n");
                continue;
            }
            I sep_child_u = lca.get_anc_at_dep(u, depth[sep] + 1);
            I sep_child_v = lca.get_anc_at_dep(v, depth[sep] + 1);
            if(desc[sep_child_u] < tin[sep] && desc[sep_child_v] < tin[sep]) {
                printf("da\n");
            } else {
                printf("ne\n");
            }
        }
    }
}


int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);
	solve();

	return 0;
}
