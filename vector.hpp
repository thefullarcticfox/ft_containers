#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <iterator>				//	std::random_access_iterator_tag
#include <exception>			//	std::length_error, std::out_of_range
#include <sstream>				//	ostringstream
#include "adapted_traits.hpp"	//	type_traits, iterator_traits
#include "allocator.hpp"		//	allocator
#include "algorithm.hpp"		//	equal, lexicographical_compare
#include "reverse_iterator.hpp"	//	reverse_iterator

namespace ft {

	template <class Ptr, class NonConstPtr>
	class vector_iterator {
	private:
		typedef typename ft::iterator_traits<Ptr>		traits_type;
		Ptr		_node;
	public:
		typedef std::random_access_iterator_tag			iterator_category;
		typedef typename traits_type::difference_type	difference_type;
		typedef typename traits_type::value_type		value_type;
		typedef typename traits_type::pointer			pointer;
		typedef typename traits_type::reference			reference;

		/*	default				*/
		vector_iterator() : _node(Ptr()) {}

		/*	construct from ptr	*/
		explicit
		vector_iterator(const Ptr& x) : _node(x) {}

		/*	copy constructor	*/
		vector_iterator(const vector_iterator& x) : _node(x._node) {}

		/*	copy constructor from nonconst iterator						*/
		/*	the idea is from libcpp (doesn't work for list/tree nodes)	*/
		template <class _Ptr>
		vector_iterator(const vector_iterator<_Ptr, NonConstPtr>& x,
			typename type_check_if_same<_Ptr, NonConstPtr>::_type* check = NULL) :
			_node(x.base()) { (void)check; }

		~vector_iterator() {}

		vector_iterator		operator=(const vector_iterator& x)
		{
			this->_node = x._node;
			return (*this);
		}

		//	becomes nonmember as i add friend
		//	needed in case i want to compare const and nonconst iterators
		friend
		bool	operator==(const vector_iterator& x, const vector_iterator& y)
		{ return (x._node == y._node); };
		friend
		bool	operator!=(const vector_iterator& x, const vector_iterator& y)
		{ return (x._node != y._node); };

		reference			operator*() const { return (*_node); }
		reference			operator[](difference_type n) const	{ return (_node[n]); }
		pointer				operator->() const { return (_node); }

		vector_iterator&	operator++() { ++_node; return (*this); }
		vector_iterator		operator++(int)
		{
			vector_iterator	tmp = *this;
			++(*this);
			return (tmp);
		}
		vector_iterator&	operator--() { --_node; return (*this); }
		vector_iterator		operator--(int)
		{
			vector_iterator	tmp = *this;
			--(*this);
			return (tmp);
		}

		/*	a + n	*/
		vector_iterator		operator+(difference_type n) const
		{	return (vector_iterator(_node + n));	}

		/*	n + a	*/
		friend
		vector_iterator		operator+(difference_type n, const vector_iterator& x)
		{	return (x + n);		}

		/*	a - n	*/
		vector_iterator		operator-(difference_type n) const
		{	return (vector_iterator(_node - n));	}

		/*	a - b	*/
		friend
		difference_type		operator-(const vector_iterator& x, const vector_iterator& y)
		{	return (x._node - y._node);		}

		/*	a += n, a -= n	*/
		vector_iterator&	operator+=(difference_type n) { _node += n; return (*this); }
		vector_iterator&	operator-=(difference_type n) { _node -= n; return (*this); }

		/*	a < b, a > b, a <= b, a >= b	*/
		friend
		bool	operator<(const vector_iterator& x, const vector_iterator& y)
		{	return (x._node < y._node);		};
		friend
		bool	operator>(const vector_iterator& x, const vector_iterator& y)
		{	return (x._node > y._node);		};
		friend
		bool	operator<=(const vector_iterator& x, const vector_iterator& y)
		{	return (x._node <= y._node);	};
		friend
		bool	operator>=(const vector_iterator& x, const vector_iterator& y)
		{	return (x._node >= y._node);	};

		Ptr			base() const { return (this->_node); }
	};

	template < class T, class Alloc = allocator<T> >
	class vector {
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
		//	a random access iterator to value_type (std::random_access_iterator_tag)
		typedef vector_iterator<pointer, pointer>		iterator;
		//	a random access iterator to const value_type
		typedef vector_iterator<const_pointer, pointer>	const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

	private:
		allocator_type	_alloc;
		size_type		_capacity;
		size_type		_size;
		pointer			_base;

		void		_vector_base_init(size_type capacity = 0)
		{
			_size = 0;
			_capacity = capacity;
			_base = _alloc.allocate(_capacity);
		}

		void		_vector_base_free()
		{
			_alloc.deallocate(_base, _capacity);
		}

		template <class InputIterator>
		size_type	_iter_range_dist(InputIterator first, InputIterator last) const
		{
			size_type	dist = 0;
			for (InputIterator it = first; it != last; it++)
				dist++;
			return (dist);
		}

		void		_vector_check_range(size_type n, const std::string &meth = "") const
		{
			if (n >= _size)
			{
				std::ostringstream	oss;
				oss << "vector::";
				if (meth.empty())
					oss << "_vector_check_range: n";
				oss << meth << " (which is " << n <<
					") >= this->size() (which is " << this->size() << ")";
				throw (std::out_of_range(oss.str()));
			}
		}

		void		_vector_fill_assign(size_type n, const value_type& val)
		{
			clear();
			if (n > _capacity)
			{
				_vector_base_free();
				_vector_base_init(n);
			}
			while (_size < n)
				push_back(val);
		}

		template <class Integer>
		void		_vector_range_assign(Integer n, Integer val, ft::type_true)
		{
			_vector_fill_assign(static_cast<size_type>(n), val);
		}

		template <class InputIterator>
		void		_vector_range_assign(InputIterator first, InputIterator last, ft::type_false)
		{
			clear();
			size_t	dist = _iter_range_dist(first, last);
			if (dist > _capacity)
			{
				_vector_base_free();
				_vector_base_init(dist);
			}
			for (InputIterator it = first; it != last; it++)
				push_back(*it);
		}

		void		_vector_copy_assign(const vector& x)
		{
			clear();
			if (x._size > this->_capacity)
			{
				_vector_base_free();
				_vector_base_init(x._size);
			}
			for (size_type i = 0; i < x._size; i++)
				push_back(x.at(i));
		}

		iterator	_vector_fill_insert(iterator pos, size_type n, const value_type& val)
		{
			if (n == 0)
				return (pos);

			iterator	res = pos;
			if (_size + n > _capacity)
			{
				vector		tmp;
				if (_size + n <= _capacity * 2)
					tmp.reserve(_capacity * 2);
				else
					tmp.reserve(_size + n);

				iterator	it = this->begin();
				while (it != pos)
					tmp.push_back(*it++);

				res = tmp.end();
				for (size_type i = 0; i < n; i++)
					tmp.push_back(val);

				while (it != this->end())
					tmp.push_back(*it++);

				swap(tmp);
			}
			else
			{
				iterator	it = end();
				while (it != pos)
				{
					--it;
					_alloc.construct(it.base() + n, *it);
					_alloc.destroy(it.base());
				}
				for (size_type i = 0; i < n; i++)
				{
					_alloc.construct(it.base(), val);
					++it;
					++_size;
				}
			}

			return (res);
		}

		template <class Integer>
		void		_vector_range_insert(iterator pos, Integer n, Integer val, ft::type_true)
		{
			_vector_fill_insert(pos, static_cast<size_type>(n), val);
		}

		template <class InputIterator>
		void		_vector_range_insert(iterator pos, InputIterator first, InputIterator last, ft::type_false)
		{
			if (first == last)
				return ;
			vector		tmp(first, last);	//	in case [first, last) are from this vector
			for (size_type i = 0; i < tmp.size(); i++)
			{
				pos = _vector_fill_insert(pos, 1, tmp.at(i));
				++pos;
			}
		}

	public:
		//	default constructor (1)
		explicit
		vector(const allocator_type& alloc = allocator_type()) : _alloc(alloc), _capacity(0), _size(0)
		{
			_vector_base_init();
		}

		//	fill constructor (2)
		explicit
		vector(size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type()) : _alloc(alloc), _capacity(0), _size(0)
		{
			_vector_base_init(n);
			_vector_fill_assign(n, val);
		}

		//	range constructor (3)
		template <class InputIterator>
		vector(InputIterator first, InputIterator last,
			const allocator_type& alloc = allocator_type()) : _alloc(alloc), _capacity(0), _size(0)
		{
			_vector_base_init();
			_vector_range_assign(first, last, ft::type_is_integer<InputIterator>());
		}

		//	copy constructor (4)
		vector(const vector& x) : _alloc(x._alloc), _capacity(0), _size(0)
		{
			_vector_base_init(x._size);
 			_vector_copy_assign(x);
		}

		//	destructor
		~vector()
		{
 			clear();
			_vector_base_free();
 		}

		//	copy (1)
		vector&		operator=(const vector& x)
		{
			if (this == &x)
				return (*this);
			_vector_copy_assign(x);
 			return (*this);
		}

		//	iterators:
		iterator				begin()
		{
			return (iterator(_base));
		}

		const_iterator			begin() const
		{
			return (const_iterator(_base));
		}

		iterator				end()
		{
			return (iterator(_base + _size));
		}

		const_iterator			end() const
		{
			return (const_iterator(_base + _size));
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

		size_type		capacity() const
		{
			return (this->_capacity);
		}

		size_type		max_size() const
		{
			return (this->_alloc.max_size());
		}

		void			resize(size_type n, value_type val = value_type())
		{
			while (n < _size)
				pop_back();
			if (n > _capacity)
				reserve(n);
			while (n > _size)
				push_back(val);
		}

		void			reserve(size_type n)
		{
			if (n > this->max_size())
				throw (std::length_error("vector::reserve"));
			if (n > this->_capacity)
			{
				vector	tmp;
				tmp._vector_base_free();
				tmp._vector_base_init(n);
				tmp._vector_copy_assign(*this);
				swap(tmp);
			}
		}

		//	element access:
 		reference			operator[](size_type n)
		{
			return (this->_base[n]);
		}

		const_reference		operator[](size_type n) const
		{
			return (this->_base[n]);
		}

		reference			at(size_type n)
		{
			_vector_check_range(n, "at: n");
			return (this->_base[n]);
		}

		const_reference		at(size_type n) const
		{
			_vector_check_range(n, "at: n");
			return (this->_base[n]);
		}

		reference			front()
		{
			return (this->_base[0]);
		}

		const_reference		front() const
		{
			return (this->_base[0]);
		}

		reference			back()
		{
			return (this->_base[this->_size - 1]);
		}

		const_reference		back() const
		{
			return (this->_base[this->_size - 1]);
		}

		//	modifiers:
		template <class InputIterator>
		void		assign(InputIterator first, InputIterator last)
		{
			_vector_range_assign(first, last, ft::type_is_integer<InputIterator>());
		}

		void		assign(size_type n, const value_type& val)
		{
			_vector_fill_assign(n, val);
		}

		void		push_back(const value_type& val)
		{
			if (_size == _capacity)
			{
				if (_capacity == 0)
					reserve(1);
				else
					reserve(_capacity * 2);
			}
			_alloc.construct(_base + _size, val);
			++_size;
		}

		void		pop_back()
		{
			if (_size == 0)		// saving from undefined behaviour for now
				return ;
			_alloc.destroy(_base + _size - 1);
			--_size;
		}

		iterator	insert(iterator position, const value_type& val)
		{
			return (_vector_fill_insert(position, 1, val));
		}

		void		insert(iterator position, size_type n, const value_type& val)
		{
			_vector_fill_insert(position, n, val);
		}

		template <class InputIterator>
		void		insert(iterator position, InputIterator first, InputIterator last)
		{
			_vector_range_insert(position, first, last, ft::type_is_integer<InputIterator>());
		}

		iterator	erase(iterator position)
		{
			_alloc.destroy(position.base());

			iterator	it = position;
			while (++it != end())
			{
				_alloc.construct(it.base() - 1, *it);
				_alloc.destroy(it.base());
			}
			--_size;
			return (position);
		}

		iterator	erase(iterator first, iterator last)
		{
			difference_type	diff = last - first;
			if (diff <= 0)			//	saving from undefined behaviour
				return (first);

			iterator	it = first;
			while (it != last)
				_alloc.destroy((it++).base());
			--it;
			while (++it != end())
			{
				_alloc.construct((it - diff).base(), *it);
				_alloc.destroy(it.base());
			}
			_size -= diff;
			return (first);
		}

		void		swap(vector& x)
		{
			size_type	swapcapacity = this->_capacity;
			size_type	swapsize = this->_size;
			pointer		swapbase = this->_base;

			this->_capacity = x._capacity;
			this->_size = x._size;
			this->_base = x._base;

			x._capacity = swapcapacity;
			x._size = swapsize;
			x._base = swapbase;
		}

		void		clear()
		{
			while (_size != 0)
				pop_back();
		}
	};

	template <class Alloc>
	class vector<bool, Alloc> {
	public:
	    // types:
		typedef bool									value_type;
		typedef Alloc									allocator_type;
		class reference {
			friend class vector;
			reference() {}	// no public constructors
			explicit
			reference(bool& bit) : bit(bit) {}

			bool&		bit;
		public:
			~reference() {}
			//	convert to bool
			operator	bool() const { return (bit); }
			//	assign from bool
			reference&	operator=(const bool x)			{ bit = x; return (*this); }
			//	assign from bit
			reference&	operator=(const reference& x)	{ this->bit = x.bit; return (*this); }
			//	flip bit value.
			void		flip() { bit = !bit; }
		};
		typedef bool									const_reference;
		typedef bool*									pointer;
		typedef const bool*								const_pointer;
		typedef typename Alloc::difference_type			difference_type;
		typedef typename Alloc::size_type				size_type;
		//	a random access iterator to value_type (std::random_access_iterator_tag)
		typedef vector_iterator<pointer, pointer>		iterator;
		//	a random access iterator to const value_type
		typedef vector_iterator<const_pointer, pointer>	const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

	private:
		allocator_type	_alloc;
		size_type		_capacity;
		size_type		_size;
		pointer			_base;

		void		_vector_base_init(size_type capacity = 0)
		{
			_size = 0;
			_capacity = capacity;
			_base = _alloc.allocate(_capacity);
		}

		void		_vector_base_free()
		{
			_alloc.deallocate(_base, _capacity);
		}

		template <class InputIterator>
		size_type	_iter_range_dist(InputIterator first, InputIterator last) const
		{
			size_type	dist = 0;
			for (InputIterator it = first; it != last; it++)
				dist++;
			return (dist);
		}

		void		_vector_check_range(size_type n, const std::string &meth = "") const
		{
			if (n >= _size)
			{
				std::ostringstream	oss;
				oss << "vector::";
				if (meth.empty())
					oss << "_vector_check_range: n";
				oss << meth << " (which is " << n <<
					") >= this->size() (which is " << this->size() << ")";
				throw (std::out_of_range(oss.str()));
			}
		}

		void		_vector_fill_assign(size_type n, const value_type& val)
		{
			clear();
			if (n > _capacity)
			{
				_vector_base_free();
				_vector_base_init(n);
			}
			while (_size < n)
				push_back(val);
		}

		template <class Integer>
		void		_vector_range_assign(Integer n, Integer val, ft::type_true)
		{
			_vector_fill_assign(static_cast<size_type>(n), val);
		}

		template <class InputIterator>
		void		_vector_range_assign(InputIterator first, InputIterator last, ft::type_false)
		{
			clear();
			size_t	dist = _iter_range_dist(first, last);
			if (dist > _capacity)
			{
				_vector_base_free();
				_vector_base_init(dist);
			}
			for (InputIterator it = first; it != last; it++)
				push_back(*it);
		}

		void		_vector_copy_assign(const vector& x)
		{
			clear();
			if (x._size > this->_capacity)
			{
				_vector_base_free();
				_vector_base_init(x._size);
			}
			for (size_type i = 0; i < x._size; i++)
				push_back(x.at(i));
		}

		iterator	_vector_fill_insert(iterator pos, size_type n, const value_type& val)
		{
			if (n == 0)
				return (pos);

			iterator	res = pos;
			if (_size + n > _capacity)
			{
				vector		tmp;
				if (_size + n <= _capacity * 2)
					tmp.reserve(_capacity * 2);
				else
					tmp.reserve(_size + n);

				iterator	it = this->begin();
				while (it != pos)
					tmp.push_back(*it++);

				res = tmp.end();
				for (size_type i = 0; i < n; i++)
					tmp.push_back(val);

				while (it != this->end())
					tmp.push_back(*it++);

				swap(tmp);
			}
			else
			{
				iterator	it = end();
				while (it != pos)
				{
					--it;
					_alloc.construct(it.base() + n, *it);
					_alloc.destroy(it.base());
				}
				for (size_type i = 0; i < n; i++)
				{
					_alloc.construct(it.base(), val);
					++it;
					++_size;
				}
			}

			return (res);
		}

		template <class Integer>
		void		_vector_range_insert(iterator pos, Integer n, Integer val, ft::type_true)
		{
			_vector_fill_insert(pos, static_cast<size_type>(n), val);
		}

		template <class InputIterator>
		void		_vector_range_insert(iterator pos, InputIterator first, InputIterator last, ft::type_false)
		{
			if (first == last)
				return ;
			vector		tmp(first, last);	//	in case [first, last) are from this vector
			for (size_type i = 0; i < tmp.size(); i++)
			{
				pos = _vector_fill_insert(pos, 1, tmp.at(i));
				++pos;
			}
		}

	public:
		//	default constructor (1)
		explicit
		vector(const allocator_type& alloc = allocator_type()) : _alloc(alloc), _capacity(0), _size(0)
		{
			_vector_base_init();
		}

		//	fill constructor (2)
		explicit
		vector(size_type n, const value_type& val = value_type(),
			const allocator_type& alloc = allocator_type()) : _alloc(alloc), _capacity(0), _size(0)
		{
			_vector_base_init(n);
			_vector_fill_assign(n, val);
		}

		//	range constructor (3)
		template <class InputIterator>
		vector(InputIterator first, InputIterator last,
			const allocator_type& alloc = allocator_type()) : _alloc(alloc), _capacity(0), _size(0)
		{
			_vector_base_init();
			_vector_range_assign(first, last, ft::type_is_integer<InputIterator>());
		}

		//	copy constructor (4)
		vector(const vector& x) : _alloc(x._alloc), _capacity(0), _size(0)
		{
			_vector_base_init(x._size);
 			_vector_copy_assign(x);
		}

		//	destructor
		~vector()
		{
 			clear();
			_vector_base_free();
 		}

		//	copy (1)
		vector&		operator=(const vector& x)
		{
			if (this == &x)
				return (*this);
			_vector_copy_assign(x);
 			return (*this);
		}

		//	iterators:
		iterator				begin()
		{
			return (iterator(_base));
		}

		const_iterator			begin() const
		{
			return (const_iterator(_base));
		}

		iterator				end()
		{
			return (iterator(_base + _size));
		}

		const_iterator			end() const
		{
			return (const_iterator(_base + _size));
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

		size_type		capacity() const
		{
			return (this->_capacity);
		}

		size_type		max_size() const
		{
			return (this->_alloc.max_size());
		}

		void			resize(size_type n, value_type val = value_type())
		{
			while (n < _size)
				pop_back();
			if (n > _capacity)
				reserve(n);
			while (n > _size)
				push_back(val);
		}

		void			reserve(size_type n)
		{
			if (n > this->max_size())
				throw (std::length_error("vector::reserve"));
			if (n > this->_capacity)
			{
				vector	tmp;
				tmp._vector_base_free();
				tmp._vector_base_init(n);
				tmp._vector_copy_assign(*this);
				swap(tmp);
			}
		}

		//	element access:
 		reference			operator[](size_type n)
		{
			return (reference(this->_base[n]));
		}

		const_reference		operator[](size_type n) const
		{
			return (this->_base[n]);
		}

		reference			at(size_type n)
		{
			_vector_check_range(n, "at: n");
			return (reference(this->_base[n]));
		}

		const_reference		at(size_type n) const
		{
			_vector_check_range(n, "at: n");
			return (this->_base[n]);
		}

		reference			front()
		{
			return (reference(this->_base[0]));
		}

		const_reference		front() const
		{
			return (this->_base[0]);
		}

		reference			back()
		{
			return (reference(this->_base[this->_size - 1]));
		}

		const_reference		back() const
		{
			return (this->_base[this->_size - 1]);
		}

		//	modifiers:
		template <class InputIterator>
		void		assign(InputIterator first, InputIterator last)
		{
			_vector_range_assign(first, last, ft::type_is_integer<InputIterator>());
		}

		void		assign(size_type n, const value_type& val)
		{
			_vector_fill_assign(n, val);
		}

		void		push_back(const value_type& val)
		{
			if (_size == _capacity)
			{
				if (_capacity == 0)
					reserve(1);
				else
					reserve(_capacity * 2);
			}
			_alloc.construct(_base + _size, val);
			++_size;
		}

		void		pop_back()
		{
			if (_size == 0)		// saving from undefined behaviour for now
				return ;
			_alloc.destroy(_base + _size - 1);
			--_size;
		}

		iterator	insert(iterator position, const value_type& val)
		{
			return (_vector_fill_insert(position, 1, val));
		}

		void		insert(iterator position, size_type n, const value_type& val)
		{
			_vector_fill_insert(position, n, val);
		}

		template <class InputIterator>
		void		insert(iterator position, InputIterator first, InputIterator last)
		{
			_vector_range_insert(position, first, last, ft::type_is_integer<InputIterator>());
		}

		iterator	erase(iterator position)
		{
			_alloc.destroy(position.base());

			iterator	it = position;
			while (++it != end())
			{
				_alloc.construct(it.base() - 1, *it);
				_alloc.destroy(it.base());
			}
			--_size;
			return (position);
		}

		iterator	erase(iterator first, iterator last)
		{
			difference_type	diff = last - first;
			if (diff <= 0)			//	saving from undefined behaviour
				return (first);

			iterator	it = first;
			while (it != last)
				_alloc.destroy((it++).base());
			--it;
			while (++it != end())
			{
				_alloc.construct((it - diff).base(), *it);
				_alloc.destroy(it.base());
			}
			_size -= diff;
			return (first);
		}

		void		swap(vector& x)
		{
			size_type	swapcapacity = this->_capacity;
			size_type	swapsize = this->_size;
			pointer		swapbase = this->_base;

			this->_capacity = x._capacity;
			this->_size = x._size;
			this->_base = x._base;

			x._capacity = swapcapacity;
			x._size = swapsize;
			x._base = swapbase;
		}

		static
		void		swap(reference ref1, reference ref2)
		{
			bool	tmp = ref1;
			ref1 = ref2;
			ref2 = tmp;
		}

		void		flip()
		{
			for (size_type i = 0; i < _size; i++)
				_base[i] = !_base[i];	//	works too:	_base[i] ^= true;
		}

		void		clear()
		{
			while (_size != 0)
				pop_back();
		}
	};

	/*	non-member function overloads	*/
	//	relational operators (vector)
	template <class T, class Alloc>
	bool	operator==(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		return (ft::equal(lhs.begin(), lhs.end(), rhs.begin()));
	}

	template <class T, class Alloc>
	bool	operator!=(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs)
	{	return (!(lhs == rhs));	}

	template <class T, class Alloc>
	bool	operator<(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class T, class Alloc>
	bool	operator<=(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs)
	{	return (!(rhs < lhs));	}

	template <class T, class Alloc>
	bool	operator>(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs)
	{	return (rhs < lhs);		}

	template <class T, class Alloc>
	bool	operator>=(const ft::vector<T, Alloc>& lhs, const ft::vector<T, Alloc>& rhs)
	{	return (!(lhs < rhs));	}

	//	swap (vector)
	template <class T, class Alloc>
	void	swap(ft::vector<T, Alloc>& x, ft::vector<T, Alloc>& y)
	{	x.swap(y);	}
}

#endif
