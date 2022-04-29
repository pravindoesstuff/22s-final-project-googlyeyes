#include <iostream>
#include "Parser.h"
#include "Query.h"
#include "Article.h"

int main(int argc, char **argv) {

    char option;
    AvlTree<std::string, Article *> article_tree;
    
    do {
        std::cout << "\n---GOOGLEYES SEARCH ENGINE---\n";

        std::cout << "0 - Parse Dataset" << '\n';
        std::cout << "1 - Display Engine Statistics" << '\n';
        std::cout << "2 - Search Dataset" << '\n';
        std::cout << "3 - Quit" << '\n';

        std::cout << "Enter option: ";
        std::cin >> option;

        switch (option) {
            case '0': {
                //Parse dataset
                std::cout << "Enter dataset path: ";
                std::string folder_path;
                std::cin >> folder_path;

                Parser parser;
                parser.parse(folder_path);

                article_tree = parser.build_AVL_tree();
                break;
            }

            case '1': {
                //Display statistics
                std::cout << "Statistics" << '\n';
                break;
            }

            case '2': {
                //Search Dataset
                std::cout << "Enter search request: ";
                std::string search_request;
                std::cin.ignore();
                std::getline(std::cin, search_request);

                Query query(search_request);
                std::set<Article *> articles = query.get_elements(article_tree);
                for(Article *article : articles) {
                    std::cout << article->id << '\n';
                }
                break;
            }

            case '3': {
                std::cout << "Bye!" << '\n';
                return 0;
            }

            default: {
                std::cout << "Bad option, Try again" << '\n';
                break;
            }
        }
    } while (true);
}