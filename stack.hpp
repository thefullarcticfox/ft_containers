#ifndef STACK_HPP
#define STACK_HPP
#include <deque>

namespace ft {

	template < class T, class Container = std::deque<T> >
	class stack {
	public:
		// types:
		typedef T									value_type;
		typedef Container							container_type;
		typedef typename container_type::size_type	size_type;
	protected:
		container_type	c;
	public:
		explicit
		stack(const container_type& ctnr = container_type()) : c(ctnr) {}

		bool		empty() const			{	return (this->c.empty());	}
		size_type	size()	const			{	return (this->c.size());	}

		value_type&			top()			{	return (this->c.back());	}
		const value_type&	top() const		{	return (this->c.back());	}

		void	push(const value_type& val) {	this->c.push_back(val);		}
		void	pop()						{	this->c.pop_back();			}

		template <class T0, class C0> friend
		bool	operator==(const ft::stack<T0, C0>& lhs, const ft::stack<T0, C0>& rhs);
		template <class T0, class C0> friend
		bool	operator<(const ft::stack<T0, C0>& lhs, const ft::stack<T0, C0>& rhs);
	};

	/*	non-member function overloads	*/
	//	relational operators (stack)
	template <class T, class Container>
	bool	operator==(const ft::stack<T, Container>& lhs, const ft::stack<T, Container>& rhs)
	{	return (lhs.c == rhs.c);	}

	template <class T, class Container>
	bool	operator!=(const ft::stack<T, Container>& lhs, const ft::stack<T, Container>& rhs)
	{	return (!(lhs == rhs));		}

	template <class T, class Container>
	bool	operator<(const ft::stack<T, Container>& lhs, const ft::stack<T, Container>& rhs)
	{	return (lhs.c < rhs.c);		}

	template <class T, class Container>
	bool	operator<=(const ft::stack<T, Container>& lhs, const ft::stack<T, Container>& rhs)
	{	return (!(rhs < lhs));		}

	template <class T, class Container>
	bool	operator>(const ft::stack<T, Container>& lhs, const ft::stack<T, Container>& rhs)
	{	return (rhs < lhs);			}

	template <class T, class Container>
	bool	operator>=(const ft::stack<T, Container>& lhs, const ft::stack<T, Container>& rhs)
	{	return (!(lhs < rhs));		}
}

#endif
