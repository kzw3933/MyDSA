#ifndef __GG_VECTOR_H
#define __GG_VECTOR_H

#include <stddef.h>
#include "GG_config.h"
#include "GG_iterator.h"
#include "GG_allocator.h"
#include "GG_alloc.h"
#include "GG_construct.h"

__GG_BEGIN_NAMESPACE

template <class T>
struct __list_node {
    typedef void* void_pointer;
    void_pointer    prev;
    void_pointer    next;
    T    data;
};

template<class T, class Ref, class Ptr>
struct __list_iterator {
    typedef __list_iterator<T, T&, T*> iterator;
    typedef __list_iterator<T, Ref, Ptr>    self;

    typedef bidirectional_iterator_tag iterator_category;
    typedef T       value_type;
    typedef Ptr     pointer;
    typedef Ref     reference;
    typedef __list_node<T>* link_type;
    typedef size_t  size_type;
    typedef ptrdiff_t   difference_type;

    link_type   node;

    __list_iterator(link_type x): node(x) {}
    __list_iterator()   {}
    __list_iterator(const iterator& iter): node(iter.node) {}

    bool operator==(const self& x) const { return node == x.node; }
    bool operator!=(const self& x) const { return node != x.node; }
    reference operator*() const {return (*node).data; }
    pointer operator->() const { return &(operator*()); }

    self& operator++() {
        node = (link_type) ((*node).next);
        return *this;
    }

    self& operator++(int) {
        self tmp = *this;
        ++ *this;
        return tmp;
    }

    self& operator--() {
        node = (link_type) ((*node).prev);
        return *this;
    }

    self& operator--(int) {
        self tmp = *this;
        -- *this;
        return tmp;
    }
};

template <class T, class Ref, class Ptr>
inline bidirectional_iterator_tag iterator_category(const __list_iterator<T, Ref, Ptr>&) {
    return bidirectional_iterator_tag();
}

template <class T, class Ref, class Ptr>
inline T* value_type(const __list_iterator<T, Ref, Ptr>&) {
    return 0;
}

template <class T, class Ref, class Ptr>
inline ptrdiff_t* distance_type(const __list_iterator<T, Ref, Ptr>&) {
    return 0;
}


template <class T, class Alloc=alloc>
class list {
    
    public:
        typedef T               value_type;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef const T         reference;
        typedef const T&        const_reference;
        typedef list_node*      link_type;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;

        typedef __list_iterator<T, T&, T*> iterator;
        typedef __list_iterator<T, const T&, const T*> const_iterator;

    protected:
        typedef void*   void_pointer;
        typedef __list_node<T> list_node;
        typedef allocator<list_node, Alloc> list_node_allocator;

    public:
        list() { empty_initialize(); }

    public:
        iterator begin() { return (link_type)((*node).next);}
        iterator end() { return node; }
        bool empty() const { return node->next == node; }
        size_type size() const {
            size_type result = 0;
            distance(begin(), end(), result);
            return result;
        }

        reference front() { return *begin(); }
        reference back() { return *(--end()); } 


        iterator insert(iterator pos, const T& val) {
            link_type tmp = create_node(val);
            tmp->next = pos.node;
            tmp->prev = pos.node->prev;
            (link_type(pos.node->prev))->next = tmp;
            pos.node->prev = tmp;
            return tmp;
        }

        void push_front(const T& x) { insert(begin(), x); }
        void push_back(const T& x) { insert(end(), x); }

        iterator erase(iterator pos) {
            link_type next_node = link_type(pos.node->next);
            link_type prev_node = link_type(pos.node->prev);
            prev_node->next = next_node;
            next_node->prev = prev_node;

            destroy_node(pos.node);
            return iterator(next_node);
        }

        // 移除头结点
        void pop_front() { erase(begin()); }

        // 移除尾结点
        void pop_back() {
            iterator tmp = end();
            erase(--tmp);
        }
        void remove(const T& val);
        void clear();
        void unique();

        // 将 x 接合于 pos 所指位置之前,x必须不同于 *this
        void splice(iterator pos, list& x) {
            if (!x.empty())
                transfer(pos, x.begin(), x.end());
        }

        // 将 i 所指元素接合于pos所指位置之前 pos 和 i 可以指向同一个list
        void splice(iterator pos, list&, iterator i) {
            iterator j = i;
            ++j;
            if (pos == i || pos == j)
                return ;
            transfer(pos,i,j);
        }

        // 将 [first, last) 内所有的元素接合于pos所指位置之前, pos与 [first, last) 可指向同一个list,但pos不能在[first, last)中
        void splice(iterator pos, list&, iterator first, iterator last) {
            if (first != last)
                transfer(pos, first, last);
        }

        void merge(list<T, Alloc>& x);
        void reverse();    

    protected:
        link_type get_node() { return list_node_allocator::allocate(); }
        void put_node(link_type node) { list_node_allocator::deallocate(node); }

        link_type create_node(const T& val) {
            link_type p = get_node();
            construct(&(p->data), val);
            return p;
        }

        void destroy_node(link_type p) {
            destroy(&(p->data));
            put_node(p);
        }

        void empty_initialize() {
            node = get_node();
            node->next = node;
            node->prev = node;
        }

        // 将[first, last) 内的所有元素移动到pos之前
        void transfer(iterator pos, iterator first, iterator last) {
            if (pos != last) {
                (last.node->prev)->next = pos.node;
                (first.node->prev)->next = last.node;
                (pos.node->prev)->next = first.node;
                link_type tmp = pos.node->prev;
                pos.node->prev= last.node->prev;
                last.node->prev = first.node->prev;
                first.node->prev = tmp; 
            }
        }


    protected:
        link_type   node;
};


// 清除所有结点
template<class T,class Alloc>
void list<T, Alloc>::clear() {
    link_type cur = (link_type) node->next;
    while(cur != node) {
        link_type tmp = cur;
        cur = (link_type) cur->next;
        destroy_node(tmp);
    }

    node->next = node;
    node->prev = node;
}

// 将值为val的所有元素移除
template<class T,class Alloc>
void list<T, Alloc>::remove(const T& val) {
    iterator first = bein();
    iterator last = end();
    while (first != last) {
        iterator next = first;
        ++next;
        if(*first == val) erase(first);
        first = next;
    }
}

// 移除数值相同的连续元素至只有一个
template<class T,class Alloc>
void list<T, Alloc>::unique() {
    iterator first = begin();
    iterator last = end();
    if (first == last ) return ;
    iterator next = first;
    while(++next != last) {
        if (*first == *next)
            erase(next);
        else 
            first = next;
        next = first;
    }
}


template<class T,class Alloc>
void list<T,Alloc>::merge(list<T, Alloc>& x) {
    iterator first1 = begin();
    iterator last1 = end();
    iterator first2 = x.begin();
    iterator last2 = x.end();

    // 前提要满足两个list已经递增排序
    while(first1 != last1 && first2 != last2) {
        if (*first2 < *first1) {
            iterator next = first2;
            transfer(first1, first2, ++next);
            first2 = next;
        }
        else 
            ++first1;
        if (first2 != last2)
            transfer(last1, first2, last2);
    }
}


template <class T, class Alloc>
void list<T, Alloc>::reverse() {
    if (node->next == node || (node->next)->next == node)
        return ;
    iterator first = begin();
    ++first;
    while(first != end()) {
        iterator old = first;
        ++first;
        transfer(begin(), old, first);
    }
}

__GG_END_NAMESPACE
#endif