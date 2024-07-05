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

class SuffTree
{
	string s;
	I l;
	struct Node
	{
		Node* parent = nullptr;
		vector<Node*> children;

		I start;
		I end;

		I size = 0;
		I count_end = 0; // count end is stuped since we len is fixed then suff is fixed
		Node():children(26, nullptr){}
		~Node()
		{
			for(auto& child : children)
			{
				delete child;
			}
		}

		Node(Node* parent, I start, I end, I size, I count_end):parent(parent),children(26, nullptr),start(start), end(end), size(size), count_end(count_end){}
	} *root = new Node();

	void insert(I q_ind)
	{
		Node* curr = root;
		I i = q_ind;
		while(i < l)
		{
			I j = curr->start;
			while(i < l && j < curr->end && s[i] == s[j])
			{
				i++;
				j++;
			}
			if(i == l && j == curr->end)
			{
				curr->size++;
				curr->count_end++;
				return;
			}
			if(j == curr->end)
			{
				curr->size++;
				if(curr->children[s[i]-'a'] == nullptr)
				{
					curr->children[s[i]-'a'] = new Node(curr, i, l, 1, 1);
					return;
				}
				curr = curr->children[s[i]-'a'];
				continue;
			}
			// either s ended first or diverged
			Node* new_node = new Node(curr->parent, curr->start, j, curr->size+1, 0);
			new_node->parent->children[s[curr->start]-'a'] = new_node;
			new_node->children[s[j]-'a'] = curr;
			curr->parent = new_node;
			curr->start = j;
			if(i == l)
			{
				new_node->count_end = 1;
			}
			else
			{
				new_node->children[s[i]-'a'] = new Node(new_node, i, l, 1, 1);
			}
			return;
		}
	}
public:
	SuffTree(){}
	~SuffTree()
	{
		delete(root);
	}
	void init(const string& s)
	{
		this->s = s;
		l = s.size();
		delete(root);
		root = new Node();
		for(I i = 0; i < l; i++)
		{
			insert(i);
		}
	}

	I count(const string& qs)
	{
		Node* curr = root;
		I n = qs.size();
		I i = 0;
		while(i < n)
		{
			I j = curr->start;
			while(i < n && j < curr->end && qs[i] == s[j])
			{
				i++;
				j++;
			}
			if(i == n)
			{
				return curr->size;
			}
			if(j == curr->end)
			{
				if(curr->children[qs[i]-'a'] == nullptr)
				{
					return 0;
				}
				curr = curr->children[qs[i]-'a'];
				continue;
			}
			return 0;
		}
		assert(false);
	}
} suff;

void solve(I t)
{
	I n, q;
	cin >> n >> q;
	string s;
	cin >> s;
	suff.init(s);
	while(q--)
	{
		string t;
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

	solve(0);

	return 0;
}
