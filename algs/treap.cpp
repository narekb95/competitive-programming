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
		Node(I priority, I key, I value, I size, I aggregate_value, Node* parent):
		parent(parent),
		priority(priority), key(key), value(value), size(size), aggregate_value(aggregate_value),  smallest_key(key), largest_key(key){}
		
		~Node()
		{
			delete left;
			delete right;
		}

		Node* left = nullptr;
		Node* right = nullptr;
		Node* parent;
		I priority;
		I key;
		I value;
		I size;
		I aggregate_value;
		I smallest_key;
		I largest_key;
	} *root = nullptr;
	
	function<I(I,I,I)> combine;
	I neutral_value;

	void print_node(Node* node)
	{
		if(node == nullptr)
		{
			return;
		}
		cout << "start " << node->key << endl;
		print_node(node->left);
		cout << node->key << " " << node->value << " " << node->size << " " << node->aggregate_value << " " << node->priority << endl;
		print_node(node->right);
		cout << "end " << node->key << endl;
	}

	void update_aggregate_value(Node* node)
	{
		node->aggregate_value = combine(node->value,
			(node->left == nullptr ? neutral_value : node->left->aggregate_value),
			(node->right == nullptr ? neutral_value : node->right->aggregate_value));

		node->smallest_key = node->left == nullptr ? node->key : node->left->smallest_key;
		node->largest_key = node->right == nullptr ? node->key : node->right->largest_key;

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

		if(node->key > parent->key)
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

	I query_node(Node* node, I qs, I qe)
	{
		if(node == nullptr || qe < node->smallest_key || qs > node->largest_key)
		{
			return neutral_value;
		}

		if(qs <= node->smallest_key && qe >= node->largest_key)
		{
			return node->aggregate_value;
		}
	
		I self_value = (node->key >= qs && node->key <= qe)? node->value : neutral_value;
		// Also do empty query since this only returns neutral value
		return combine(self_value, query_node(node->left, qs, qe), query_node(node->right, qs, qe));
	}
public:

	Treap(
		function<I(I,I,I)> combine = [](I x,I l, I r){return max({x, l, r});},
		I neutral_value = 0): combine(combine), neutral_value(neutral_value){}

	~Treap()
	{
		delete root;
	}

	void print()
	{
		print_node(root);
	}

	void insert(I key, I value)
	{
		I priority = rnd.rand_I();
		cout << "Inserting " << key << " " << value << " " << priority << endl;
		if(root == nullptr)
		{
			root = new Node(priority, key, value, 1, value, nullptr);
			return;
		}
		
		Node* node = root;
		Node* parent = nullptr;
		while(node != nullptr)
		{
			// cout << node->key << " ";
			node->size++;
			parent = node;
			node = key < node->key ? node->left : node->right;
		}
		// cout << endl;

		if(key < parent->key)
		{
			node = parent->left = new Node(priority, key, value, 1, value, parent);
		}
		else
		{
			node = parent->right = new Node(priority, key, value, 1, value, parent);
		}

		while(node->parent != nullptr && node->priority > node->parent->priority)
		{
			// cout << "Rotating upwards " << node->key << " " << node->parent->key << endl;
			rotate_upwards(node);
		}
		while(node != nullptr)
		{
			update_aggregate_value(node);
			node = node->parent;
		
		}
	}

	I find_by_order(I order)
	{
		assert(order < root->size);

		Node* node = root;
		while(true)
		{
			I left_size = node->left == nullptr ? 0 : node->left->size;
			if(order == left_size)
			{
				return node->key;
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
	I find(I key)
	{
		Node* node = root;
		while(node != nullptr)
		{
			if(key == node->key)
			{
				return node->value;
			}
			node = (key < node->key ? node->left : node->right);
		}
		return -1;
	}

	I query(I qs, I qe)
	{
		return query_node(root, qs, qe);
	}
};

void solve(I t)
{
	I neutral = - (1LL<< 60);
	Treap treap([](I x,I l, I r){return max({x, l, r});}, neutral);
	I n;
	cin >> n;
	I ind = 0;
	while(n--)
	{
		string q;
		I a, b;
		cin >> q >> a >> b;
		if(q == "A")
		{
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
