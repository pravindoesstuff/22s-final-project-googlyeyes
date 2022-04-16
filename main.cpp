#include "Parser.h"
#include <iostream>

int main(int argc, char **argv) {
    if (argc == 1) {
        return 1;
    }
    Parser parser;
    //std::string search_term = argv[1];
    parser.parse(argv[2]);
    //AvlTree<std::string, Article *> pairs = parser.build_AVL_tree();
    //std::cout << pairs.search(search_term);
    return 0;
}
