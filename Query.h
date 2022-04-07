//
// Created by pravin on 4/6/22.
//

#ifndef INC_22S_FINAL_PROJ_QUERY_H
#define INC_22S_FINAL_PROJ_QUERY_H

#include <vector>
#include <string>

using std::vector;
using std::string;

class Query {
    vector<string> and_keywords, or_keywords, not_words, organizations, persons;

    explicit Query(const string& query);
};


#endif //INC_22S_FINAL_PROJ_QUERY_H
