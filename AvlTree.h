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
        T element;
        AvlNode *left;
        AvlNode *right;
        int height;

        explicit AvlNode(const T &element) : element(element), left(nullptr), right(nullptr), height(0) {}
    };


    /// \description    -> Inserts a new node into the AVL tree
    void insert_node(const T &value, AvlNode *&node);

    /// \description    -> Search node in the AVL tree
    T *search_node(const T &value, AvlNode *&node);

    /// \description    -> Empties AVL tree
    void make_empty();

    /// \param node     -> Node of imbalance
    /// \description    -> Performs "case 1" rotation
    void rotate_with_left_child(AvlNode *&node);


    /// \param node     -> Node of imbalance
    /// \description    -> Performs "case 2" rotation
    void double_with_left_child(AvlNode *&node);

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
    } else if (value < node->element) {
        insert_node(value, node->left);
    } else if (node->element < value) {
        insert_node(value, node->right);
    } else;

    balance(node);
}


#endif //INC_22S_FINAL_PROJ_AVLTREE_H
