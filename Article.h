/**
 * @Author(s):      Pravin and Kassi
 * @filename:       Article.h
 * @date:           04-06-2022
 * @description:    Representation of a processed JSON file
 */

#ifndef INC_22S_FINAL_PROJ_ARTICLE_H
#define INC_22S_FINAL_PROJ_ARTICLE_H

#include <string>
#include <vector>

struct Article {
    std::string id;
    std::string title;
    std::string text;
    std::vector<std::string> persons;
    std::vector<std::string> organizations;
    std::vector<std::string> tokens;
};
#endif //INC_22S_FINAL_PROJ_ARTICLE_H
