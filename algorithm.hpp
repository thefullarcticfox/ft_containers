#ifndef ALGORITHM_HPP
#define ALGORITHM_HPP
#include "functional.hpp"

namespace ft {

	//	additional function for heap algorithm
	template <class Iterator>
	void	_swap_iter_values(Iterator first, Iterator second) {
		typename ft::iterator_traits<Iterator>::value_type	tmp = *first;
		*first = *second;
		*second = tmp;
	}

	//	temp solution is using std::make_heap function
	#include <algorithm>

	/*	heap algorithm	*/
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
