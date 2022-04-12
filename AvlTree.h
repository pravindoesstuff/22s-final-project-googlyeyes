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
        AvlNode<T> *left;
        AvlNode<T> *right;
        int height;

        AvlNode(const T& the_element,
                AvlNode<T> *lt,
                AvlNode<T> *rt,
                int h = 0) : element(the_element),
                             left(lt),
                             right(rt),
                             height(h) {}
    };


    /// \description    -> Inserts a new node into the AVL tree
    void insert_node(const T &value, AvlNode *&node);

    /// \description    -> Search node in the AVL tree
    T* search_node(const T &value, AvlNode *&node);

    /// \description    -> Empties AVL tree
    void make_empty();

    /// \param node     -> Node of imbalance
    /// \description    -> Performs "case 1" rotation
    void rotate_with_left_child(AvlNode<T> *& node);


    /// \param node     -> Node of imbalance
    /// \description    -> Performs "case 2" rotation
    void double_with_left_child(AvlNode<T> *& node);

    /// \param node     -> Node of imbalance
    /// \description    -> Performs "case 3" rotation
    void double_with_right_child(AvlNode<T> *& node);

    /// \param node     -> Node of imbalance
    /// \description    -> Performs "case 4" rotation
    void rotate_with_right_child(AvlNode<T> *& node);


    /// \param  node    -> Node of imbalance
    /// \description    -> balances AVL tree
    void balance(AvlNode<T> *&node);

    //AVL tree root node
    AvlNode<T> *root;
public:

    //constructors
    AvlTree() : root(nullptr) {}

    AvlTree(const AvlTree<T> &rhs);

    ~AvlTree() {
        make_empty();
    }

    /// \param value    -> Value to be added to AVL tree
    /// \description    -> Insert a new node into the AVL tree
    void insert(const T& value) {
        insert_node(value, root);
    }

    /// \param value    -> Element to find
    /// \return T*      -> Pointer to value or NULL
    /// \description    -> Search
    T* search(const T& value){
        return search_node(value, root);
    }
};

//insert_node implementation
template<typename T>
void AvlTree<T>::insert_node(const T &value, AvlNode *&node) {
    if (node == nullptr) {
        node = new AvlNode<T>(value, nullptr, nullptr);
    }
    else if (value < node->element) {
        insert_node(value, node->left);
    }
    else if (node->element < value) {
        insert_node(value, node->right);
    } else;

    balance(node);
}


#endif //INC_22S_FINAL_PROJ_AVLTREE_H
