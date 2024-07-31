#ifndef MAP_HPP
#define MAP_HPP

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include "exception.hpp"
#include "type_traits.hpp"
#include "__tree.hpp"

namespace ft
{

	template <class Key, class T, class Compare = ft::less<Key>, class Alloc = std::allocator<ft::pair<const Key, T> > >
	class map
	{

	public:
		//
		// Member types
		//
		typedef Key key_type;
		typedef T mapped_type;
		typedef ft::pair<const key_type, mapped_type> value_type;
		typedef Compare key_compare;
		typedef Alloc allocator_type;
		typedef typename Alloc::template rebind<bt_node<value_type> >::other node_allocator_type; // ?
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef ft::map_iterator<Key, T, bt_node<value_type> > iterator;
		typedef ft::map_iterator<Key, T, const bt_node<value_type> > const_iterator;
		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef typename iterator::difference_type difference_type;
		typedef typename iterator::size_type size_type;

		//
		// Value comp (nested class)
		//
		class value_compare
		{
			friend class map;

		protected:
			key_compare comp;
			value_compare(key_compare c) : comp(c) {}

		public:
			typedef value_type first_argument_type;
			typedef value_type second_argument_type;
			bool operator()(const first_argument_type &x, const second_argument_type &y) const { return comp(x.first, y.first); }
		};

	private:
		RedBlackTree<value_type, Compare, allocator_type> tree;

	public:
		/**
		 * Constructs an empty map with the specified comparison object and allocator.
		 * 
		 * @param comp Comparison object.
		 * @param alloc Allocator object.
		 * 
		 * @complexity O(1)
		 */
		explicit map(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
			: tree(comp, alloc) {}

		/**
		 * Constructs a map with elements from range [first, last) with the specified comparison object and allocator.
		 * 
		 * @param first Iterator to the beginning of the range.
		 * @param last Iterator to the end of the range.
		 * @param comp Comparison object.
		 * @param alloc Allocator object.
		 * 
		 * @complexity O(n log n)
		 */
		template <class InputIt>
		map(InputIt first, InputIt last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
			: tree(comp, alloc)
		{
			insert(first, last);
		}

		/**
		 * Copy constructor. Constructs a map with a copy of each of the elements in other.
		 * 
		 * @param other The map to copy.
		 * 
		 * @throws Any exception thrown by the allocator.
		 * @complexity O(n log n)
		 */
		map(const map &other) : tree(other.tree._comp, other.tree._alloc)
		{
			clear();

			const_iterator it = other.begin();
			const_iterator ite = other.end();
			for (; it != ite; ++it)
			{
				insert(*it);
			}
		}

		/**
		 * Destructor. Destroys the map.
		 * 
		 * @complexity O(n)
		 */
		~map() { clear(); }

		/**
		 * Copy assignment operator. Replaces the contents with a copy of the contents of other.
		 * 
		 * @param other The map to copy.
		 * @return *this
		 * 
		 * @complexity O(n log n)
		 */
		map &operator=(const map &other)
		{
			if (this != &other)
			{
				map tmp(other);
				this->swap(tmp);
			}
			return *this;
		}

		/**
		 * Returns an iterator to the first element.
		 * 
		 * @return Iterator to the first element.
		 * 
		 * @complexity O(log n)
		 */
		iterator begin()
		{
			return iterator(tree.left_most(tree.root));
		}

		/**
		 * Returns a const iterator to the first element.
		 * 
		 * @return Const iterator to the first element.
		 * 
		 * @complexity O(log n)
		 */
		const_iterator begin() const
		{
			return const_iterator(tree.left_most(tree.root));
		}

		/**
		 * Returns an iterator to the element following the last element.
		 * 
		 * @return Iterator to the element following the last element.
		 * 
		 * @complexity O(1)
		 */
		iterator end()
		{
			return iterator(NULL, tree.right_most(tree.root));
		}

		/**
		 * Returns a const iterator to the element following the last element.
		 * 
		 * @return Const iterator to the element following the last element.
		 * 
		 * @complexity O(1)
		 */
		const_iterator end() const
		{
			return const_iterator(NULL, tree.right_most(tree.root));
		}

		/**
		 * Returns a reverse iterator to the first element of the reversed map.
		 * 
		 * @return Reverse iterator to the first element.
		 * 
		 * @complexity O(1)
		 */
		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		/**
		 * Returns a const reverse iterator to the first element of the reversed map.
		 * 
		 * @return Const reverse iterator to the first element.
		 * 
		 * @complexity O(1)
		 */
		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}

		/**
		 * Returns a reverse iterator to the element following the last element of the reversed map.
		 * 
		 * @return Reverse iterator to the element following the last element.
		 * 
		 * @complexity O(1)
		 */
		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		/**
		 * Returns a const reverse iterator to the element following the last element of the reversed map.
		 * 
		 * @return Const reverse iterator to the element following the last element.
		 * 
		 * @complexity O(1)
		 */
		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		/**
		 * Checks if the map is empty.
		 * 
		 * @return true if the map is empty, false otherwise.
		 * 
		 * @complexity O(1)
		 */
		bool empty() const
		{
			return begin() == end();
		}

		/**
		 * Returns the number of elements in the map.
		 * 
		 * @return Number of elements in the map.
		 * 
		 * @complexity O(n)
		 */
		size_type size() const
		{
			size_type n = 0;
			for (const_iterator it = begin(); it != end(); ++it)
				n++;
			return n;
		}

		/**
		 * Returns the maximum number of elements the map can hold.
		 * 
		 * @return Maximum number of elements.
		 * 
		 * @complexity O(1)
		 */
		size_type max_size() const
		{
			return tree._alloc.max_size();
		}

		/**
		 * Accesses the specified element with bounds checking.
		 * 
		 * @param key Key of the element to find.
		 * @return Reference to the mapped value.
		 * 
		 * @throws ft::out_of_range if the key is not found.
		 * @complexity O(log n)
		 */
		mapped_type &at(const key_type &key)
		{
			iterator it = find(key);
			if (it == end())
				throw ft::out_of_range("map : ");
			return it->second;
		}

		/**
		 * Accesses the specified element with bounds checking.
		 * 
		 * @param key Key of the element to find.
		 * @return Const reference to the mapped value.
		 * 
		 * @throws ft::out_of_range if the key is not found.
		 * @complexity O(log n)
		 */
		const mapped_type &at(const key_type &key) const
		{
			const_iterator it = find(key);
			if (it == end())
				throw ft::out_of_range("map : ");
			return it->second;
		}

		/**
		 * Accesses the specified element.
		 * 
		 * @param k Key of the element to find.
		 * @return Reference to the mapped value.
		 * 
		 * @complexity O(log n)
		 */
		mapped_type &operator[](const key_type &k)
		{
			iterator it = find(k);
			if (it == end())
				it = insert(ft::make_pair(k, mapped_type())).first;
			return it->second;
		}

		/**
		 * Clears the contents of the map.
		 * 
		 * @complexity O(n)
		 */
		void clear()
		{
			if (tree.root)
			{
				delete_tree(tree.root);
				tree.root = NULL;
			}
		}

		/**
		 * Swaps the contents with another map.
		 * 
		 * @param x Map to swap with.
		 * 
		 * @complexity O(1)
		 */
		void swap(map &x)
		{
			ft::swap(tree.root, x.tree.root);
			ft::swap(tree._comp, x.tree._comp);
			ft::swap(tree._alloc, x.tree._alloc);
		}

		/**
		 * Inserts a value into the map.
		 * 
		 * @param val Value to insert.
		 * @return Pair consisting of an iterator to the inserted element (or to the element that prevented the insertion) and a bool denoting whether the insertion took place.
		 * 
		 * @complexity O(log n)
		 */
		ft::pair<iterator, bool> insert(const value_type &val)
		{
			iterator ite = find(val.first);
			if (ite != end())
				return ft::make_pair(ite, false);
			return ft::make_pair(iterator(tree.insert_node(val)), true);
		}

		/**
		 * Inserts a value into the map with a hint.
		 * 
		 * @param hint Iterator to the position where the value should be inserted.
		 * @param val Value to insert.
		 * @return Iterator to the inserted element.
		 * 
		 * @complexity O(log n)
		 */
		iterator insert(iterator hint, const value_type &val)
		{
			if (hint != end() && tree._comp(hint->first, val.first))
				return tree.insert_node(hint.base(), val);
			return insert(val).first;
		}

		/**
		 * Inserts elements from range [first, last) into the map.
		 * 
		 * @param first Iterator to the beginning of the range.
		 * @param last Iterator to the end of the range.
		 * 
		 * @complexity O(n log n)
		 */
		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			for (InputIterator it = first; it != last; ++it)
			{
				iterator ite = find(it->first);
				if (ite == end())
					tree.insert_node(*it);
			}
		}

		/**
		 * Erases the element at pos.
		 * 
		 * @param pos Iterator to the element to remove.
		 * 
		 * @complexity O(log n)
		 */
		void erase(iterator pos)
		{
			if (pos != end())
				tree.delete_node(pos.base());
		}

		/**
		 * Erases the element with the specified key.
		 * 
		 * @param k Key of the element to remove.
		 * @return Number of elements removed.
		 * 
		 * @complexity O(log n)
		 */
		size_type erase(const key_type &k)
		{
			iterator it = find(k);
			if (it != end())
			{
				erase(it++);
				return 1;
			}
			return 0;
		}

		/**
		 * Erases elements in the range [first, last).
		 * 
		 * @param first Iterator to the beginning of the range.
		 * @param last Iterator to the end of the range.
		 * 
		 * @complexity O(n log n)
		 */
		void erase(iterator first, iterator last)
		{
			while (first != last)
				tree.delete_node((first++).base());
		}

		/**
		 * Finds an element with the specified key.
		 * 
		 * @param k Key of the element to find.
		 * @return Iterator to the found element, or end() if the element is not found.
		 * 
		 * @complexity O(log n)
		 */
		iterator find(const key_type &k)
		{
			return iterator(find_node(k));
		}

		/**
		 * Finds an element with the specified key.
		 * 
		 * @param k Key of the element to find.
		 * @return Const iterator to the found element, or end() if the element is not found.
		 * 
		 * @complexity O(log n)
		 */
		const_iterator find(const key_type &k) const
		{
			return const_iterator(find_node(k));
		}

		/**
		 * Returns the number of elements with the specified key.
		 * 
		 * @param k Key of the element to count.
		 * @return Number of elements with the specified key.
		 * 
		 * @complexity O(log n)
		 */
		size_type count(const key_type &k) const
		{
			return find(k) != end();
		}

		/**
		 * Returns the range of elements matching the specified key.
		 * 
		 * @param key Key of the element to find.
		 * @return Pair of iterators to the range of elements matching the key.
		 * 
		 * @complexity O(log n)
		 */
		ft::pair<iterator, iterator> equal_range(const key_type &key)
		{
			return ft::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
		}

		/**
		 * Returns the range of elements matching the specified key.
		 * 
		 * @param key Key of the element to find.
		 * @return Pair of const iterators to the range of elements matching the key.
		 * 
		 * @complexity O(log n)
		 */
		ft::pair<const_iterator, const_iterator> equal_range(const key_type &key) const
		{
			return ft::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
		}

		/**
		 * Returns an iterator to the first element not less than the specified key.
		 * 
		 * @param key Key of the element to find.
		 * @return Iterator to the first element not less than the key.
		 * 
		 * @complexity O(log n)
		 */
		iterator lower_bound(const key_type &key)
		{
			iterator it = begin();
			while (it != end())
			{
				if (!tree._comp(it->first, key))
					return it;
				it++;
			}
			return it;
		}

		/**
		 * Returns a const iterator to the first element not less than the specified key.
		 * 
		 * @param key Key of the element to find.
		 * @return Const iterator to the first element not less than the key.
		 * 
		 * @complexity O(log n)
		 */
		const_iterator lower_bound(const key_type &key) const
		{
			const_iterator it = begin();
			while (it != end())
			{
				if (!tree._comp(it->first, key))
					return it;
				it++;
			}
			return it;
		}

		/**
		 * Returns an iterator to the first element greater than the specified key.
		 * 
		 * @param key Key of the element to find.
		 * @return Iterator to the first element greater than the key.
		 * 
		 * @complexity O(log n)
		 */
		iterator upper_bound(const key_type &key)
		{
			iterator it = begin();
			while (it != end())
			{
				if (tree._comp(key, it->first))
					return it;
				it++;
			}
			return it;
		}

		/**
		 * Returns a const iterator to the first element greater than the specified key.
		 * 
		 * @param key Key of the element to find.
		 * @return Const iterator to the first element greater than the key.
		 * 
		 * @complexity O(log n)
		 */
		const_iterator upper_bound(const key_type &key) const
		{
			const_iterator it = begin();
			while (it != end())
			{
				if (tree._comp(key, it->first))
					return it;
				it++;
			}
			return it;
		}

		/**
		 * Returns the comparison object used to order the keys.
		 * 
		 * @return Key comparison object.
		 * 
		 * @complexity O(1)
		 */
		key_compare key_comp() const
		{
			return tree._comp;
		}

		/**
		 * Returns a comparison object that can be used to compare two elements.
		 * 
		 * @return Value comparison object.
		 * 
		 * @complexity O(1)
		 */
		value_compare value_comp() const
		{
			return value_compare(tree._comp);
		}

	private:
		/**
		 * Finds a node with the specified key.
		 * 
		 * @param key Key of the node to find.
		 * @return Pointer to the node.
		 * 
		 * @complexity O(log n)
		 */
		bt_node<value_type> *find_node(const key_type &key) const
		{
			return find_node(tree.root, key);
		}

		/**
		 * Finds a node with the specified key starting from the specified root.
		 * 
		 * @param r Root node.
		 * @param key Key of the node to find.
		 * @return Pointer to the node.
		 * 
		 * @complexity O(log n)
		 */
		bt_node<value_type> *find_node(bt_node<value_type> *r, const key_type &key) const
		{
			if (!r || equal(r->data.first, key))
				return r;
			else if (tree._comp(key, r->data.first))
				return find_node(r->left, key);
			return find_node(r->right, key);
		}

		/**
		 * Checks if two keys are equal.
		 * 
		 * @param lhs Left-hand side key.
		 * @param rhs Right-hand side key.
		 * @return true if the keys are equal, false otherwise.
		 * 
		 * @complexity O(1)
		 */
		bool equal(const key_type &lhs, const key_type &rhs) const
		{
			return !tree._comp(lhs, rhs) && !tree._comp(rhs, lhs);
		}

		/**
		 * Deletes the entire tree starting from the specified node.
		 * 
		 * @param ptr Root node.
		 * 
		 * @complexity O(n)
		 */
		void delete_tree(bt_node<value_type> *ptr)
		{
			if (ptr->left)
				delete_tree(ptr->left);
			if (ptr->right)
				delete_tree(ptr->right);
			tree._alloc.destroy(ptr);
			tree._alloc.deallocate(ptr, 1);
		}
	};

	/**
	 * Checks if two maps are equal.
	 * 
	 * @param lhs Left-hand side map.
	 * @param rhs Right-hand side map.
	 * @return true if the maps are equal, false otherwise.
	 * 
	 * @complexity O(n)
	 */
	template <class Key, class T, class Compare, class Alloc>
	bool operator==(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		if (lhs.size() == rhs.size())
		{
			typename ft::map<Key, T, Compare, Alloc>::const_iterator ite_lhs = lhs.begin();
			typename ft::map<Key, T, Compare, Alloc>::const_iterator ite_rhs = rhs.begin();
			while (ite_lhs != lhs.end())
			{
				if (ite_lhs->first != ite_rhs->first || ite_lhs->second != ite_rhs->second)
					return false;
				ite_lhs++, ite_rhs++;
			}
			return true;
		}
		return false;
	}

	/**
	 * Checks if two maps are not equal.
	 * 
	 * @param lhs Left-hand side map.
	 * @param rhs Right-hand side map.
	 * @return true if the maps are not equal, false otherwise.
	 * 
	 * @complexity O(n)
	 */
	template <class Key, class T, class Compare, class Alloc>
	bool operator!=(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return !(lhs == rhs);
	}

	/**
	 * Checks if one map is less than another.
	 * 
	 * @param lhs Left-hand side map.
	 * @param rhs Right-hand side map.
	 * @return true if lhs is less than rhs, false otherwise.
	 * 
	 * @complexity O(n)
	 */
	template <class Key, class T, class Compare, class Alloc>
	bool operator<(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	/**
	 * Checks if one map is less than or equal to another.
	 * 
	 * @param lhs Left-hand side map.
	 * @param rhs Right-hand side map.
	 * @return true if lhs is less than or equal to rhs, false otherwise.
	 * 
	 * @complexity O(n)
	 */
	template <class Key, class T, class Compare, class Alloc>
	bool operator<=(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return !(ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));
	}

	/**
	 * Checks if one map is greater than another.
	 * 
	 * @param lhs Left-hand side map.
	 * @param rhs Right-hand side map.
	 * @return true if lhs is greater than rhs, false otherwise.
	 * 
	 * @complexity O(n)
	 */
	template <class Key, class T, class Compare, class Alloc>
	bool operator>(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
	}

	/**
	 * Checks if one map is greater than or equal to another.
	 * 
	 * @param lhs Left-hand side map.
	 * @param rhs Right-hand side map.
	 * @return true if lhs is greater than or equal to rhs, false otherwise.
	 * 
	 * @complexity O(n)
	 */
	template <class Key, class T, class Compare, class Alloc>
	bool operator>=(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return !(ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	/**
	 * Swaps the contents of two maps.
	 * 
	 * @param lhs Left-hand side map.
	 * @param rhs Right-hand side map.
	 * 
	 * @complexity O(1)
	 */
	template <class Key, class T, class Compare, class Alloc>
	void swap(ft::map<Key, T, Compare, Alloc> &lhs, ft::map<Key, T, Compare, Alloc> &rhs)
	{
		lhs.swap(rhs);
	}

#pragma region multimap
	// MULTIMAP * ========================================================================================================

	template <class Key, class T, class Compare = ft::less<Key>, class Alloc = std::allocator<ft::pair<const Key, T> > >
	class multimap
	{

	public:
		//
		// Member types
		//
		typedef Key key_type;
		typedef T mapped_type;
		typedef ft::pair<const key_type, mapped_type> value_type;
		typedef Compare key_compare;
		typedef Alloc allocator_type;
		typedef typename allocator_type::reference reference;
		typedef typename allocator_type::const_reference const_reference;
		typedef typename allocator_type::pointer pointer;
		typedef typename allocator_type::const_pointer const_pointer;
		typedef ft::map_iterator<Key, T, bt_node<value_type> > iterator;
		typedef ft::map_iterator<Key, T, const bt_node<value_type> > const_iterator;
		typedef ft::reverse_iterator<iterator> reverse_iterator;
		typedef ft::reverse_iterator<const_iterator> const_reverse_iterator;
		typedef typename iterator::difference_type difference_type;
		typedef typename iterator::size_type size_type;

		//
		// Value comp (nested class)
		//
		class value_compare
		{
			friend class multimap;

		protected:
			key_compare comp;
			value_compare(key_compare c) : comp(c) {}

		public:
			typedef value_type first_argument_type;
			typedef value_type second_argument_type;
			bool operator()(const first_argument_type &x, const second_argument_type &y) const { return comp(x.first, y.first); }
		};

	private:
		RedBlackTree<value_type, Compare, allocator_type> tree;

	public:
		/**
		 * Constructs an empty multimap with the specified comparison object and allocator.
		 * 
		 * @param comp Comparison object.
		 * @param alloc Allocator object.
		 * 
		 * @complexity O(1)
		 */
		explicit multimap(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
			: tree(comp, alloc) {}

		/**
		 * Constructs a multimap with elements from range [first, last) with the specified comparison object and allocator.
		 * 
		 * @param first Iterator to the beginning of the range.
		 * @param last Iterator to the end of the range.
		 * @param comp Comparison object.
		 * @param alloc Allocator object.
		 * 
		 * @complexity O(n log n)
		 */
		template <class InputIt>
		multimap(InputIt first, InputIt last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
			: tree(comp, alloc)
		{
			insert(first, last);
		}

		/**
		 * Copy constructor. Constructs a multimap with a copy of each of the elements in other.
		 * 
		 * @param other The multimap to copy.
		 * 
		 * @throws Any exception thrown by the allocator.
		 * @complexity O(n log n)
		 */
		multimap(const multimap &other) : tree(other.tree._comp, other.tree._alloc)
		{
			clear();
			insert(other.begin(), other.end());
		}

		/**
		 * Destructor. Destroys the multimap.
		 * 
		 * @complexity O(n)
		 */
		~multimap() { clear(); }

		/**
		 * Copy assignment operator. Replaces the contents with a copy of the contents of other.
		 * 
		 * @param other The multimap to copy.
		 * @return *this
		 * 
		 * @complexity O(n log n)
		 */
		multimap &operator=(const multimap &other)
		{
			if (this != &other)
			{
				multimap tmp(other);
				this->swap(tmp);
			}
			return *this;
		}

		/**
		 * Returns an iterator to the first element.
		 * 
		 * @return Iterator to the first element.
		 * 
		 * @complexity O(log n)
		 */
		iterator begin()
		{
			return iterator(tree.left_most(tree.root));
		}

		/**
		 * Returns a const iterator to the first element.
		 * 
		 * @return Const iterator to the first element.
		 * 
		 * @complexity O(log n)
		 */
		const_iterator begin() const
		{
			return const_iterator(tree.left_most(tree.root));
		}

		/**
		 * Returns an iterator to the element following the last element.
		 * 
		 * @return Iterator to the element following the last element.
		 * 
		 * @complexity O(1)
		 */
		iterator end()
		{
			return iterator(NULL, tree.right_most(tree.root));
		}

		/**
		 * Returns a const iterator to the element following the last element.
		 * 
		 * @return Const iterator to the element following the last element.
		 * 
		 * @complexity O(1)
		 */
		const_iterator end() const
		{
			return const_iterator(NULL, tree.right_most(tree.root));
		}

		/**
		 * Returns a reverse iterator to the first element of the reversed multimap.
		 * 
		 * @return Reverse iterator to the first element.
		 * 
		 * @complexity O(1)
		 */
		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		/**
		 * Returns a const reverse iterator to the first element of the reversed multimap.
		 * 
		 * @return Const reverse iterator to the first element.
		 * 
		 * @complexity O(1)
		 */
		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}

		/**
		 * Returns a reverse iterator to the element following the last element of the reversed multimap.
		 * 
		 * @return Reverse iterator to the element following the last element.
		 * 
		 * @complexity O(1)
		 */
		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		/**
		 * Returns a const reverse iterator to the element following the last element of the reversed multimap.
		 * 
		 * @return Const reverse iterator to the element following the last element.
		 * 
		 * @complexity O(1)
		 */
		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		/**
		 * Checks if the multimap is empty.
		 * 
		 * @return true if the multimap is empty, false otherwise.
		 * 
		 * @complexity O(1)
		 */
		bool empty() const
		{
			return begin() == end();
		}

		/**
		 * Returns the number of elements in the multimap.
		 * 
		 * @return Number of elements in the multimap.
		 * 
		 * @complexity O(n)
		 */
		size_type size() const
		{
			size_type n = 0;
			for (const_iterator it = begin(); it != end(); ++it)
				n++;
			return n;
		}

		/**
		 * Returns the maximum number of elements the multimap can hold.
		 * 
		 * @return Maximum number of elements.
		 * 
		 * @complexity O(1)
		 */
		size_type max_size() const
		{
			return tree._alloc.max_size();
		}

		/**
		 * Accesses the specified element with bounds checking.
		 * 
		 * @param key Key of the element to find.
		 * @return Reference to the mapped value.
		 * 
		 * @throws ft::out_of_range if the key is not found.
		 * @complexity O(log n)
		 */
		mapped_type &at(const key_type &key)
		{
			iterator it = find(key);
			if (it == end())
				throw ft::out_of_range("multimap : ");
			return it->second;
		}

		/**
		 * Accesses the specified element with bounds checking.
		 * 
		 * @param key Key of the element to find.
		 * @return Const reference to the mapped value.
		 * 
		 * @throws ft::out_of_range if the key is not found.
		 * @complexity O(log n)
		 */
		const mapped_type &at(const key_type &key) const
		{
			const_iterator it = find(key);
			if (it == end())
				throw ft::out_of_range("multimap : ");
			return it->second;
		}

		/**
		 * Accesses the specified element.
		 * 
		 * @param k Key of the element to find.
		 * @return Reference to the mapped value.
		 * 
		 * @complexity O(log n)
		 */
		mapped_type &operator[](const key_type &k)
		{
			iterator it = find(k);
			if (it == end())
				it = insert(ft::make_pair(k, mapped_type())).first;
			return it->second;
		}

		/**
		 * Clears the contents of the multimap.
		 * 
		 * @complexity O(n)
		 */
		void clear()
		{
			if (tree.root)
			{
				delete_tree(tree.root);
				tree.root = NULL;
			}
		}

		/**
		 * Swaps the contents with another multimap.
		 * 
		 * @param x Multimap to swap with.
		 * 
		 * @complexity O(1)
		 */
		void swap(multimap &x)
		{
			ft::swap(tree.root, x.tree.root);
			ft::swap(tree._comp, x.tree._comp);
			ft::swap(tree._alloc, x.tree._alloc);
		}

		/**
		 * Inserts a value into the multimap.
		 * 
		 * @param val Value to insert.
		 * @return Iterator to the inserted element.
		 * 
		 * @complexity O(log n)
		 */
		iterator insert(const value_type &val)
		{
			return iterator(tree.insert_node(val));
		}

		/**
		 * Inserts a value into the multimap with a hint.
		 * 
		 * @param hint Iterator to the position where the value should be inserted.
		 * @param val Value to insert.
		 * @return Iterator to the inserted element.
		 * 
		 * @complexity O(log n)
		 */
		iterator insert(iterator hint, const value_type &val)
		{
			if (hint != end() && !tree._comp(val.first, hint->first))
				return tree.insert_node(hint.base(), val);
			return insert(val);
		}

		/**
		 * Inserts elements from range [first, last) into the multimap.
		 * 
		 * @param first Iterator to the beginning of the range.
		 * @param last Iterator to the end of the range.
		 * 
		 * @complexity O(n log n)
		 */
		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			for (InputIterator it = first; it != last; ++it)
				tree.insert_node(*it);
		}

		/**
		 * Erases the element at pos.
		 * 
		 * @param pos Iterator to the element to remove.
		 * 
		 * @complexity O(log n)
		 */
		void erase(iterator pos)
		{
			if (pos != end())
				tree.delete_node(pos.base());
		}

		/**
		 * Erases the element with the specified key.
		 * 
		 * @param k Key of the element to remove.
		 * @return Number of elements removed.
		 * 
		 * @complexity O(log n)
		 */
		size_type erase(const key_type &k)
		{
			iterator it = find(k);
			if (it != end())
			{
				erase(it++);
				return 1;
			}
			return 0;
		}

		/**
		 * Erases elements in the range [first, last).
		 * 
		 * @param first Iterator to the beginning of the range.
		 * @param last Iterator to the end of the range.
		 * 
		 * @complexity O(n log n)
		 */
		void erase(iterator first, iterator last)
		{
			while (first != last)
				erase(first++);
		}

		/**
		 * Finds an element with the specified key.
		 * 
		 * @param k Key of the element to find.
		 * @return Iterator to the found element, or end() if the element is not found.
		 * 
		 * @complexity O(log n)
		 */
		iterator find(const key_type &k)
		{
			return iterator(find_node(k));
		}

		/**
		 * Finds an element with the specified key.
		 * 
		 * @param k Key of the element to find.
		 * @return Const iterator to the found element, or end() if the element is not found.
		 * 
		 * @complexity O(log n)
		 */
		const_iterator find(const key_type &k) const
		{
			return const_iterator(find_node(k));
		}

		/**
		 * Returns the number of elements with the specified key.
		 * 
		 * @param k Key of the element to count.
		 * @return Number of elements with the specified key.
		 * 
		 * @complexity O(log n)
		 */
		size_type count(const key_type &k) const
		{
			return find(k) != end();
		}

		/**
		 * Returns the range of elements matching the specified key.
		 * 
		 * @param key Key of the element to find.
		 * @return Pair of iterators to the range of elements matching the key.
		 * 
		 * @complexity O(log n)
		 */
		ft::pair<iterator, iterator> equal_range(const key_type &key)
		{
			return ft::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
		}

		/**
		 * Returns the range of elements matching the specified key.
		 * 
		 * @param key Key of the element to find.
		 * @return Pair of const iterators to the range of elements matching the key.
		 * 
		 * @complexity O(log n)
		 */
		ft::pair<const_iterator, const_iterator> equal_range(const key_type &key) const
		{
			return ft::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
		}

		/**
		 * Returns an iterator to the first element not less than the specified key.
		 * 
		 * @param key Key of the element to find.
		 * @return Iterator to the first element not less than the key.
		 * 
		 * @complexity O(log n)
		 */
		iterator lower_bound(const key_type &key)
		{
			iterator it = begin();
			while (it != end())
			{
				if (!tree._comp(it->first, key))
					return it;
				it++;
			}
			return it;
		}

		/**
		 * Returns a const iterator to the first element not less than the specified key.
		 * 
		 * @param key Key of the element to find.
		 * @return Const iterator to the first element not less than the key.
		 * 
		 * @complexity O(log n)
		 */
		const_iterator lower_bound(const key_type &key) const
		{
			const_iterator it = begin();
			while (it != end())
			{
				if (!tree._comp(it->first, key))
					return it;
				it++;
			}
			return it;
		}

		/**
		 * Returns an iterator to the first element greater than the specified key.
		 * 
		 * @param key Key of the element to find.
		 * @return Iterator to the first element greater than the key.
		 * 
		 * @complexity O(log n)
		 */
		iterator upper_bound(const key_type &key)
		{
			iterator it = begin();
			while (it != end())
			{
				if (tree._comp(key, it->first))
					return it;
				it++;
			}
			return it;
		}

		/**
		 * Returns a const iterator to the first element greater than the specified key.
		 * 
		 * @param key Key of the element to find.
		 * @return Const iterator to the first element greater than the key.
		 * 
		 * @complexity O(log n)
		 */
		const_iterator upper_bound(const key_type &key) const
		{
			const_iterator it = begin();
			while (it != end())
			{
				if (tree._comp(key, it->first))
					return it;
				it++;
			}
			return it;
		}

		/**
		 * Returns the comparison object used to order the keys.
		 * 
		 * @return Key comparison object.
		 * 
		 * @complexity O(1)
		 */
		key_compare key_comp() const
		{
			return tree._comp;
		}

		/**
		 * Returns a comparison object that can be used to compare two elements.
		 * 
		 * @return Value comparison object.
		 * 
		 * @complexity O(1)
		 */
		value_compare value_comp() const
		{
			return value_compare(tree._comp);
		}

	private:
		/**
		 * Finds a node with the specified key.
		 * 
		 * @param key Key of the node to find.
		 * @return Pointer to the node.
		 * 
		 * @complexity O(log n)
		 */
		bt_node<value_type> *find_node(const key_type &key) const
		{
			return find_node(tree.root, key);
		}

		/**
		 * Finds a node with the specified key starting from the specified root.
		 * 
		 * @param r Root node.
		 * @param key Key of the node to find.
		 * @return Pointer to the node.
		 * 
		 * @complexity O(log n)
		 */
		bt_node<value_type> *find_node(bt_node<value_type> *r, const key_type &key) const
		{
			if (!r || equal(r->data.first, key))
				return r;
			else if (tree._comp(key, r->data.first))
				return find_node(r->left, key);
			return find_node(r->right, key);
		}

		/**
		 * Checks if two keys are equal.
		 * 
		 * @param lhs Left-hand side key.
		 * @param rhs Right-hand side key.
		 * @return true if the keys are equal, false otherwise.
		 * 
		 * @complexity O(1)
		 */
		bool equal(const key_type &lhs, const key_type &rhs) const
		{
			return !tree._comp(lhs, rhs) && !tree._comp(rhs, lhs);
		}

		/**
		 * Deletes the entire tree starting from the specified node.
		 * 
		 * @param ptr Root node.
		 * 
		 * @complexity O(n)
		 */
		void delete_tree(bt_node<value_type> *ptr)
		{
			if (ptr->left)
				delete_tree(ptr->left);
			if (ptr->right)
				delete_tree(ptr->right);
			tree._alloc.destroy(ptr);
			tree._alloc.deallocate(ptr, 1);
		}
	};

	/**
	 * Checks if two multimaps are equal.
	 * 
	 * @param lhs Left-hand side multimap.
	 * @param rhs Right-hand side multimap.
	 * @return true if the multimaps are equal, false otherwise.
	 * 
	 * @complexity O(n)
	 */
	template <class Key, class T, class Compare, class Alloc>
	bool operator==(const ft::multimap<Key, T, Compare, Alloc> &lhs, const ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		if (lhs.size() == rhs.size())
		{
			typename ft::multimap<Key, T, Compare, Alloc>::const_iterator ite_lhs = lhs.begin();
			typename ft::multimap<Key, T, Compare, Alloc>::const_iterator ite_rhs = rhs.begin();
			while (ite_lhs != lhs.end())
			{
				if (ite_lhs->first != ite_rhs->first || ite_lhs->second != ite_rhs->second)
					return false;
				ite_lhs++, ite_rhs++;
			}
			return true;
		}
		return false;
	}

	/**
	 * Checks if two multimaps are not equal.
	 * 
	 * @param lhs Left-hand side multimap.
	 * @param rhs Right-hand side multimap.
	 * @return true if the multimaps are not equal, false otherwise.
	 * 
	 * @complexity O(n)
	 */
	template <class Key, class T, class Compare, class Alloc>
	bool operator!=(const ft::multimap<Key, T, Compare, Alloc> &lhs, const ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		return !(lhs == rhs);
	}

	/**
	 * Checks if one multimap is less than another.
	 * 
	 * @param lhs Left-hand side multimap.
	 * @param rhs Right-hand side multimap.
	 * @return true if lhs is less than rhs, false otherwise.
	 * 
	 * @complexity O(n)
	 */
	template <class Key, class T, class Compare, class Alloc>
	bool operator<(const ft::multimap<Key, T, Compare, Alloc> &lhs, const ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	/**
	 * Checks if one multimap is less than or equal to another.
	 * 
	 * @param lhs Left-hand side multimap.
	 * @param rhs Right-hand side multimap.
	 * @return true if lhs is less than or equal to rhs, false otherwise.
	 * 
	 * @complexity O(n)
	 */
	template <class Key, class T, class Compare, class Alloc>
	bool operator<=(const ft::multimap<Key, T, Compare, Alloc> &lhs, const ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		return !(ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));
	}

	/**
	 * Checks if one multimap is greater than another.
	 * 
	 * @param lhs Left-hand side multimap.
	 * @param rhs Right-hand side multimap.
	 * @return true if lhs is greater than rhs, false otherwise.
	 * 
	 * @complexity O(n)
	 */
	template <class Key, class T, class Compare, class Alloc>
	bool operator>(const ft::multimap<Key, T, Compare, Alloc> &lhs, const ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		return ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
	}

	/**
	 * Checks if one multimap is greater than or equal to another.
	 * 
	 * @param lhs Left-hand side multimap.
	 * @param rhs Right-hand side multimap.
	 * @return true if lhs is greater than or equal to rhs, false otherwise.
	 * 
	 * @complexity O(n)
	 */
	template <class Key, class T, class Compare, class Alloc>
	bool operator>=(const ft::multimap<Key, T, Compare, Alloc> &lhs, const ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		return !(ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	/**
	 * Swaps the contents of two multimaps.
	 * 
	 * @param lhs Left-hand side multimap.
	 * @param rhs Right-hand side multimap.
	 * 
	 * @complexity O(1)
	 */
	template <class Key, class T, class Compare, class Alloc>
	void swap(ft::multimap<Key, T, Compare, Alloc> &lhs, ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		lhs.swap(rhs);
	}

} // namespace ft

#endif
