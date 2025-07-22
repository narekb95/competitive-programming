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
    I tot_count = 0;
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
        Node* current = root;
        root->tot_count++;
        for(auto c : s)
        {
            I index = c - ALPHABET_OFFSET;
            Node* child = current->children[index];
            if(child == nullptr)
            {
                child = new Node();
                current->children[index] = child;
            }
            current = child;
            current->tot_count++;
        }
        current->end_count++;
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
                cout << 0 << endl;
                return;
            }
            current = child;
        }
        cout << current->tot_count << endl;
    }
};

void solve(I t)
{
    int n, q;
    cin >> n >> q;
    Trie trie;
    string s;
    for(I i = 0; i < n; i++)
    {
        cin >> s;
        trie.insert(s);        
    }
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
