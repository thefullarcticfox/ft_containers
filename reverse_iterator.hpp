#ifndef REVERSE_ITERATOR_HPP
#define REVERSE_ITERATOR_HPP
#include "adapted_traits.hpp"	//	iterator_traits

namespace ft {
	/*	adapted reverse iterator	*/
	template <class Iterator>
	class reverse_iterator {
	public:
		typedef Iterator												iterator_type;
		typedef typename iterator_traits<Iterator>::iterator_category	iterator_category;
		typedef typename iterator_traits<Iterator>::value_type			value_type;
		typedef typename iterator_traits<Iterator>::difference_type		difference_type;
		typedef typename iterator_traits<Iterator>::pointer				pointer;
		typedef typename iterator_traits<Iterator>::reference			reference;
	private:
		iterator_type	_it;
	public:
		reverse_iterator() {}
		explicit
		reverse_iterator(iterator_type it) : _it(it)  {}
		template <class Iter>
		reverse_iterator(const reverse_iterator<Iter>& rev_it) : _it(rev_it.base()) {}

		reference			operator*() const
		{
			iterator_type		tmp = _it;
			return (*(--tmp));
		}
		reference			operator[](difference_type n) const { return (_it[-n - 1]); }
		pointer				operator->() const { return &(operator*()); }

		reverse_iterator	operator+(difference_type n) const { return (reverse_iterator(_it - n)); }
		reverse_iterator&	operator+=(difference_type n) { _it -= n; return (*this); }
		reverse_iterator&	operator++() { --_it; return (*this); }
		reverse_iterator 	operator++(int)
		{
			reverse_iterator	tmp = *this;
			++(*this);
			return (tmp);
		}
		reverse_iterator	operator-(difference_type n) const { return (reverse_iterator(_it + n)); }
		reverse_iterator&	operator-=(difference_type n) { _it += n; return (*this); }
		reverse_iterator&	operator--() { ++_it; return (*this); }
		reverse_iterator 	operator--(int)
		{
			reverse_iterator	tmp = *this;
			--(*this);
			return (tmp);
		}

		iterator_type		base() const { return (_it); }
	};

	template <class Iterator>
	bool	operator==(const ft::reverse_iterator<Iterator>& lhs, const ft::reverse_iterator<Iterator>& rhs)
	{	return (lhs.base() == rhs.base());	}

	template <class Iterator>
	bool	operator!=(const ft::reverse_iterator<Iterator>& lhs, const ft::reverse_iterator<Iterator>& rhs)
	{	return (lhs.base() != rhs.base());	}

	template <class Iterator>
	bool	operator<(const ft::reverse_iterator<Iterator>& lhs, const ft::reverse_iterator<Iterator>& rhs)
	{	return (lhs.base() > rhs.base());	}

	template <class Iterator>
	bool	operator<=(const ft::reverse_iterator<Iterator>& lhs, const ft::reverse_iterator<Iterator>& rhs)
	{	return (lhs.base() >= rhs.base());	}

	template <class Iterator>
	bool	operator>(const ft::reverse_iterator<Iterator>& lhs, const ft::reverse_iterator<Iterator>& rhs)
	{	return (lhs.base() < rhs.base());	}

	template <class Iterator>
	bool	operator>=(const ft::reverse_iterator<Iterator>& lhs, const ft::reverse_iterator<Iterator>& rhs)
	{	return (lhs.base() <= rhs.base());	}

	template <class Iterator>
	ft::reverse_iterator<Iterator>	operator+(typename ft::reverse_iterator<Iterator>::difference_type n,
		const ft::reverse_iterator<Iterator>& rev_it)
	{	return (rev_it + n);				}

	template <class Iterator>
	typename ft::reverse_iterator<Iterator>::difference_type	operator-(
		const ft::reverse_iterator<Iterator>& lhs, const ft::reverse_iterator<Iterator>& rhs)
	{	return (rhs.base() - lhs.base());	}

	/*	const/nonconst ops require additional overloads	*/
	template <class Iterator1, class Iterator2>
	bool	operator==(const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs)
	{	return (lhs.base() == rhs.base());	}

	template <class Iterator1, class Iterator2>
	bool	operator!=(const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs)
	{	return (lhs.base() != rhs.base());	}

	template <class Iterator1, class Iterator2>
	bool	operator<(const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs)
	{	return (lhs.base() > rhs.base());	}

	template <class Iterator1, class Iterator2>
	bool	operator<=(const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs)
	{	return (lhs.base() >= rhs.base());	}

	template <class Iterator1, class Iterator2>
	bool	operator>(const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs)
	{	return (lhs.base() < rhs.base());	}

	template <class Iterator1, class Iterator2>
	bool	operator>=(const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs)
	{	return (lhs.base() <= rhs.base());	}

	template <class Iterator1, class Iterator2>
	typename ft::reverse_iterator<Iterator1>::difference_type	operator-(
		const ft::reverse_iterator<Iterator1>& lhs, const ft::reverse_iterator<Iterator2>& rhs)
	{	return (rhs.base() - lhs.base());	}
}

#endif
