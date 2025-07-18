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

template<class T>
class SegmentTree {
    I n;
    const vector<T>& a;
    vector<T> tree;
    function<T(T, T)> accumulate;
    T identity;
public:
    SegmentTree(const vector<T>& arr, function<T(T, T)> func, T identity): a(arr), n(arr.size()), tree(4 * n), accumulate(func), identity(identity) {
        build(0, 0, n - 1);
    }
    
    T query(I l, I r) {
        return query(0, 0, n - 1, l, r);
    }
    void update(I idx, T value) {
        update(0, 0, n - 1, idx, value);
    }

private:
    void build(I node, I start, I end) {
        if (start == end) {
            tree[node] = a[start];
            return;
        }
        I mid = (start + end) / 2;
        build(2 * node + 1, start, mid);
        build(2 * node + 2, mid + 1, end);
        tree[node] = accumulate(tree[2 * node + 1], tree[2 * node + 2]);
    }

    T query(I node, I start, I end, I l, I r) {
        if(l >= start && r <= end) {
            return tree[node];
        }
        if(r < start || l > end) {
            return identity;
        }
        I mid = (start + end) / 2;
        T left = query(2 * node + 1, start, mid, l, r);
        T right = query(2 * node + 2, mid + 1, end, l, r);
        return accumulate(left, right);
    }

    void update(I node, I start, I end, I idx, T value) {
        if (start == end) {
            tree[node] = value;
            return;
        }
        I mid = (start + end) / 2;
        if (idx <= mid) {
            update(2 * node + 1, start, mid, idx, value);
        } else {
            update(2 * node + 2, mid + 1, end, idx, value);
        }
        tree[node] = accumulate(tree[2 * node + 1], tree[2 * node + 2]);
    }

};

void solve(I t)
{

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
