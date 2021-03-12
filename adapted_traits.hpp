#ifndef ADAPTED_TRAITS_HPP
#define ADAPTED_TRAITS_HPP
#include <iterator>		//	std::random_access_iterator_tag

namespace ft {
	/*	adapted type traits to fix fill/range constuctors	*/
	struct type_false	{};
	struct type_true	{};
	template <class T>
				struct type_is_integer                     : public type_false {};
	template <>	struct type_is_integer<bool>               : public type_true {};
	template <>	struct type_is_integer<char>               : public type_true {};
	template <>	struct type_is_integer<signed char>        : public type_true {};
	template <>	struct type_is_integer<unsigned char>      : public type_true {};
	template <>	struct type_is_integer<wchar_t>            : public type_true {};
	#if (defined(__APPLE__) && !defined(_LIBCPP_NO_HAS_CHAR8_T)) || defined(_GLIBCXX_USE_CHAR8_T)
	template <>	struct type_is_integer<char8_t>            : public type_true {};
	#endif
	#if (defined(__APPLE__) && !defined(_LIBCPP_HAS_NO_UNICODE_CHARS)) || (__cplusplus >= 201103L)
	template <>	struct type_is_integer<char16_t>           : public type_true {};
	template <>	struct type_is_integer<char32_t>           : public type_true {};
	#endif
	template <>	struct type_is_integer<short>              : public type_true {};
	template <>	struct type_is_integer<unsigned short>     : public type_true {};
	template <>	struct type_is_integer<int>                : public type_true {};
	template <>	struct type_is_integer<unsigned int>       : public type_true {};
	template <>	struct type_is_integer<long>               : public type_true {};
	template <>	struct type_is_integer<unsigned long>      : public type_true {};
	template <>	struct type_is_integer<long long>          : public type_true {};
	template <>	struct type_is_integer<unsigned long long> : public type_true {};
	#if (defined(__APPLE__) && !defined(_LIBCPP_HAS_NO_INT128)) || defined(_GLIBCXX_USE_INT128)
	template <>	struct type_is_integer<__int128_t>         : public type_true {};
	template <>	struct type_is_integer<__uint128_t>        : public type_true {};
	#endif
	/*	instead of:
		#ifdef __APPLE__
		typedef std::true_type			type_true;
		typedef std::false_type			type_false
		typedef typename std::is_integral<InputIterator>			Integral;
		#else
		typedef std::__true_type		type_true;
		typedef std::__false_type		type_false;
		typedef typename std::__is_integer<InputIterator>::__type	Integral;
		#endif
	*/

	/*	simplified type check to fix random access iterator conversion	*/
	template <class, class>
	struct type_check_if_same	{};
	template <class Same>
	struct type_check_if_same<Same, Same>	{ typedef Same _type; };

	/*	adapted iterator traits in case someone says it's use of stl	*/
	template <class Iterator>
	class iterator_traits {
	public:
		typedef typename Iterator::difference_type		difference_type;
		typedef typename Iterator::value_type			value_type;
		typedef typename Iterator::pointer				pointer;
		typedef typename Iterator::reference			reference;
		typedef typename Iterator::iterator_category	iterator_category;
	};

	template <class T>
	class iterator_traits<T*> {
	public:
		typedef std::ptrdiff_t	difference_type;
		typedef T				value_type;
		typedef T*				pointer;
		typedef T&				reference;
		typedef std::random_access_iterator_tag	iterator_category;
	};

	template <class T>
	class iterator_traits<const T*> {
	public:
		typedef std::ptrdiff_t	difference_type;
		typedef T				value_type;
		typedef const T*		pointer;
		typedef const T&		reference;
		typedef std::random_access_iterator_tag	iterator_category;
	};
}

#endif
