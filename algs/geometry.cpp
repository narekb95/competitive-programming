#include<bits/stdc++.h>

using namespace std;

using I = long long int;
using V = vector<I>;
using VV = vector<V>;

using P = pair<I, I>;
using VP = vector<P>;
using VVP = vector<VP>;

template<class T>
class Point{
    T x, y;
public:
    Point(T x = 0, T y = 0) : x(x), y(y) {}

    T getX() const { return x; }
    T getY() const { return y; }

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
template<class T>
using Segment = pair<Point<T>, Point<T>>;

// Prints a vector
template<class T>
ostream& operator <<(ostream& out, const vector<T>& a){
	for(const auto& x : a){
		if(&x != &a[0]) out << " ";
		out << x;
	}
	return out;
}


template<class T>
istream& operator >>(istream& in, pair<T, T>& p){
    in >> p.first >> p.second;
    return in;
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


void update(const pair<double, double>& src,  pair<double, double>& trg, const Segment<double>& s1, const Segment<double>& s2)
{
    // update first free:
    double vf1 = src.first + s1.first.dist(s2.second);
    double vf2 = src.second + s1.second.dist(s2.second);
    trg.first = min(trg.first, min(vf1, vf2));
    // update second free:
    double vs1 = src.first + s1.first.dist(s2.first);
    double vs2 = src.second + s1.second.dist(s2.first);
    trg.second = min(trg.second, min(vs1, vs2));
}

void solve(I t, I n)
{
    vector<Segment<double>> segments(n);
    cin >> segments;


    vector<double> lengths(n);
    transform(segments.begin(), segments.end(), lengths.begin(), [](const Segment<double>& s) {
        return s.first.dist(s.second);
    });
    double total_length = accumulate(lengths.begin(), lengths.end(), 0.0);

    const double MAX = 1e9;
    vector<vector<pair<double, double>>> dp(1<<n, vector<pair<double, double>>(n, {MAX, MAX}));

    for(I i = 0; i < n; i++)
    {
        dp[1 << i][i] = {0, 0};
    }
    for(I mask = 0; mask < (1<<n); mask++)
    {
        for(I i = 0; i < n; i++){
            if(!(mask & (1 << i))) continue;
            for(I j = 0; j < n; j++)
            {
                if(mask & (1 << j)) continue;
                const auto &src = dp[mask][i];
                auto &trg = dp[mask|(1 << j)][j];
                update(src, trg, segments[i], segments[j]);
            }
        }
    }
    double ans = MAX;
    const double FULL = (1<<n)- 1;
    for(I i = 0; i < n; i++)
    {
        ans = min(ans, min(dp[FULL][i].first, dp[FULL][i].second));
    }
    ans += total_length;
    cout << "Case " << t << ": " << ans << endl;

}

int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);
    cout << fixed << setprecision(5);

	I n;
    I t = 1;
    cin >> n;
    while(n)
	{
		solve(t++, n);
        cin >> n;
	}

	// solve(0);

	return 0;
}
