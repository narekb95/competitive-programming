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

class SuffTree
{
	struct Node
	{
		I parent = -1;
		vector<I> children;

		I start = 0;
		I end = 0;

		I size = 0;
		Node():children(26, -1){}

		Node(I parent, I start, I end, I size):parent(parent),children(26, -1),start(start), end(end), size(size){}
	};
	
	string s;
	I l;
	vector<Node> nodes;

	void insert(I q_ind)
	{
		I ind = 0;
		I i = q_ind;
		while(i < l)
		{
			Node& curr = nodes[ind];
			// cout << "\"" << s.substr(curr.start, curr.end-curr.start) << "\" " << curr.size << endl;
			I j = curr.start;
			while(i < l && j < curr.end && s[i] == s[j])
			{
				i++;
				j++;
			}
			if(i == l && j == curr.end)
			{
				curr.size++;
				return;
			}
			if(j == curr.end)
			{
				curr.size++;
				if(curr.children[s[i]-'a'] == -1)
				{
					curr.children[s[i]-'a'] = nodes.size();
					nodes.push_back(Node(ind, i, l, 1));
					return;
				}
				ind = curr.children[s[i]-'a'];
				continue;
			}
			// either s ended first or diverged
			// cut node at index j
			I new_ind = nodes.size();
			nodes.push_back(Node(curr.parent, curr.start, j, curr.size+1));
			Node& new_node = nodes.back();
			new_node.children[s[j]-'a'] = ind;
			// update child of parent
			nodes[new_node.parent].children[s[nodes[ind].start]-'a'] = new_ind;
			// update current
			nodes[ind].parent = new_ind;
			nodes[ind].start = j;
			// process new node
			if(i < l)
			{
				new_node.children[s[i]-'a'] = nodes.size();
				nodes.push_back(Node(new_ind, i, l, 1));
			}
			return;
		}
	}
public:
	SuffTree(){}
	~SuffTree()
	{
	}
	void init(const string& s)
	{
		this->s = s;
		l = s.size();
		nodes.assign(1, Node());
		for(I i = 0; i < l; i++)
		{
			insert(i);
		}
	}

	I count(const string& qs)
	{
		I ind = 0;
		I n = qs.size();
		I i = 0;
		while(i < n)
		{
			const Node& curr = nodes[ind];
			// cout << curr.start << " " <<curr.end << " " << "\"" << s.substr(curr.start, curr.end-curr.start) << "\" " << curr.size << endl;
			I j = curr.start;
			while(i < n && j < curr.end && qs[i] == s[j])
			{
				i++;
				j++;
			}
			if(i == n)
			{
				return curr.size;
			}
			if(j == curr.end)
			{
				if(curr.children[qs[i]-'a'] == -1)
				{
					return 0;
				}
				ind = curr.children[qs[i]-'a'];
				continue;
			}
			return 0;
		}
		assert(false);
	}
} suff;

void solve()
{
	I n, q;
	cin >> n >> q;
	string s;
	cin >> s;
	suff.init(s);
	string t;
	while(q--)
	{
		cin >> t;
		cout << suff.count(t) << '\n';
	}
}

int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);

	// I t;
	// cin >> t;
	// for(int i = 0; i < t; i++)
	// {
	// 	solve(i+1);
	// }

	solve();

	return 0;
}
