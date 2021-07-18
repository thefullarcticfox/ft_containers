#ifndef LIST_HPP
#define LIST_HPP
#include <iterator>				//	std::bidirectional_iterator_tag
#include "adapted_traits.hpp"	//	type_traits
#include "allocator.hpp"		//	allocator
#include "algorithm.hpp"		//	equal, lexicographical_compare
#include "functional.hpp"		//	less, equal_to
#include "reverse_iterator.hpp"	//	reverse_iterator

namespace ft {

	template <class Alloc>
	struct list_base {
	private:
		typedef typename Alloc::value_type	value_type;
		//	no need to construct without data
		list_base() : prev(this), next(this) {}
	public:
		value_type	data;
		list_base*	prev;
		list_base*	next;

		//	i don't use those but i keep them just in case
		list_base(const value_type& data) : data(data), prev(this), next(this) {}
		list_base(const list_base& x) : data(x.data), prev(x.prev), next(x.next) {}
		list_base&	operator=(const list_base& x)
		{
			if (this == &x)
				return (*this);
			this->data = x.data;
			this->prev = x.prev;
			this->next = x.next;
			return (*this);
		}
		~list_base() {}
	};

	/*	std::iterator inheritance is useless and deprecated since C++17	*/
	template <class Alloc>
	class list_iterator {
	private:
		typedef list_base<Alloc>		list_node;
		list_node*			_node;
	public:
		typedef std::bidirectional_iterator_tag		iterator_category;
		typedef typename Alloc::difference_type		difference_type;
		typedef typename Alloc::value_type			value_type;
		typedef typename Alloc::pointer				pointer;
		typedef typename Alloc::reference			reference;

		/*	default				*/
		list_iterator() : _node(NULL) {}

		/*	construct from ptr	*/
		explicit
		list_iterator(list_node* x) : _node(x) {}

		/*	copy constructor	*/
		list_iterator(const list_iterator& x) : _node(x._node) {}

		~list_iterator() {}

		list_iterator&	operator=(const list_iterator& x)
		{
			if (this == &x)
				return (*this);
			this->_node = x._node;
			return (*this);
		}

		//	becomes nonmember as i add friend
		//	needed in case i want to compare const and nonconst iterators
		friend
		bool	operator==(const list_iterator& x, const list_iterator& y)
		{ return (x._node == y._node); };
		friend
		bool	operator!=(const list_iterator& x, const list_iterator& y)
		{ return (x._node != y._node); };

		reference		operator*() const { return (_node->data); }
		pointer			operator->() const { return (&(_node->data)); }

		list_iterator&	operator++() { _node = _node->next; return (*this); }
		list_iterator	operator++(int)
		{
			list_iterator	tmp = *this;
			++(*this);
			return (tmp);
		}
		list_iterator&	operator--() { _node = _node->prev; return (*this); }
		list_iterator	operator--(int)
		{
			list_iterator	tmp = *this;
			--(*this);
			return (tmp);
		}

		list_node*		base() const { return (this->_node); }
	};

	/*	additional const iterator struct so iterator is convertible to const_iterator  	*/
	template <class Alloc>
	class list_const_iterator {
	private:
		typedef const list_base<Alloc>	list_node;
		typedef list_iterator<Alloc>	iterator;
		const list_node*	_node;
	public:
		typedef std::bidirectional_iterator_tag		iterator_category;
		typedef typename Alloc::difference_type		difference_type;
		typedef typename Alloc::value_type			value_type;
		typedef typename Alloc::const_pointer		pointer;
		typedef typename Alloc::const_reference		reference;

		/*	default				*/
		list_const_iterator() : _node(NULL) {}

		/*	construct from ptr	*/
		explicit
		list_const_iterator(const list_node* x) : _node(x) {}

		/*	copy constructor 	*/
		list_const_iterator(const list_const_iterator& x) : _node(x._node) {}

		/*	copy constructor from nonconst iterator	*/
		list_const_iterator(const iterator& x) : _node(x.base()) {}

		~list_const_iterator() {}

		list_const_iterator&	operator=(const list_const_iterator& x)
		{
			if (this == &x)
				return (*this);
			this->_node = x._node;
			return (*this);
		}

		list_const_iterator&	operator=(const iterator& x)
		{
			if (this == &x)
				return (*this);
			this->_node = x.base();
			return (*this);
		}

		//	becomes nonmember as i add friend
		//	needed in case i want to compare const and nonconst iterators
		friend
		bool	operator==(const list_const_iterator& x, const list_const_iterator& y)
		{ return (x._node == y._node); };
		friend
		bool	operator!=(const list_const_iterator& x, const list_const_iterator& y)
		{ return (x._node != y._node); };

		reference				operator*() const { return (_node->data); }
		pointer					operator->() const { return (&(_node->data)); }

		list_const_iterator&	operator++() { _node = _node->next; return (*this); }
		list_const_iterator		operator++(int)
		{
			list_const_iterator	tmp = *this;
			++(*this);
			return (tmp);
		}
		list_const_iterator&	operator--() { _node = _node->prev; return (*this); }
		list_const_iterator		operator--(int)
		{
			list_const_iterator	tmp = *this;
			--(*this);
			return (tmp);
		}

		const list_node*	base() const { return (this->_node); }
	};

	template < class T, class Alloc = allocator<T> >
	class list {
	public:
	    // types:
		typedef T										value_type;
		typedef Alloc									allocator_type;
		typedef typename Alloc::reference				reference;
		typedef typename Alloc::const_reference			const_reference;
		typedef typename Alloc::pointer					pointer;
		typedef typename Alloc::const_pointer			const_pointer;
		typedef typename Alloc::difference_type			difference_type;
		typedef typename Alloc::size_type				size_type;
		//	a bidirectional iterator to value_type (std::bidirectional_iterator_tag)
		typedef list_iterator<Alloc>					iterator;
		//	a bidirectional iterator to const value_type
		typedef list_const_iterator<Alloc>				const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

	private:
		typedef list_base<allocator_type>							list_node;
		typedef typename Alloc::template rebind<list_node>::other	node_allocator_type;
		node_allocator_type		_alloc;
		size_type				_size;
		list_node*				_end;

		list_node*	_list_node_create(const value_type& val)
		{
			allocator_type	data_alloc(_alloc);
			list_node*		node = _alloc.allocate(1);
			node->next = node;
			node->prev = node;
			data_alloc.construct(&(node->data), val);

		//	this could have many more actions (better keep object copies to minimum)
		//	_alloc.construct(node, list_node(val));
			return (node);
		}

		void		_list_node_destroy(list_node* node)
		{
			_alloc.destroy(node);
			_alloc.deallocate(node, 1);
		}

		void		_list_end_init()
		{
			_end = _alloc.allocate(1);
			_end->prev = _end;		// prev is back()
			_end->next = _end;		// next is front()
		}

		void		_list_fill_assign(size_type n, const value_type& val)
		{
			clear();
			while (_size < n)
				push_back(val);
		}

		template <class Integer>
		void		_list_range_assign(Integer n, Integer val, ft::type_true)
		{
			_list_fill_assign(static_cast<size_type>(n), val);
		}

		template <class InputIterator>
		void		_list_range_assign(InputIterator first, InputIterator last, ft::type_false)
		{
			clear();
			InputIterator	it = first;
			while (it != last)
			{
				push_back(*it);
				++it;
			}
		}

	public:
		//	default constructor (1)
		explicit
		list(const allocator_type& alloc = allocator_type()) : _alloc(alloc), _size(0)
		{
			_list_end_init();
		}

		//	fill constructor (2)
		explicit
		list(size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type()) : _alloc(alloc), _size(0)
		{
			_list_end_init();
			_list_fill_assign(n, val);
		}

		//	range constructor (3)
		template <class InputIterator>
		list(InputIterator first, InputIterator last,
			const allocator_type& alloc = allocator_type()) : _alloc(alloc), _size(0)
		{
			_list_end_init();
			_list_range_assign(first, last, ft::type_is_integer<InputIterator>());
		}

		//	copy constructor (4)
		list(const list& x) : _alloc(x._alloc), _size(0)
		{
			_list_end_init();
			assign(x.begin(), x.end());
		}

		//	destructor
		~list()
		{
			clear();
			_alloc.deallocate(_end, 1);
		}

		//	copy (1)
		list&	operator=(const list& x)
		{
			if (this == &x)
				return (*this);
			assign(x.begin(), x.end());
			return (*this);
		}

		//	iterators:
		iterator				begin()
		{
			return (iterator(this->_end->next));
		}

		const_iterator			begin() const
		{
			return (const_iterator(this->_end->next));
		}

		iterator				end()
		{
			return (iterator(this->_end));
		}

		const_iterator			end() const
		{
			return (const_iterator(this->_end));
		}

		reverse_iterator		rbegin()
		{
			return (reverse_iterator(end()));
		}

		const_reverse_iterator	rbegin() const
		{
			return (const_reverse_iterator(end()));
		}

		reverse_iterator		rend()
		{
			return (reverse_iterator(begin()));
		}

		const_reverse_iterator	rend() const
		{
			return (const_reverse_iterator(begin()));
		}

		//	capacity:
		bool			empty() const
		{
			return (this->_size == 0);
		}

		size_type		size() const
		{
			return (this->_size);
		}

		size_type		max_size() const
		{
			return (_alloc.max_size());
		}

		//	element access:
		reference				front()
		{
			return (this->_end->next->data);
		}

		const_reference			front() const
		{
			return (this->_end->next->data);
		}

		reference				back()
		{
			return (this->_end->prev->data);
		}

		const_reference			back() const
		{
			return (this->_end->prev->data);
		}

		//	modifiers:
		template <class InputIterator>
		void		assign(InputIterator first, InputIterator last)
		{
			_list_range_assign(first, last, ft::type_is_integer<InputIterator>());
		}

		void		assign(size_type n, const value_type& val)
		{
			_list_fill_assign(n, val);
		}

		void		push_front(const value_type& val)
		{
			list_node*	newnode = _list_node_create(val);
			newnode->prev = _end;			//	newfront->prev	= end aka rend
			newnode->next = _end->next;		//	newfront->next	= end->next (lastfront)
			newnode->next->prev = newnode;	//	lastfront->prev	= newfront
			_end->next = newnode;			//	end->next		= newfront
			++_size;
		}

		void		pop_front()
		{
			list_node*	_front = _end->next;
			if (_front != _end)				// preventing undefined behaviour for now
			{
				_end->next = _front->next;
				_end->next->prev = _end;	// both directions updated
				_list_node_destroy(_front);
				if (--_size == 0)
					_end->prev = _end;
			}
		}

		void		push_back(const value_type& val)
		{
			list_node*	newnode = _list_node_create(val);
			newnode->next = _end;			//	newback->next	= end
			newnode->prev = _end->prev;		//	newback->prev	= end->prev (lastback)
			newnode->prev->next = newnode;	//	lastback->next	= newback
			_end->prev = newnode;			//	end->prev		= newback
			++_size;
		}

		void		pop_back()
		{
			list_node*	_back = _end->prev;
			if (_back != _end)				// preventing undefined behaviour for now
			{
				_end->prev = _back->prev;
				_end->prev->next = _end;	// both directions updated
				_list_node_destroy(_back);
				if (--_size == 0)
					_end->next = _end;
			}
		}

		iterator	insert(iterator position, const value_type& val)
		{
			list_node*	pos = position.base();
			list_node*	newnode = _list_node_create(val);
			pos->prev->next = newnode;
			newnode->prev = pos->prev;
			newnode->next = pos;
			pos->prev = newnode;
			++_size;
			return (iterator(newnode));
		}

		void		insert(iterator position, size_type n, const value_type& val)
		{
			list	tmplist(n, val, _alloc);
			this->splice(position, tmplist);
		}

		template <class InputIterator>
		void		insert(iterator position, InputIterator first, InputIterator last)
		{
			list	tmplist(first, last, _alloc);
			this->splice(position, tmplist);
		}

		iterator	erase(iterator position)
		{
			if (position == end())
				return (position);
			list_node*	pos = position.base();
			iterator	ret(pos->next);
			pos->next->prev = pos->prev;
			pos->prev->next = pos->next;
			_list_node_destroy(pos);
			--_size;
			return (ret);
		}

		iterator	erase(iterator first, iterator last)
		{
			iterator	it = first;
			while (it != last)
				it = erase(it);
			return (it);
		}

		void		swap(list& x)
		{
			size_type	swapsize = this->_size;
			list_node*	swapnode = this->_end;

			this->_size = x._size;
			this->_end = x._end;

			x._size = swapsize;
			x._end = swapnode;
		}

		void		resize(size_type n, value_type val = value_type())
		{
			while (n < _size)
				pop_back();
			while (n > _size)
				push_back(val);
		}

		void		clear()
		{
			while (_size != 0)
				pop_back();
		}

		//	operations:
		void		splice(iterator position, list& x)
		{
			if (x.empty() || &x == this)
				return ;	// saving from undefined behaviour for now

			list_node*	next = position.base();
			list_node*	prev = next->prev;
			list_node*	xfront = x._end->next;
			list_node*	xback = x._end->prev;

			//	double link back of x into this list
			xback->next = next;
			next->prev = xback;

			//	double link front of x into this list
			xfront->prev = prev;
			prev->next = xfront;

			this->_size += x._size;

			//	loop x end as if it was empty
			x._end->next = x._end;
			x._end->prev = x._end;
			x._size = 0;
		}

		void		splice(iterator position, list& x, iterator i)
		{
			if (x.empty() || i == x.end() || i == position)
				return ;

			list_node*	next = position.base();
			list_node*	prev = next->prev;
			list_node*	elem = i.base();

			//	close the gap before elem transfer
			elem->prev->next = elem->next;
			elem->next->prev = elem->prev;
			x._size--;

			//	double link elem into this list
			elem->next = next;
			elem->prev = prev;
			next->prev = elem;
			prev->next = elem;
			this->_size++;
		}

		void		splice(iterator position, list& x, iterator first, iterator last)
		{
			if (x.empty() || first == x.end())
				return ;

			iterator	tmp = first;
			while (first != last)
			{
				++tmp;
				splice(position, x, first);
				first = tmp;
			}
		}

		void		remove(const value_type& val)
		{
			iterator	it = begin();
			while (it != end())
				if (*it == val)
					it = erase(it);
				else
					it++;
		}

		template <class Predicate>
		void		remove_if(Predicate pred)
		{
			iterator	it = begin();
			while (it != end())
				if (pred(*it))
					it = erase(it);
				else
					it++;
		}

		void		unique()
		{
			unique(ft::equal_to<value_type>());
		}

		template <class BinaryPredicate>
		void		unique(BinaryPredicate binary_pred)
		{
			if (_size == 0)
				return ;

			iterator	it = begin();
			iterator	it2 = it;
			while (++it2 != end())
			{
				if (binary_pred(*it, *it2))
					erase(it2);
				else
					++it;
				it2 = it;
			}
		}

		void		merge(list& x)
		{
			merge(x, ft::less<value_type>());
		}

		template <class Compare>
		void		merge(list& x, Compare comp)
		{
			if (&x == this)
				return ;

			iterator	it = this->begin();
			iterator	xit = x.begin();
			while (it != this->end() && xit != x.end())
			{
				if (comp(*xit, *it))
				{
					iterator	elem = xit;
					++xit;
					splice(it, x, elem);
				}
				else
					++it;
			}
			if (xit != x.end())
				splice(this->end(), x);
		}

		void		sort()
		{
			sort(ft::less<value_type>());
		}

		template <class Compare>
		void		sort(Compare comp)
		{
			if (_size == 0 || _size == 1)
				return ;

			// finding the middle
			iterator	it = this->begin();
			for (size_type i = 0; i * 2 < _size && it != this->end(); i++)
				++it;

			// split in half
			list		rhalf;
			rhalf.splice(rhalf.begin(), *this, it, this->end());

			// sort both halves
			this->sort(comp);
			rhalf.sort(comp);

			// merge back
			this->merge(rhalf, comp);
		}

		void		reverse()
		{
			list_node*	pos = _end->next;
			while (pos != _end)
			{
				list_node*	next = pos->next;
				pos->next = pos->prev;
				pos->prev = next;
				pos = next;
			}
			pos = _end->next;
			_end->next = _end->prev;
			_end->prev = pos;
		}
	};

	/*	non-member function overloads	*/
	//	relational operators (list)
	template <class T, class Alloc>
	bool	operator==(const ft::list<T, Alloc>& lhs, const ft::list<T, Alloc>& rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class T, class Alloc>
	bool	operator!=(const ft::list<T, Alloc>& lhs, const ft::list<T, Alloc>& rhs)
	{	return (!(lhs == rhs));	}

	template <class T, class Alloc>
	bool	operator<(const ft::list<T, Alloc>& lhs, const ft::list<T, Alloc>& rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class T, class Alloc>
	bool	operator<=(const ft::list<T, Alloc>& lhs, const ft::list<T, Alloc>& rhs)
	{	return (!(rhs < lhs));	}

	template <class T, class Alloc>
	bool	operator>(const ft::list<T, Alloc>& lhs, const ft::list<T, Alloc>& rhs)
	{	return (rhs < lhs);		}

	template <class T, class Alloc>
	bool	operator>=(const ft::list<T, Alloc>& lhs, const ft::list<T, Alloc>& rhs)
	{	return (!(lhs < rhs));	}

	//	swap (list)
	template <class T, class Alloc>
	void	swap(ft::list<T, Alloc>& x, ft::list<T, Alloc>& y)
	{	x.swap(y);	}
}

#endif
