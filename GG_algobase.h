#ifndef __GG_ALGOBASE_H
#define __GG_ALGOBASE_H

#include <string.h>
#include "GG_iterator.h"
#include "GG_config.h"
#include "GG_pair.h"
#include "GG_type_traits.h"

__GG_BEGIN_NAMESPACE


/*
** equal  判断两个区间相等与否
**
*/

template <class InputIterator1, class InputIterator2>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
    for(; first1 != last1; first1++, first2++) {
        if (*first1 != *first2) {
            return false;
        }
    }
    return true;
}


template <class InputIterator1, class InputIterator2, class BinaryPredicate>
inline bool equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
    for(; first1 != last1; first1++, first2++) {
        if (! binary_pred(*first1, *first2) {
            return false;
        }
    }
    return true;
}

/*
** fill  改填元素值
**
*/

template <class ForwardIterator, class T>
void fill(ForwardIterator first, ForwardIterator last, const T& val) {
    for(; first != last; ++first) 
        *first = val;
}


/*
** fill_n  改填元素值n次
**
*/
template <class OutputIterator, class Size, class T>
void fill(OutputIterator first, Size n, const T& val) {
    for(; n > 0; n--) 
        *first = val;
    return first;
}


/*
** iter_swap  元素互换
**
*/

template <class ForwardIterator1, class ForwardIterator2>
inline void iter_swap(ForwardIterator1 a, ForwardIterator2 b) {
    __iter_swap(a, b, value_type(a));
}

template <class ForwardIterator1, class ForwardIterator2, class T>
inline void __iter_swap(ForwardIterator1 a, ForwardIterator2 b, T*) {
    T tmp = *a;
    *a = *b;
    *b = tmp;
}

/*
** lexicographical_compare: 以字典序进行比较
**
*/
template <class InputIterator1, class InputIterator2>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2){
    for(; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (*first1 < *first2)
            return true;
        if(*first2 < *first1)
            return false;
    }
    return first1 == last1 && first2 != last2;
}

template <class InputIterator1, class InputIterator2, class Compare>
bool lexicographical_compare(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp){
    for(; first1 != last1 && first2 != last2; ++first1, ++first2) {
        if (comp(*first1, *first2))
            return true;
        if(comp(*first2, *first1))
            return false;
    }
    return first1 == last1 && first2 != last2;
}

/*
**  max 最大值
**
*/

template <class T>
inline const T& max(const T& a, const T& b) {
    return a <b ? b: a;
}

template <class T, class Compare>
inline const T& max(const T& a, const T& b) {
    return comp(a,b) ? b: a;
}


/*
**  min 最小值
**
*/

template <class T>
inline const T& min(const T& a, const T& b) {
    return b <a ? b: a;
}

template <class T, class Compare>
inline const T& min(const T& a, const T& b) {
    return comp(b,a) ? b: a;
}

/*
**  mismatch 找出不匹配点
**
*/

//!TODO: pair

template <class InputIterator1, class InputIterator2>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2) {
    while(first1 != last1 && *first1 == *first2){
        ++first1;
        ++first2;
    }
    return pair<InputIterator1, InputIterator2>(first1, first2);
}

template <class InputIterator1, class InputIterator2, class BinaryPredicate>
pair<InputIterator1, InputIterator2> mismatch(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, BinaryPredicate binary_pred) {
    while(first1 != last1 && binary_pred(*first1, *first2)){
        ++first1;
        ++first2;
    }
    return pair<InputIterator1, InputIterator2>(first1, first2);
}


/*
** swap: 交换
**
*/
template <class T>
inline void swap(T& a, T& b) {
    T tmp = a;
    a = b;
    b = tmp; 
}



/*
** copy: 复制
**
*/

template <class InputIterator, class OutputIterator>
inline OutputIterator __copy(InputIterator first, InputIterator last, OutputIterator result, input_iterator_tag) {
    for(; first != last; ++result,++first) 
        *result = *first;
    return result;
}

template <class RandomAccessIterator, class OutputIterator, class Distance>
inline OutputIterator __copy_d(RandomAccessIterator first, RandomAccessIterator last, OutputIterator result, Distance*) {
    for(Distance n=last - first; n>0; --n,++result,++first)
        *result = *first;
    return result;
}

template <class RandomIterator, class OutputIterator>
inline OutputIterator __copy(RandomIterator first, RandomIterator last, OutputIterator result, random_access_iterator_tag) {
    return __copy_d(first, last, result, distance_type(first));
}

template <class InputIterator, class OutputIterator>
struct __copy_dispatch {
    OutputIterator operator()(InputIterator first, InputIterator last, OutputIterator result) {
        return __copy(first, last, result, iterator_category(first));
    }
};

template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __true_type) {
    memmove(result, first, sizeof(T)*(last-first));
    return result + (last-first);
}

template <class T>
inline T* __copy_t(const T* first, const T* last, T* result, __false_type) {
    return __copy_d(first, last, result, (ptrdiff_t*)0);
}

template <class T>
struct __copy_dispatch<T*, T*>{
    T* operator()(T* first, T* last, T* result) {
        typedef typename __type_traits<T>::has_trival_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

template <class T>
struct __copy_dispatch<const T*, const T*>{
    T* operator()(const T* first, const T* last, T* result) {
        typedef typename __type_traits<T>::has_trival_assignment_operator t;
        return __copy_t(first, last, result, t());
    }
};

template <class InputIterator, class OutputIterator>
inline OutputIterator copy(InputIterator first, InputIterator last, OutputIterator result) {
    return __copy_dispatch<InputIterator,OutputIterator>()(first, last, result);
}

inline char* copy(const char* first, const char* last, char* result) {
    memmove(result, first, last - first);
    return result + (last-first);
}

inline wchar_t* copy(const wchar_t* first, const wchar_t* last, wchar_t* result) {
    memmove(result, first, sizeof(wchar_t) * (last - first));
    return result + (last-first);
}

/*
** copy_backward: 逆向复制
**
*/

template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 __copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
    while (first != last) 
        *--result = *--last;
    return result;
}


template <class BidirectionalIterator1, class BidirectionalIterator2>
struct __copy_backward_dispatch {
    BidirectionalIterator2 operator()(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
        return __copy_backward(first, last, result);
    }
};

template <class T>
inline T* __copy_backward_t(const T* first, const T* last, T* result,__true_type) {
    const ptrdiff_t N = last - first;
    memmove(result - N, first,sizeof(T) * N);
    return result - N;
} 

template <class T>
inline T* __copy_backward_t(const T* first, const T* last, T* result,__false_type) {
    __copy_backward(first, last, result);
} 


template <class T>
struct __copy_backward_dispatch<T*, T*> {
    T* operator()(T* first, T* last, T* result) {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;  
        return __copy_backward_t(first, last, result, t());
    }
};

template <class T>
struct __copy_backward_dispatch<const T*, const T*> {
    T* operator()(const T* first,const T* last, T* result) {
        typedef typename __type_traits<T>::has_trivial_assignment_operator t;  
        return __copy_backward_t(first, last, result, t());
    }
};

template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator2 copy_backward(BidirectionalIterator1 first, BidirectionalIterator1 last, BidirectionalIterator2 result) {
    return __copy_backward_dispatch<BidirectionalIterator1,BidirectionalIterator2>()(first, last, result);
}


__GG_END_NAMESPACE
#endif