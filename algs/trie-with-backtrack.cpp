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

const I ALPHABET_SIZE = 26;
const I ALPHABET_OFFSET = 'a';

struct Node
{
    I max_ind = -2; // -1 if self
    I max_count = 0;
    I end_count = 0;
    vector<Node*> children;
    Node():children(ALPHABET_SIZE, nullptr){}
    ~Node()
    {
        for(auto child : children)
        {
            delete child;
        }
    }
};

class Trie{
    Node* root;
public:
    Trie(): root(new Node()){}
    ~Trie()
    {
        delete root;
    }

    void insert(const string& s)
    {
        stack<pair<Node*, I>> node_ind;
        Node* current = root;
        for(auto c : s)
        {
            I index = c - ALPHABET_OFFSET;
            Node* child = current->children[index];
            node_ind.push({current, index});
            if(child == nullptr)
            {
                child = new Node();
                current->children[index] = child;
            }
            current = child;
        }
        node_ind.push({current, -1}); // -1 for self
        I new_count = ++(current->end_count);

        // Update max_count and max_ind in the path
        while(!node_ind.empty())
        {
            auto [node, index] = node_ind.top();
            node_ind.pop();
            // lexicographically smallest
            if(node->max_count > new_count || 
                (node->max_count == new_count && node->max_ind <=  index))
            {
                break;
            }
            node->max_count = new_count;
            node->max_ind = index;
        }

    }

    void query_prefix(string s)
    {
        Node* current = root;
        for(auto c : s)
        {
            I index = c - ALPHABET_OFFSET;
            Node* child = current->children[index];
            if(child == nullptr)
            {
                cout << -1 << endl;
                return;
            }
            current = child;
        }
        
        while(current->max_ind >= 0)
        {
            I ind = current->max_ind;
            char c = ALPHABET_OFFSET + ind;
            s+=c;
            assert(current->max_count > current->end_count);
            assert(current->children[ind] != nullptr);
            current = current->children[ind];
        }
        assert(current->max_ind == -1);
        cout << s << " " << current->max_count << endl;
    }
};

void solve(I t)
{
    int n;
    cin >> n;
    Trie trie;
    string s;
    for(I i = 0; i < n; i++)
    {
        cin >> s;
        trie.insert(s);        
    }
    I q;
    cin >> q;
    while(q--)
    {
        cin >> s;
        trie.query_prefix(s);
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
