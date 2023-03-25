#ifndef __GG_UNINITIALIZED_H
#define __GG_UNINITIALIZED_H

#include "GG_config.h"
#include "GG_type_traits.h"
#include "GG_iterator.h"
#include "GG_construct.h"
#include "GG_algobase.h"

__GG_BEGIN_NAMESPACE


/*
**  uninitialized_copy
**
*/

template<class InputIterator, class ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __true_type) {
    memcpy(result, first, (last-first)*sizeof(*first));
    return result + (last-first);
}

template<class InputIterator, class ForwardIterator>
ForwardIterator __uninitialized_copy_aux(InputIterator first, InputIterator last, ForwardIterator result, __false_type) {
    int i = 0;
    for(; first != last;++first,++i){
        construct((result+i),*first);
    }

    return (result+i);
}

template<class InputIterator, class ForwardIterator>
ForwardIterator uninitialized_copy(InputIterator first, InputIterator last, ForwardIterator result) {
    typedef typename  __type_traits<typename iterator_traits<InputIterator>::value_type>::is_POD_type  isPODType;
    return __uninitialized_copy_aux(first, last, result, isPODType());
}


/*
**  uninitialized_fill
**
*/

template<class ForwardIterator, class T>
void uninitialized_fill(ForwardIterator first, ForwardIterator last, const T& val) {
    typedef typename  __type_traits<T>::is_POD_type  isPODType;
    __uninitialized_fill_aux(first, last, val, isPODType());
}

template<class ForwardIterator, class T>
void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& val, __true_type) {
   fill(first,last,val);
}

template<class ForwardIterator, class T>
void __uninitialized_fill_aux(ForwardIterator first, ForwardIterator last, const T& val, __false_type) {
    for(; first != last;++first){
        construct(first,val);
    }
}

/*
**  uninitialized_fill_n
**
*/

template<class ForwardIterator, class Size, class T>
ForwardIterator uninitialized_fill_n(ForwardIterator first, Size n, const T& x) {
    typedef typename  __type_traits<T>::is_POD_type  isPODType;
    return __uninitialized_fill_n_aux(first, n, x, isPODType());
}

template<class ForwardIterator, class Size, class T>
ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __true_type) {
    return fill_n(first,n,x);
}

template<class ForwardIterator, class Size, class T>
ForwardIterator __uninitialized_fill_n_aux(ForwardIterator first, Size n, const T& x, __false_type) {
    int i = 0;
    ForwardIterator result = first;
    for(; i != n; ++i,++result){
        construct(&*result, x);
    }
    return result;
}

__GG_END_NAMESPACE
#endif