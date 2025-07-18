/**
 * @class Treap
 * @brief Implements a randomized balanced binary search tree (Treap) with augmented range queries.
 *
 * The Treap class provides efficient insertion, deletion, and range query operations on a set of key-value pairs.
 * Each node in the Treap is assigned a random priority, ensuring expected logarithmic time complexity for operations.
 * The Treap is augmented to support associative and commutative aggregate queries (e.g., min, max, sum) over key ranges.
 *
 * Features:
 * - Insert or upsert key-value pairs, maintaining balance via randomized priorities.
 * - Query aggregate values over a range of keys using a user-defined combine function.
 * - Find the k-th smallest key (order statistics).
 * - Efficient removal of nodes.
 * - Customizable aggregate operation and neutral value.
 *
 * Template Parameters:
 * - The combine function must be associative and commutative.
 *
 * Usage Example:
 * @code
 * Treap treap([](I x, I l, I r){ return min({x, l, r}); }, LLONG_MAX);
 * treap.insert(5, 10);
 * I min_value = treap.query(3, 7);
 * @endcode
 */
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

struct Rand{

	mt19937_64 gen;
	Rand(): gen(chrono::steady_clock::now().time_since_epoch().count()){}

	void shuffle_v(V& a)
	{
		shuffle(a.begin(), a.end(), gen);
	}
	I rand_range(I a, I b)
	{
		return uniform_int_distribution<I>(a, b)(gen);
	}
	I rand_I()
	{
		return uniform_int_distribution<I>(0, LLONG_MAX)(gen);
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

    void remove_node(Node* node)
    {
        if(node == nullptr) return;
		// cout << "Rotating" << endl;
		while(rotate_downwards(node));
		// cout << "done" << endl;
		auto parent = node->parent;
		if(parent != nullptr)
		{
			if(parent->left == node)
			{
				parent->left = nullptr;
			}
			else
			{
				parent->right = nullptr;
			}
		}
		else
		{
			root = nullptr;
		}
		delete node;
		update_aggregate_values_up(parent);
	}

	// doesn't change aggregate values
	// but all changes become ancestors?
	bool rotate_downwards(Node* node) 
	{
		auto left = node->left;
		auto right = node->right;
		auto parent = node->parent;
		if(left == nullptr && right == nullptr)
		{
			return false;
		}
		if(left == nullptr || (right != nullptr && right->priority > left->priority))
		{
			node->parent = right;
			right->parent = parent;
			node->right = right->left;
			right->left = node;
			if(parent == nullptr)
			{
				root = right;
			}
			else if(parent->left == node)
			{
				parent->left = right;
			}
			else
			{
				parent->right = right;
			}
		}
		else
		{
			node->parent = left;
			left->parent = parent;
			node->left = left->right;
			left->right = node;
			if(parent == nullptr)
			{
				root = left;
			}
			else if(parent->left == node)
			{
				parent->left = left;
			}
			else
			{
				parent->right = left;
			}
		}
		return true;
	}	

    void update_aggregate_values_up(Node* node)
    {
        while(node != nullptr)
        {
            update_aggregate_value(node);
            node = node->parent;
        }
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


	Node* find_key(I key)
	{
		Node* node = root;
		while(node != nullptr)
		{
			if(key == node->key)
			{
				return node;
			}
			node = (key < node->key ? node->left : node->right);
		}
		return nullptr;
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

	void upsert(I key, I value)
	{
		// cout << "searching" << endl;
		auto* node = find_key(key);
		if(node != nullptr)
		{
			// cout << "Removing" << endl;
			value = combine(value, node->value, neutral_value);
			remove_node(node);
		}
		// cout << "Inserting key: " << key << ", value: " << value << endl;
		insert(key, value);
	}

	void insert(I key, I value)
	{
		I priority = rnd.rand_I();
		if(root == nullptr)
		{
			root = new Node(priority, key, value, 1, value, nullptr);
			return;
		}
		
		Node* node = root;
		Node* parent = nullptr;
		while(node != nullptr)
		{
			parent = node;
			node = key < node->key ? node->left : node->right;
		}

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
			rotate_upwards(node);
		}
		if(node->parent == nullptr)
		{
			root = node;
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

	I query(I qs, I qe)
	{
		return query_node(root, qs, qe);
	}
};

void solve()
{
	I neutral = (1LL<< 60);
	I n, k;
	cin >> n >> k;
    V a(n);
    cin >> a;
    V pre(n, 0);
    pre[0] = a[0];
    for(I i = 1; i < n; i++)
    {
        pre[i] = pre[i-1] + a[i];
    }

    I min_sum = -30000 * 15000 - 10;
    I max_sum = 30000 * 15000 + 10;
    I start = min_sum, end = max_sum + 1;
    while(start < end-1)
    {
        I m = (start + end) / 2;
	    Treap treap([](I x,I l, I r){return min({x, l, r});}, neutral);
        treap.upsert(0, 0);
        bool bad = false;
        // cout << start << " " << end << " " << m << endl;
        for(I i = 0; i < n; i++)
        {
            I pref_target = pre[i] - m;
            I bst = treap.query(pref_target, max_sum);
            // cout <<  treap.query(pref_target - 1, max_sum) << endl;
            // cout << "i= " << i << ", a[i]: " << a[i] << ", pre[i]: " << pre[i] << ", target: " << pref_target << ", best: " << bst << endl;

            if (bst == neutral)
            {
                bad = true;
                // cout << "breaking for too large value" << endl;
                break;
            }
            if(i == n-1)
                bad = (bst + 1 > k);
			// cout << "upserting: " << pre[i] << ", bst + 1: " << bst + 1 << endl;
            treap.upsert(pre[i], bst + 1); // allowing duplicate indices for now
			// cout << "done" << endl;
        }
        if(bad)
        {
            start = m;
        }
        else
        {
            end = m;
        }
    }
    cout << end << endl;
}

int main()
{
	//Make IO fast
	ios_base::sync_with_stdio(0);

 
    I t;
    cin >> t;
    while(t--)
    {
        solve();
    }
	return 0;
}
