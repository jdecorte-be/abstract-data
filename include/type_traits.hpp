#ifndef TYPE_TRAITS_HPP
#define TYPE_TRAITS_HPP

namespace ft
{
	/**
	 * Defines a type only if the condition is true.
	 * 
	 * @tparam Cond Boolean condition
	 * @tparam T Type to be defined if the condition is true
	 * 
	 * @complexity O(1)
	 */
	template <bool Cond, class T = void>
	struct enable_if
	{
	};

	/**
	 * Specialization of enable_if for true condition.
	 * 
	 * @tparam T Type to be defined
	 * 
	 * @complexity O(1)
	 */
	template <class T>
	struct enable_if<true, T>
	{
		typedef T type;
	};

	/**
	 * Checks if two types are the same.
	 * 
	 * @tparam T First type
	 * @tparam U Second type
	 * @return value True if T and U are the same type, false otherwise
	 * 
	 * @complexity O(1)
	 */
	template <class T, class U>
	struct is_same
	{
		static const bool value = false;
	};

	/**
	 * Specialization of is_same for the same type.
	 * 
	 * @tparam T Type to check
	 * @return value True if T and T are the same type
	 * 
	 * @complexity O(1)
	 */
	template <class T>
	struct is_same<T, T>
	{
		static const bool value = true;
	};

	/**
	 * Checks if a type is const.
	 * 
	 * @tparam T Type to check
	 * @return value True if T is const, false otherwise
	 * 
	 * @complexity O(1)
	 */
	template <class T>
	struct is_const
	{
		static const bool value = false;
	};

	/**
	 * Specialization of is_const for const types.
	 * 
	 * @tparam T Type to check
	 * @return value True if T is const
	 * 
	 * @complexity O(1)
	 */
	template <class T>
	struct is_const<const T>
	{
		static const bool value = true;
	};

	/**
	 * Selects one of two types based on a boolean condition.
	 * 
	 * @tparam B Boolean condition
	 * @tparam T Type if B is true
	 * @tparam U Type if B is false
	 * 
	 * @complexity O(1)
	 */
	template <bool B, class T = void, class U = void>
	struct conditional
	{
	};

	/**
	 * Specialization of conditional for true condition.
	 * 
	 * @tparam T Type to be selected if the condition is true
	 * @tparam U Ignored
	 * @return type The selected type
	 * 
	 * @complexity O(1)
	 */
	template <class T, class U>
	struct conditional<true, T, U>
	{
		typedef T type;
	};

	/**
	 * Specialization of conditional for false condition.
	 * 
	 * @tparam T Ignored
	 * @tparam U Type to be selected if the condition is false
	 * @return type The selected type
	 * 
	 * @complexity O(1)
	 */
	template <class T, class U>
	struct conditional<false, T, U>
	{
		typedef U type;
	};
};

#endif
