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

I mst(const VVP& a)
{
    I n = a.size();
    V dist(n, -1);
    V vis (n, 0);
    priority_queue<P, VP, Cmp> q;
    dist[0] = 0;
    q.push(P(0, 0));
    I cost = 0;
    while(!q.empty())
    {
        auto p = q.top();
        q.pop();
        auto v = p.first;
        auto d = p.second;
        if(vis[v])
        {
            continue;
        }
        vis[v] = true;
        cost += d;
        for(auto np : a[v])
        {
            auto w = np.first;
            auto e = np.second;
            if(dist[w] == -1 || dist[w] > e)
            {
                dist[w] = e;
                q.push(P(w, dist[w]));
            } 
        }
    }
    return cost;
}

I blog(I x)
{
    I ans = 0;
    while(x > 1)
    {
        x>>=1;
        ans++;
    }
    return ans;
}
void solve()
{
    I n, m;
    cin >> n >> m;
    VVP a(n);
    for(I i = 0; i < m; i++)
    {
        I x, y, z;
        cin >> x >> y >> z;
		x--; y--;
        z = blog(z);
        a[x].push_back(P(y, z));
        a[y].push_back(P(x, z));
    }
    I cost = mst(a);
    cout << cost + 1 << endl;    
}
int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);
	I n; 
	cin >> n;
	while(n--){
		solve();
	}
	return 0;
}
