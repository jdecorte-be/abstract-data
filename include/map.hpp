#ifndef MAP_HPP
#define MAP_HPP

#include "utility.hpp"
#include "iterator.hpp"
#include "algorithm.hpp"
#include "exception.hpp"
#include "type_traits.hpp"
#include "string.hpp"

namespace ft
{

	template <class Key, class T, class Compare = ft::less<Key>, class Alloc = std::allocator<ft::pair<const Key, T> > >
	class map
	{
	public:
		//
		// Node
		//
		struct bt_node
		{
			bt_node *left;
			bt_node *right;
			bt_node *parent;

			ft::pair<const Key, T> data;

			bt_node(ft::pair<const Key, T> data) : left(NULL), right(NULL), parent(NULL), data(data) {}
			const Key &getKey() { return data.first; }
			T &getVal() { return data.second; }
		};

		//
		// Member types
		//
	public:
		typedef Key key_type;
		typedef T mapped_type;
		typedef ft::pair<const key_type, mapped_type> value_type;
		typedef Compare key_compare;
		typedef typename Alloc::template rebind<bt_node>::other allocator_type;
		typedef typename Alloc::reference reference;
		typedef typename Alloc::const_reference const_reference;
		typedef typename Alloc::pointer pointer;
		typedef typename Alloc::const_pointer const_pointer;
		typedef ft::map_iterator<Key, T, bt_node> iterator;
		typedef ft::map_iterator<Key, T, const bt_node> const_iterator;
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
		bt_node *root;
		allocator_type _alloc;
		key_compare _comp;

	public:
		//
		// Constructor
		//
		explicit map(const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
			: root(NULL), _alloc(alloc), _comp(comp)
		{
		}

		template <class InputIt>
		map(InputIt first, InputIt last, const key_compare &comp = key_compare(), const allocator_type &alloc = allocator_type())
			: root(NULL), _alloc(alloc), _comp(comp)
		{
			insert(first, last);
		}

		map(const map &other) : root(NULL), _alloc(other._alloc), _comp(other._comp)
		{
			clear();
			insert(other.begin(), other.end());
		}

		~map() { clear(); }

		map &operator=(const map &other)
		{
			if (this == &other)
				return *this;
			clear();
			insert(other.begin(), other.end());
			_comp = other._comp;
			_alloc = other._alloc;
			return *this;
		}

		//
		// Iterators
		//
		iterator begin()
		{
			return iterator(left_most(root));
		}

		const_iterator begin() const
		{
			return const_iterator(left_most(root));
		}

		iterator end()
		{
			return iterator(NULL, right_most(root));
		}

		const_iterator end() const
		{
			return const_iterator(NULL, right_most(root));
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
			return count_node();
		}

		size_type max_size() const
		{
			return _alloc.max_size();
		}

		//
		// Element access
		//
		T &at(const key_type &key)
		{
			iterator it = find(key);
			if (it == end())
				throw std::out_of_range("Map : ");
			return it->second;
		}

		const T &at(const key_type &key) const
		{
			const_iterator it = find(key);
			if (it == end())
				throw std::out_of_range("Map : ");
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
			if (root)
			{
				delete_tree(root);
				root = NULL;
			}
		}

		void swap(map &x)
		{
			std::swap(root, x.root);
			std::swap(_comp, x._comp);
			std::swap(_alloc, x._alloc);
		}

		//
		// Modifiers Insert
		//
		ft::pair<iterator, bool> insert(const value_type &val)
		{
			iterator ite = find(val.first);
			if (ite != end())
				return ft::make_pair(ite, false);
			else
				return ft::make_pair(iterator(insert_node(val)), true);
		}

		iterator insert(iterator position, const value_type &val)
		{
			(void)position;
			return insert(val).first;
		}

		template <class InputIterator>
		void insert(InputIterator first, InputIterator last)
		{
			while (first != last)
				insert(*first++);
		}

		//
		// Modifiers Erase
		//
		void erase(iterator pos)
		{
			if (pos != end())
				delete_node(pos.base());
		}

		size_type erase(const key_type &k)
		{
			iterator it = find(k);
			if (it != end())
			{
				erase(it);
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
			return find(k) != end() ? 1 : 0;
		}

		ft::pair<iterator, iterator> equal_range(const Key &key)
		{
			return ft::pair<iterator, iterator>(lower_bound(key), upper_bound(key));
		}

		ft::pair<const_iterator, const_iterator> equal_range(const Key &key) const
		{
			return ft::pair<const_iterator, const_iterator>(lower_bound(key), upper_bound(key));
		}

		iterator lower_bound(const Key &key)
		{
			iterator it = begin();
			while (it != end())
			{
				if (!(_comp(it->first, key)))
					return it;
				it++;
			}
			return it;
		}

		const_iterator lower_bound(const Key &key) const
		{
			const_iterator it = begin();
			while (it != end())
			{
				if (!(_comp(it->first, key)))
					return it;
				it++;
			}
			return it;
		}

		iterator upper_bound(const Key &key)
		{
			iterator it = begin();
			while (it != end())
			{
				if (_comp(key, it->first))
					return it;
				it++;
			}
			return it;
		}

		const_iterator upper_bound(const Key &key) const
		{
			const_iterator it = begin();
			while (it != end())
			{
				if (_comp(key, it->first))
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
			return _comp;
		}

		value_compare value_comp() const
		{
			return value_compare(_comp);
		}

	private:
		bool equal(const key_type &lhs, const key_type &rhs) const
		{
			return !_comp(lhs, rhs) && !_comp(rhs, lhs);
		}

		bt_node *new_node(const value_type &val)
		{
			bt_node *newnode = _alloc.allocate(1);
			_alloc.construct(newnode, val);
			return newnode;
		}

		bt_node *insert_node(const value_type &val)
		{
			bt_node *newnode = new_node(val);

			if (root == NULL)
				root = newnode;
			else
			{
				bt_node *r = root;
				while (r)
				{
					if (_comp(r->getKey(), val.first))
					{
						if (!r->right)
						{
							r->right = newnode;
							break;
						}
						r = r->right;
					}
					else
					{
						if (!r->left)
						{
							r->left = newnode;
							break;
						}
						r = r->left;
					}
				}
				newnode->parent = r;
			}
			return newnode;
		}

		bt_node *find_node(const Key &key) const
		{
			return find_node(root, key);
		}

		bt_node *find_node(bt_node *r, const Key &key) const
		{
			if (!r || equal(r->getKey(), key))
				return r;
			if (_comp(key, r->getKey()))
				return find_node(r->left, key);
			return find_node(r->right, key);
		}

		void delete_node(bt_node *ptr)
		{
			bt_node *tmp = ptr;
			bt_node *x = NULL;

			if (!ptr->left)
			{
				x = ptr->right;
				transplant(ptr, ptr->right);
			}
			else if (!ptr->right)
			{
				x = ptr->left;
				transplant(ptr, ptr->left);
			}
			else if (ptr->right && ptr->left)
			{
				tmp = left_most(ptr->right);
				x = tmp->right;
				if (tmp->parent == ptr && x)
					x->parent = tmp;
				else
				{
					transplant(tmp, tmp->right);
					tmp->right = ptr->right;
					if (tmp->right)
						tmp->right->parent = tmp;
				}
				transplant(ptr, tmp);
				tmp->left = ptr->left;
				if (tmp->left)
					tmp->left->parent = tmp;
			}
			_alloc.destroy(ptr);
			_alloc.deallocate(ptr, 1);
		}

		void transplant(bt_node *oldNode, bt_node *newNode)
		{
			if (!oldNode->parent)
				root = newNode;
			else if (oldNode == oldNode->parent->left)
				oldNode->parent->left = newNode;
			else
				oldNode->parent->right = newNode;
			if (newNode)
				newNode->parent = oldNode->parent;
		}

		size_type count_node() const
		{
			size_type n = 0;
			for (const_iterator it = begin(); it != end(); it++)
				n++;
			return n;
		}

		bt_node *left_most(bt_node *root) const
		{
			bt_node *r = root;
			if (!r)
				return NULL;
			if (!r->left)
				return r;
			while (r && r->left)
				r = r->left;
			return r;
		}

		bt_node *right_most(bt_node *root) const
		{
			bt_node *r = root;
			if (!r)
				return NULL;
			if (!r->right)
				return r;
			while (r->right)
				r = r->right;
			return r;
		}

		void delete_tree(bt_node *ptr)
		{
			std::allocator<bt_node> allocate;
			if (ptr->left)
				delete_tree(ptr->left);
			if (ptr->right)
				delete_tree(ptr->right);
			allocate.destroy(ptr);
			allocate.deallocate(ptr, 1);
			ptr = NULL;
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
		return lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
	}
	template <class Key, class T, class Compare, class Alloc>
	bool operator<=(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return !(lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end()));
	}
	template <class Key, class T, class Compare, class Alloc>
	bool operator>(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return lexicographical_compare(rhs.begin(), rhs.end(), lhs.begin(), lhs.end());
	}
	template <class Key, class T, class Compare, class Alloc>
	bool operator>=(const ft::map<Key, T, Compare, Alloc> &lhs, const ft::map<Key, T, Compare, Alloc> &rhs)
	{
		return !(lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}
	template <class Key, class T, class Compare, class Alloc>
	void swap(ft::map<Key, T, Compare, Alloc> &lhs, ft::map<Key, T, Compare, Alloc> &rhs)
	{
		lhs.swap(rhs);
	}

}

#endif
