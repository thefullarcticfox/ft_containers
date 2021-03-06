#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP
#include "adapted_traits.hpp"
#include "functional.hpp"

namespace ft {

	template <class T1, class T2>
	struct _equal_twotypes	: binary_function<T1, T2, bool> {
		bool	operator()(const T1& x, const T2& y) const
		{	return (x == y);	}
	};

	template <class T1, class T2>
	struct _less_twotypes	: binary_function<T1, T2, bool> {
		bool	operator()(const T1& x, const T2& y) const
		{	return (x < y);		}
	};

	template <class InputIterator1, class InputIterator2, class BinaryPredicate>
	bool	equal(InputIterator1 first1, InputIterator1 last1,
				InputIterator2 first2, BinaryPredicate pred)
	{
		while (first1 != last1)
		{
			if (!pred(*first1, *first2))
				return (false);
			++first1; ++first2;
		}
		return (true);
	}

	template <class InputIterator1, class InputIterator2, class Compare>
	bool	lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
									InputIterator2 first2, InputIterator2 last2, Compare comp)
	{
		while (first1 != last1)
		{
			if (first2 == last2 || comp(*first2, *first1))
				return (false);
			else if (comp(*first1, *first2))
				return (true);
			++first1;	++first2;
		}
		return (first2 != last2);
	}

	template <class InputIterator1, class InputIterator2>
	bool	equal(InputIterator1 first1, InputIterator1 last1, InputIterator2 first2)
	{
		typedef typename ft::iterator_traits<InputIterator1>::value_type	type1;
		typedef typename ft::iterator_traits<InputIterator2>::value_type	type2;
		return (ft::equal(first1, last1, first2, _equal_twotypes<type1, type2>()));
	}

	template <class InputIterator1, class InputIterator2>
	bool	lexicographical_compare(InputIterator1 first1, InputIterator1 last1,
									InputIterator2 first2, InputIterator2 last2)
	{
		typedef typename ft::iterator_traits<InputIterator1>::value_type	type1;
		typedef typename ft::iterator_traits<InputIterator2>::value_type	type2;
		return (ft::lexicographical_compare(first1, last1, first2, last2, _less_twotypes<type1, type2>()));
	}

	template <class Iterator>
	typename iterator_traits<Iterator>::difference_type
			_find_distance(Iterator first, Iterator last, std::input_iterator_tag)
	{
		typename iterator_traits<Iterator>::difference_type	res = 0;
		for (; first != last; first++)
			++res;
		return (res);
	}

	template <class Iterator>
	typename iterator_traits<Iterator>::difference_type
			_find_distance(Iterator first, Iterator last, std::random_access_iterator_tag)
	{	return (last - first);	}

	template <class InputIterator>
	typename iterator_traits<InputIterator>::difference_type
			distance(InputIterator first, InputIterator last)
	{
		return (_find_distance(first, last,
			typename iterator_traits<InputIterator>::iterator_category()));
	}

	//	additional function for heap algorithm
	template <class Iterator>
	void	_swap_values(Iterator first, Iterator second)
	{
		typename iterator_traits<Iterator>::value_type	tmp = *first;
		*first = *second;
		*second = tmp;
	}

	/*	heap algorithm	*/
	template <class Iterator, class Compare, class DiffType>
	void	_heapify(Iterator first, Iterator last, Compare comp, DiffType index)
	{
		DiffType	size = distance(first, last);
		DiffType	left = index * 2;
		DiffType	right = index * 2 + 1;
		DiffType	largest = index;

		if (left <= size && comp(first[largest - 1], first[left - 1]))
			largest = left;
		if (right <= size && comp(first[largest - 1], first[right - 1]))
			largest = right;
		if (index != largest) {
			_swap_values(first + (index - 1), first + (largest - 1));
			_heapify(first, last, comp, largest);
		}
	}

	template <class RandomAccessIterator, class Compare>
	void	make_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		typedef typename iterator_traits<RandomAccessIterator>::difference_type	t_diff;
		for (t_diff i = distance(first, last) / 2; i >= 1; i--)
			_heapify(first, last, comp, i);
	}

	template <class RandomAccessIterator>
	void	make_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		make_heap(first, last, less<typename iterator_traits<RandomAccessIterator>::value_type>());
	}

	template <class RandomAccessIterator, class Compare>
	void	push_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		typedef	iterator_traits<RandomAccessIterator>	traits;
		typename traits::difference_type	i = distance(first, last);
		while (i > 1 && comp(first[i / 2 - 1], first[i - 1])) {
			_swap_values(first + (i / 2 - 1), first + (i - 1));
			i /= 2;
		}
	}

	template <class RandomAccessIterator>
	void	push_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		push_heap(first, last, less<typename iterator_traits<RandomAccessIterator>::value_type>());
	}

	template <class RandomAccessIterator, class Compare>
	void	pop_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		_swap_values(first, --last);
		make_heap(first, last, comp);
	}

	template <class RandomAccessIterator>
	void	pop_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		pop_heap(first, last, less<typename iterator_traits<RandomAccessIterator>::value_type>());
	}

	template <class RandomAccessIterator, class Compare>
	void	sort_heap(RandomAccessIterator first, RandomAccessIterator last, Compare comp)
	{
		while (distance(first, last) > 1)
			pop_heap(first, last--, comp);
	}

	template <class RandomAccessIterator>
	void	sort_heap(RandomAccessIterator first, RandomAccessIterator last)
	{
		sort_heap(first, last, less<typename iterator_traits<RandomAccessIterator>::value_type>());
	}
}

#endif
