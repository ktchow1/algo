#include<iostream>
#include<stack>
#include<queue>

 
// 1. Tree definitions and properties
// -  what is a tree
// -  what is a binary tree
// -  what is a binary complete tree
// -  what is a binary complete sorted tree
// 2. Tree functions 
// -  depth
// -  search 
// -  insert 
// -  traverse : BFS, DFS_pre_order, DFS_in_order, DFS_post_order
// -  rotate
// -  balance
// 3. Check a vector follows post-order
// 4. Check a tree is sorted
// 5. Build a tree from sorted vector
// 6. Build a doubly list from sorted tree 




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

template<typename T>
std::uint32_t depth(const node<T>* root)
{
    if (root) 
    {
        return 1 + std::max(depth(root->lhs, root->rhs));
    }
    else return 0;
}

template<typename T>
node<T>* search(const node<T>* root, const T& target)
{
    if (root) 
    {
        if (target < root->value) return search(root->lhs, target);
        if (target > root->value) return search(root->rhs, target);
        return root;
    }
    else return nullptr;
}

template<typename T>
node<T>* insert(const node<T>* root, const T& x)
{
    if (root)
    {
        if (x < root->value) return insert(root->lhs, x);
        else                 return insert(root->rhs, x);
    }
    else
    {
        root = new node<T>(x);
        return root;
    }
}
  
// ***************** //
// *** Iterative *** //
// ***************** //
template<typename T, typename F> requires std::invocable<F, T> 
void BFS_iterative(node<T>* root, const F& fct) 
{
    if (!root) return;

    std::queue<node<T>*> q; 
    q.push(root);
    while(!q.empty())
    {
        node<T>* this_node = q.front();
        fct(this_node->value);
        q.pop();
        q.push(this_node->lhs); // we can check nullity here ... (optional)
        q.push(this_node->rhs); // we don't check nullity simply for consistent code pattern
    }
}

template<typename T, typename F> requires std::invocable<F, T> 
void DFS_pre_order_iterative(node<T>* root, const F& fct) 
{
    if (!root) return;

    std::stack<node<T>*> s;
    s.push(root);
    while(!s.empty())
    {
        node<T>* this_node = s.top();
        fct(this_node->value);
        s.pop();                // line 1
        s.push(this_node->lhs); // line 2
        s.push(this_node->rhs); // line 3 
    //  No matter how we order these 3 lines, fct is invoked on this_node first, hence pre_order.
    }
}

// In order to implement in order :
// 1. unlike DFS_pre_order, 
//    in which, stack stores all visited-but-not-processed nodes, regardless of node value
//    in here, stack stores only nodes along the path starting from root to this_node, where :
// -  all nodes on the LHS of this path are processed    (having values less than this_node->value)
// -  all nodes on the RHS of this path are not in stack (having values more than this_node->value)
// -  evolve this path so that we are looking for the minimum unprocessed this_node
//
// 2. Whenever this_node is NOT minimum unprocessed node :
// -  there must be an offspring with smaller value (i.e. lhs subtree)
// -  push this_node into stack 
// -  move this_node to lhs
//
// 3. Whenever this_node is the minimum unprocessed node :
// -  process this_node
// -  next (slightly larger) node should be either :
//    * its rhs subtree (if it exists) OR
//    * its parent in the stack
//
// 4. Stack node cannot be nullptr, must ensure that before push
//    this_node map be nullptr, update this_node without nullity checking 
//
template<typename T, typename F> requires std::invocable<F, T> 
void DFS_in_order_iterative(node<T>* root, const F& fct) 
{
    std::stack<node<T>*> s;
    node<T>* this_node = root;
    while(!s.empty() || this_node)
    {
        // when this_node is not minimum unprocessed node (i.e. can go further lhs)
        if (this_node)
        {
            s.push(this_node);
            this_node = this_node->lhs;
        }
        // when this_node is the minimum unprocessed node (i.e. cannot go further lhs)
        else
        {
            this_node = s.top(); s.pop();
            fct(this_node->value);
            this_node = this_node->rhs;
        }
    }
}

// My implementation assumes no nullptr in both stack and this_node.
template<typename T, typename F> requires std::invocable<F, T> 
void DFS_in_order_iterative2(node<T>* root, const F& fct) 
{
    if (!root) return;
    node<T>* this_node = root;
    bool this_node_is_min = false;

    std::stack<node<T>*> s;
    s.push(this_node);  
    while(!s.empty())
    {
        // step 1 : goto the most LHS unprocessed node
        if (!this_node_is_min)
        {
            while(this_node->lhs)
            {
                s.push(this_node);
                this_node = this_node->lhs;
            }
            this_node_is_min = true;
        }

        // step 2 : invoke on this_node and goto next node
        fct(this_node->value);
        if (this_node->rhs)
        {
            this_node = this_node->rhs;
            this_node_is_min = false;
        }
        else
        {
            this_node = s.top();
            s.pop();
            // all stack nodes have their lhs subtree processed, dont reset this_node_is_min
        }
    }
}

// ***************** //
// *** Recursive *** //
// ***************** //
template<typename T, typename F> requires std::invocable<F, T> 
void DFS_pre_order_recursive(node<T>* root, const F& fct)
{
    if (!root) return;
    fct(root->value);
    DFS_pre_order_recursive(root->lhs, fct);
    DFS_pre_order_recursive(root->rhs, fct);
}

template<typename T, typename F> requires std::invocable<F, T> 
void DFS_in_order_recursive(node<T>* root, const F& fct)
{
    if (!root) return;
    DFS_in_order_recursive(root->lhs, fct);
    fct(root->value);
    DFS_in_order_recursive(root->rhs, fct);
}

template<typename T, typename F> requires std::invocable<F, T> 
void DFS_post_order_recursive(node<T>* root, const F& fct)
{
    if (!root) return;
    DFS_post_order_recursive(root->lhs, fct);
    DFS_post_order_recursive(root->rhs, fct);
    fct(root->value);
}

//      Y                        X
//     / \      rotate lhs      / \
//    X   C     <---------     A   Y
//   / \        --------->        / \
//  A   B       rotate rhs       B   C
//

template<typename T> 
node<T>* rotate_lhs(node<T>* root)
{
    node<T>* new_root = root->rhs;
    if (!new_root) return root; // cannot perform rotate_lhs, invalid tree

    root->rhs = new_root->lhs;
    new_root->lhs = root;
    return new_root;
}

template<typename T> 
node<T>* rotate_rhs(node<T>* root)
{
    node<T>* new_root = root->lhs;
    if (!new_root) return root; // cannot perform rotate_rhs, invalid tree

    root->lhs = new_root->rhs;
    new_root->rhs = root;
    return new_root;
}

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

void test_DFS_in_order_iterative()
{
    using N = node<std::uint32_t>;
    auto rn = []()->std::uint32_t { return rand()%500; };

    N* root = 
    new N { rn(),
        new N { rn(), 
            new N { rn(),
                new N { rn() }, 
                new N { rn() }
            },
            new N { rn(),
                new N { rn() },
                new N { rn(),
                    new N { rn() },
                    new N { rn(),
                        new N { rn() },
                        new N { rn() }
                    }
                }
            }
        },
        new N { rn(),
            new N { rn(), 
                new N { rn(),
                    new N { rn(),
                        new N { rn() },
                        new N { rn() }
                    },
                    new N { rn() }
                },
                new N { rn(),
                    new N { rn() },
                    new N { rn() }
                }
            },
            new N { rn() }
        }
    };


    std::cout << "\nDFS in order recursive                   : ";
    DFS_in_order_recursive(root, [](const std::uint32_t& x) { std::cout << x << " "; });
    std::cout << "\nDFS in order iterative (wiki approach)   : ";
    DFS_in_order_iterative(root, [](const std::uint32_t& x) { std::cout << x << " "; });
    std::cout << "\nDFS in order iterative (my own approach) : ";
    DFS_in_order_iterative2(root, [](const std::uint32_t& x) { std::cout << x << " "; });
}

void test_tree()
{
    test_DFS_in_order_iterative();
}
