/**
 * @Author(s):      Pravin and Kassi
 * @filename:       Pair.h
 * @date:           04-30-2022
 * @description:    Represents a pair within the AVL tree
 */

#ifndef INC_22S_FINAL_PROJ_PAIR_H
#define INC_22S_FINAL_PROJ_PAIR_H

#include <string>

struct Pair {
    std::string word;
    unsigned int articles = 0;

    Pair(std::string &wrd, unsigned int tot_article){
        word = wrd;
        articles = tot_article;
    }

    bool operator<(const Pair& p)const{
        return articles < p.articles;
    }
};


#endif //INC_22S_FINAL_PROJ_PAIR_H
