#include "Parser.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        return 1;
    }
    Parser parser;
    std::string search_term = argv[1];
    Porter2Stemmer::trim(search_term);
    Porter2Stemmer::stem(search_term);
    parser.parse(argv[2]);
    AvlTree<std::string, Article *> article_tree = parser.build_AVL_trees();
//    std::vector<Article *> matches = parser.search_article_trees(search_term, pairs);
//    std::cout << matches.size();
//    for (Article *article: matches) std::cout << article->id << '\n';
    return 0;
}
