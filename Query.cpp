//
// Created by pravin on 4/6/22.
//

#include "Query.h"
#include <sstream>

using std::stringstream;

enum Tokenizer {
    AND,
    OR,
    NOT,
    ORG,
    PERSON,
};

Query::Query(const string &query) {
    stringstream qstream(query);
    string token;
    Tokenizer current_tokenizer = OR;
    while (qstream >> token) {
        if (token == "AND") current_tokenizer = AND;
        if (token == "OR") current_tokenizer = OR;
        if (token == "NOT") current_tokenizer = NOT;
        if (token == "ORG") current_tokenizer = ORG;
        if (token == "PERSON") current_tokenizer = PERSON;
        else {
            switch (current_tokenizer) {
                case AND:
                    this->and_keywords.push_back(token);
                    break;
                case OR:
                    this->or_keywords.push_back(token);
                    break;
                case NOT:
                    this->not_words.push_back(token);
                    break;
                case ORG:
                    this->organizations.push_back(token);
                    break;
                case PERSON :
                    this->people.push_back(token);
                    break;
            }
        }
    }
}