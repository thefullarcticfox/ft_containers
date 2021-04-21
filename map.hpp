#ifndef MAP_HPP
#define MAP_HPP
#include <iterator>				//	std::bidirectional_iterator_tag
#include <utility>				//	std::pair (to compare with std::map)
#include "allocator.hpp"		//	allocator
#include "functional.hpp"		//	binary_function, less, equal_to, pair
#include "reverse_iterator.hpp"	//	reverse_iterator

namespace ft {

	template <class Alloc>
	struct map_base {
	private:
		typedef typename Alloc::value_type	value_type;
		typedef map_base<Alloc>				map_node;
		//	no need to construct without data
		map_base() : parent(NULL), left(NULL), right(NULL), isred(false) {}
	public:
		value_type		data;
		map_node*		parent;
		map_node*		left;
		map_node*		right;
		bool			isred;

		//	i don't use those but i keep them just in case
		map_base(map_node* treenull, const value_type& data) :
		data(data), parent(treenull), left(treenull), right(treenull), isred(true) {}
		map_base(const map_base& x) :
		data(x.data), parent(x.parent), left(x.left), right(x.right), isred(x.isred) {}
		map_base	&operator=(const map_base& x)
		{
			this->data = x.data;
			this->parent = x.parent;
			this->left = x.left;
			this->right = x.right;
			this->isred = x.isred;
			return (*this);
		}
		~map_base() {}

		template<class NodePtr>
		static NodePtr	get_min(NodePtr node, const map_node* _null)
		{
			while (node != _null && node->left != _null)
				node = node->left;
			return (node);
		}

		template<class NodePtr>
		static NodePtr	get_max(NodePtr node, const map_node* _null)
		{
			while (node != _null && node->right != _null)
				node = node->right;
			return (node);
		}

		template<class NodePtr>
		static void		increment(NodePtr& _node, const map_node* _null)
		{
			if (_node->right != _null)		//	going down right
				_node = get_min(_node->right, _null);
			else if (_node == _null || _null->left == _node)
				_node = _node->right;		//	last node or _null case
			else							//	going up
			{
				while (_node->parent != _null && _node == _node->parent->right)
					_node = _node->parent;
				_node = _node->parent;
			}
		}

		template<class NodePtr>
		static void		decrement(NodePtr& _node, const map_node* _null)
		{
			if (_node->left != _null)
				_node = get_max(_node->left, _null);
			else if (_node == _null || _null->right == _node)
				_node = _node->left;
			else
			{
				while (_node->parent != _null && _node == _node->parent->left)
					_node = _node->parent;
				_node = _node->parent;
			}
		}
	};

	template <class Alloc>
	class map_iterator {
	private:
		typedef map_base<Alloc>					map_node;
		map_node*			_node;
		const map_node*		_null;
	public:
		typedef std::bidirectional_iterator_tag		iterator_category;
		typedef typename Alloc::difference_type		difference_type;
		typedef typename Alloc::value_type			value_type;
		typedef typename Alloc::pointer				pointer;
		typedef typename Alloc::reference			reference;

		/*	default				*/
		map_iterator() : _node(NULL), _null(NULL) {}

		/*	construct from ptr	*/
		explicit
		map_iterator(map_node* x, const map_node* tnull) : _node(x), _null(tnull) {}

		/*	copy constructor	*/
		map_iterator(const map_iterator& x) : _node(x._node), _null(x._null) {}

		~map_iterator() {}

		map_iterator	operator=(const map_iterator& x)
		{
			this->_node = x._node;
			this->_null = x._null;
			return (*this);
		}

		//	becomes nonmember as i add friend
		//	needed in case i want to compare const and nonconst iterators
		friend
		bool	operator==(const map_iterator& x, const map_iterator& y)
		{ return (x._node == y._node && x._null == y._null); };
		friend
		bool	operator!=(const map_iterator& x, const map_iterator& y)
		{ return (x._node != y._node || x._null != y._null); };

		reference			operator*() const { return (_node->data); }
		pointer				operator->() const { return (&(_node->data)); }

		map_iterator&		operator++()
		{
			map_node::increment(_node, _null);
			return (*this);
		}

		map_iterator		operator++(int)
		{
			map_iterator	tmp = *this;
			++(*this);
			return (tmp);
		}

		map_iterator&		operator--()
		{
			map_node::decrement(_node, _null);
			return (*this);
		}

		map_iterator		operator--(int)
		{
			map_iterator	tmp = *this;
			--(*this);
			return (tmp);
		}

		map_node*			base() const { return (this->_node); }
		const map_node*		null() const { return (this->_null); }
	};

	template <class Alloc>
	class map_const_iterator {
	private:
		typedef const map_base<Alloc>				map_node;
		typedef map_iterator<Alloc>					iterator;
		const map_node*		_node;
		const map_node*		_null;
	public:
		typedef std::bidirectional_iterator_tag		iterator_category;
		typedef typename Alloc::difference_type		difference_type;
		typedef typename Alloc::value_type			value_type;
		typedef typename Alloc::const_pointer		pointer;
		typedef typename Alloc::const_reference		reference;

		/*	default				*/
		map_const_iterator() : _node(NULL), _null(NULL) {}

		/*	construct from ptr	*/
		explicit
		map_const_iterator(const map_node* x, const map_node* tnull) : _node(x), _null(tnull) {}

		/*	copy constructor 	*/
		map_const_iterator(const map_const_iterator& x) : _node(x._node), _null(x._null) {}

		/*	copy constructor from nonconst iterator	*/
		map_const_iterator(const iterator& x) : _node(x.base()), _null(x.null()) {}

		~map_const_iterator() {}

		map_const_iterator	operator=(const map_const_iterator& x)
		{
			this->_node = x._node;
			this->_null = x._null;
			return (*this);
		}

		map_const_iterator	operator=(const iterator& x)
		{
			this->_node = x.base();
			this->_null = x.null();
			return (*this);
		}

		//	becomes nonmember as i add friend
		//	needed in case i want to compare const and nonconst iterators
		friend
		bool	operator==(const map_const_iterator& x, const map_const_iterator& y)
		{ return (x._node == y._node && x._null == y._null); };
		friend
		bool	operator!=(const map_const_iterator& x, const map_const_iterator& y)
		{ return (x._node != y._node || x._null != y._null); };

		reference				operator*() const { return (_node->data); }
		pointer					operator->() const { return (&(_node->data)); }

		map_const_iterator&		operator++()
		{
			map_node::increment(_node, _null);
			return (*this);
		}

		map_const_iterator		operator++(int)
		{
			map_const_iterator	tmp = *this;
			++(*this);
			return (tmp);
		}

		map_const_iterator&		operator--()
		{
			map_node::decrement(_node, _null);
			return (*this);
		}

		map_const_iterator		operator--(int)
		{
			map_const_iterator	tmp = *this;
			--(*this);
			return (tmp);
		}

		const map_node*		base() const { return (this->_node); }
		const map_node*		null() const { return (this->_null); }
	};

	template <
		class Key,											// map::key_type
		class T,											// map::mapped_type
		class Compare = less<Key>,							// map::key_compare
		class Alloc = allocator< std::pair<const Key, T> >	// map::allocator_type
		>
	class map {
	public:
	    // types:
		typedef Key										key_type;
		typedef T										mapped_type;
		typedef std::pair<const key_type, mapped_type>	value_type;
		typedef Compare									key_compare;
		typedef Alloc									allocator_type;
		typedef typename Alloc::reference				reference;
		typedef typename Alloc::const_reference			const_reference;
		typedef typename Alloc::pointer					pointer;
		typedef typename Alloc::const_pointer			const_pointer;
		typedef typename Alloc::difference_type			difference_type;
		typedef typename Alloc::size_type				size_type;

		//	a bidirectional iterator to value_type
		typedef map_iterator<Alloc>						iterator;
		//	a bidirectional iterator to const value_type
		typedef map_const_iterator<Alloc>				const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

		//	C++98 requires to inherit binary_function<value_type, value_type, bool>
	    class value_compare : public binary_function<value_type, value_type, bool> {
		private:
			friend class	map;
		protected:
			key_compare		comp;
			value_compare(key_compare c) : comp(c) {}
		public:
			typedef bool		result_type;
			typedef value_type	first_argument_type;
			typedef value_type	second_argument_type;
			bool	operator()(value_type const &x, value_type const &y) const
			{	return (comp(x.first, y.first));	}
		};

	private:
		typedef map_base<Alloc>				map_node;
		typedef typename Alloc::template rebind<map_node>::other
											node_allocator_type;
		typedef std::pair<iterator, bool>	map_insres;

		node_allocator_type	_alloc;
		value_compare		_comp;
		size_type			_size;
		map_node*			_root;		//	tree root
		map_node*			_null;		//	sentinel (tree null) and also end

		map_node*	_map_node_create(const value_type& val)
		{
			allocator_type	data_alloc(_alloc);
			map_node*		node = _alloc.allocate(1);
			node->isred = true;
			node->parent = _null;
			node->right = _null;
			node->left = _null;
			data_alloc.construct(&(node->data), val);

		//	this could have many more actions (better keep object copies to minimum)
		//	_alloc.construct(node, map_node(_null, val));
			return (node);
		}

		void		_map_node_destroy(map_node* node)
		{
			_alloc.destroy(node);
			_alloc.deallocate(node, 1);
		}

		void		_map_init_null()
		{
			_null = _alloc.allocate(1);
			_null->isred = false;
			_null->parent = _null;
			_null->right = _null;		//	is min map element
			_null->left = _null;		//	is max map element
			_root = _null;
		}

		void		_map_node_rotate_left(map_node* node)
		{
			if (node == _null || node->right == _null)
				return ;

			map_node*	tmp = node->right;		//	saving the node to the right

			node->right = tmp->left;			//	turn tmp’s left subtree
			if (node->right != _null)			//	into node’s right subtree
				node->right->parent = node;

			tmp->parent = node->parent;			//	node's parent is now tmp's parent
			if (tmp->parent == _null)
				_root = tmp;					//	if node was root => new root
			else if (tmp->parent->left == node)	//	else
				tmp->parent->left = tmp;		//	connect tmp with parent of node
			else
				tmp->parent->right = tmp;

			tmp->left = node;					//	this node becomes left
			node->parent = tmp;					//	it's parent updated
		}

		void		_map_node_rotate_right(map_node* node)
		{
			if (node == _null || node->left == _null)
				return ;

			map_node*	tmp = node->left;		//	saving the node to the left

			node->left = tmp->right;			//	turn tmp’s right subtree
			if (node->left != _null)			//	into node’s left subtree
				node->left->parent = node;

			tmp->parent = node->parent;			//	node's parent is now tmp's parent
			if (tmp->parent == _null)
				_root = tmp;					//	if node was root => new root
			else if (tmp->parent->left == node)	//	else
				tmp->parent->left = tmp;		//	connect tmp with parent of node
			else
				tmp->parent->right = tmp;

			tmp->right = node;					//	this node becomes right
			node->parent = tmp;					//	it's parent updated
		}

		map_node*	_map_find_by_key(const key_type& key) const
		{
			map_node*	pos = _root;
			while (pos != _null && pos->data.first != key)
				if (_comp.comp(key, pos->data.first))
					pos = pos->left;
				else
					pos = pos->right;
			return (pos);
		}

		map_node*	_map_find_insert_point(const value_type& val)
		{
			map_node*	pos = _root;
			while (pos != _null && pos->data.first != val.first)
			{
				if (_comp(val, pos->data))
				{
					if (pos->left == _null)
						return (pos);
					pos = pos->left;
				}
				else
				{
					if (pos->right == _null)
						return (pos);
					pos = pos->right;
				}
			}
			return (pos);
		}

		void		_map_insert_fix_recolor(map_node*& node, map_node* uncle)
		{
			uncle->isred = false;
			node->parent->isred = false;
			node->parent->parent->isred = true;
			node = node->parent->parent;
		}

		void		_map_insert_fix(map_node* node)
		{
			map_node*	uncle;
			while (node != _root && node->parent->isred)
				if (node->parent == node->parent->parent->left)
				{
					uncle = node->parent->parent->right;
					if (uncle->isred)	//	case 1: uncle is red. node stays red
						_map_insert_fix_recolor(node, uncle);
					else
					{
						if (node == node->parent->right)
						{				//	case 2: uncle is black and node is right child
							node = node->parent;
							_map_node_rotate_left(node);
						}				//	case 3: uncle is black and node is left child
						node->parent->isred = false;
						node->parent->parent->isred = true;
						_map_node_rotate_right(node->parent->parent);
					}
				}
				else					//	symmetrical case
				{
					uncle = node->parent->parent->left;
					if (uncle->isred)
						_map_insert_fix_recolor(node, uncle);
					else
					{
						if (node == node->parent->left)
						{
							node = node->parent;
							_map_node_rotate_right(node);
						}
						node->parent->isred = false;
						node->parent->parent->isred = true;
						_map_node_rotate_left(node->parent->parent);
					}
				}
			_root->isred = false;
		}

		map_insres	_map_insert(const value_type& val, iterator hint = iterator())
		{
			map_node*	found = NULL;

			//	optimizes insertion time if hint points to the element that
			//	will precede the inserted element, so *hint < val < *(++hint)
			if (hint.base() != NULL && hint.base() != _null && _comp(*hint, val))
			{
				iterator	next = hint; ++next;
				if ((_null->left == hint.base() ||	//	if hint is rightmost node
					(next.base() != _null &&
					_comp(val, *next))) &&			//	or val is less than hint's successor
					hint.base()->right == _null)	//	and only if right node is free
					found = hint.base();			//	so we can instantly insert
			}

			if (found == NULL)						//	finding insert point if no good hint
				found = _map_find_insert_point(val);

			map_insres	res;							//	insert result is pair<it, bool>
			if (found != _null && found->data.first == val.first)
			{
				res.first = iterator(found, _null);		//	iterator returned
				res.second = false;						//	bool for not inserted
			}
			else
			{
				//	creating element with _null on left and right and red color
				//	parent is always created as _null so i set it later
				map_node*	node = _map_node_create(val);
				if (found == _null)
				{
					_root = node;
					_null->right = node;
					_null->left = node;
				}
				else if (_comp(val, found->data))
				{
					found->left = node;
					if (_null->right == found)	//	if inserted node less than min
						_null->right = node;
				}
				else
				{
					found->right = node;
					if (_null->left == found)	//	if inserted node greater than max
						_null->left = node;
				}
				node->parent = found;
				res.first = iterator(node, _null);
				res.second = true;
				++_size;

				//	fixing violations here
				_map_insert_fix(node);
				_null->parent = _null;
			}

			return (res);
		}

		void	_map_node_transplant(map_node* node1, map_node* node2)
		{
			if (node1->parent == _null)
				_root = node2;
			else if (node1->parent->left == node1)
				node1->parent->left = node2;
			else
				node1->parent->right = node2;
			node2->parent = node1->parent;
			//	it's okay even if any or both of them are _null
		}

		/*	consider that node is double black so we keep black heights (prop 5)
			by doing *this* fix we're making sure to get rid of that double black
			so we don't break the rule about being single red or black (prop 1)	*/
		void	_map_delete_fix(map_node* node)
		{
			map_node*	sibl;
			//	in while loop node is always nonroot double black
			while (node != _root && node->isred == false)
				if (node == node->parent->left)
				{
					sibl = node->parent->right;
					if (sibl->isred)
					{	//	case 1: sibling is red
						sibl->isred = false;
						node->parent->isred = true;
						_map_node_rotate_left(node->parent);
						sibl = node->parent->right;
					}
					if (sibl->left->isred == false && sibl->right->isred == false)
					{	//	case 2: sibling is black and it's children are black
						sibl->isred = true;
						node = node->parent;
					}
					else	//	cases 3-4: one or more of sibling's children are red
					{
						if (sibl->right->isred == false)
						{	//	case 3: sibling is black and it's right child is black
							sibl->left->isred = false;
							sibl->isred = true;
							_map_node_rotate_right(sibl);
							sibl = node->parent->right;
						}	//	case 4: sibling is black and it's right child is red
						sibl->isred = node->parent->isred;
						node->parent->isred = false;
						sibl->right->isred = false;
						_map_node_rotate_left(node->parent);
						node = _root;
					}
				}
				else	//	symmetrical case: sibling is to the left from node
				{
					sibl = node->parent->left;
					if (sibl->isred)
					{
						sibl->isred = false;
						node->parent->isred = true;
						_map_node_rotate_right(node->parent);
						sibl = node->parent->left;
					}
					if (sibl->left->isred == false && sibl->right->isred == false)
					{
						sibl->isred = true;
						node = node->parent;
					}
					else
					{
						if (sibl->left->isred == false)
						{
							sibl->right->isred = false;
							sibl->isred = true;
							_map_node_rotate_left(sibl);
							sibl = node->parent->left;
						}
						sibl->isred = node->parent->isred;
						node->parent->isred = false;
						sibl->left->isred = false;
						_map_node_rotate_right(node->parent);
						node = _root;
					}
				}
			node->isred = false;
		}

		void	_map_delete_node(map_node* node)
		{
			if (node == _null)
				return ;

			if (node == _null->right)	//	if node was min
				map_node::increment(_null->right, _null);
			if (node == _null->left)	//	if node was max
				map_node::decrement(_null->left, _null);

			//	saving min and max just in case
			map_node*	tmpnullleft = _null->left;
			map_node*	tmpnullright = _null->right;
			_null->left = _null;
			_null->right = _null;

			map_node*	node_child;
			//	this is the node that will take node's or sccsr's place
			bool		orig_isred = node->isred;
			if (node->left == _null)
			{
				node_child = node->right;
				_map_node_transplant(node, node_child);
			}
			else if (node->right == _null)
			{
				node_child = node->left;
				_map_node_transplant(node, node_child);
			}
			else
			{	//	find successor to replace deleted node with
				map_node*	sccsr = map_node::get_min(node->right, _null);
				orig_isred = sccsr->isred;
				node_child = sccsr->right;
				if (sccsr->parent == node)
					node_child->parent = sccsr;
				else
				{
					_map_node_transplant(sccsr, sccsr->right);
					sccsr->right = node->right;
					sccsr->right->parent = sccsr;
				}	//	even if sccsr->right is _null we return by parent to sccsr
				_map_node_transplant(node, sccsr);
				sccsr->left = node->left;
				sccsr->left->parent = sccsr;
				sccsr->isred = node->isred;
			}

			if (orig_isred == false)
				_map_delete_fix(node_child);

			//	restoring min and max on _null
			_null->left = tmpnullleft;
			_null->right = tmpnullright;
			_null->parent = _null;

			_map_node_destroy(node);
			--_size;
		}

		template <class InputIterator>
		void		_map_range_init(InputIterator first, InputIterator last)
		{
			_map_init_null();
			while (first != last)
				_map_insert(*first++);
		}

	public:
		//	default constructor (1)
		explicit
		map(const key_compare& comp = key_compare(),
			const allocator_type& alloc = allocator_type()) : _alloc(alloc), _comp(comp), _size(0)
		{
			_map_init_null();
		}

		//	range constructor (2)
		template <class InputIterator>
		map (InputIterator first, InputIterator last,
			const key_compare& comp = key_compare(),
			const allocator_type& alloc = allocator_type()) : _alloc(alloc), _comp(comp), _size(0)
		{
			_map_range_init(first, last);
		}

		//	copy constructor (3)
		map(const map& x) : _alloc(x._alloc), _comp(x._comp), _size(0)
		{
			_map_range_init(x.begin(), x.end());
		}

		//	destructor
		~map()
		{
			clear();
			_alloc.deallocate(_null, 1);
		}

		//	copy (1)
		map&	operator=(const map& x)
		{
			if (this == &x)
				return (*this);
			clear();
			insert(x.begin(), x.end());
			return (*this);
		}

		//	iterators:
		iterator				begin()
		{
			return (iterator(this->_null->right, this->_null));
		}

		const_iterator			begin() const
		{
			return (const_iterator(this->_null->right, this->_null));
		}

		iterator				end()
		{
			return (iterator(this->_null, this->_null));
		}

		const_iterator			end() const
		{
			return (const_iterator(this->_null, this->_null));
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
		bool					empty() const
		{
			return (this->_size == 0);
		}

		size_type				size() const
		{
			return (this->_size);
		}

		size_type				max_size() const
		{
			return (this->_alloc.max_size());
		}

		//	element access:
		mapped_type&			operator[](const key_type& k)
		{
			return ((*((this->insert(value_type(k, mapped_type()))).first)).second);
		}

		//	modifiers:
		std::pair<iterator, bool>	insert(const value_type& val)
		{
			return (_map_insert(val));
		}

		iterator					insert(iterator position, const value_type& val)
		{
			map_insres	res = _map_insert(val, position);
			return (res.first);
		}

		template <class InputIterator>
		void						insert(InputIterator first, InputIterator last)
		{
			while (first != last)
				_map_insert(*first++);
		}

		void		erase(iterator position)
		{
			_map_delete_node(position.base());
		}

		size_type	erase(const key_type& k)
		{
			size_type	oldsz = _size;
			_map_delete_node(_map_find_by_key(k));
			return (oldsz - _size);
		}

		void		erase(iterator first, iterator last)
		{
			iterator	tmp;
			while (first != last)
			{
				tmp = first++;
				erase(tmp);
			}
		}

		void		swap(map& x)
		{
			size_type	swapsize = this->_size;
			map_node*	swapnull = this->_null;
			map_node*	swaproot = this->_root;

			this->_size = x._size;
			this->_null = x._null;
			this->_root = x._root;

			x._size = swapsize;
			x._null = swapnull;
			x._root = swaproot;
		}

		void		clear()
		{
			while (_size != 0)
				_map_delete_node(_null->left);
		}

		//	observers:
		key_compare		key_comp() const
		{
			return (this->_comp.comp);
		}

		value_compare	value_comp() const
		{
			return (this->_comp);
		}

		//	operations:
		iterator		find(const key_type& k)
		{
			return (iterator(_map_find_by_key(k), _null));
		}

		const_iterator	find(const key_type& k) const
		{
			return (const_iterator(_map_find_by_key(k), _null));
		}

		size_type		count(const key_type& k) const
		{
			if (find(k) != end())
				return (1);
			return (0);
		}

		iterator		lower_bound(const key_type& k)
		{
			iterator		it;
			for (it = begin(); it != end(); ++it)
				if (!(_comp.comp(it->first, k)))
					break ;
			return (it);
		}

		const_iterator	lower_bound(const key_type& k) const
		{
			const_iterator	it;
			for (it = begin(); it != end(); ++it)
				if (!(_comp.comp(it->first, k)))
					break ;
			return (it);
		}

		iterator		upper_bound(const key_type& k)
		{
			iterator		it;
			for (it = begin(); it != end(); ++it)
				if (_comp.comp(k, it->first))
					break ;
			return (it);
		}

		const_iterator	upper_bound(const key_type& k) const
		{
			const_iterator	it;
			for (it = begin(); it != end(); ++it)
				if (_comp.comp(k, it->first))
					break ;
			return (it);
		}

		std::pair<iterator, iterator>				equal_range(const key_type& k)
		{
			return (std::pair<iterator, iterator>(lower_bound(k), upper_bound(k)));
		}

		std::pair<const_iterator, const_iterator>	equal_range(const key_type& k) const
		{
			return (std::pair<const_iterator, const_iterator>(lower_bound(k), upper_bound(k)));
		}
	};

	/*	non-member function overloads	*/
	//	relational operators (map)
	template <class Key, class T, class Compare, class Alloc>
	bool	operator==(const ft::map<Key, T, Compare, Alloc>& lhs,
						const ft::map<Key, T, Compare, Alloc>& rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		typename ft::map<Key, T, Compare, Alloc>::const_iterator	it1 = lhs.begin();
		typename ft::map<Key, T, Compare, Alloc>::const_iterator	it2 = rhs.begin();
		while (it1 != lhs.end())
		{
			if (!(*it1 == *it2))
				return (false);
			++it1;	++it2;
		}
		return (true);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator!=(const ft::map<Key, T, Compare, Alloc>& lhs,
						const ft::map<Key, T, Compare, Alloc>& rhs)
	{	return (!(lhs == rhs));	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator<(const ft::map<Key, T, Compare, Alloc>& lhs,
						const ft::map<Key, T, Compare, Alloc>& rhs)
	{
		typename ft::map<Key, T, Compare, Alloc>::const_iterator	it1 = lhs.begin();
		typename ft::map<Key, T, Compare, Alloc>::const_iterator	it2 = rhs.begin();

		while (it1 != lhs.end())
		{
			if (it2 == rhs.end() || *it2 < *it1)
				return (false);
			else if (*it1 < *it2)
				return (true);
			++it1;	++it2;
		}
		return (it2 != rhs.end());
	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator<=(const ft::map<Key, T, Compare, Alloc>& lhs,
						const ft::map<Key, T, Compare, Alloc>& rhs)
	{	return (!(rhs < lhs));	}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator>(const ft::map<Key, T, Compare, Alloc>& lhs,
						const ft::map<Key, T, Compare, Alloc>& rhs)
	{	return (rhs < lhs);		}

	template <class Key, class T, class Compare, class Alloc>
	bool	operator>=(const ft::map<Key, T, Compare, Alloc>& lhs,
						const ft::map<Key, T, Compare, Alloc>& rhs)
	{	return (!(lhs < rhs));	}

	//	swap (map)
	template <class Key, class T, class Compare, class Alloc>
	void	swap(ft::map<Key, T, Compare, Alloc>& x,
				ft::map<Key, T, Compare, Alloc>& y)
	{	x.swap(y);	}
}

#endif
