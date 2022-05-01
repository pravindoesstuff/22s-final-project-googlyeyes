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
        else if (token == "OR") current_tokenizer = OR;
        else if (token == "NOT") current_tokenizer = NOT;
        else if (token == "ORG") current_tokenizer = ORG;
        else if (token == "PERSON") current_tokenizer = PERSON;
        else {
            switch (current_tokenizer) {
                case AND:
                    Porter2Stemmer::trim(token);
                    this->and_keywords.push_back(token);
                    break;
                case OR:
                    Porter2Stemmer::trim(token);
                    this->or_keywords.push_back(token);
                    break;
                case NOT:
                    Porter2Stemmer::trim(token);
                    this->not_words.push_back(token);
                    break;
                case ORG:
                    this->organization += ' ' + token;
                    break;
                case PERSON :
                    this->person += ' ' + token;
                    break;
            }
        }
    }
    if (!this->organization.empty()) {
        this->organization.erase(this->organization.begin());
    }
    if (!this->person.empty()) {
        this->person.erase(this->person.begin());
    }
}

std::set<Article *> Query::get_elements(const AvlTree<std::string, Article *> &article_tree) {
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    start = std::chrono::high_resolution_clock::now();

    std::set<Article *> article_set;
    if (and_keywords.empty()) {
        for (const std::string &keyword: or_keywords) {
            auto keyword_vec = article_tree.search(keyword);
            if (keyword_vec != nullptr) {
                article_set.insert(keyword_vec->cbegin(), keyword_vec->cend());
            }
        }
    }
    bool first_pass = true;
    for (const std::string &keyword: and_keywords) {
        auto keyword_vec = article_tree.search(keyword);
        if (keyword_vec != nullptr) {
            if (first_pass) {
                article_set = {keyword_vec->cbegin(), keyword_vec->cend()};
                first_pass = false;
                continue;
            }
            std::set<Article *> intersection;
            std::set<Article *> temp_set(keyword_vec->cbegin(), keyword_vec->cend());
            std::set_intersection(article_set.cbegin(), article_set.cend(), temp_set.cbegin(), temp_set.cend(),
                                  std::inserter(intersection, intersection.cbegin()));
            article_set = intersection;
        } else {
            return {};
        }
    }
    std::set<Article *> filtered_set;
    for (Article *article: article_set) {
        if (!organization.empty() &&
            std::find(article->organizations.cbegin(), article->organizations.cend(), organization) ==
            article->organizations.cend()) {
            continue;
        }
        if (!person.empty() &&
            std::find(article->persons.cbegin(), article->persons.cend(), person) == article->persons.cend()) {
            continue;
        }
        filtered_set.insert(article);
    }

    end = std::chrono::high_resolution_clock::now();
    //calculate the duration between "start" and "end"
    std::chrono::duration<double> time_in_seconds = end - start;
    query_processing_time = time_in_seconds.count();
    return filtered_set;
}
