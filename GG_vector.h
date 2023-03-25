#ifndef __GG_VECTOR_H
#define __GG_VECTOR_H

#include <stddef.h>
#include "GG_config.h"
#include "GG_allocator.h"
#include "GG_alloc.h"
#include "GG_construct.h"
#include "GG_uninitialized.h"
#include "GG_algobase.h"

__GG_BEGIN_NAMESPACE


template <class T,class Alloc = alloc>
class vector { 
    public:
        typedef T               value_type;
        typedef T*              pointer;
        typedef const T*        const_pointer;
        typedef T*              iterator;
        typedef const T*        const_iterator;
        typedef reverse_iterator<T*> reverse_iterator;
        typedef reverse_iterator<const T*> const_reverse_iterator;
        typedef T&              reference;
        typedef const T&        const_reference;
        typedef size_t          size_type;
        typedef ptrdiff_t       difference_type;

    protected:
        typedef allocator<value_type, Alloc>  data_allocator;

    public:
        vector(): start(0),finish(0),end_of_storage(0) { }
        vector(size_type n, const T& val) { fill_initialize(n,val); }
        vector(size_type n) { fill_initialize(n, T()); }
        vector(const vector<T, Alloc>& x) {
            start = allocate_and_copy(x.begin(), x.end());
            finish = start + (x.end() - x.begin());
            end_of_storage = finish;
        }

        vector(const_iterator first, const_iterator last) {
            size_type n = 0;
            distance(first, last, n);
            start = allocate_and_copy(first, last);
            finish = start +n;
            end_of_storage = finish;
        }

        ~vector() { 
            destroy(start,finish);
            deallocate();
        } 
    
    public:
        iterator begin() {  return start;}
        const_iterator cbegin() const { return start; }
        iterator end() { return finish; }
        const_iterator cend() { return finish; }
        reverse_iterator rbegin() { return reverse_iterator(finish); }
        reverse_iterator rend() { return reverse_iterator(start); }
        size_type size() const {  return size_type(end()-begin());}
        size_type capacity() const {  return size_type(end_of_storage - begin()); }
        bool empty() const {  return begin() == end(); }

        reference front() { return *begin();}
        reference back() { return *(end()-1);}

        void push_back(const T& x){ 
            if (finish != end_of_storage){ 
                construct(finish, x);
                ++finish;
            }
            else { 
                insert_aux(end(), x);
            }
        }

        iterator insert(iterator pos, const T& x){
            size_type n = pos -begin();
            if( finish != end_of_storage && pos == end()) {
                construct(finish, x);
                ++finish;
            }
            else 
                insert_aux(pos, x);
            return begin() + n;
        }

        iterator insert(iterator positioin) { return insert(positioin, T()); }

        void insert(iterator pos, const_iterator first, const_iterator last);
        void insert(iterator pos, size_type n, const T& x);

        void pop_back() { 
            --finish;
            destroy(finish);
        }

        iterator erase(iterator pos) { 
            if (pos+1 != end()) { 
                copy(pos+1, finish, pos);
            }
            --finish;
            destroy(finish);
            return pos;
        }

        iterator erase(iterator first, iterator last) {
            iterator i = copy(last, finish, first);
            destroy(i,finish);
            finish = finish - (last - first);
            return first;
        }

        void resize(size_type new_sz, const T& x) { 
            if (new_sz < size()) 
                erase(begin()+new_sz, end());
            else 
                insert(end(), new_sz - size(), x);
        }

        void resize(size_type new_sz) { resize(new_sz, T()); }
        void clear() {  erase(begin(), end()); }
        void reserve(size_type n);
        void shrink_to_fit();

    public:
        reference operator[] (size_type n){ 
            return *(begin()+n);
        } 

        vector<T, Alloc>& operator=(const vector<T, Alloc>& x);

        bool operator==(const vector& v) const;
        bool operator!=(const vector& v) const;  

    public:
        template<class T, class Alloc>
        friend bool operator==(const vector<T,Alloc>& v1, const vector<T,Alloc>& v2);
        template<class T, class Alloc>
        friend bool operator!=(const vector<T,Alloc>& v1, const vector<T,Alloc>& v2);

    protected:

        iterator allocate_and_fill(size_type n, const T& x) { 
            iterator result = data_allocator::allocate(n);
            gadget::uninitialized_fill_n(result, n, x);
            return result;
        }

        iterator allocate_and_copy(const_iterator first, const_iterator last) { 
            start = data_allocator::allocate(last - first);
            finish = gadget::uninitialized_copy(first, last, start);
            end_of_storage = finish;
            return start;
        }

        void insert_aux(iterator pos, const T& x);

        void deallocate() { 
            if(start) { 
                data_allocator::deallocate(start, end_of_storage-start);
            }
        }

        void fill_initialize(size_type n, const T& val) { 
            start = allocate_and_fill(n, val);
            finish = start + n;
            end_of_storage = finish;
        }

    protected: 
        iterator    start;
        iterator    finish;
        iterator    end_of_storage;

            
};

template <class T, class Alloc>
inline bool operator==(const vector<T, Alloc>& x, const vector<T, Alloc>& y){
    return x.size() == y.size() && equal(x.begin(), x.end(), y.begin());
}

template <class T, class Alloc>
vector<T, Alloc>& vector<T, Alloc>::operator=(const vector<T, Alloc>& x) {
    if(&x != this) {
        if(x.size() > capacity()) {
            iterator tmp = allocate_and_copy(x.begin(), x.end());
            destroy(start, finish);
            deallocate();
            start = tmp;
            end_of_storage = start + (x.end() - x.begin());
        }
        else if (size() >= x.size()) {
            iterator i = copy(x.begin(), x.end(), begin());
            destroy(i, finish);
        }
        else {
            copy(x.begin(), x.begin()+size(), start);
            uninitialized_copy(x.begin()+size(), x.end(), finish);
        }
        finish = start +x.size();
    }
    return *this;
}


template <class T, class Alloc>
void vector<T, Alloc>::insert_aux(iterator pos, const T& x){
    if (finish != end_of_storage) {
        construct(finish,*(finish-1));
        ++finish;
        T x_copy = x;
        copy_backward(pos, finish -2, finish-1);
        *pos = x_copy;
    }
    
    else {
        const size_type old_sz = size();
        const size_type len = old_sz != 0 ? 2*old_sz : 1;
        iterator new_start = data_allocator::allocate(len);
        iterator new_finish = new_start;
        new_finish = uninitialized_copy(start, pos, new_start);
        construct(new_finish, x);
        ++new_finish;
        new_finish = uninitialized_copy(pos, finish, new_finish);

        destroy(begin(), end());
        deallocate();
        start = new_start;
        finish = new_finish;
        end_of_storage = new_start + len;
    }
}

template <class T, class Alloc>
void vector<T, Alloc>::insert(iterator pos, size_type n, const T& x) {
    if (0 != n) {
        if(size_type(end_of_storage - finish) >= n){
            T x_copy = x;
            const size_type elems_after = finish - pos;
            iterator old_finish = finish;
            if(elems_after > n) {
                uninitialized_copy(finish-n, finish, finish);
                finish += n;
                copy_backward(pos, old_finish-n,old_finish);
                fill(pos, pos+n,x_copy);
            }
            else {
                uninitialized_fill_n(finish, n-elems_after, x_copy);
                finish += n-elems_after;
                uninitialized_copy(pos, old_finish, finish);
                finish += elems_after;
                fill(pos,old_finish, x_copy);
            }
        }
        else {
            const size_type old_size = size();
            const size_type len = old_size + max(old_size, n);
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;

            new_finish = uninitialized_copy(start, pos, new_start);
            new_finish = uninitialized_fill_n(new_finish, n, x);
            new_finish = uninitialized_copy(pos, finish, new_finish);

            destroy(start, finish);
            deallocate();
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }
}


template <class T,class Alloc>
void vector<T, Alloc>::insert(iterator pos, const_iterator first, const_iterator last) {
    if(first != last) {
        size_type n = 0;
        distance(first, last, n);
        if(size_type(end_of_storage - finish) >= n) {
            const size_type elem_after = finish - pos;
            iterator old_finish = finish;
            if(elem_after > n) {
                uninitialized_copy(finish-n,finish, finish);
                finish += n;
                copy_backward(pos, old_finish-n, old_finish);
                copy(first, last, pos);
            }
            else {
                uninitialized_copy(first+elem_after, last,finish);
                finish += n-elem_after;
                uninitialized_copy(pos,old_finish,finish);
                finish += elem_after;
                copy(first, first+elem_after,pos);
            }
        }
        else {
            const size_type old_size = size();
            const size_type len = old_size + max(old_size, n);
            iterator new_start = data_allocator::allocate(len);
            iterator new_finish = new_start;

            new_finish = uninitialized_copy(start, pos, new_start);
            new_finish = uninitialized_copy(first, last, new_finish);
            new_finish = uninitialized_copy(pos, finish, new_finish);

            destroy(start, finish);
            deallocate();
            start = new_start;
            finish = new_finish;
            end_of_storage = new_start + len;
        }
    }
}

template <class T, class Alloc>
bool operator==(const vector<T, Alloc>& v1, const vector<T, Alloc>& v2) {
    return v1.operator==(v2);
}

template <class T, class Alloc>
bool operator!=(const vector<T, Alloc>& v1, const vector<T, Alloc>& v2) {
    return !(v1.operator==(v2));
}

template<class T, class Alloc>
void vector<T, Alloc>::reserve(size_type n) {
    if (n <= capacity())
        return ;
    T* new_start = data_allocator::allocate(n);
    T* new_finish = uninitialized_copy(begin(), end(), new_start);
    destroy(start, finish);
    deallocate();
    start = new_start;
    finish = new_finish;
    end_of_storage = start + n;
}

template<class T, class Alloc>
void vector<T, Alloc>::shrink_to_fit() {
    T* t = (T*)data_allocator::allocate(size());
    finish = uninitialized_copy(start, finish, t);
    deallocate(start, capacity());
    start = t;
    end_of_storage = finish;
}

__GG_END_NAMESPACE
#endif