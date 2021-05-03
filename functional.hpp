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

	/*	heap algorithm	*/
	//	temp solution is using std::*_heap stl functions
	#include <algorithm>

	template <class RandomAccessIterator>
	void	make_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		std::make_heap(first, last);
	}

	template <class RandomAccessIterator, class Compare>
	void	make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		std::make_heap(first, last, comp);
	}

	template <class RandomAccessIterator>
	void	push_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		std::push_heap(first, last);
	}

	template <class RandomAccessIterator, class Compare>
	void	push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		std::push_heap(first, last, comp);
	}

	template <class RandomAccessIterator>
	void	pop_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		std::pop_heap(first, last);
	}

	template <class RandomAccessIterator, class Compare>
	void	pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		std::pop_heap(first, last, comp);
	}

}

#endif
