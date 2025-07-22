#include<bits/stdc++.h>

using namespace std;

typedef long long int I;
typedef vector<I> V;
typedef vector<V> VV;

typedef pair<I, I> P;
typedef vector<P> VP;
typedef vector<VP> VVP;

typedef pair<I, P> IP;
typedef vector<IP> VIP;

#define X  first
#define Y  second

const I MD = 1000LL * 1000 * 1000;
// Prints a vector
template<class T>
ostream& operator <<(ostream& out, const vector<T>& a){
	for(int i = 0; i < (int)a.size(); i++){
		out << a[i] << " " ;
	}
	return out;
}


template<class T>
istream& operator >>(istream& in, pair<T,T> & a)
{
	in >> a.X >> a.Y;
    return in;
}


// Reads a vector
template<class T>
istream& operator >>(istream& in, vector<T> & a)
{
    for(int i = 0; i < (int)a.size(); i++)
    {
        in >> a[i];
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
	I num_components;
    DSU(I n): parent(n), size(n, 1), num_components(n)
    {
		for(int i = 0; i < n; i++) {
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
		num_components--;
        if(size[x] > size[y])
        {
            swap(x, y);
        }

        size[y] += size[x];
        parent[x] = y;
    }
	I get_num_components() const
	{
		return num_components;
	}
    bool same_set(I x, I y)
    {
        return find_root(x) == find_root(y);
    }
};

template<class T>
class Point{
    T x, y;
public:
    Point(T x = 0, T y = 0) : x(x), y(y) {}

    T getX() const { return x; }
    T getY() const { return y; }


	bool close(const Point& other, T r) const {
		return (x - other.x) * (x - other.x) + (y - other.y) * (y - other.y) <= r * r;
	}

    bool operator <(const Point& other) const {
        if (x != other.x) return x < other.x;
        return y < other.y;
    }

    bool operator ==(const Point& other) const {
        return x == other.x && y == other.y;
    }

    Point operator +(const Point& other) const {
        return Point(x + other.x, y + other.y);
    }
    
    double dist() const {
        return sqrt(x * x + y * y);
    }

    double dist(const Point& other) const {
        return sqrt((x - other.x) * (x - other.x) + (y - other.y) * (y - other.y));
    }

    friend ostream& operator <<(ostream& out, const Point& p) {
        out << "(" << p.x << ", " << p.y << ")";
        return out;
    }

    friend istream& operator >>(istream& in, Point& p) {
        in >> p.x >> p.y;
        return in;
    }
};


struct Edge {
	I from, to, type;
	double weight;
	Edge(I f, I t, double w, I ty) : from(f), to(t), type(ty), weight(w) {}
};

bool compareEdges(const Edge& a, const Edge& b) {
	if (a.weight != b.weight)
		return a.weight < b.weight;
	else
		return a.type < b.type;
}

void solve(I t)
{
    I n, r, w, u, v;
	cin >> n >> r >> w >> u >> v;
	vector<Point<I> > a(n);
	cin >> a;
	vector<Edge> G;
	for(I i = 0; i < n; i++)
	{
		for(I j = i + 1; j < n; j++)
		{
			if(a[i].close(a[j], r))
			{
				G.push_back(Edge(i, j, a[i].dist(a[j]) * u, 0));
			}
			else
			{
				G.push_back(Edge(i, j, a[i].dist(a[j]) * v, 1));
			}
		}
	}
	
	sort(G.begin(), G.end(), compareEdges);
    DSU dsu(n);

	I ccs = n;
	double cost[2];
	cost[0] = 0;
	cost[1] = 0;
	for(int i = 0; i < (int)G.size(); i++)
	{
		const Edge& e = G[i];
		if(ccs <= w)
		{
			break;
		}
		if(!dsu.same_set(e.from, e.to))
		{
			dsu.join(e.from, e.to);
			cost[e.type] += e.weight;
			ccs--;
		}
	}
	cout << "Caso #" << t << ": " << cost[0] << " " << cost[1] << "\n";
	
}
int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);
	cout << fixed << setprecision(3);
	I t;
	cin >> t;
	for(int i = 0; i < t; i++)
    {
        solve(i+1);
    }


	return 0;
}
 