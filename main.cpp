#include "Parser.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc == 1) return 1;
    Parser parser;
    std::string search_term = argv[1];
    std::deque<Article> articles = parser.parse(argv[2]);
    for (const Article &article: articles) {
        if (std::find(article.tokens.begin(), article.tokens.end(), search_term) != article.tokens.end()) {
            std::cout << article.title << '\n';
        }
    }
    return 0;
}
