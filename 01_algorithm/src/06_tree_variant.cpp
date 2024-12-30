#include<iostream>
#include<cstdint>
#include<vector>
#include<string>
#include<map>
#include<optional>


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

