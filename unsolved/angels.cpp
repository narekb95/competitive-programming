#include<bits/stdc++.h>

using namespace std;

using I = int;
using V = vector<I>;
using VV = vector<V>;

using P = pair<I, I>;
using IP = pair<I, P>;
using VP = vector<P>;
using VIP = vector<IP>;
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

const I INF = 1<<30;
class Flow
{
	VV cap, flow;
	I src, sink;
	I flow_val = 0;

	I find_aug_path(const VV& G)
	{
		I n = G.size();
		queue<I> q;
		V parent(n, -1);
		q.push(src);
		parent[src]= src;
		while(!q.empty())
		{
			I u = q.front();
			q.pop();
			if(u == sink)
			{
				I c = INF;
				for(I v = sink; v != src; v = parent[v])
				{
					c = min(c, cap[parent[v]][v] - flow[parent[v]][v]);
				}
				augment(parent, c);
				return c;
			}
			for(const I& v : G[u])
			{
				if(parent[v] == -1 && cap[u][v] > flow[u][v])
				{
					parent[v] = u;
					q.push(v);
				}
			}
		}
		return 0;
	}
	void augment(const V& parent, I c)
	{
		for(I v = sink; v != src; v = parent[v])
		{
			flow[parent[v]][v] += c;
			flow[v][parent[v]] -= c;
		}
	}
	void run(const VV& G)
	{
		I n = G.size();
		for(I u = 0; u < n; u++)
		{
			for(const I& v : G[u])
			{
				cap[u][v] = 1;
			}
		}
		I c;
		while((c = find_aug_path(G)))
		{
			flow_val += c;
		}
	}
public:
	Flow(){}
	void init(const VV& G, I src, I sink)
	{
		I n = G.size();
		this->src = src;
		this->sink = sink;
		cap.assign(n, V(n));
		flow.assign(n, V(n));
		flow_val = 0;
		run(G);
	}
	I get_flow() const
	{
		return flow_val;
	}
} flow;

class BipMat{
public:
	BipMat(){}
	void run(VV& G, I n0) 
	{
		VV F(G);
		I n = F.size();
		I n1 = n - n0;
		I src = n;
		I sink = n + 1;
		F.push_back(V(n0));
		F.push_back(V());

		iota(F.back().begin(), F.back().end(), 0);
		for(I i = 0; i < n1; i++)
		{
			F[n0 + i].push_back(n+1);
		}

		flow.init(F, src, sink);
	}
	I get_val() const
	{
		return flow.get_flow();
	}
} matching;

void solve(I t)
{
	I n, m;
	cin >> n >> m;
	VV a(n, V(m));
	char x;
	for(I i = 0; i < n; i++)
	{
		for(I j = 0; j < m; j++)
		{
			cin >> x;
			if(x == 'H')
			{
				a[i][j] = 0;
			}
			else if(x == 'A'){
				a[i][j] = 1;
			}
			else
			{
				a[i][j] = 2;
			}
		}
	}
	// cout <<a << endl;

	VIP row_segs;
	VIP col_segs;
	for(I i = 0; i < n; i++)
	{
		I s = 0;
		bool contains_cells = false;
		for(I j = 0; j < m; j++)
		{
			if(a[i][j] == 0)
			{
				contains_cells = true;
			}
			else if(a[i][j] == 1)
			{
				if(contains_cells)
				{
					row_segs.push_back({i, {s, j-1}});
				}
				s = j+1;
				contains_cells = false;
			}
		}
		if(contains_cells)
		{
			row_segs.push_back({i, {s, m-1}});
		}
	}
	// cout << "Rows: " << row_segs << endl;

	for(I j = 0; j < m; j++)
	{
		I s = 0;
		bool contains_cells = false;
		for(I i = 0; i < n; i++)
		{
			if(a[i][j] == 0)
			{
				contains_cells = true;
			}
			else if(a[i][j] == 1)
			{
				if(contains_cells)
				{
					col_segs.push_back({j, {s, i-1}});
				}
				s = i+1;
				contains_cells = false;
			}
		}
		if(contains_cells)
		{
			col_segs.push_back({j, {s, n-1}});
		}
	}
	// cout << "Columns: " << col_segs << endl;

	auto intersect = [](IP x, IP y)
	{
		if(x.first < y.second.first || x.first > y.second.second || y.first < x.second.first || y.first > x.second.second)
		{
			return P(-1, -1);
		}
		return P(x.first, y.first);
	};

	I n1 = row_segs.size();
	I n2 = col_segs.size();
	VV G(n1 + n2);
	for(I i = 0; i < n1; i++)
	{
		for(I j =0 ; j < n2; j++)
		{
			const auto& x = row_segs[i];
			const auto& y = col_segs[j];
			P p = intersect(x, y);
			if(p.first == -1)
			{
				continue;
			}
			if(a[p.first][p.second] == 0)
			{
				G[i].push_back(n1 + j);
			}
		}
	}
	// for(I i = 0; i < n1; i++)
	// {
	// 	cout << i << ":";
	// 	for(auto v: G[i])
	// 	{
	// 		cout << " " << v - n1;
	// 	}
	// 	cout << endl;
	// }


	// cout << "Calling matching" << endl;
	matching.run(G, n1);
	cout << matching.get_val() << endl;
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
