#include "Query.h"

enum Tokenizer {
    AND,
    OR,
    NOT,
    ORG,
    PERSON,
};

Query::Query(const std::string &query) {
    std::stringstream qstream(query);
    std::string token;
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
                    this->organization = token;
                    break;
                case PERSON :
                    this->person = token;
                    break;
            }
        }
    }
}