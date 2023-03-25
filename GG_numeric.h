#ifndef __GG_NUMERIC_H
#define __GG_NUMERIC_H

#include "GG_config.h"
#include "GG_iterator.h"
#include "GG_functional.h"

__GG_BEGIN_NAMESPACE


/*
**  accumulate: 元素累计
**
*/

template <class InputIterator, class T>
T accumulate(InputIterator first, InputIterator last, T init) {
    for (; first != last; ++first)
        init = init + *first;
    return init;
}

template <class InputIterator, class T, class BinaryOperation>
T accumulate(InputIterator first, InputIterator last, T init, BinaryOperation binary_op) {
    for (; first != last; ++first)
        init = binary_op(init, *first);             // 对每个元素执行二元操作
    return init;
}


/*
**  adjacent_difference: 相邻元素的差额
**
*/

template <class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
    if (first == last)
        return result;
    *result = *first;
    iterator_traits<InputIterator>::value_type val = *first;
    while(++first != last) {
        T tmp = *first;
        *++result = tmp -val;
        val = tmp;
    }
    return ++result;
}


template <class InputIterator, class OutputIterator>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result) {
    if (first == last)
        return result;
    *result = *first;
    iterator_traits<InputIterator>::value_type val = *first;
    while(++first != last) {
        iterator_traits<InputIterator>::value_type tmp = *first;
        *++result = tmp -val;
        val = tmp;
    }
    return ++result;
}

template <class InputIterator, class OutputIterator, class BinaryOperation>
OutputIterator adjacent_difference(InputIterator first, InputIterator last, OutputIterator result, BinaryOperation binary_op) {
    if (first == last)
        return result;
    *result = *first;
    iterator_traits<InputIterator>::value_type val = *first;
    while(++first != last) {
        iterator_traits<InputIterator>::value_type tmp = *first;
        *++result = binary_op(tmp, val);
        val = tmp;
    }
    return ++result;
}


/*
**  inner_product 内积
**
*/

template <class InputIterator1, class InputIterator2, class T>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init) {
    for(; first1 != last1; ++first1,++first2) {
        init +=  (*first1) * (*first2);
    }

    return init;
}



template <class InputIterator1, class InputIterator2, class T, class BinaryOperation1, class BinaryOperation2>
T inner_product(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2, T init, BinaryOperation1 binary_op1, BinaryOperation2 binary_op2) {
    for(; first1 != last1; ++first1,++first2) {
        init = binary_op1(init, binary_op2(*first1, *first2));
    }

    return init;
}

/*
**  partial_sum 部分和
**
*/
template <class InputIterator, class OutputIterator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
    if(first == last) 
        return result;
    *result = *first;
    iterator_traits<InputIterator>::value_type val = *first;
    while(++first != last) {
        val = val + *first;
        *++result = val;
    }
    return ++result;
}


template <class InputIterator, class OutputIterator, class BinaryOperator>
OutputIterator partial_sum(InputIterator first, InputIterator last, OutputIterator result) {
    if(first == last) 
        return result;
    *result = *first;
    iterator_traits<InputIterator>::value_type val = *first;
    while(++first != last) {
        val = binary_op(val,*first);
        *++result = val;
    }
    return ++result;
}

/*
**  power  乘幂
**
*/

//!TODO: 仿函数multiplies
template <class T, class Integer>
inline T power(T x, Integer n) {
    return power(x, n, multiplies<T>());
}

//!TODO: 证同函数 identity_element
template <class T, class Integer, class MonoidOperation>
inline T power(T x, Integer n, MonoidOperation op) {
    if (n == 0)
        return identity_element(op);
    else {
        while((n&1) == 0) {
            n >>= 1;
            x = op(x,x);
        }

        T result = x;
        n >>= 1;
        while (n != 0) {
            x = op(x, x);
            if ((n&1) != 0)
                result = op(result, x);
            n >>= 1;
        } 
        return result;
    }
    
}

/*
**  iota  在某区间填入某指定值得递增序列
**
*/

template <class ForwardIterator, class T>
void iota(ForwardIterator first, ForwardIterator last, T val) {
    while(first != last)
        *first++ = val++;
}



__GG_END_NAMESPACE
#endif