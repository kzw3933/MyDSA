#ifndef __GG_ALGO_H
#define __GG_ALGO_H

#include "GG_config.h"
#include "GG_iterator.h"
#include "GG_algobase.h"

__GG_BEGIN_NAMESPACE


/*
**  adjacent_find: 查找相邻的重复元素
**
*/
template <class ForwardIterator>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last) {
    if (first == last ) 
        return last; 
    ForwardIterator next = first;
    while(++next != last ) {
        if(*first == *next)
            return first;
        first = next;
    }    
    return last;
}

template <class ForwardIterator, class BinaryPredicate>
ForwardIterator adjacent_find(ForwardIterator first, ForwardIterator last, BinaryPredicate binary_pred) {
    if (first == last ) 
        return last; 
    ForwardIterator next = first;
    while(++next != last ) {
        if(binary_pred(*first, *next))
            return first;
        first = next;
    }    
    return last;
}



/*
**  count: 计数
**
*/
template <class InputIterator, class T>
typename iterator_traits<InputIterator>::difference_type count(InputIterator first, InputIterator last, const T& val) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for(;first != last ; ++first) 
        if (*first == val)
            ++n;

    return n;
}

/*
**  count_if: 在特定情况下计数
**
*/

template <class InputIterator, class Predicate>
typename iterator_traits<InputIterator>::difference_type count_if(InputIterator first, InputIterator last, Predicate pred) {
    typename iterator_traits<InputIterator>::difference_type n = 0;
    for(;first != last ; ++first) 
        if ( pred(*first) )
            ++n;

    return n;
}

/*
**  find: 循序查找
**
*/

template <class InputIterator, class T>
InputIterator find(InputIterator first, InputIterator last, const T& val) {
    while(first != last && *first != val) {
        ++first;
    }
    return first;
}


/*
**  find_if: 循序查找复合特定条件者
**
*/

template <class InputIterator, class Predicate>
InputIterator find_if(InputIterator first, InputIterator last, Predicate pred) {
    while(first != last && !pred(*first)) {
        ++first;
    }
    return first;
}


/*
**  find_end: 查找某个子序列最后一次出现点
**
*/

//!TODO: search的完成
template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 __find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2, forward_iterator_tag, forward_iterator_tag) {
    if (first2 == last2) 
        return last1;
    else {
        ForwardIterator1 result = last1;
        while(1) {
            ForwardIterator1 new_result = search(first1, last1, first2, last2);
            if(new_result == last1)
                return result;
            else {
                result = new_result;
                first1 = new_result;
                ++first1;
            }
        }
    }
}


//!TODO: reverse_iterator的完成
template <class BidirectionalIterator1, class BidirectionalIterator2>
inline BidirectionalIterator1 __find_end(BidirectionalIterator1 first1, BidirectionalIterator1 last1, BidirectionalIterator2 first2, BidirectionalIterator2 last2, bidirectional_iterator_tag, bidirectional_iterator_tag) {
    typedef reverse_iterator<BidirectionalIterator1>  reviter1;
    typedef reverse_iterator<BidirectionalIterator2>  reviter2; 


    reviter1 rlast1(first1);
    reviter2 rlast2(first2);

    reviter1 rresult = search(reviter1(last1), rlast1, reviter2(last2), rlast2);

    if(rresult == rlast1)
        return last1;
    else {
        BidirectionalIterator1 result = rresult.base();
        advance(result, -distance(first2,last2));
        return result;
    }   
}

template <class ForwardIterator1, class ForwardIterator2>
inline ForwardIterator1 find_end(ForwardIterator1 first1, ForwardIterator1 last1, ForwardIterator2 first2, ForwardIterator2 last2) {
    typedef typename iterator_traits<ForwardIterator1>::iterator_category category1;
    typedef typename iterator_traits<ForwardIterator2>::iterator_category category2;

    return __find_end(first1, last1, first2, last2, category1(), category2());
}

/*
**   find_first_of: 查找某些元素的首次出现点
**
*/

template <class InputIterator, class ForwardIterator>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2) {
    for(; first1 != last1; ++first1) {
        for (ForwardIterator iter = first2; iter != last2; ++iter)
            if(*first1 == *iter)
                return first1;
    }
    return last1;
}


template <class InputIterator, class ForwardIterator, class BinaryPredicate>
InputIterator find_first_of(InputIterator first1, InputIterator last1, ForwardIterator first2, ForwardIterator last2, BinaryPredicate comp) {
    for(; first1 != last1; ++first1) {
        for (ForwardIterator iter = first2; iter != last2; ++iter)
            if(comp(*first1, *iter))
                return first1;
    }
    return last1;
}

 
/*
**   for_each: 对区间中的每个元素施行某操作
**
*/

template <class InputIterator, class Function>
Function for_each(InputIterator first, InputIterator last, Function f) {
    for(; first != last; ++first)
        f(*first);
    return f;
}


/*
**   generate: 以特定操作之运算结果填充特定区间内的元素
**
*/

template <class ForwardIterator, class Generator>
void generate(ForwardIterator first, ForwardIterator last, Generator gen) {
    for(; first != last; ++first)
        *first = gen();
    return f;
}


/*
**   generate_n: 以特定操作之运算结果填充n个元素内容
**
*/
template <class OutputIterator, class Size, class Generator>
OutputIterator generate_n(OutputIterator first, Size n, Generator gen) {
    for(; n >0 ; --n, ++first)
        *first = gen();
    return first;
}


/*
**   includes: 是否涵盖于某序列之中
**
*/

template <class InputIterator1, class InputIterator2>
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2) {
    while (first1 != last1 && first2 != last2) {
        if(*first2 < *first1)
            return false;
        else if(*first1 < *first2)
            ++first1;
        else 
            ++first1,++first2;
    }
    return first2 == last2;
}

template <class InputIterator1, class InputIterator2, class Compare>
bool includes(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, Compare comp) {
    while (first1 != last1 && first2 != last2) {
        if(comp(*first2 , *first1))
            return false;
        else if(comp(*first1, *first2))
            ++first1;
        else 
            ++first1,++first2;
    }
    return first2 == last2;
}

/*
**   max_element: 最大值所在位置
**
*/

template <class ForwardIterator>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last) {
    if (first == last)
        return first;
    ForwardIterator result = first;
    while(++first != last)
        if(* result < *first)
            result = first;
    return result;
}

template <class ForwardIterator, class Compare>
ForwardIterator max_element(ForwardIterator first, ForwardIterator last, Compare comp) {
    if (first == last)
        return first;
    ForwardIterator result = first;
    while(++first != last)
        if(comp(* result, *first))
            result = first;
    return result;
}

/*
**   merge: 合并两个有序序列
**
*/

template <class InputIterator1, class InputIterator2, class OutputIterator>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result) {
    while(first1 != last1 && first2 != last2) {
        if(*first2 < *first1){
            *result = *first2;
            ++first2;
        }
        else {
            *result = *first1;
            ++first1;
        }  
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

template <class InputIterator1, class InputIterator2, class OutputIterator, class Compare>
OutputIterator merge(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, InputIterator2 last2, OutputIterator result, Compare comp) {
    while(first1 != last1 && first2 != last2) {
        if(comp(*first2, *first1)){
            *result = *first2;
            ++first2;
        }
        else {
            *result = *first1;
            ++first1;
        }  
        ++result;
    }
    return copy(first2, last2, copy(first1, last1, result));
}

/*
**   min_element: 最小值所在位置
**
*/

template <class ForwardIterator>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last) {
    if (first == last)
        return first;
    ForwardIterator result = first;
    while(++first != last)
        if(*first < * result)
            result = first;
    return result;
}

template <class ForwardIterator, class Compare>
ForwardIterator min_element(ForwardIterator first, ForwardIterator last, Compare comp) {
    if (first == last)
        return first;
    ForwardIterator result = first;
    while(++first != last)
        if(comp(*first, * result))
            result = first;
    return result;
}


/*
**   partition: 分割
**
*/

template <class BidirectionalIterator, class Predicate>
BidirectionalIterator partition(BidirectionalIterator first, BidirectionalIterator last, Predicate pred) {
    while(true) {
        while(true){
            if (first == last)
                return first;
            else if(pred(*first))
                ++first;
            else 
                break;
        }
        --last;
        while(true) {
            if(first == last)
                return first;
            else if(!pred(*last))
                --last;
            else
                break;
        }
        iter_swap(first, last);
        ++first;
    }
}

/*
**   remove_copy: 移除某类元素并将结果复制到另一个容器
**
*/
template <class InputIterator, class OutputIterator, class T>
OutputIterator remove_copy(InputIterator first, InputIterator last, OutputIterator result, const T& val) {
    for(; first != last; ++first)
        if (*first != val) {
            *result = *first;
            ++result;
        }
        return result;
}


/*
**   remove: 移除某类元素但不删除   
**
*/

template <class ForwardIterator, class T>
ForwardIterator remove(ForwardIterator first, ForwardIterator last, const T& val) {
    first = find(first, last, val);
    ForwardIterator next = first;
    return first == last ? first : remove_copy(++next, last, first, val);
}

/*
**   remove_copy_if: 有条件移除某类元素并将结果复制到另一个容器
**
*/
template <class InputIterator, class OutputIterator, class Predicate>
OutputIterator remove_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred) {
    for(; first != last; ++first)
        if (!pred(*first)) {
            *result = *first;
            ++result;
        }
        return result;
}


/*
**   remove_if: 有条件移除某类元素   
**
*/

template <class ForwardIterator, class Predicate>
ForwardIterator remove_if(ForwardIterator first, ForwardIterator last, Predicate pred) {
    first = find_if(first, last, pred);
    ForwardIterator next = first;
    return first == last ? first : remove_copy_if(++next, last, first, pred);
}



/*
**   replace: 替换某类元素   
**
*/
template<class ForwardIterator, class T>
void replace(ForwardIterator first, ForwardIterator last, const T& old_val, const T& new_val) {
    for(; first != last; ++first)
        if (*first == old_val)
            *first = new_val;
}

/*
**   replace_copy: 替换某类元素,并将结果复制到另一个容器   
**
*/
template<class InputIterator,class OutputIterator, class T>
OutputIterator replace_copy(InputIterator first, InputIterator last,OutputIterator result, const T& old_val, const T& new_val) {
    for(; first != last; ++first,++result) {
        if (*first == old_val)
            *result = new_val;
        else  
            *result = *first;
    }
    return result;
}


/*
**   replace_if: 有条件替换某类元素   
**
*/
template<class ForwardIterator, class Predicate, class T>
void replace_if(ForwardIterator first, ForwardIterator last, Predicate pred, const T& new_val) {
    for(; first != last; ++first)
        if (pred(*first))
            *first = new_val;
}


/*
**   replace_copy_if: 有条件替换某类元素   
**
*/
template<class InputIterator, class OutputIterator, class Predicate, class T>
OutputIterator replace_copy_if(InputIterator first, InputIterator last, OutputIterator result, Predicate pred, const T& new_val) {
    for(; first != last; ++first,++result) {
        if (pred(*first))
            *result = new_val;
        else  
            *result = *first;
    }
    return result;
}

/*
**   reverse: 反转元素次序   
**
*/

template <class BidirectionalIterator>
inline void __reverse(BidirectionalIterator first, BidirectionalIterator last, bidirectional_iterator_tag) {
    while(true) {
        if (first == last || first == --last) 
            return 
        else 
            iter_swap(first++, last);
    }
}

template <class RandomAccessIterator>
inline void __reverse(RandomAccessIterator first, RandomAccessIterator last, random_access_iterator_tag) {
    while(first < last)                     // 只有random iterator才能做 first < last 比较
        iter_swap(first++, --last);
}


template <class BidirectionalIterator>
inline void reverse(BidirectionalIterator first, BidirectionalIterator last) {
    __reverse(first, last, iterator_category(first));
}

/*
**   reverse_copy: 反转元素次序,并将结果复制到另一个容器   
**
*/


template <class BidirectionalIterator, class OutputIterator>
OutputIterator reverse_copy(BidirectionalIterator first, BidirectionalIterator last, OutputIterator result) {
    while(first != last) {
        --last;
        *result = *last;
        ++result;
    }
    return result;
}


/*
** lower_bound: 将指定元素插入区间之内而不影响区间原本排序的最低位置
**
*/

template <class ForwardIterator, class T, class Distance>
ForwardIterator __lower_bound(ForwardIterator first, ForwardIterator last, const T& val, Distance*, forward_iterator_tag) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIterator middle;

    while(len >0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if(*middle < val) {
            first = middle;
            ++first;
            len = len - half -1;
        }
        else 
            len = half;
    }
    return first;
}

template <class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __lower_bound(RandomAccessIterator first, RandomAccessIterator last, const T& val, Distance*, random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;

    while(len >0 ) {
        half = len >> 1;
        middle = first + half;
        if(*middle < val) {
            first = middle +1;
            len = len -half - 1;
        }
        else 
            len = half;
    }
    return first;
}



template<class ForwardIterator, class T>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& val) {
    return __lower_bound(first, last, val, distance_type(first), iterator_category(first));
}

template<class ForwardIterator, class T, class Compare>
inline ForwardIterator lower_bound(ForwardIterator first, ForwardIterator last, const T& val, Compare comp) {
    return __lower_bound(first, last, val, comp, distance_type(first), iterator_category(first));
}


/*
** upper_bound: 将指定元素插入区间之内而不影响区间原本排序的最低位置
**
*/

template <class ForwardIterator, class T, class Distance>
ForwardIterator __upper_bound(ForwardIterator first, ForwardIterator last, const T& val, Distance*, forward_iterator_tag) {
    Distance len = 0;
    distance(first, last, len);
    Distance half;
    ForwardIterator middle;

    while(len >0) {
        half = len >> 1;
        middle = first;
        advance(middle, half);
        if( val < *middle) {
            len = half;
        }
        else {
            first = middle;
            ++first;
            len = len - half -1;
        }       
    }
    return first;
}

template <class RandomAccessIterator, class T, class Distance>
RandomAccessIterator __upper_bound(RandomAccessIterator first, RandomAccessIterator last, const T& val, Distance*, random_access_iterator_tag) {
    Distance len = last - first;
    Distance half;
    RandomAccessIterator middle;

    while(len >0 ) {
        half = len >> 1;
        middle = first + half;
        if( val < *middle) 
            len = half;
        else {
            first = middle +1;
            len = len -half - 1;
        }   
    }
    return first;
}



template<class ForwardIterator, class T>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& val) {
    return __upper_bound(first, last, val, distance_type(first), iterator_category(first));
}

template<class ForwardIterator, class T, class Compare>
inline ForwardIterator upper_bound(ForwardIterator first, ForwardIterator last, const T& val, Compare comp) {
    return __upper_bound(first, last, val, comp, distance_type(first), iterator_category(first));
}


/*
**  binary_search: 二分查找
**
*/

template <class ForwardIterator, class T>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& val) {
    ForwardIterator i = lower_bound(first, last, val);

    return i != last && !(val < *i);
}

template <class ForwardIterator, class T, class Compare>
bool binary_search(ForwardIterator first, ForwardIterator last, const T& val, Compare comp) {
    ForwardIterator i = lower_bound(first, last, val, comp);

    return i != last && !comp(val < *i);
}


__GG_END_NAMESPACE 

#endif