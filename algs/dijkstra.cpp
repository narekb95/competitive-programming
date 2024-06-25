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
struct Cmp{
    bool operator()(const P& a, const P& b)
    {
        return b.second < a.second;
    }
};

void dijkstra(const I& src, const VVP& a, V& dist)
{
    int n = a.size();
    dist.assign(n, -1);
    priority_queue<P, VP, Cmp> q;
    dist[src] = 0;
    q.push(P(src, 0));
    while(!q.empty())
    {
        auto p = q.top();
        q.pop();
        auto v = p.first;
        auto d = p.second;
        if(d > dist[v])
        {
            continue;
        }
        for(auto np : a[v])
        {
            auto w = np.first;
            auto e = np.second;
            if(dist[w] == -1 || dist[w] > dist[v] + e)
            {
                dist[w] = dist[v] + e;
                q.push(P(w, dist[w]));
            } 
        }
    }
}

int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);
    I n, m;
    cin >> n >> m;
    VVP a(n);
    for(int i = 0; i < m; i++)
    {
        int x, y, z;
        cin >> x >> y >> z;
        a[x].push_back(P(y, z));
        a[y].push_back(P(x, z));
    }
    V dist;
    dijkstra(0, a, dist);
    cout << dist << endl;
	return 0;
}
