#include<iostream>
// **************************************** //
// List functions
// 1. insert head
// 2. insert before
// 3. insert after
// 4. erase this
// 5. erase next
// 6. reverse
//
// Repeat 2/3/4/6 for doubly-linked-list
// **************************************** //

template<typename T>
struct node
{
    template<typename...ARGS>
    node(ARGS&&...args) : value(std::forward<ARGS>(args)...), next(nullptr), prev(nullptr)
    {
    }

    T value;
    node<T>* next;
    node<T>* prev;
};

template<typename T>
class singly_list
{
public:
    singly_list() : head(nullptr)
    {
    }

    template<typename...ARGS>
    void insert_front(ARGS&&...args) // <--- we have thread-safe lockfree version for this one
    {
        node<T>* new_node = new node<T>{std::forward<ARGS>(args)...};
        new_node->next = head;
        head = new_node;
    }

    template<typename...ARGS>
    void insert_after(node<T>* this_node, ARGS&&...args) 
    {
        node<T>* new_node = new node<T>{std::forward<ARGS>(args)...};
        new_node->next = this_node->next;
        this_node->next = new_node;
    }

    template<typename...ARGS>
    void insert_before(node<T>* this_node, ARGS&&...args)
    {
        // *** Special case *** //
        if (this_node == head)
        {
            insert_front(std::forward<ARGS>(args)...);
        }
        // *** Normal case *** //
        else
        {   
            node<T>* prev_node = head;
            while(prev_node->next != this_node)
            {
                prev_node = prev_node->next;
            }
            insert_after(prev_node, std::forward<ARGS>(args)...);
        }
    }

    void erase_front() 
    {
        node<T>* del_node = head;
        head = del_node->next;
        delete del_node;
    }

    void erase_next(node<T>* this_node) 
    {
        node<T>* del_node = this_node->next;
        this_node->next = del_node->next;
        delete del_node;
    }

    void erase_this(node<T>* this_node)
    {
        // *** Special case *** //
        if (this_node == head)
        {
            erase_front();
        }
        // *** Normal case *** //
        else
        {   
            node<T>* prev_node = head;
            while(prev_node->next != this_node)
            {
                prev_node = prev_node->next;
            }
            erase_next(prev_node);
        }
    }

    void reverse()
    {
        if (!head) return;

        node<T>* prev_node = head;
        node<T>* this_node = head->next;
        while(this_node)
        {
            node<T>* next_node = this_node->next;
            this_node->next = prev_node;
            prev_node = this_node;
            this_node = next_node;
        }
        head = prev_node;
    }

// private:
    node<T>* head;
};

template<typename T>
class doubly_list
{
public:
    doubly_list() : head(nullptr), tail(nullptr)
    {
    }

    template<typename...ARGS>
    void insert_before(node<T>* this_node, ARGS&&...args) // it works for this_node == head
    {
        node<T>* new_node = new node<T>{std::forward<ARGS>(args)...};
        node<T>* prev_node = this_node->prev_node;
        new_node->next = this_node;
        new_node->prev = prev_node;
        this_node->prev = new_node;

        if (prev_node) prev_node->next = new_node;
        else head = new_node;
    }

    template<typename...ARGS>
    void insert_after(node<T>* this_node, ARGS&&...args) // it works for this_node == tail
    {
        node<T>* new_node = new node<T>{std::forward<ARGS>(args)...};
        node<T>* next_node = this_node->next;
        new_node->next = next_node;
        new_node->prev = this_node;
        this_node->next = new_node;

        if (next_node) next_node->prev = new_node; 
        else tail = new_node;
    }

    void erase_this(node<T>* this_node)
    {
        node<T>* prev_node = this_node->prev;
        node<T>* next_node = this_node->next;

        if (prev_node) prev_node->next = next_node;
        else head = next_node;
        if (next_node) next_node->prev = prev_node;
        else tail = prev_node;
        delete this_node;
    }

    void reverse()
    {
        node<T>* this_node = head;
        while(this_node)
        {
            node<T>* next_node = this_node->next;
            std::swap(this_node->prev, this_node->next);
            next_node = next_node;
        }
        std::swap(head, tail);
    }

// private:
    node<T>* head;
    node<T>* tail;
};

template<typename T>
node<T>* last_n_node(singly_list<T>& list, std::uint32_t N)
{
    node<T>* this_node = list.head;
    node<T>* next_node = this_node->next;
    for(std::uint32_t n=0; n!=N; ++n)
    {
        next_node = next_node->next;
    }
    while(next_node)
    {
        this_node = this_node->next;
        next_node = next_node->next;
    }
    return this_node;
}

/*

0 1 2 3 * * * m m+1 * * *
              n         *
              *         *
              *         *
              * * * * k *

slow pointer travelled distance = k
fast pointer travelled distance = (n+1) + (k-m)
hence we have : 2k = (n+1) + (k-m)
                 m =  n+1-k
hence pointer starting from 0 
and   pointer starting from k 
will meet at m if they travel at same speed

*/ 
