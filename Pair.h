/**
 * @Author(s):      Pravin and Kassi
 * @filename:       Parser.h
 * @date:           04-14-2022
 * @description:    Represents a (word, article) pair to be stored in the AVL tree
 */

#ifndef INC_22S_FINAL_PROJ_PAIR_H
#define INC_22S_FINAL_PROJ_PAIR_H


struct Pair {
    std::string token;
    std::vector<Article*> articles;

    bool operator<(const Pair &pair) const { return this->token < pair.token; }

    bool operator==(const Pair &pair) const { return this->token == pair.token; }

    void operator+=(const Pair &pair) {
        articles.insert(articles.begin(), pair.articles.begin(), pair.articles.end());
    }

    Pair(const std::string &key) { this->token = key; }

    Pair(const std::string &token, const std::vector<Article*> &articles) {
        this->token = token;
        this->articles = articles;
    }
};


#endif //INC_22S_FINAL_PROJ_PAIR_H
