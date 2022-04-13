#include "Parser.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc == 1) return 1;
    Parser parser;
    std::string search_term = argv[1];
    parser.parse(argv[2]);
    parser.wait();
    std::vector<Article> articles = parser.articles;
    for (const Article &article: articles) {
        if (std::find(article.tokens.begin(), article.tokens.end(), search_term) != article.tokens.end()) {
            std::cout << article.id << '\n';
        }
    }
    return 0;
}
