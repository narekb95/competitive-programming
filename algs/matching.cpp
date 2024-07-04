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

const I INF = 1<<30;

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


class BipMat{
	//only call for vertices on the right side
	bool try_to_augment(I v, V& del, V& matched, const V& dst, const VV& G)
	{
		del[v] = 1; // either will be used now or no path at all
		assert(dst[v] & 1);
		for(auto w: G[v])
		{
			if(del[w] || dst[w] != dst[v] - 1)
			{
				continue;
			}
			if(dst[w] == 0)
			{
				matched[v] = w;
				matched[w] = v;
				del[w] = true;
				return true;
			}
			if(del[matched[w]])
			{
				continue;
			}
			if(try_to_augment(matched[w], del, matched, dst, G))
			{
				matched[v] = w;
				matched[w] = v;
				return true;
			}
		}
		return false;
	}
	I augment(V& matched, const V& dst, const VV& G, I n0, I n1)
	{
		I c = 0;
		V del(n0 + n1);
		for(I i = n0; i < n0 + n1; i++)
		{
			if(matched[i] == -1 && dst[i] != -1)
			{
				c+=try_to_augment(i, del, matched,dst, G);
			}
		}
		return c;
	}
	I sap(V& matched, const VV& G, const I& n0, const I& n1)
	{
		V d(n0 + n1, -1);
		I min_dist = INF;
		queue<I> q;
		for(I i = 0; i < n0; i++)
		{
			if(matched[i] == -1)
			{
				q.push(i);
				d[i] = 0;
			}
		}
		while(!q.empty())
		{
			I i = q.front();
			q.pop();
			if(d[i] > min_dist - 1)
			{
				break;
			}
			if(i < n0)
			{
				for(const I& j : G[i])
				{
					if(d[j] != -1)
					{
						continue;
					}
					d[j] = d[i] + 1;
					if(matched[j] == -1)
					{
						assert(min_dist == d[j] || min_dist == INF);
						min_dist = d[i] + 1;
					}
					else
					{
						q.push(j);
					}
				}
			}
			else
			{
				I j = matched[i];
				if(d[j] != -1)
				{
					continue;
				}
				d[j] = d[i] + 1;
				q.push(j);
			}
		}
		if(min_dist == INF)
		{
			return 0;
		}
		I c = augment(matched, d, G, n0, n1);
		assert(c != 0);
		// cout << "L Matched: ";
		// for(I i = 0; i < n0; i++){
		// 	cout  << matched[i] - n1 << " ";
		// }
		// cout << endl;
		// cout << "R Matched: ";
		// for(I i = 0; i < n1; i++){
		// 	cout  << matched[n0+i] << " ";
		// }
		// cout << endl << endl;
		return c;
	}
	I value;
public:
	BipMat(){}
	I run(VV& G, I n0, I n1) 
	{
		value = 0;
		V matched(n0+n1, -1);
		I v;
		while((v=sap(matched, G, n0, n1)))
		{
			value += v;
		}
		return value;
	}
	I get_val() const
	{
		return value;
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

	I n0 = row_segs.size();
	I n1 = col_segs.size();
	VV G(n0 + n1);
	for(I i = 0; i < n0; i++)
	{
		for(I j =0 ; j < n1; j++)
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
				G[i].push_back(n0 + j);
				G[n0 + j].push_back(i);
			}
		}
	}
	// for(I i = 0; i < n0; i++)
	// {
	// 	cout << i << ":";
	// 	for(auto v: G[i])
	// 	{
	// 		cout << " " << v - n0;
	// 	}
	// 	cout << endl;
	// }


	// cout << "Calling matching" << endl;
	cout << matching.run(G, n0, n1) << endl;
}

int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);

	I t;
	cin >> t;
	for(I i = 0; i < t; i++)
	{
		solve(i+1);
	}

	// solve(0);

	return 0;
}
