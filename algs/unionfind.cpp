#include<bits/stdc++.h>

using namespace std;

using I = long long int;
using V = vector<I>;
using VV = vector<V>;

using P = pair<I, I>;
using VP = vector<P>;
using VVP = vector<VP>;

using IP = pair<I, P>;
using VIP = vector<IP>;

#define X  first
#define Y  second

const I MD = 1000LL * 1000 * 1000;
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
	out <<"("<<p.X<<", "<< p.Y<<")";
    return out;
}


struct DSU{
    V parent;
    V size;
    I total_component_pairs = 0;
    DSU(I n): parent(n), size(n, 1)
    {
        for(I i = 0; i < n; i++)
        {
            parent[i] = i;
        }
    }
    I find_root(I v)
    {
        queue<I> q;
        while(parent[v] != v)
        {
            q.push(v);
            v = parent[v];
        }
        while(!q.empty())
        {
            I x = q.front();
            q.pop();
            parent[x] = v;
        }
        return v;
    }
    void join(I x, I y)
    {
        x = find_root(x);
        y = find_root(y);
        if(x == y)
        {
            return;
        }
        if(size[x] > size[y])
        {
            swap(x, y);
        }

        const auto num_pairs = [](I x){return (x*(x-1))/2;};
        
        I s1 = size[x];
        I s2 = size[y];
        total_component_pairs += num_pairs(s1+s2) - num_pairs(s1) - num_pairs(s2);
        total_component_pairs %= MD;

        size[y] += size[x];
        parent[x] = y;
    }
    bool same_set(I x, I y)
    {
        return find_root(x) == find_root(y);
    }
};


void solve()
{
    I n, m;
    cin >> n >> m;
    // VVP a(n);
    VIP a;
    for(I i = 0; i < m; i++)
    {
        I x, y, z;
        cin >> x >> y >> z;
        x--;
        y--;
        // a[x].push_back(P(y, z));
        // a[y].push_back(P(x, z));
        a.push_back(IP(z, P(x, y)));
    }
    sort(a.begin(), a.end(), greater<IP>());
    DSU dsu(n);
    auto ans = 0;
    for(const auto e : a)
    {
        I u = e.Y.X;
        I v = e.Y.Y;
        dsu.join(u, v);
        ans += dsu.total_component_pairs * e.X;
        ans %= MD;
    }
    cout << ans << endl;
}
int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);

    solve();


	return 0;
}
 