/**
 * @Author(s):      Pravin and Kassi
 * @filename:       AvlTree.h
 * @date:           04-11-2022
 * @description:    Implementation of an AVL tree
 */

#ifndef INC_22S_FINAL_PROJ_AVLTREE_H
#define INC_22S_FINAL_PROJ_AVLTREE_H

#include <algorithm>
#include <iostream>
#include <queue>
#include "Pair.h"
#include <string>
#include <fstream>
#include "rapidjson/document.h"
#include "rapidjson/writer.h"
#include "rapidjson/stringbuffer.h"

template<typename K, typename V>
class AvlTree {
private:
    int total_articles = 0;
    int total_tokens = 0;

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
    std::vector<V> *search_node(const K &key, AvlNode *node) const;

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

    /// \param node         -> Starting node
    /// \return tree_JSON   -> A JSON representation of an AVL tree
    /// \description        -> Visit AVL tree nodes (Level Order style), turn them to JSON, and
    ///                 add them to "tree_JSON". Expected result:
    ///                    {
    ///                        "nodes": [
    ///                                node_1,
    ///                                node_n,
    ///                        ]
    ///                    }
    std::string from_tree_to_JSON(AvlNode* node);

    /// \param node     -> Current node
    /// \param level    -> Current level
    /// \description    -> Will turn each nodes (at a given level in AVL tree) into JSON and stores them
    ///                 into a rapidJSON array, using "tree_JSON" allocator. This function and from_tree_to_JSON work together
    void jsonify_AVL_nodes(AvlNode* node, int level, rapidjson::Value& arr, rapidjson::Document::AllocatorType& allocator);

    /// \param article*     -> Processed JSON object
    /// \return string      -> A JSON representation of an Article
    /// \description        -> Turn an "Article" object into a JSON string. The following is the expected result:
    ///                       {
    ///                         "id": "id_1",
    ///                         "title": "title_1",
    ///                         "persons": ["person_1", "person_n"],
    ///                         "orgs": ["org_1", "org_n"],
    ///                         "tokens": ["token_1", "token_n"]
    ///                       }
    std::string from_article_to_JSON (V article);

    /// \param node*        -> AVL tree node
    /// \return string      -> A JSON representation of an AVL node
    /// \description        -> Turn an AVL node into a JSON string
    ///                        {
    ///                            "word": "word",
    ///                            "articles": [
    ///                                    article_1,
    ///                                    article_n,
    ///                            ]
    ///                        }
    std::string from_node_to_JSON(AvlNode *node);

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
        total_articles = tree.total_articles;
        total_tokens = tree.total_tokens;

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
    std::vector<V> *search(const K &key) const {
        return search_node(key,  root);
    }

    /// \param          -> N/A
    /// \return         -> Total documents
    /// \description    -> returns total number of documents in the tree
    int get_total_articles() {     return total_articles;     }

    /// \description    -> Updates the total number of documents in the tree
    void set_total_articles(int new_total_document){
        total_articles = new_total_document;
    }

    /// \param          -> N/A
    /// \description    -> returns total number of documents in the tree
    float get_word_article_ratio() {
        return total_tokens / total_articles;
    }

    /// \param number   -> number to increase total number by
    /// \description    -> Increases total_tokens variable
    void add_tokens(int number) { total_tokens += number; }

    /// \param node             -> Starting node
    /// \return None            -> N/A
    /// \desciption             -> Implement Pre Order traversal method, and populate priority queue for each visit
    void pre_order(AvlNode *node, std::priority_queue<Pair>& queue);

    /// \param None             -> N/A
    /// \return None            ->
    /// \description            -> Prints a list of the most 25 frequent words in the AVL tree to the console
    void proposition_279();

    /// \param None             -> N/A
    /// \return None            -> N/A
    /// \description            -> Turns this AVL tree into a JSON string, and write it to same level as the executable
    void form_persistent_file();

    /// \param None             -> N/A
    /// \return None            -> N/A
    /// \description            -> clears persistent file content
    void clear_persistent_file();

    size_t size() {
        return size(root);
    }
    size_t size(AvlNode *node) {
        if (node == nullptr) {
            return 0;
        }
        return size(node->left) + size(node->right) + 1;
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
std::vector<V> *AvlTree<K, V>::search_node(const K &key, AvlTree::AvlNode *node) const {
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

template<typename K, typename V>
void AvlTree<K, V>::pre_order(AvlNode *node, std::priority_queue<Pair>& queue){
    //1- Visit all nodes in AVL tree starting from root (PreOrder Traversal)
    if(node != nullptr){
        //2- For each (Make a "pair" object) && (Add to "queue")
        Pair p(node->key, node->values.size());
        queue.push(p);

        //continue visiting and adding to "queue"
        pre_order(node->left, queue);
        pre_order(node->right, queue);
    }
}

template<typename K, typename V>
void AvlTree<K, V>::proposition_279() {
    std::priority_queue<Pair> p_queue;
    pre_order(root, p_queue);
    //3- 25x (Print priority queue top, then pop)

    for(int i = 0; i < 25; i++){
        std::cout << p_queue.top().word << " -> " << p_queue.top().articles << '\n';
        p_queue.pop();
    }
}

template<typename K, typename V>
std::string AvlTree<K, V>::from_tree_to_JSON(AvlNode *node) {
    rapidjson::Document tree_JSON; //Null
    tree_JSON.SetObject();

    // must pass an allocator when the object may need to allocate memory
    rapidjson::Document::AllocatorType& allocator = tree_JSON.GetAllocator();

    //Create a rapidjson "value" and "array" types
    rapidjson::Value array(rapidjson::kArrayType);

    //Turn nodes at each level into JSON, and store them into "array", using "allocator"
    for(int i = 1; i < node->height; i++){
        jsonify_AVL_nodes(node, i, array, allocator);
    }

    //populate tree_JSON with "nodes"
    tree_JSON.AddMember("nodes", array, allocator);

    //stringify and return "tree_JSON"
    rapidjson::StringBuffer str_buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(str_buf);
    tree_JSON.Accept(writer);

    return str_buf.GetString();
}

template<typename K, typename V>
void AvlTree<K, V>::jsonify_AVL_nodes(AvlNode *node, int level, rapidjson::Value& arr, rapidjson::Document::AllocatorType& allocator) {
    if(node == nullptr)
        return;
    if(level == 1){
        //Turn node into JSON
        std::string node_json = from_node_to_JSON(node);
        //add it to "array"
        rapidjson::Value value(rapidjson::kObjectType);
        value.SetString(node_json.c_str(), static_cast<rapidjson::SizeType>(node_json.length()), allocator);
        arr.PushBack(value, allocator);
    }
    else if(level > 1){
        jsonify_AVL_nodes(node->left, level - 1, arr, allocator);
        jsonify_AVL_nodes(node->right, level - 1, arr, allocator);
    }
}

template<typename K, typename V>
std::string AvlTree<K,V>::from_article_to_JSON(V article){
    rapidjson::Document article_JSON; //Null

    //set "article_JSON" as an empty object
    article_JSON.SetObject();

    // must pass an allocator when the object may need to allocate memory
    rapidjson::Document::AllocatorType& allocator = article_JSON.GetAllocator();

    //Create a rapidjson "value" and "array" types
    rapidjson::Value value(rapidjson::kObjectType);
    rapidjson::Value array(rapidjson::kArrayType);

    //Populate "article_JSON" with "id"
    value.SetString(article->id.c_str(), static_cast<rapidjson::SizeType>(article->id.length()), allocator);
    article_JSON.AddMember("id", value, allocator);

    //Populate "article_JSON" with title
    value.SetString(article->title.c_str(), static_cast<rapidjson::SizeType>(article->title.length()), allocator);
    article_JSON.AddMember("title", value, allocator);

    //Populate "article_JSON" with "persons"
    for(std::string &person: article->persons){
        value.SetString(person.c_str(), static_cast<rapidjson::SizeType>(person.length()), allocator);
        array.PushBack(value, allocator);
    }
    article_JSON.AddMember("persons", array, allocator);
    array.Clear();

    //Populate "article_JSON" with "orgs"
    for(std::string &org: article->organizations){
        value.SetString(org.c_str(), static_cast<rapidjson::SizeType>(org.length()), allocator);
        array.PushBack(value, allocator);
    }
    article_JSON.AddMember("orgs", array, allocator);
    array.Clear();

    //Populate "article_JSON" with "tokens"
    for(std::string &token: article->tokens){
        value.SetString(token.c_str(), static_cast<rapidjson::SizeType>(token.length()), allocator);
        array.PushBack(value, allocator);
    }
    article_JSON.AddMember("tokens", array, allocator);
    array.Clear();

    //return stringified "article_JSON"
    rapidjson::StringBuffer str_buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(str_buf);
    article_JSON.Accept(writer);

    return str_buf.GetString();
}

template<typename K, typename V>
std::string AvlTree<K, V>::from_node_to_JSON(AvlNode *node) {
    rapidjson::Document node_JSON; //Null

    //set "node_JSON" as an empty object
    node_JSON.SetObject();

    // must pass an allocator when the object may need to allocate memory
    rapidjson::Document::AllocatorType& allocator = node_JSON.GetAllocator();

    //Create a rapidjson "value" and "array" types
    rapidjson::Value value(rapidjson::kObjectType);
    rapidjson::Value array(rapidjson::kArrayType);

    //Populate "node_JSON" with "word"
    value.SetString(node->key.c_str(), static_cast<rapidjson::SizeType>(node->key.length()), allocator);
    node_JSON.AddMember("word", value, allocator);

    //Populate "node_JSON" with "articles"
    for(V article: node->values){
        std::string article_JSON = from_article_to_JSON(article);
        value.SetString(article_JSON.c_str(), static_cast<rapidjson::SizeType>(article_JSON.length()), allocator);
        array.PushBack(value, allocator);
    }
    node_JSON.AddMember("articles", array, allocator);
    array.Clear();

    //return stringified "node_JSON"
    rapidjson::StringBuffer str_buf;
    rapidjson::Writer<rapidjson::StringBuffer> writer(str_buf);
    node_JSON.Accept(writer);

    return str_buf.GetString();
}

template<typename K, typename V>
void AvlTree<K, V>::form_persistent_file() {
    //Turn this AVL tree into JSON string
    std::string tree_JSON = from_tree_to_JSON(root);

    //write JSON string to a file in the current directory
    std::ofstream output_stream( "tree.txt", std::ios::out);
    output_stream << tree_JSON;

    //close stream
    output_stream.close();
}

template<typename K, typename V>
void AvlTree<K, V>::clear_persistent_file() {
    std::ifstream input_stream("tree.txt", std::ios::in);

    //if "output.txt" exits...
    if(input_stream.good()){
        //close then open it back in "trunc" mode to clear all content.
        input_stream.close();
        std::ofstream output_stream("tree.txt", std::ios::trunc);
        output_stream.close();
    }
        //if not...
    else{
        return;
    }
}
#endif //INC_22S_FINAL_PROJ_AVLTREE_H
