#include<iostream>
#include<cstdint>
#include<stack>
#include<queue>


template<typename T> 
struct node
{
    node(const T& x) : value(x), lhs(nullptr), rhs(nullptr)
    {
    }

    node(const T& x, 
         node<T>* lhs_subtree, 
         node<T>* rhs_subtree) : value(x), lhs(lhs_subtree), rhs(rhs_subtree)
    {
    }

    T value;
    node<T>* lhs;
    node<T>* rhs;
};

  

// ********************* //
// *** Tree problems *** //
// ********************* //
template<typename ITER>
bool is_post_order(ITER begin, ITER end)
{
    if (begin == end) return true;

    ITER back = end-1;
    ITER mid  = back;
    for(ITER i=begin; i!=back; ++i)
    {
        if (*i >= *back) { mid = i; break; }
    }
    for(ITER i=mid; i!=back; ++i)
    {
        if (*i < *back) return false;
    }
    return is_post_order(begin, mid) && is_post_order(mid, back);
}

template<typename T>
bool is_in_between(node<T>* root, const T& lower, const T& upper) // assume root != nullptr
{
    if (root->value < lower) return false;
    if (root->value > upper) return false;
    if (root->lhs && !is_in_between(root->lhs, lower, root->value)) return false;
    if (root->rhs && !is_in_between(root->rhs, root->value, upper)) return false;
    return true;
}

template<typename T>
bool is_sorted(node<T>* root) // assume root != nullptr
{
    return is_in_between(root, std::numeric_limits<T>::min(), 
                               std::numeric_limits<T>::max());
}

template<typename ITER>
auto construct_tree_from_sorted_vector(ITER begin, ITER end) // work for vec size 1,2,3
{
    if (begin==end) return nullptr;

    auto mid = begin() + (end-begin)/2;
    return new node<typename std::iterator_traits<ITER>::value_type>
    {
        *mid, 
        construct_tree_from_sorted_vector(begin,mid), 
        construct_tree_from_sorted_vector(mid+1,end) 
    };
}

template<typename T>
std::pair<node<T>*, node<T>*> construct_list_from_tree(node<T>* root) // assume root != nullptr
{
    node<T>* head = root;
    node<T>* tail = root;

    if (root->lhs) 
    {
        auto ans = construct_list_from_tree(root->lhs);
        head = ans.first;
        root->lhs = ans.second;
        ans.second->rhs = root;
    }
    if (root->rhs) 
    {
        auto ans = construct_list_from_tree(root->rhs);
        tail = ans.second;
        root->rhs = ans.first;
        ans.first->lhs = root;
    }
    return std::make_pair(head, tail);
}

