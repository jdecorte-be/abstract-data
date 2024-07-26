#ifndef TREE_HPP
#define TREE_HPP

#include "utility.hpp"
#include <memory>

namespace ft
{
    template <class Key, class Value, class Compare = std::less<Key> >
    struct pair_compare
    {
        Compare comp;
        pair_compare(Compare c = Compare()) : comp(c) {}

        bool operator()(const ft::pair<const Key, Value> &lhs, const ft::pair<const Key, Value> &rhs) const
        {
            return comp(lhs.first, rhs.first);
        }

        bool operator()(const ft::pair<const Key, Value> &lhs, const Key &rhs) const
        {
            return comp(lhs.first, rhs);
        }

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

        bt_node(const T &data) : left(NULL), right(NULL), parent(NULL), data(data), color(true) {}
    };

    template <class T>
    struct getKey
    {
        const T &operator()(const T &data) const { return data; }
    };

    template <class Key, class Value>
    struct getKey<ft::pair<const Key, Value> >
    {
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

        RedBlackTree(const Compare &comp = Compare(), const allocator_type &alloc = allocator_type())
            : root(NULL), _alloc(alloc), _comp(comp) {}



        
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

    public:
        node_type *new_node(const T &val)
        {
            node_type *newnode = _alloc.allocate(1);
            _alloc.construct(newnode, val);
            return newnode;
        }

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

        void right_rotate(node_type *y)
        {
            if(y == NULL || y->left == NULL)
                return;
            node_type *x = y->left;
            y->left = x->right;
            if (x->right != NULL)
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
                else // mirro cases
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
                    if (w \
                    && (w->left == NULL || \
                     w->left->color == false) \
                     && (w->right == NULL \
                     || w->right->color == false))
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

        node_type *left_most(node_type *root) const
        {
            node_type *r = root;
            if (!r)
                return NULL;
            while (r->left)
                r = r->left;
            return r;
        }

        node_type *right_most(node_type *root) const
        {
            node_type *r = root;
            if (!r)
                return NULL;
            while (r->right)
                r = r->right;
            return r;
        }

        bool equal(const T &lhs, const T &rhs) const
        {
            return !_comp(lhs, rhs) && !_comp(rhs, lhs);
        }

        node_type *find_node(const T &key) const
        {
            return find_node(root, key);
        }

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
