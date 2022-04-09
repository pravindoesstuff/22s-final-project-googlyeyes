/**
 * @Author(s):      Pravin and Kassi
 * @filename:       Parser.cpp
 * @date:           04-05-2022
 * @description:    This class is represents the parser of the project
 *                  it reads JSON files and convert them into a vector
 *                  of normalized tokens
 */

#ifndef INC_22S_FINAL_PROJ_PARSER_H
#define INC_22S_FINAL_PROJ_PARSER_H

#include <string>

using std::string;

class Parser {
public:
    static void build_data(const string& kaggle_path);

};


#endif //INC_22S_FINAL_PROJ_PARSER_H
