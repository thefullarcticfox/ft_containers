#ifndef PRIORITY_QUEUE_HPP
#define PRIORITY_QUEUE_HPP
#include "functional.hpp"
#include "algorithm.hpp"
#include "vector.hpp"

namespace ft {

	template < class T, class Container = vector<T>, class Compare = less<typename Container::value_type> >
	class priority_queue {
	public:
		// types:
		typedef T									value_type;
		typedef Container							container_type;
		typedef typename container_type::size_type	size_type;
	protected:
		container_type	c;
		Compare			comp;
	public:
		explicit
		priority_queue(const Compare& comp = Compare(),
			const Container& ctnr = Container()) : c(ctnr), comp(comp)
		{
			ft::make_heap(c.begin(), c.end(), comp);
		}

		template <class InputIterator>
		priority_queue(InputIterator first, InputIterator last, const Compare& comp = Compare(),
			const Container& ctnr = Container()) : c(ctnr), comp(comp)
		{
			c.insert(c.end(), first, last);
			ft::make_heap(c.begin(), c.end(), comp);
		}

		bool		empty() const			{	return (this->c.empty());	}
		size_type	size()	const			{	return (this->c.size());	}

		value_type&			top()			{	return (this->c.front());	}
		const value_type&	top() const		{	return (this->c.front());	}

		void	push(const value_type& val)
		{
			c.push_back(val);
			ft::push_heap(c.begin(), c.end(), comp);
		}

		void	pop()
		{
			ft::pop_heap(c.begin(), c.end(), comp);
			c.pop_back();
		}
	};

}

#endif
