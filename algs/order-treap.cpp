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

template<class S, class T>
ostream& operator <<(ostream& out, const pair<S,T>& p){
	out <<"("<<p.X<<", "<<p.Y<<")";
	return out;
}

struct Pseudo{
	// V a = {28, 19, 26, 27};
	V a = {4, 1, 2, 3};
	I index = 0;
	int rand_I()
	{
		return a[index++];
	}
} prnd;
struct Rand{

	mt19937_64 gen;
	Rand(): gen(chrono::steady_clock::now().time_since_epoch().count()){}

	void shuffle_v(V& a)
	{
		shuffle(a.begin(), a.end(), gen);
	}
	int rand_range(I a, I b)
	{
		return uniform_int_distribution<I>(a, b)(gen);
	}
	int rand_I()
	{
		return gen();
	}
} rnd;


// Assumes that values are associative and commutative
class Treap{

	struct Node
	{
		Node(I priority, I value, I size, I aggregate_value, Node* parent):
		parent(parent),
		priority(priority), value(value), size(size), aggregate_value(aggregate_value){}
		
		~Node()
		{
			delete left;
			delete right;
		}

		Node* left = nullptr;
		Node* right = nullptr;
		Node* parent;
		I priority;
		I value;
		I size;
		I aggregate_value;
	} *root = nullptr;
	
	function<I(I,I,I)> combine;
	I neutral_value;

	void update_aggregate_value(Node* node)
	{
		node->aggregate_value = combine(node->value,
			(node->left == nullptr ? neutral_value : node->left->aggregate_value),
			(node->right == nullptr ? neutral_value : node->right->aggregate_value));

		node->size = 1 
			+ (node->left == nullptr ? 0 : node->left->size)
			+ (node->right == nullptr ? 0 : node->right->size);
	}

	void rotate_upwards(Node* node)
	{
		assert(node != root);
		Node* parent = node->parent;
		if(parent == root)
		{
			root = node;
		}
		else 
		{
			Node* grand_parent = parent->parent;
			if(parent == grand_parent->left)
			{
				grand_parent->left = node;
			}
			else
			{
				grand_parent->right = node;
			}
		}
		node->parent = parent->parent;
		parent->parent = node;

		if(node == parent->right)
		{
			Node* left_child = node->left;
			node->left = parent;
			parent->right = left_child;
			if(left_child != nullptr)
			{
				left_child->parent = parent;
			}
		}
		else
		{
			Node* right_child = node->right;
			node->right = parent;
			parent->left = right_child;
			if(right_child != nullptr)
			{
				right_child->parent = parent;
			}
		}

		update_aggregate_value(parent);
	}

	// offset is how many vertices prcede current subtree globally
	// i.e. offset is the smallest order in the current subtree
	// subtree orders are [offset, offset + size - 1]
	// root order is offset + left_size
	I query_node(Node* node, I so, I eo, I offset)
	{
		if(node == nullptr || so >= node->size + offset || eo < offset)
		{
			return neutral_value;
		}

		if(so <= offset && eo >= offset + node->size - 1)
		{
			return node->aggregate_value;
		}
	
		I self_order = offset + (node->left == nullptr ? 0 : node->left->size);
		I self_value = self_order >= so && self_order <= eo? node->value : neutral_value;
		// Also do empty query since this only returns neutral value
		return combine(self_value, query_node(node->left, so, eo, offset), query_node(node->right, so, eo, offset + 1 + (node->left == nullptr ? 0 : node->left->size)));
	}
public:

	Treap(
		function<I(I,I,I)> combine = [](I x,I l, I r){return max({x, l, r});},
		I neutral_value = 0): combine(combine), neutral_value(neutral_value){}

	~Treap()
	{
		delete root;
	}
	
	void insert_by_order(I order, I value)
	{
		
		I priority = rnd.rand_I();
		// cout << "Inserting " << order << " " << value << " " << priority << endl;
		if(root == nullptr)
		{
			root = new Node(priority, value, 1, value, nullptr);
			return;
		}
		Node * node = nullptr;
		Node * parent = nullptr;

		if(order == root->size)
		{
			parent = root;
			while(parent->right != nullptr)
			{
				parent = parent->right;
			}
			node = parent->right = new Node(priority, value, 1, value, parent);
		}
		else
		{
			parent = find_by_order(order);
			// cout << "Previous order has value " << parent->value << endl;
			if(parent->left != nullptr)
			{
				parent = parent->left;
				while(parent->right != nullptr)
				{
					parent = parent->right;
				}
				node = parent->right = new Node(priority, value, 1, value, parent);
			}
			else
			{
				node = parent->left = new Node(priority, value, 1, value, parent);
			}
		}
		while(node->parent != nullptr && node->priority > node->parent->priority)
		{
			// cout << "Rotating upwards " << node->value << " " << node->parent->value << endl;
			rotate_upwards(node);
		}
		while(node != nullptr)
		{
			update_aggregate_value(node);
			node = node->parent;
		
		}
	}

	Node* find_by_order(I order)
	{
		assert(order < root->size);

		Node* node = root;
		while(true)
		{
			I left_size = node->left == nullptr ? 0 : node->left->size;
			if(order == left_size)
			{
				return node;
			}
			if(order < left_size)
			{
				node = node->left;
			}
			else
			{
				node = node->right;
				order -= left_size + 1;
			}
		}
	}

	I query(I qs, I qe)
	{
		return query_node(root, qs, qe, 0);
	}
};

void solve(I t)
{
	I neutral = - (1LL<< 60);
	Treap treap([](I x,I l, I r){return max({x, l, r});}, neutral);
	I n;
	cin >> n;
	while(n--)
	{
		string q;
		cin >> q;
		if(q == "A")
		{
			I v, o;
			cin >> v >> o;
			o--;
			treap.insert_by_order(o, v);
		}
		else
		{
			I s, e;
			cin >> s >> e;
			s--; e--;
			cout << treap.query(s, e) << endl;
		}
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
