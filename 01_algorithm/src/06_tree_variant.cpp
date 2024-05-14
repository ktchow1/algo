#include<iostream>
#include<vector>
#include<string>
#include<map>
#include<optional>

// 1. heaptree
// 2. btree
// 3. skiplist
// 4. prefix tree


// given node n, its children are : 2n+1 and 2n+2
// given node n, its parent is : (n-1)/2
template<typename T>
class heaptree
{
public:
    bool empty() const noexcept
    {
        return impl.empty();
    }

    const T& top() const noexcept
    {
        return impl[0];
    }

    void push(const T& x)
    {
        impl.push_back(x);
        float_last_leaf(); 
    }

    void pop()
    {
        impl.front() = std::move(impl.back());
        impl.pop_back();
        sink_root();
    }

    void float_last_leaf()
    {
        std::uint32_t n = impl.size()-1;
        while(n>0)
        {
            std::uint32_t m = (n-1)/2;
            if (impl[n] < impl[m]) 
            {
                std::swap(impl[n], impl[m]);
                n = m;
            }
            else return;
        }
    }

    void sink_root()
    {
        std::uint32_t n = 0;
        while(true)
        {
            std::uint32_t lhs = 2*n+1;
            std::uint32_t rhs = 2*n+2;
            if (impl.size() > rhs) // both lhs, rhs exist
            {
                if (impl[lhs] <= impl[rhs])
                {
                    if (impl[n] > impl[lhs])
                    {
                        std::swap(impl[n], impl[lhs]);
                        n = lhs;
                    }
                    else return;
                }
                else
                {
                    if (impl[n] > impl[rhs])
                    {
                        std::swap(impl[n], impl[rhs]);
                        n = rhs;
                    }
                    else return;
                }
            }
            else if (impl.size() == rhs) // only lhs exists
            {
                std::swap(impl[n], impl[lhs]);
                n = lhs;
            }
            else // no child
            {
                return;
            }
        }
    }

private:
    std::vector<T> impl; 
};

// **************************************************************************** //
// Differences between btree and binary-tree
// 1. number of children = M
//    number of keys     = M-1
// 2. all leaves have same depth
// 3. insertion algorithm is bottom up approach
// 4. insertion algorithm needs child-to-parent link, so btree is doubly linked   
// **************************************************************************** //
namespace btree 
{
template<typename K, typename V>
struct node
{
    node(const K& key, const V& value) :  last_child(nullptr), parent(nullptr) 
    {
        impl[key] = std::make_pair(value, nullptr);
    }

    node(const K& key, const V& value, node<K,V>* x) : last_child(nullptr), parent(nullptr)
    {
        impl[key] = std::make_pair(value, nullptr);
    }

    // ***************************************** //
    // The following :
    // 1. ensures #children = #keys + 1
    // 2. either impl[any_key].second == nullptr or
    //                     last_child == nullptr  
    //    then this_node is considered as a leaf
    // ***************************************** //
    std::map<K, std::pair<V, node<K,V>*>> impl; // "pair::second" is the child with value below "pair::first"
    node<K,V>* last_child;
    node<K,V>* parent;
};

template<typename K, typename V>
std::optional<V> search(const node<K,V>* root, const K& key)
{
    if (!root) return std::nullopt;

    node<K,V>* this_node = root;
    while(this_node)
    {
        // find the first iter with key >= target key
        auto iter = this_node->impl.lower_bound(key); 

        // [step 1] check against key (return straight away)
        if (iter != this_node->impl.end())
        {
            if (iter->first == key) return std::make_optional(iter->second.first);
        }

        // [step 2] check between keys (traverse to child node)
        if (iter != this_node->impl.end())
        {
            this_node = iter->second.second;
        }
        else 
        {
            this_node = this_node->last_child;
        }
    }
    return std::nullopt;
}

template<typename K, typename V>
void insert(const node<K,V>* root, const K& key, const V& value)
{
    // When implement promote for btree, find promoted_key as this_node.impl's median
    // -  modify this_node.impl (remove promoted key and all element beyond it)
    // -  create next_node.impl (contain all element beyond promoted key)
    //
    // please consider 3 cases (actually 2 only) :
    // 1. if this_node is the root, then create one parent ... 
    //    parent.impl[promoted_key] = {promoted_value, this_node}
    //    parent.last_child         =                  next_node
    //    next_node.parent = parent
    //
    // 2. if this_node is not root and if it is parent's last_child ...
    //    same as above
    //
    // 3. if this_node is not root and if it is NOT parent's last_child ...
    //    parent.impl[original_key] = {original_value, this_node}
    //    parent.impl[promoted_key] = {promoted_value, next_node}
    //    next_node.parent = parent
    //
    // If parent.impl.size() >= M, then promote(parent))
}
}

namespace skiplist
{
template<typename K, typename V>
struct node
{
    K key;
    V value;

    std::vector<node<K,V>*> shortcuts; 
    // shortcut[0] is native list's link
    // shortcut[N] is faster shortcut (bigger N means faster)
};

template<typename K, typename V>
std::optional<V> search(const node<K,V>* head, const K& key)
{
    if (!head) return std::nullopt;

    // head should have all shortcuts
    node<K,V>* this_node = head;
    std::uint32_t lane = head->shortcuts.size()-1;
    
    // 2 dimensional search
    while(true)
    {
        assert(lane < this_node->shortcuts.size(), "bug in skiplist topology");
        if (this_node->key == key)
        {
            return std::make_optional(this_node->value);
        }
        // horizontal move
        if (this_node->shortcuts[lane] != nullptr &&
            this_node->shortcuts[lane]->key <= key)
        {
            this_node = this_node->shortcuts[lane];
        }
        // vertical move
        else if (lane > 0)
        {
            --lane;
        }
        // next node in shortcut[0] is either greater than key or nullptr
        else
        {
            return std::nullopt;
        }
    }
}
}

namespace prefix_tree
{
// Unlike btree and skiplist
// 1. key is not stored explicitly inside node of prefix_tree
// 2. key type must be string
template<typename V> 
struct node
{
    node() : value(std::nullopt)
    {
    }

    node(const V& v) : value(v)
    {
    }

    std::optional<V> value;
    std::map<char, node<V>*> impl;
};

template<typename V> 
std::optional<V> search(const node<V>* root, const std::string& key)
{
    if (!root) return std::nullopt;
    const node<V>* this_node = root;
    std::uint32_t n=0; 

    while(true) // Todo : please change to for(const auto& c:key)
    {
        auto iter = this_node->impl.find(key[n]);
        if (iter != this_node->impl.end())
        {
            if (n == key.size()-1)
            {
                return iter->second->value; // note : it can be std::nullopt, for example, key = TEMPLA
            }
            else
            {
                this_node = iter->second;
                ++n;
            }
        }
        else 
        {
            return std::nullopt;
        }
    }
    return std::nullopt;
}

template<typename V>
void insert(node<V>* root, const std::string& key, const V& value)
{
    if (!root) return;
    node<V>* this_node = root; // BUG : you copy this line from search() above, forget to remove const, result in pages of errors
    std::uint32_t n=0; 

    while(true) // Todo : please change to for(const auto& c:key)
    {
        auto iter = this_node->impl.find(key[n]);
        if (iter != this_node->impl.end())
        {
            if (n == key.size()-1)
            {
                iter->second->value = std::make_optional(value); // overwrite existing node
                return;
            }
            else
            {
                this_node = iter->second;
                ++n;
            }
        }
        else 
        { 
            // leaf
            if (n == key.size()-1)
            {
                this_node->impl.insert(std::make_pair(key[n], new node<V>(value)));
                return;
            }
            // non-leaf
            else
            {
                auto [next_iter, flag] = this_node->impl.insert(std::make_pair(key[n], new node<V>()));
                this_node = next_iter->second;
                ++n;
            }
        }
    }
}
}

// ************ //
// *** Test *** //
// ************ //
void test_heaptree()
{
    heaptree<std::uint32_t> tree;
    for(std::uint32_t n=0; n!=900; ++n)
    {
        tree.push(rand()%1000);
    }

    std::cout << "\nheap tree";
    std::uint32_t n=0;
    while(!tree.empty())
    {
        if (n%30 ==0) std::cout << "\n";
        std::cout << tree.top() << " "; 
        tree.pop();
        ++n;
    }
}

using namespace std::string_literals;
void test_prefix_tree()
{
    prefix_tree::node<std::uint32_t>* root = new prefix_tree::node<std::uint32_t>();

    std::string s0  = "abc";   prefix_tree::insert(root, s0,  (std::uint32_t)100); 
    std::string s1  = "a";     prefix_tree::insert(root, s1,  (std::uint32_t)101); 
    std::string s2  = "abcd";  prefix_tree::insert(root, s2,  (std::uint32_t)102); 
    std::string s3  = "abf";   prefix_tree::insert(root, s3,  (std::uint32_t)103); 
    std::string s4  = "acd";   prefix_tree::insert(root, s4,  (std::uint32_t)104); 
    std::string s5  = "ace";   prefix_tree::insert(root, s5,  (std::uint32_t)105); 
    std::string s6  = "acef";  prefix_tree::insert(root, s6,  (std::uint32_t)106); 
    std::string s7  = "acg";   prefix_tree::insert(root, s7,  (std::uint32_t)107); 
    std::string s8  = "acgh";  prefix_tree::insert(root, s8,  (std::uint32_t)108); 
    std::string s9  = "abg";   prefix_tree::insert(root, s9,  (std::uint32_t)109); 
    std::string s10 = "abi";   prefix_tree::insert(root, s10, (std::uint32_t)110); 
    std::string s11 = "ab";    prefix_tree::insert(root, s11, (std::uint32_t)111); 
    std::string s12 = "be";    prefix_tree::insert(root, s12, (std::uint32_t)112); 
    

    std::optional<std::uint32_t> ans;
    ans = prefix_tree::search(root, s0);  std::cout << "\n" << *ans;
    ans = prefix_tree::search(root, s1);  std::cout << "\n" << *ans;
    ans = prefix_tree::search(root, s2);  std::cout << "\n" << *ans;
    ans = prefix_tree::search(root, s3);  std::cout << "\n" << *ans;
    ans = prefix_tree::search(root, s4);  std::cout << "\n" << *ans;
    ans = prefix_tree::search(root, s5);  std::cout << "\n" << *ans;
    ans = prefix_tree::search(root, s6);  std::cout << "\n" << *ans;
    ans = prefix_tree::search(root, s7);  std::cout << "\n" << *ans;
    ans = prefix_tree::search(root, s8);  std::cout << "\n" << *ans;
    ans = prefix_tree::search(root, s9);  std::cout << "\n" << *ans;
    ans = prefix_tree::search(root, s10); std::cout << "\n" << *ans;
    ans = prefix_tree::search(root, s11); std::cout << "\n" << *ans;
    ans = prefix_tree::search(root, s12); std::cout << "\n" << *ans;
}

void test_tree_variant()
{
//  test_heaptree();
    test_prefix_tree();
}
