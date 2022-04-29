/**
 * @Author(s):      Pravin and Kassi
 * @filename:       Query.h
 * @date:           04-06-2022
 * @description:    Implementation of our boolean query processor
 */

#ifndef INC_22S_FINAL_PROJ_QUERY_H
#define INC_22S_FINAL_PROJ_QUERY_H

#include <vector>
#include <string>
#include <sstream>
#include <set>
#include "Article.h"
#include "AvlTree.h"


class Query {

private:
    std::vector<std::string> and_keywords;
    std::vector<std::string> or_keywords;
    std::vector<std::string> not_words;
    std::string organization;
    std::string person;

public:
    explicit Query(const std::string &query);

    std::set<Article*> get_elements(const AvlTree<std::string, Article *>& article_tree);
};


#endif //INC_22S_FINAL_PROJ_QUERY_H
