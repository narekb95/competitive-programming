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

const I INF = 1LL<<60;
class Dinic
{
	struct Edge {
		I to, cap, flow;
		I resid() const { return cap - flow; }
	};
	vector<Edge> edges;
	I n;
	VV G;
	I src, trg;
	I flow_value;
	void add_edge(I u, I v, I cap = 1)
	{
		edges.push_back({v, cap});
		edges.push_back({u, 0});
		G[u].push_back(edges.size()-2);
		G[v].push_back(edges.size()-1);
	}

	I dfs(I v, I c, V& itr, const V& dst, const I& delta = 0)
	{
		if(v == trg) return c;
		for(I& i = itr[v]; i < (I)G[v].size(); i++)
		{
			Edge& e = edges[G[v][i]];
			I w = e.to;
			if(dst[w] != dst[v] + 1 || e.resid() == 0) continue;
			I f = dfs(w, min(c, e.resid()), itr, dst, delta);
			if(f)
			{
				e.flow += f;
				edges[G[v][i]^1].flow -= f;
				return f;
			}
		}
		return 0;
	}

	I getLevel(V& dst, const I& delta = 0)
	{
		dst.assign(n, INF);
		dst[src] = 0;
		queue<I> q;
		q.push(src);
		while(!q.empty())
		{
			I u = q.front();
			q.pop();
			for(I i : G[u])
			{
				Edge& e = edges[i];
				I w = e.to;
				if(e.resid() > delta && dst[w] == INF)
				{
					dst[w] = dst[u] + 1;
					q.push(w);
				}
			}
		
		}
		return dst[trg];
	}
public:
	Dinic(){}
	I run(const VVP& a, I src, I trg)
	{
		n = a.size();
		G.assign(n, V());
		edges.clear();
		flow_value = 0;
		this->src = src;
		this->trg = trg;
		for(I i = 0; i < n; i++)
		{
			for(const auto& p : a[i])
			{
				add_edge(i, p.first, p.second);
			}
		}
		V dst;
		while(getLevel(dst) != INF)
		{
			I c;
			V itr(n, 0);
			while((c=dfs(src, INF, itr, dst)))
			{
				flow_value += c;
			}
		}
		return flow_value;
	}	
} dinics;

void solve()
{
	I a, b;
	cin >> a >> b;
	I n = a+b+2;
	I s = n - 2, t = n-1;
	VVP G(n);
	I sum = 0;
	for(I i = 0; i < a; i++)
	{
		I c;
		cin >> c;
		sum += c;
		G[s].push_back({i, c});
	}
	for(I i = 0; i < b; i++)
	{
		I c;
		cin >> c;
		G[a+i].push_back({t, c});
	}
	for(I i = 0; i < b; i++)
	{
		I k;
		cin >> k;
		for(I j = 0; j < k; j++)
		{
			I x;
			cin >> x;
			x--;
			G[x].push_back({a+i, INF});
		}
	}
	cout << sum - dinics.run(G, s, t) << endl;
}

int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);

	I t;
	cin >> t;
	for(int i = 0; i < t; i++)
	{
		solve();
	}

	// solve(0);

	return 0;
}
