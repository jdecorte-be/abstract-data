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

        bool operator()(const ft::pair<const Key, Value>& lhs, const ft::pair<const Key, Value>& rhs) const
        {
            return comp(lhs.first, rhs.first);
        }

        bool operator()(const ft::pair<const Key, Value>& lhs, const Key& rhs) const
        {
            return comp(lhs.first, rhs);
        }

        bool operator()(const Key& lhs, const ft::pair<const Key, Value>& rhs) const
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
        const T& operator()(const T& data) const { return data; }
    };

    template <class Key, class Value>
    struct getKey<ft::pair<const Key, Value> >
    {
        const Key& operator()(const ft::pair<const Key, Value>& data) const { return data.first; }
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



node_type* insert_node(const T& val)
{
    node_type* newnode = new_node(val);
    
    if (root == NULL)
    {
        root = newnode;
        root->color = false; // Root is always black
        return root;
    }

    node_type* parent = NULL;
    node_type* current = root;

    // Ignore hint and always find the correct position
    while (current != NULL)
    {
        parent = current;
        if (_comp(getKey<T>()(val), getKey<T>()(current->data)))
            current = current->left;
        else if (_comp(getKey<T>()(current->data), getKey<T>()(val)))
            current = current->right;
        else
        {
            // Key already exists, don't insert
            _alloc.destroy(newnode);
            _alloc.deallocate(newnode, 1);
            return current;
        }
    }

    // Insert new node
    newnode->parent = parent;
    if (_comp(getKey<T>()(val), getKey<T>()(parent->data)))
        parent->left = newnode;
    else
        parent->right = newnode;

    // Fix the tree
    insert_fixup(newnode);

    return newnode;
}

        void delete_node(node_type *ptr)
        {
            node_type *tmp = ptr;
            node_type *x = NULL;

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

    public:
        node_type *new_node(const T &val)
        {
            node_type *newnode = _alloc.allocate(1);
            _alloc.construct(newnode, val);
            return newnode;
        }

        void left_rotate(node_type *x)
        {
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
            while (z->parent != NULL && z->parent->color == true)
            {
                if (z->parent == z->parent->parent->left)
                {
                    node_type *y = z->parent->parent->right;
                    if (y != NULL && y->color == true)
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
                else
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
            node_type *w;
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
                    if ((w->left == NULL || w->left->color == false) && (w->right == NULL || w->right->color == false))
                    { // Case 2
                        w->color = true;
                        x = x->parent;
                    }
                    else
                    {
                        if (w->right == NULL || w->right->color == false)
                        { // Case 3
                            if (w->left)
                                w->left->color = false;
                            w->color = true;
                            right_rotate(w);
                            w = x->parent->right;
                        }
                        // Case 4
                        w->color = x->parent->color;
                        x->parent->color = false;
                        if (w->right)
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
                    if ((w->right == NULL || w->right->color == false) && (w->left == NULL || w->left->color == false))
                    {
                        w->color = true;
                        x = x->parent;
                    }
                    else
                    {
                        if (w->left == NULL || w->left->color == false)
                        {
                            if (w->right)
                                w->right->color = false;
                            w->color = true;
                            left_rotate(w);
                            w = x->parent->left;
                        }
                        w->color = x->parent->color;
                        x->parent->color = false;
                        if (w->left)
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
