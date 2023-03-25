#ifndef __GG_ITERATOR_H
#define __GG_ITERATOR_H

#include <cstddef>                                          // for ptrdiff_t
#include "GG_config.h"
__GG_BEGIN_NAMESPACE

/*
** 5种迭代器类型
**
*/ 
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag :public input_iterator_tag {};
struct bidirectional_iterator_tag :public forward_iterator_tag {};
struct random_access_iterator_tag :public bidirectional_iterator_tag {};

template <class T, class Distance> struct input_iterator{
	typedef input_iterator_tag	iterator_category;
	typedef T					value_type;
	typedef Distance			difference_type;
	typedef T*					pointer;
	typedef T&					reference;
};

struct output_iterator {
	typedef output_iterator_tag iterator_category;
	typedef void                value_type;
	typedef void                difference_type;
	typedef void                pointer;
	typedef void                reference;
};


template <class T, class Distance> struct forward_iterator{
	typedef forward_iterator_tag	iterator_category;
	typedef T						value_type;
	typedef Distance				difference_type;
	typedef T*						pointer;
	typedef T&						reference;
};


template <class T, class Distance> struct bidirectional_iterator{
	typedef bidirectional_iterator_tag	iterator_category;
	typedef T							value_type;
	typedef Distance					difference_type;
	typedef T*							pointer;
	typedef T&							reference;
};


template <class T, class Distance> struct random_access_iterator{
	typedef random_access_iterator_tag	iterator_category;
	typedef T							value_type;
	typedef Distance					difference_type;
	typedef T*							pointer;
	typedef T&							reference;
};


template <class Category, class T, class Distance = ptrdiff_t,
class Pointer = T*, class Reference = T& >
struct iterator {
    typedef Category                    iterator_category;
    typedef T                           value_type;
    typedef Distance                    difference_type;
    typedef Pointer                     pointer;
    typedef Reference                   reference;
};


/*
**  traits
**
*/

template < class Iterator>
struct iterator_traits {
    typedef typename Iterator::iterator_category        iterator_category;
    typedef typename Iterator::value_type               value_type;
    typedef typename Iterator::difference_type          difference_type;
    typedef typename Iterator::pointer                  pointer;
    typedef typename Iterator::reference                reference;
};

//  针对原生指针的设计的traits特化版本

template <class T>
struct iterator_traits<T*> {
    typedef random_access_iterator_tag          iterator_category;
    typedef T                                   value_type;
    typedef ptrdiff_t                           difference_type;
    typedef T*                                  pointer;
    typedef T&                                  reference;
};


template <class T>
struct iterator_traits<const T*> {
    typedef random_access_iterator_tag          iterator_category;
    typedef T                                   value_type;
    typedef ptrdiff_t                           difference_type;
    typedef const T*                            pointer;
    typedef const T&                            reference;
};



// 方便获得某个迭代器的类型(category)
template <class Iterator>
inline typename iterator_traits<Iterator>::iterator_category
iterator_category(const Iterator&){
    typedef typename iterator_traits<Iterator>::iterator_category category;
    return category();
}

// 方便获得某个迭代器的value type
template <class Iterator>
inline typename iterator_traits<Iterator>::value_type*
value_type(const Iterator&){
    return static_cast<typename iterator_traits<Iterator>::value_type*>(0);
}


// 方便获得某个迭代器的distance type
template <class Iterator>
inline typename iterator_traits<Iterator>::difference_type*
distance_type(const Iterator&){
    return static_cast<typename iterator_traits<Iterator>::difference_type*>(0);
}

template <class InputIterator, class Distance>
inline void __distance(InputIterator first, InputIterator last, Distance& n, input_iterator_tag) {
	while (first != last) {
		++first;
		++n;
	}
}

template <class RandomAccessIterator, class Distance>
inline void __distance(RandomAccessIterator first, RandomAccessIterator last, Distance& n, random_access_iterator_tag) {
	n += last - first;
}

template <class InputIterator, class Distance>
inline void distance(InputIterator first, InputIterator last, Distance& n ){
	__distance(first, last, n, iterator_category(first));
}



/*
** 	reverse_iterator
**
*/


template <class Iterator>
class reverse_iterator {
	public:
		typedef Iterator	iterator_type;
		typedef typename iterator_traits<iterator>::iterator_category	iterator_category;
		typedef typename iterator_traits<iterator>::value_type	value_type;
		typedef typename iterator_traits<iterator>::difference_type	difference_type;
		typedef typename iterator_traits<iterator>::pointer	pointer;
		typedef const pointer	const_pointer;
		typedef typename iterator_traits<iterator>::reference	reference;


	public:
		reverse_iterator() : __base(0), __cur(0) {}
		explicit reverse_iterator(const iterator_type& iter): __base(iter) {
			auto temp = iter;
			__cur = --iter;
		}

		template <class Iter>
		reverse_iterator(const reverse_iterator<Iter>& rev_iter) {
			__base = (iterator_type)rev_iter.base();
			auto temp = __base;
			__cur = --temp;
		}

	public:
		iterator_type base() { return __base; }



	public:
		reference operator*() { return (*cur_); }
		pointer operator->() { return &(operator*());}
		reverse_iterator& operator++() {
			-- __base;
			-- __cur;
			return *this;
		}

		reverse_iterator& operator++(int) {
			reverse_iterator temp = *this;
			++(*this);
			return temp;
		}
		reverse_iterator& operator--() {
			++ __base;
			++ __cur;
			return *this;
		}

		reverse_iterator& operator--(int) {
			reverse_iterator temp = *this;
			--(*this);
			return temp;
		}

		reference operator[](difference_type n) {
			return base()[-n-1];
		}

		reverse_iterator operator+(difference_type n) const;
		reverse_iterator& operator+=(difference_type n);
		reverse_iterator operator-(difference_type n) const;
		reverse_iterator& operator-=(difference_type n);

	private:
		Iterator advance_n_step(Iterator iter, difference_type n, bool right, random_access_iterator_tag) {
			if (right)
				iter += n;
			else 
				iter -= n;
			return iter;
		}

		Iterator advance_n_step(Iterator iter, difference_type n, bool right, bidirectional_iterator_tag) {
			difference_type absn = (n>=0) ? n : -n;
			if ((right && n>0) || (!right && n < 0)) {
				for(int i=0; i < absn; ++i) 
					iter++;
			}
			else if((!right && n>0) || (right && n < 0)) {
				for(int i=0; i<absn; i++) 
					iter--;
			}

			return iter;
		}

	public:
		template<class Iterator>
		friend bool operator == (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) ;
		template<class Iterator>
		friend bool operator != (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) ;
		template<class Iterator>
		friend bool operator < (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) ;
		template<class Iterator>
		friend bool operator <= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) ;
		template<class Iterator>
		friend bool operator > (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) ;
		template<class Iterator>
		friend bool operator >= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) ;
		
		template<class Iterator>
		friend reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& rev_iter) ;
		template<class Iterator>
		friend typename reverse_iterator<Iterator>::difference_type operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) ;

	private:
		Iterator 	__base;
		Iterator	__cur;

};


template<class Iterator>
reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator+=(difference_type n) {
	__base = advance_n_step(__base, n, false, iterator_category());
	__cur = advance_n_step(__cur, n, false, iterator_category());

	return *this;
}

template<class Iterator>
reverse_iterator<Iterator>& reverse_iterator<Iterator>::operator-=(difference_type n) {
	__base = advance_n_step(__base, n, true, iterator_category());
	__cur = advance_n_step(__cur, n, true, iterator_category());

	return *this;
}

template<class Iterator>
reverse_iterator<Iterator> reverse_iterator<Iterator>::operator+(difference_type n) const {
	reverse_iterator<Iterator> result = *this;
	result += n;
	return result;
}

template<class Iterator>
reverse_iterator<Iterator> reverse_iterator<Iterator>::operator-(difference_type n) const {
	reverse_iterator<Iterator> result = *this;
	result -= n;
	return result;
}

template<class Iterator>
bool operator == (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
	return lhs.__cur == rhs.__cur;
}

template<class Iterator>
bool operator != (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
	return !(lhs == rhs);
}

template<class Iterator>
bool operator < (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
	return lhs.__cur < rhs.__cur;
}
		
template<class Iterator>
bool operator <= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
	return !(lhs.__cur > rhs.__cur);
}
		
template<class Iterator>
bool operator > (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
	return lhs.__cur > rhs.__cur;
}
		
template<class Iterator>
bool operator >= (const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
	return !(lhs.__cur < rhs.__cur);
}
		
template<class Iterator>
reverse_iterator<Iterator> operator+(typename reverse_iterator<Iterator>::difference_type n, const reverse_iterator<Iterator>& rev_iter) {
	return rev_iter + n;
}
		
template<class Iterator>
typename reverse_iterator<Iterator>::difference_type operator-(const reverse_iterator<Iterator>& lhs, const reverse_iterator<Iterator>& rhs) {
	return lhs.__cur - rhs.__cur;
}



__GG_END_NAMESPACE
#endif