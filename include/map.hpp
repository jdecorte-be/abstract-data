#ifndef MAP_HPP
#define MAP_HPP

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include "exception.hpp"
#include "type_traits.hpp"
#include "string.hpp"
#include "tree.hpp"

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
		//
		// Constructor
		//
		explicit map(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
			: tree(comp, alloc) {}

		template <class InputIt>
		map(InputIt first, InputIt last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
			: tree(comp, alloc)
		{
			insert(first, last);
		}

		map(const map &other) : tree(other.tree._comp, other.tree._alloc)
		{
			try
			{
				// Clear any existing elements (should be empty, but just in case)
				clear();

				// Copy all elements from the other map
				const_iterator it = other.begin();
				const_iterator ite = other.end();
				for (; it != ite; ++it)
				{
					insert(*it);
				}
			}
			catch (...)
			{
				// If an exception occurs, ensure we clean up
				clear();
				throw;
			}
		}

		~map() { clear(); }

		map &operator=(const map &other)
		{
			if (this != &other)
			{
				map tmp(other);
				this->swap(tmp);
			}
			return *this;
		}

		//
		// Iterators
		//
		iterator begin()
		{
			return iterator(tree.left_most(tree.root));
		}

		const_iterator begin() const
		{
			return const_iterator(tree.left_most(tree.root));
		}

		iterator end()
		{
			return iterator(NULL, tree.right_most(tree.root));
		}

		const_iterator end() const
		{
			return const_iterator(NULL, tree.right_most(tree.root));
		}

		//
		// Reverse Iterators
		//
		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		//
		// Capacity
		//
		bool empty() const
		{
			return begin() == end();
		}

		size_type size() const
		{
			size_type n = 0;
			for (const_iterator it = begin(); it != end(); ++it)
				n++;
			return n;
		}

		size_type max_size() const
		{
			return tree._alloc.max_size();
		}

		//
		// Element access
		//
		mapped_type &at(const key_type &key)
		{
			iterator it = find(key);
			if (it == end())
				throw std::out_of_range("map : ");
			return it->second;
		}

		const mapped_type &at(const key_type &key) const
		{
			const_iterator it = find(key);
			if (it == end())
				throw std::out_of_range("map : ");
			return it->second;
		}

		mapped_type &operator[](const key_type &k)
		{
			iterator it = find(k);
			if (it == end())
				it = insert(ft::make_pair(k, mapped_type())).first;
			return it->second;
		}

		//
		// Modifiers
		//
		void clear()
		{
			if (tree.root)
			{
				delete_tree(tree.root);
				tree.root = NULL;
			}
		}

		void swap(map &x)
		{
			ft::swap(tree.root, x.tree.root);
			ft::swap(tree._comp, x.tree._comp);
			ft::swap(tree._alloc, x.tree._alloc);
		}

		//
		// Modifiers Insert
		//
		ft::pair<iterator, bool> insert(const value_type &val)
		{
			iterator ite = find(val.first);
			if (ite != end())
				return ft::make_pair(ite, false);
			return ft::make_pair(iterator(tree.insert_node(val)), true);
		}

		iterator insert(iterator hint, const value_type &val)
		{
			if(hint != end() && tree._comp(hint->first, val.first))
				return tree.insert_node(hint.base(), val);
			return insert(val).first;
		}

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			for (InputIterator it = first; it != last; ++it)
			{
        		iterator ite = find(it->first);
				if(ite == end())
					tree.insert_node(*it);
			}
		}

		//
		// Modifiers Erase
		//
		void erase(iterator pos)
		{
			if (pos != end())
				tree.delete_node(pos.base());
		}

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

		void erase(iterator first, iterator last)
		{
			// if (!(first == last || (first != end() && last != end())))
        		// return;

			while (first != last)
				tree.delete_node((first++).base());
		}

		//
		// Lookup
		//
		iterator find(const key_type &k)
		{
			return iterator(find_node(k));
		}

		const_iterator find(const key_type &k) const
		{
			return const_iterator(find_node(k));
		}

		size_type count(const key_type &k) const
		{
			return find(k) != end();
		}

		ft::pair<iterator, iterator> equal_range(const key_type &key)
		{
			return ft::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
		}

		ft::pair<const_iterator, const_iterator> equal_range(const key_type &key) const
		{
			return ft::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
		}

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

		//
		// Observers
		//
		key_compare key_comp() const
		{
			return tree._comp;
		}

		value_compare value_comp() const
		{
			return value_compare(tree._comp);
		}

	private:
		bt_node<value_type> *find_node(const key_type &key) const
		{
			return find_node(tree.root, key);
		}

		bt_node<value_type> *find_node(bt_node<value_type> *r, const key_type &key) const
		{
			if (!r || equal(r->data.first, key))
				return r;
			else if (tree._comp(key, r->data.first))
				return find_node(r->left, key);
			return find_node(r->right, key);
		}

		bool equal(const key_type &lhs, const key_type &rhs) const
		{
			return !tree._comp(lhs, rhs) && !tree._comp(rhs, lhs);
		}

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

	//
	// Non-member functions (operators)
	//
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

	template <class Key, class T, class Compare, class Alloc>
	bool operator!=(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return !(lhs == rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<=(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return !(ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator>(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator>=(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return !(ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

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
		//
		// Constructor
		//
		explicit multimap(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
			: tree(comp, alloc) {}

		template <class InputIt>
		multimap(InputIt first, InputIt last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
			: tree(comp, alloc)
		{
			insert(first, last);
		}

		multimap(const multimap &other) : tree(other.tree._comp, other.tree._alloc)
		{
			try
			{
				clear();
				insert(other.begin(), other.end());
			}
			catch (...)
			{
				clear();
				throw;
			}
		}

		~multimap() { clear(); }

		multimap &operator=(const multimap &other)
		{
			if (this != &other)
			{
				multimap tmp(other);
				this->swap(tmp);
			}
			return *this;
		}

		//
		// Iterators
		//
		iterator begin()
		{
			return iterator(tree.left_most(tree.root));
		}

		const_iterator begin() const
		{
			return const_iterator(tree.left_most(tree.root));
		}

		iterator end()
		{
			return iterator(NULL, tree.right_most(tree.root));
		}

		const_iterator end() const
		{
			return const_iterator(NULL, tree.right_most(tree.root));
		}

		//
		// Reverse Iterators
		//
		reverse_iterator rbegin()
		{
			return reverse_iterator(end());
		}

		const_reverse_iterator rbegin() const
		{
			return const_reverse_iterator(end());
		}

		reverse_iterator rend()
		{
			return reverse_iterator(begin());
		}

		const_reverse_iterator rend() const
		{
			return const_reverse_iterator(begin());
		}

		//
		// Capacity
		//
		bool empty() const
		{
			return begin() == end();
		}

		size_type size() const
		{
			size_type n = 0;
			for (const_iterator it = begin(); it != end(); ++it)
				n++;
			return n;
		}

		size_type max_size() const
		{
			return tree._alloc.max_size();
		}

		//
		// Element access
		//
		mapped_type &at(const key_type &key)
		{
			iterator it = find(key);
			if (it == end())
				throw std::out_of_range("multimap : ");
			return it->second;
		}

		const mapped_type &at(const key_type &key) const
		{
			const_iterator it = find(key);
			if (it == end())
				throw std::out_of_range("multimap : ");
			return it->second;
		}

		mapped_type &operator[](const key_type &k)
		{
			iterator it = find(k);
			if (it == end())
				it = insert(ft::make_pair(k, mapped_type())).first;
			return it->second;
		}

		//
		// Modifiers
		//
		void clear()
		{
			if (tree.root)
			{
				delete_tree(tree.root);
				tree.root = NULL;
			}
		}

		void swap(multimap &x)
		{
			ft::swap(tree.root, x.tree.root);
			ft::swap(tree._comp, x.tree._comp);
			ft::swap(tree._alloc, x.tree._alloc);
		}

		//
		// Modifiers Insert
		//
		iterator insert(const value_type &val)
		{
			return iterator(tree.insert_node(val));
		}

		iterator insert(iterator hint, const value_type &val)
		{
			if (hint != end() && !tree._comp(val.first, hint->first))
				return tree.insert_node(hint.base(), val);
			return insert(val);
		}

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			for (InputIterator it = first; it != last; ++it)
				tree.insert_node(*it);
		}

		//
		// Modifiers Erase
		//
		void erase(iterator pos)
		{
			if (pos != end())
				tree.delete_node(pos.base());
		}

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

		void erase(iterator first, iterator last)
		{
			while (first != last)
				erase(first++);
		}

		//
		// Lookup
		//
		iterator find(const key_type &k)
		{
			return iterator(find_node(k));
		}

		const_iterator find(const key_type &k) const
		{
			return const_iterator(find_node(k));
		}

		size_type count(const key_type &k) const
		{
			return find(k) != end();
		}

		ft::pair<iterator, iterator> equal_range(const key_type &key)
		{
			return ft::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
		}

		ft::pair<const_iterator, const_iterator> equal_range(const key_type &key) const
		{
			return ft::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
		}

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

		//
		// Observers
		//
		key_compare key_comp() const
		{
			return tree._comp;
		}

		value_compare value_comp() const
		{
			return value_compare(tree._comp);
		}

	private:
		bt_node<value_type> *find_node(const key_type &key) const
		{
			return find_node(tree.root, key);
		}

		bt_node<value_type> *find_node(bt_node<value_type> *r, const key_type &key) const
		{
			if (!r || equal(r->data.first, key))
				return r;
			else if (tree._comp(key, r->data.first))
				return find_node(r->left, key);
			return find_node(r->right, key);
		}

		bool equal(const key_type &lhs, const key_type &rhs) const
		{
			return !tree._comp(lhs, rhs) && !tree._comp(rhs, lhs);
		}

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

	//
	// Non-member functions (operators)
	//
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

	template <class Key, class T, class Compare, class Alloc>
	bool operator!=(const ft::multimap<Key, T, Compare, Alloc> &lhs, const ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		return !(lhs == rhs);
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<(const ft::multimap<Key, T, Compare, Alloc> &lhs, const ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		return ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator<=(const ft::multimap<Key, T, Compare, Alloc> &lhs, const ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		return !(ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator>(const ft::multimap<Key, T, Compare, Alloc> &lhs, const ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		return ft::lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
	}

	template <class Key, class T, class Compare, class Alloc>
	bool operator>=(const ft::multimap<Key, T, Compare, Alloc> &lhs, const ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		return !(ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template <class Key, class T, class Compare, class Alloc>
	void swap(ft::multimap<Key, T, Compare, Alloc> &lhs, ft::multimap<Key, T, Compare, Alloc> &rhs)
	{
		lhs.swap(rhs);
	}

} // namespace ft

#endif
