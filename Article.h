//
// Created by pravin on 4/6/22.
//

#ifndef INC_22S_FINAL_PROJ_ARTICLE_H
#define INC_22S_FINAL_PROJ_ARTICLE_H

#include <string>
#include <vector>

using std::vector, std::string;
struct Article {
    string text;
    vector<string> persons, organizations;
};
#endif //INC_22S_FINAL_PROJ_ARTICLE_H
