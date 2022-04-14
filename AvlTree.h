/**
 * @Author(s):      Pravin and Kassi
 * @filename:       AvlTree.h
 * @date:           04-11-2022
 * @description:    Implementation of an AVL tree
 */

#ifndef INC_22S_FINAL_PROJ_AVLTREE_H
#define INC_22S_FINAL_PROJ_AVLTREE_H

template<typename T>
class AvlTree {
private:

    //AvlNode class declaration
    class AvlNode {
    public:
        T value;
        AvlNode *left;
        AvlNode *right;
        int height;

        explicit AvlNode(const T &value) : value(value), left(nullptr), right(nullptr), height(0) {}
    };

    /// \description    -> If the node ptr is non-null, return the node's height. If the node is a nullptr return -1
    int height(AvlNode *&node) { return node != nullptr ? node->height : -1; }

    /// \description    -> Inserts a new node into the AVL tree
    void insert_node(const T &value, AvlNode *&node);

    /// \description    -> Search node in the AVL tree
    T *search_node(const T &value, AvlNode *&node);

    /// \description    -> Empties AVL tree
    void make_empty();

    /// \param k2     -> Node of imbalance
    /// \description    -> Performs "case 1" rotation
    void rotate_with_left_child(AvlNode *&k2);


    /// \param k2     -> Node of imbalance
    /// \description    -> Performs "case 2" rotation
    void double_with_left_child(AvlNode *&k2);

    /// \param node     -> Node of imbalance
    /// \description    -> Performs "case 3" rotation
    void double_with_right_child(AvlNode *&node);

    /// \param node     -> Node of imbalance
    /// \description    -> Performs "case 4" rotation
    void rotate_with_right_child(AvlNode *&node);


    /// \param  node    -> Node of imbalance
    /// \description    -> balances AVL tree
    void balance(AvlNode *&node);

    //AVL tree root node
    AvlNode *root;
public:

    //constructors
    AvlTree() : root(nullptr) {}

    AvlTree(const AvlTree<T> &tree);

    ~AvlTree() {
        make_empty();
    }

    /// \param value    -> Value to be added to AVL tree
    /// \description    -> Insert a new node into the AVL tree
    void insert(const T &value) {
        insert_node(value, root);
    }

    /// \param value    -> Element to find
    /// \return T*      -> Pointer to value or NULL
    /// \description    -> Search
    T *search(const T &value) {
        return search_node(value, root);
    }
};

//insert_node implementation
template<typename T>
void AvlTree<T>::insert_node(const T &value, AvlNode *&node) {
    if (node == nullptr) {
        node = new AvlNode(value);
    } else if (value < node->value) {
        insert_node(value, node->left);
    } else if (node->value < value) {
        insert_node(value, node->right);
    } else;
    balance(node);
}

template<typename T>
T *AvlTree<T>::search_node(const T &value, AvlTree::AvlNode *&node) {
    if (node == nullptr) return nullptr;
    if (node->value < value) {
        return search_node(value, node->right);
    } else if (value < node->value) {
        return search_node(value, node->left);
    } else return node->value;
}

template<typename T>
void AvlTree<T>::balance(AvlTree::AvlNode *&node) {
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
    node->height = max(height(node->left), height(node->right)) + 1;
}

template<typename T>
void AvlTree<T>::rotate_with_left_child(AvlTree::AvlNode *&k2) {
    AvlNode *k1 = k2->left;
    k2->left = k1->right;
    k1->right = k2;
    k2->height = max(height(k2->left), height(k2->right)) + 1;
    k1->height = max(height(k1->left), k2->height) + 1;
    k2 = k1;
}

template<typename T>
void AvlTree<T>::double_with_left_child(AvlTree::AvlNode *&k3) {
    rotateWithRightChild(k3->left);
    rotateWithLeftChild(k3);
}


#endif //INC_22S_FINAL_PROJ_AVLTREE_H
