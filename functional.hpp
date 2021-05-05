#ifndef FUNCTIONAL_HPP
#define FUNCTIONAL_HPP

namespace ft {
	/*	binary_function is just like std::binary_function for ft::map	*/
	template <class Arg1, class Arg2, class Result>
	struct binary_function {
		typedef Arg1	first_argument_type;
		typedef Arg2	second_argument_type;
		typedef Result	result_type;
	};

	/*	equal_to, less and greater templates for ft::list and ft::map	*/
	template <class T>
	struct equal_to	: binary_function<T, T, bool> {
		bool	operator()(const T& x, const T& y) const { return (x == y); }
	};

	template <class T>
	struct less		: binary_function<T, T, bool> {
		bool	operator()(const T& x, const T& y) const { return (x < y); }
	};

	template <class T>
	struct greater	: binary_function<T, T, bool> {
		bool	operator()(const T& x, const T& y) const { return (x > y); }
	};

	/*	pair is just like std::pair for ft::map (but i don't use it)	*/
	template <class T1, class T2>
	struct pair {
		typedef T1		first_type;
		typedef T2		second_type;

		first_type		first;
		second_type		second;

		//	default (1)
		pair() : first(first_type()), second(second_type()) {}

		//	copy (2)
		template <class U, class V>
		pair(const pair<U, V>& pr) : first(pr.first), second(pr.second) {}

		//	initialization (3)
		pair(const first_type& a, const second_type& b) : first(a), second(b) {}

		//	implicitly declared:
		pair&	operator=(const pair& pr)
		{
			this->first = pr.first;
			this->second = pr.second;
			return (*this);
		}
	};

	/*	non-member function overloads (pair)	*/
	//	relational operators (pair)
	template <class T1, class T2>
	bool		operator==(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs)
	{	return (lhs.first == rhs.first && lhs.second == rhs.second);	}

	template <class T1, class T2>
	bool		operator!=(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs)
	{	return (!(lhs == rhs));	}

	template <class T1, class T2>
	bool		operator<(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs)
	{
		return (lhs.first < rhs.first ||
			(!(rhs.first < lhs.first) &&
			lhs.second < rhs.second));
	}

	template <class T1, class T2>
	bool		operator<=(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs)
	{	return (!(rhs < lhs));	}

	template <class T1, class T2>
	bool		operator>(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs)
	{	return (rhs < lhs);		}

	template <class T1, class T2>
	bool		operator>=(const ft::pair<T1, T2>& lhs, const ft::pair<T1, T2>& rhs)
	{	return (!(lhs < rhs));	}

	//	construct pair object
	template <class T1, class T2>
	ft::pair<T1, T2>	make_pair(T1 x, T2 y)
	{	return (ft::pair<T1, T2>(x, y));	}

	//	additional function for heap algorithm
	template <class Iterator>
	void	_swap_iter_values(Iterator first, Iterator second) {
		typename ft::iterator_traits<Iterator>::value_type	tmp = *first;
		*first = *second;
		*second = tmp;
	}

	/*	heap algorithm	*/
	//	temp solution is using std::make_heap function
	#include <algorithm>
	template <class RandomAccessIterator, class Compare>
	void	make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		typedef	ft::iterator_traits<RandomAccessIterator>	traits;
		typename traits::difference_type	dist = last - first - 1;
		if (dist < 1)
			return;
		std::make_heap(first, last, comp);
	}

	template <class RandomAccessIterator>
	void	make_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		make_heap(first, last, ft::less<typename ft::iterator_traits<RandomAccessIterator>::value_type>());
	}

	template <class RandomAccessIterator, class Compare>
	void	push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		typedef	ft::iterator_traits<RandomAccessIterator>	traits;
		typename traits::difference_type	i = last - first;
		while (i > 1 && comp(first[i / 2 - 1], first[i - 1])) {
			_swap_iter_values(first + (i / 2) - 1, first + i - 1);
			i /= 2;
		}
	}

	template <class RandomAccessIterator>
	void	push_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		push_heap(first, last, ft::less<typename ft::iterator_traits<RandomAccessIterator>::value_type>());
	}

	template <class RandomAccessIterator, class Compare>
	void	pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		_swap_iter_values(first, --last);
		make_heap(first, last, comp);
	}

	template <class RandomAccessIterator>
	void	pop_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		pop_heap(first, last, ft::less<typename ft::iterator_traits<RandomAccessIterator>::value_type>());
	}

}

#endif
