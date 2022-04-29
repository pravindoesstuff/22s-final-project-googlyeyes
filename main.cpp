#include "Parser.h"
#include "Query.h"

int main(int argc, char **argv) {
    if (argc == 1) {
        return 1;
    }
    Parser parser;
    parser.parse(argv[1]);
    AvlTree<std::string, Article *> article_tree = parser.build_AVL_trees();
    Query query("AND semiconductor stock ORG nvidia");
    auto q = query.get_elements(article_tree);
    for (auto c : q) {
        std::cout << c->id << '\n';
    }
    return 0;
}
