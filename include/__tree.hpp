#ifndef __TREE_HPP
#define __TREE_HPP

#include "utility.hpp"

namespace ft
{
    template <class Key, class Value, class Compare = ft::less<Key> >
    struct pair_compare
    {
        Compare comp;
        pair_compare(Compare c = Compare()) : comp(c) {}

        /**
         * Compares two pairs using the key comparison function.
         * 
         * @param lhs Left-hand side pair
         * @param rhs Right-hand side pair
         * @return True if lhs is less than rhs, false otherwise
         * 
         * @complexity O(1)
         */
        bool operator()(const ft::pair<const Key, Value> &lhs, const ft::pair<const Key, Value> &rhs) const
        {
            return comp(lhs.first, rhs.first);
        }

        /**
         * Compares a pair and a key using the key comparison function.
         * 
         * @param lhs Left-hand side pair
         * @param rhs Right-hand side key
         * @return True if lhs is less than rhs, false otherwise
         * 
         * @complexity O(1)
         */
        bool operator()(const ft::pair<const Key, Value> &lhs, const Key &rhs) const
        {
            return comp(lhs.first, rhs);
        }

        /**
         * Compares a key and a pair using the key comparison function.
         * 
         * @param lhs Left-hand side key
         * @param rhs Right-hand side pair
         * @return True if lhs is less than rhs, false otherwise
         * 
         * @complexity O(1)
         */
        bool operator()(const Key &lhs, const ft::pair<const Key, Value> &rhs) const
        {
            return comp(lhs, rhs.first);
        }
    };

    template <class T>
    struct bt_node
    {
        bt_node *left;
        bt_node *right;
        bt_node *parent;

        T data;
        bool color; // true for Red, false for Black

        /**
         * Constructs a binary tree node with the given data.
         * 
         * @param data The data to be stored in the node
         * 
         * @complexity O(1)
         */
        bt_node(const T &data) : left(NULL), right(NULL), parent(NULL), data(data), color(true) {}
    };

    template <class T>
    struct getKey
    {
        /**
         * Returns the key from the data.
         * 
         * @param data The data
         * @return The key
         * 
         * @complexity O(1)
         */
        const T &operator()(const T &data) const { return data; }
    };

    template <class Key, class Value>
    struct getKey<ft::pair<const Key, Value> >
    {
        /**
         * Returns the key from the pair.
         * 
         * @param data The pair
         * @return The key
         * 
         * @complexity O(1)
         */
        const Key &operator()(const ft::pair<const Key, Value> &data) const { return data.first; }
    };

    template <class T, class Compare, class Alloc>
    class RedBlackTree
    {
    public:
        typedef bt_node<T> node_type;
        typedef typename Alloc::template rebind<node_type>::other allocator_type;

        node_type *root;
        allocator_type _alloc;
        Compare _comp;

        /**
         * Constructs an empty Red-Black Tree with the given comparison object and allocator.
         * 
         * @param comp Comparison function object
         * @param alloc Allocator object
         * 
         * @complexity O(1)
         */
        RedBlackTree(const Compare &comp = Compare(), const allocator_type &alloc = allocator_type())
            : root(NULL), _alloc(alloc), _comp(comp) {}

        /**
         * Inserts a value into the tree.
         * 
         * @param val Value to be inserted
         * @return Pointer to the newly inserted node
         * 
         * @complexity O(log n)
         */
        node_type *insert_node(const T &val)
        {
            node_type *y = NULL;
            node_type *x = root;

            while (x != NULL)
            {
                y = x;
                if (_comp(getKey<T>()(val), getKey<T>()(x->data)))
                    x = x->left;
                else
                    x = x->right;
                // Remove the equality check to allow duplicate keys
            }

            node_type *z = _alloc.allocate(1);
            _alloc.construct(z, val);

            z->left = NULL;
            z->right = NULL;

            z->parent = y;
            if (y == NULL)
                root = z;
            else if (_comp(getKey<T>()(val), getKey<T>()(y->data)))
                y->left = z;
            else
                y->right = z;

            z->left = NULL;
            z->right = NULL;
            z->color = true; // Red

            insert_fixup(z);

            return z;
        }

        /**
         * Inserts a value into the tree using a hint for the position.
         * 
         * @param hint Hint for the position where the value should be inserted
         * @param val Value to be inserted
         * @return Pointer to the newly inserted node
         * 
         * @complexity O(log n)
         */
        node_type *insert_node(node_type *hint, const T &val)
        {
            if (hint != NULL)
            {
                // Use the hint if it's in the correct position
                if ((_comp(getKey<T>()(hint->data), getKey<T>()(val)) || 
                     !_comp(getKey<T>()(val), getKey<T>()(hint->data))) && 
                    hint->right == NULL)
                {
                    node_type *z = _alloc.allocate(1);
                    _alloc.construct(z, val);
                    z->parent = hint;
                    hint->right = z;
                    z->left = NULL;
                    z->right = NULL;
                    z->color = true; // Red
                    insert_fixup(z);
                    return z;
                }
            }
            // If hint is not useful, fall back to regular insert
            return insert_node(val);
        }

        /**
         * Deletes a node from the tree.
         * 
         * @param z Pointer to the node to be deleted
         * 
         * @complexity O(log n)
         */
        void delete_node(node_type *z)
        {
            node_type *y = z;
            node_type *x;
            bool y_original_color = y->color;

            if (z->left == NULL)
            {
                x = z->right;
                transplant(z, z->right);
            }
            else if (z->right == NULL)
            {
                x = z->left;
                transplant(z, z->left);
            }
            else
            {
                y = left_most(z->right);
                y_original_color = y->color;
                x = y->right;

                if (y->parent == z)
                {
                    if (x)
                        x->parent = y;
                }
                else
                {
                    transplant(y, y->right);
                    y->right = z->right;
                    y->right->parent = y;
                }

                transplant(z, y);
                y->left = z->left;
                y->left->parent = y;
                y->color = z->color;
            }

            if (y_original_color == false)
                delete_fixup(x);

            _alloc.destroy(z);
            _alloc.deallocate(z, 1);
        }

        /**
         * Creates a new node with the given value.
         * 
         * @param val Value to be stored in the new node
         * @return Pointer to the newly created node
         * 
         * @complexity O(1)
         */
        node_type *new_node(const T &val)
        {
            node_type *newnode = _alloc.allocate(1);
            _alloc.construct(newnode, val);
            return newnode;
        }

        /**
         * Performs a left rotation on the given node.
         * 
         * @param x Pointer to the node to be rotated
         * 
         * @complexity O(1)
         */
        void left_rotate(node_type *x)
        {
            if(x == NULL || x->right == NULL)
                return;
            node_type *y = x->right;
            x->right = y->left;
            if (y->left != NULL)
            {
                y->left->parent = x;
            }
            y->parent = x->parent;
            if (x->parent == NULL)
            {
                root = y;
            }
            else if (x == x->parent->left)
            {
                x->parent->left = y;
            }
            else
            {
                x->parent->right = y;
            }
            y->left = x;
            x->parent = y;
        }

        /**
         * Performs a right rotation on the given node.
         * 
         * @param y Pointer to the node to be rotated
         * 
         * @complexity O(1)
         */
        void right_rotate(node_type *y)
        {
            if(y == NULL || y->left == NULL)
                return;
            node_type *x = y->left;
            y->left = x->right;
            if (x->right != NULL

)
            {
                x->right->parent = y;
            }
            x->parent = y->parent;
            if (y->parent == NULL)
            {
                root = x;
            }
            else if (y == y->parent->right)
            {
                y->parent->right = x;
            }
            else
            {
                y->parent->left = x;
            }
            x->right = y;
            y->parent = x;
        }

        /**
         * Fixes the tree after an insertion to maintain Red-Black properties.
         * 
         * @param z Pointer to the newly inserted node
         * 
         * @complexity O(log n)
         */
        void insert_fixup(node_type *z)
        {
            while (z->parent && z->parent->color == true)
            {
                if (z->parent == z->parent->parent->left)
                {
                    node_type *y = z->parent->parent->right;
                    if (y && y->color == true)
                    {
                        z->parent->color = false;
                        y->color = false;
                        z->parent->parent->color = true;
                        z = z->parent->parent;
                    }
                    else
                    {
                        if (z == z->parent->right)
                        {
                            z = z->parent;
                            left_rotate(z);
                        }
                        z->parent->color = false;
                        z->parent->parent->color = true;
                        right_rotate(z->parent->parent);
                    }
                }
                else // mirror cases
                {
                    node_type *y = z->parent->parent->left;
                    if (y != NULL && y->color == true)
                    {
                        z->parent->color = false;
                        y->color = false;
                        z->parent->parent->color = true;
                        z = z->parent->parent;
                    }
                    else
                    {
                        if (z == z->parent->left)
                        {
                            z = z->parent;
                            right_rotate(z);
                        }
                        z->parent->color = false;
                        z->parent->parent->color = true;
                        left_rotate(z->parent->parent);
                    }
                }
            }
            root->color = false;
        }

        /**
         * Fixes the tree after a deletion to maintain Red-Black properties.
         * 
         * @param x Pointer to the node that replaces the deleted node
         * 
         * @complexity O(log n)
         */
        void delete_fixup(node_type *x)
        {
            node_type *w = NULL;
            while (x && x != root && x->color == false)
            {
                if (x == x->parent->left)
                {
                    w = x->parent->right;
                    if (w && w->color == true)
                    { // Case 1
                        w->color = false;
                        x->parent->color = true;
                        left_rotate(x->parent);
                        w = x->parent->right;
                    }
                    if (w && (w->left == NULL || w->left->color == false) && (w->right == NULL || w->right->color == false))
                    { // Case 2
                        w->color = true;
                        x = x->parent;
                    }
                    else
                    {
                        if (w && (w->right == NULL || w->right->color == false))
                        { // Case 3
                            if (w->left)
                                w->left->color = false;
                            w->color = true;
                            right_rotate(w);
                            w = x->parent->right;
                        }
                        // Case 4
                        if(w)
                            w->color = x->parent->color;
                        x->parent->color = false;
                        if (w && w->right)
                            w->right->color = false;
                        left_rotate(x->parent);
                        x = root; // This will break the loop
                    }
                }
                else
                { // Mirror cases
                    w = x->parent->left;
                    if (w && w->color == true)
                    {
                        w->color = false;
                        x->parent->color = true;
                        right_rotate(x->parent);
                        w = x->parent->left;
                    }
                    if (w && (w->right == NULL || w->right->color == false) && (w->left == NULL || w->left->color == false))
                    {
                        w->color = true;
                        x = x->parent;
                    }
                    else
                    {
                        if (w && (w->left == NULL || w->left->color == false))
                        {
                            if (w->right)
                                w->right->color = false;
                            w->color = true;
                            left_rotate(w);
                            w = x->parent->left;
                        }
                        if(w)
                            w->color = x->parent->color;
                        x->parent->color = false;
                        if (w && w->left)
                            w->left->color = false;
                        right_rotate(x->parent);
                        x = root; // This will break the loop
                    }
                }
            }
            if (x)
                x->color = false; // Ensure the root is always black
        }

        /**
         * Replaces one subtree with another.
         * 
         * @param oldNode Pointer to the subtree to be replaced
         * @param newNode Pointer to the subtree that will replace the old one
         * 
         * @complexity O(1)
         */
        void transplant(node_type *oldNode, node_type *newNode)
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

        /**
         * Returns the left-most node (minimum) starting from the given root.
         * 
         * @param root Pointer to the root node
         * @return Pointer to the left-most node
         * 
         * @complexity O(log n)
         */
        node_type *left_most(node_type *root) const
        {
            node_type *r = root;
            if (!r)
                return NULL;
            while (r->left)
                r = r->left;
            return r;
        }

        /**
         * Returns the right-most node (maximum) starting from the given root.
         * 
         * @param root Pointer to the root node
         * @return Pointer to the right-most node
         * 
         * @complexity O(log n)
         */
        node_type *right_most(node_type *root) const
        {
            node_type *r = root;
            if (!r)
                return NULL;
            while (r->right)
                r = r->right;
            return r;
        }

        /**
         * Checks if two values are equal.
         * 
         * @param lhs Left-hand side value
         * @param rhs Right-hand side value
         * @return True if the values are equal, false otherwise
         * 
         * @complexity O(1)
         */
        bool equal(const T &lhs, const T &rhs) const
        {
            return !_comp(lhs, rhs) && !_comp(rhs, lhs);
        }

        /**
         * Finds a node with the given key.
         * 
         * @param key Key to search for
         * @return Pointer to the node with the given key, or NULL if not found
         * 
         * @complexity O(log n)
         */
        node_type *find_node(const T &key) const
        {
            return find_node(root, key);
        }

        /**
         * Finds a node with the given key starting from a specific root.
         * 
         * @param r Pointer to the root node to start the search from
         * @param key Key to search for
         * @return Pointer to the node with the given key, or NULL if not found
         * 
         * @complexity O(log n)
         */
        node_type *find_node(node_type *r, const T &key) const
        {
            if (!r || equal(getKey<T>()(r->data), key))
                return r;
            if (_comp(key, getKey<T>()(r->data)))
                return find_node(r->left, key);
            return find_node(r->right, key);
        }
    };

} // namespace ft

#endif