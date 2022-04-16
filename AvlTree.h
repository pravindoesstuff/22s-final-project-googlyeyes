/**
 * @Author(s):      Pravin and Kassi
 * @filename:       AvlTree.h
 * @date:           04-11-2022
 * @description:    Implementation of an AVL tree
 */

#ifndef INC_22S_FINAL_PROJ_AVLTREE_H
#define INC_22S_FINAL_PROJ_AVLTREE_H

#include <algorithm>

template<typename K, typename V>
class AvlTree {
private:
    //AvlNode class declaration
    class AvlNode {
    public:
        K key;
        std::vector<V> values;
        AvlNode *left;
        AvlNode *right;
        int height;

        explicit AvlNode(const K &key, AvlNode *lt = nullptr, AvlNode *rt = nullptr,
                         int h = 0) : key(key),
                                      left(lt),
                                      right(rt),
                                      height(h) {}
    };

    /// \description    -> Return the node's height.
    int height(AvlNode *&node) { return node != nullptr ? node->height : -1; }

    /// \description    -> Inserts a new node into the AVL tree
    void insert_node(const K &key, const V &value, AvlNode *&node);

    /// \description    -> Search node in the AVL tree
    std::vector<V> *search_node(const K &key, AvlNode *&node);

    /// \param node     -> A node in the AVL tree
    /// \description    -> Internal function responsible for emptying a node subtrees
    void make_empty(AvlNode *&node);

    /// \param node     -> A node in the AVL tree
    /// \description    -> responsible for cloning subtree
    AvlNode *clone(AvlNode *node) const;

    /// \param alpha     -> Node of imbalance
    /// \description    -> Performs "case 1" rotation
    void rotate_with_left_child(AvlNode *&alpha);


    /// \param alpha     -> Node of imbalance
    /// \description    -> Performs "case 2" rotation
    void double_with_left_child(AvlNode *&alpha);

    /// \param alpha     -> Node of imbalance
    /// \description    -> Performs "case 3" rotation
    void double_with_right_child(AvlNode *&alpha);

    /// \param node     -> Node of imbalance
    /// \description    -> Performs "case 4" rotation
    void rotate_with_right_child(AvlNode *&alpha);


    /// \param  node    -> Node of imbalance
    /// \description    -> balances AVL tree
    void balance(AvlNode *&node);

    //AVL tree root node
    AvlNode *root;
public:

    //constructors
    AvlTree() : root(nullptr) {}

    AvlTree(const AvlTree<K, V> &tree) : root(nullptr) {
        root = clone(tree.root);
    }

    AvlTree(AvlTree<K, V> &&tree) noexcept: root(nullptr) {
        *this = std::move(tree);
    }

    ~AvlTree() {
        make_empty(root);
    }

    AvlTree<K, V> &operator=(AvlTree<K, V> &&tree) noexcept {
        if (this != &tree) {
            this->~AvlTree();
            this->root = tree.root;
            tree.root = nullptr;
        }
        return *this;
    }

    /// \param value    -> Value to be added to AVL tree
    /// \description    -> Insert a new node into the AVL tree
    void insert(const K &key, const V &value) {
        insert_node(key, value, root);
    }

    /// \param value    -> Element to find
    /// \return T*      -> Pointer to value or NULL
    /// \description    -> Search
    std::vector<V> *search(const K &key) {
        return search_node(key, root);
    }
};

template<typename K, typename V>
void AvlTree<K, V>::make_empty(AvlTree::AvlNode *&node) {
    if (node != nullptr) {
        make_empty(node->left);
        make_empty(node->right);
        delete node;
    }
    node = nullptr;
}

//FIXME: Please Make sure nodes VALUES are also being copied. I am tired XD
template<typename K, typename V>
typename AvlTree<K, V>::AvlNode *AvlTree<K, V>::clone(AvlNode *node) const {
    if (node == nullptr)
        return nullptr;
    else {
        return new AvlNode{node->key, clone(node->left), clone(node->right), node->height};
    }
}

//insert_node implementation
template<typename K, typename V>
void AvlTree<K, V>::insert_node(const K &key, const V &value, AvlNode *&node) {
    if (node == nullptr) {
        node = new AvlNode(key);
        node->values.emplace_back(value);
    } else if (key < node->key) {
        insert_node(key, value, node->left);
    } else if (node->key < key) {
        insert_node(key, value, node->right);
    } else {
        node->values.emplace_back(value);
    }
    balance(node);
}

template<typename K, typename V>
std::vector<V> *AvlTree<K, V>::search_node(const K &key, AvlTree::AvlNode *&node) {
    if (node == nullptr) return nullptr;
    if (node->key < key) {
        return search_node(key, node->right);
    } else if (key < node->key) {
        return search_node(key, node->left);
    } else {
        return &node->values;
    }
}

template<typename K, typename V>
void AvlTree<K, V>::balance(AvlTree::AvlNode *&node) {
    if (node == nullptr) {
        return;
    }
    // Is the height of the left subtree greater than the right subtree?
    if (height(node->left) - height(node->right) > 1) {
        // If so, check if the LL is the node of imbalance
        if (height(node->left->left) >= height(node->left->right)) {
            rotate_with_left_child(node);
        } // Else, the LR node is the node of imbalance
        else {
            double_with_left_child(node);
        }
    }
        // Is the height of the right subtree greater than the left subtree?
    else if (height(node->right) - height(node->left) > 1) {
        if (height(node->right->right) >= height(node->right->left)) {
            // Check if RR is the node of imbalance
            if (height(node->right->right) >= height(node->right->left)) {
                rotate_with_right_child(node);
            } // Else the RL node is the node of imbalance
            else {
                double_with_right_child(node);
            }
        }
    }
    node->height = std::max(height(node->left), height(node->right)) + 1;
}

template<typename K, typename V>
void AvlTree<K, V>::rotate_with_left_child(AvlTree::AvlNode *&alpha) {
    AvlNode *beta = alpha->left;
    alpha->left = beta->right;
    beta->right = alpha;
    alpha->height = std::max(height(alpha->left), height(alpha->right)) + 1;
    beta->height = std::max(height(beta->left), alpha->height) + 1;
    alpha = beta;
}

template<typename K, typename V>
void AvlTree<K, V>::double_with_left_child(AvlTree::AvlNode *&alpha) {
    rotate_with_right_child(alpha->left);
    rotate_with_left_child(alpha);
}

template<typename K, typename V>
void AvlTree<K, V>::double_with_right_child(AvlTree::AvlNode *&alpha) {
    rotate_with_left_child(alpha->right);
    rotate_with_right_child(alpha);
}


template<typename K, typename V>
void AvlTree<K, V>::rotate_with_right_child(AvlTree::AvlNode *&alpha) {
    AvlNode *beta = alpha->right;
    alpha->right = beta->left;
    beta->left = alpha;
    alpha->height = std::max(height(alpha->left), height(alpha->right)) + 1;
    beta->height = std::max(height(beta->right), alpha->height) + 1;
    alpha = beta;
}

#endif //INC_22S_FINAL_PROJ_AVLTREE_H
