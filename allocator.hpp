#ifndef ALLOCATOR_HPP
#define ALLOCATOR_HPP
#include <exception>	//	std::bad_alloc
#include <limits>		//	std::numeric_limits
#include <new>			//	::operator new, ::operator delete

namespace ft {
	/*	allocator works just like std::allocator	*/
	template <class T>
	class allocator {
	public:
		typedef T				value_type;
		typedef T*				pointer;
		typedef T&				reference;
		typedef const T*		const_pointer;
		typedef const T&		const_reference;
		typedef std::size_t		size_type;
		typedef std::ptrdiff_t	difference_type;

		template <class Type>
		struct rebind { typedef allocator<Type>	other; };

		allocator() throw() {}
		allocator(const allocator& alloc) throw() { (void)alloc; }
		template <class U>
		allocator(const allocator<U>& alloc) throw() { (void)alloc; }
		~allocator() throw() {}

		pointer			address(reference x) const { return (&x); }
		const_pointer	address(const_reference x) const { return (&x); }

		pointer			allocate(size_type n, const void* hint = 0)
		{
			(void)hint;
			if (n > this->max_size())
				throw std::bad_alloc();
			return (reinterpret_cast<pointer>(::operator new(n * sizeof(value_type))));
		}

		void			deallocate(pointer p, size_type n)
		{	::operator delete(p);	(void)n;						}

		void			construct(pointer p, const_reference val)
		{	::new (reinterpret_cast<void*>(p)) value_type(val);		}

		void			destroy(pointer p)
		{	p->~value_type();										}

		size_type		max_size() const throw()
		{
			#if defined(__APPLE__) || defined(_WIN32)
			return (std::numeric_limits<size_type>::max() / sizeof(value_type));
			#else
			return (static_cast<size_type>(std::numeric_limits<difference_type>::max()) / sizeof(value_type));
			#endif
		}
	};

	template <>
	class allocator<void> {
	public:
		typedef void*			pointer;
		typedef const void*		const_pointer;
		typedef void			value_type;

		template <class Type>
		struct rebind { typedef allocator<Type>	other; };
	};
}

#endif
