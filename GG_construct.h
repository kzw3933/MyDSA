#ifndef __GG_CONSTRUCT_H
#define __GG_CONSTRUCT_H



#include <new.h>                                                                    // for placement new
#include "GG_config.h"                                                              
#include "GG_type_traits.h"
#include "GG_iterator.h"

__GG_BEGIN_NAMESPACE

/*
**  construct  
**
*/

template <class T1,class T2>
inline void construct(T1* ptr, const T2& val){
    new (ptr) T1(val);                                                              // placement new
}

/*
**  destroy 
**
*/

// destroy第一个版本: 接受一个指针
template < class T>
inline void destroy(T* ptr) {
    ptr->~T();
}

// 如果元素型别(value type) 有trivial destructor
template <class ForwardIterator>
inline void
__destroy_aux(ForwardIterator first, ForwardIterator last, __true_type){}

// 如果元素型别(value type) 有non-trivial destructor
template <class ForwardIterator>
inline void
__destroy_aux(ForwardIterator first, ForwardIterator last, __false_type){
    for (; first < last; ++first)
        destroy(&*first);
}

// 判断元素的数值类型(value type)是否有trivial destructor
template <class ForwardIterator, class T>
inline void __destroy(ForwardIterator first, ForwardIterator last,T*){
    typedef typename __type_traits<T>::has_trivial_destructor trivial_destructor;
    __destroy_aux(first, last, trivial_destructor());
}

// destroy第二个版本: 接受两个迭代器
template <class ForwardIterator>
inline void destroy(ForwardIterator first, ForwardIterator last) {
    __destroy(first,last, value_type(first));
}

// destroy第二个版本对迭代器为char* 和wchar_t*的特化版本
inline void destroy(char*, char*) {}
inline void destroy(wchar_t*, wchar_t*) {}


//!TODO:   针对其他指针类型的destroy第二版本的特化版本? 

__GG_END_NAMESPACE

#endif