#ifndef __GG_TREE_H
#define __GG_TREE_H

#include "GG_config.h"
#include "GG_iterator.h"
#include "GG_construct.h"
#include "GG_alloc.h"
#include "GG_allocator.h"
#include "GG_vector.h"
#include "GG_algo.h"
#include "GG_pair.h"


__GG_BEGIN_NAMESPACE
template <class Value>
struct __hashtable_node {
    __hashtable_node* next;
    Value val;
};

template <class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc=alloc>
class hashtable ;

template <class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
struct __hashtable_iterator {
    typedef __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc> iterator;
    typedef __hashtable_node<Value> node;
    typedef forward_iterator_tag iterator_category;
    typedef Value value_type;
    typedef ptrdiff_t   difference_type;
    typedef size_t size_type;
    typedef Value* pointer;
    typedef Value& reference;

    node* cur;
    hashtable* ht;

    __hashtable_iterator(node* n, hashtable* tab): cur(n), ht(tab) {}
    __hashtable_iterator() {}
    reference operator*() const { return cur->val; }
    pointer operator->() const { return &(operator*()); }
    iterator& operator++();
    iterator operator++(int);
    bool operator==(const iterator& iter) const { return cur == iter.cur; }
    bool operator!=(const iterator& iter) const { return cur != iter.cur; }

};

template <class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
__hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>& 
__hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::operator++() {
    const node* old = cur;
    cur = cur->next;
    if(! cur) {
        size_type bucket = ht->bkt_num(old->val);
        while(!cur && ++bucket < ht->buckets.size())
            cur = ht->buckets[bucket];
    }
    return *this;
}

template <class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
__hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>
__hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
}



static const int __num_primes = 28;
static const unsigned long __prime_list[__num_primes] = {
    53,             97,         193,        389,        769,
    1543,           3079,       6151,       12289,      24593,
    49157,          98317,      196613,     393241,     786433,
    1572869,        3145739,    6291469,    12582917,   25165843,
    50331653,       100663319,  201326611,  402653189,  805306457,
    1610612741,     3221225473ul, 4294967291ul
};

inline unsigned long __next_prime(unsigned long n) {
    const unsigned long * first = __prime_list;
    const unsigned long * last = __prime_list + __num_primes;
    const unsigned long * pos = lower_bound(first, last, n);

    return pos == last ? *(last-1) : *pos;
}


template <class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
class hashtable {
    public:
        typedef Key     key_type;
        typedef HashFunc hasher;
        typedef EqualKey key_equal;
        typedef size_t  size_type;
        typedef Value   value_type;
        typedef __hashtable_iterator<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc> iterator;

    private:
        typedef __hashtable_node<Value> node;
        typedef allocator<node, Alloc>  node_allocator;

    public:
        hashtable(size_type n, const HashFunc& hf, const EqualKey& eql): hash(hf), equals(eql), get_key(ExtractKey()), num_elements(0) {
            initialize_buckets(n);
        }

    public:
        size_type bucket_count() const { return buckets.size(); }

        size_type max_bucket_count() const {
            return __prime_list[__num_primes-1];
        }

        void clear();

        void copy_from(const hashtable& ht);

        void resize(size_type num_elements_hints);

        pair<iterator, bool> insert_unique(const value_type& obj) {
            resize(num_elements+1);
            return insert_unique_noresize(obj);
        }

        pair<iterator, bool> insert_unique_noresize(const value_type& obj);

        iterator insert_equal(const value_type& obj) {
            resize(num_elements+1);
            return insert_equal_noresize(obj);
        }

        iterator insert_equal_noresize(const value_type& obj) ;
    private:
        node* new_node(const value_type& obj) {
            node* n = node_allocator::allocate();
            n->next = 0;
            construct(&n->val, obj);
            return n;
        }

        void delete_node(node* n) {
            destroy(&n->val);
            node_allocator::deallocate(n);
        }

        void initialize_buckets(size_type n) {
            const size_type n_buckets = next_size(n);
            buckets.reserve(n_buckets);
            buckets.insert(buckets.end(), n_buckets, (node*)0);
            num_elements = 0;
        }

        size_type next_size(size_type n) const { return __next_prime(n); }

        size_type bkt_num(const value_type& obj, size_type n) const {
            return bkt_num_key(get_key(obj), n);
        }

        size_type bkt_num(const value_type& obj) const {
            return bkt_num_key(get_key(obj));
        }

        size_type bkt_num_key(const key_type& key) const {
            return bkt_num_key(key, buckets.size());
        }

        size_type bkt_num_key(const key_type& key, size_type n) const {
            return hash(key) % n;
        }

    private:
        hasher hash;
        key_equal equals;
        ExtractKey get_key;

        vector<node*, Alloc>buckets;
        size_type num_elements;


};


template <class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::clear() {
    for(size_type i=0; i < buckets.size(); ++i) {
        node* cur = buckets[i];
        while(cur != 0) {
            node* next = cur->next;
            delete_node(cur);
            cur = next;
        }
        buckets[i] = 0;
    }
    num_elements = 0;
}


//!TODO: has memory leak?
template <class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::copy_from(const hashtable& ht) {
    buckets.clear();
    buckets.reserve(ht.buckets.size());
    buckets.insert(buckets.end(), ht.buckets.size(), (node*)0);
    for(size_type i =0; i< ht.buckets.size(); i++) {
        if(const node* cur = ht.buckets[i]) {
            node* copy = new_node(cur->val);
            buckets[i] = copy;

            for(node* next = cur->next; next; cur=next, next = cur->next) {
                copy->next = new_node(next->val);
                copy = copy->next;
            }
        }
    }
    num_elements = ht.num_elements;
} 

template <class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
void hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::resize(size_type num_elements_hint) {
    const size_type old_n = buckets.size();
    if(num_elements_hint > old_n) {
        const size_type n = next_size(num_elements_hint);
        if (n > old_n) {
            vector<node*, Alloc> tmp(n, (node*)0);
            for(size_type bucket=0; bucket < old_n; ++bucket) {
                node* first = buckets[bucket];
                while(first) {
                    size_type new_bucket = bkt_num(first->val, n);
                    buckets[bucket] = first->next;
                    first->next = tmp[new_bucket];
                    tmp[new_bucket] = first;
                    first = buckets[bucket];
                }
            }
            buckets.swap(tmp);
        }
    }
}

template <class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
pair<typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator, bool> 
hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::insert_unique_noresize(const value_type& obj) {
    const size_type n = bkt_num(obj);
    node* first = buckets[n];

    for(node* cur=first; cur;cur = cur->next) {
        if(equals(get_key(cur->val), get_key(obj))) 
            return pair<iterator, bool>(iterator(cur,this), false);
    }

    node* tmp = new_node(obj);
    tmp->next = first;
    buckets[n] = tmp;
    ++num_elements;
    return pair<iterator, bool>(iterator(tmp, this), true);
}


template <class Value, class Key, class HashFunc, class ExtractKey, class EqualKey, class Alloc>
typename hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::iterator
hashtable<Value, Key, HashFunc, ExtractKey, EqualKey, Alloc>::insert_equal_noresize(const value_type& obj) {
    const size_type n = bkt_num(obj);
    node* first = buckets[n];
    for(node* cur=first; cur;cur=cur->next) {
        if(equals(get_key(cur->val), get_key(obj))) {
            node* tmp = new_node(obj);
            tmp->next = cur->next;
            cur->next = tmp;
            ++num_elements;
            return iterator(tmp, this);
        }
    }
    node* tmp = new_node(obj);
    tmp->next = first;
    buckets[n] = tmp;
    ++num_elements;
    return iterator(tmp, this);
}

__GG_END_NAMESPACE
#endif