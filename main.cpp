#include <iostream>
#include "Parser.h"
#include "Query.h"
#include "Article.h"

int main(int argc, char **argv) {

    char option;
    AvlTree<std::string, Article *> article_tree;
    Parser parser;
    do {
        std::cout << "\n---GOOGLEYES SEARCH ENGINE---\n";

        std::cout << "0 - Parse Dataset" << '\n';
        std::cout << "1 - Form Persistent File" << '\n';
        std::cout << "2 - Clear Persistent File" << '\n';
        std::cout << "3 - Display Engine Statistics" << '\n';
        std::cout << "4 - Search Dataset" << '\n';
        std::cout << "5 - Quit" << '\n';

        std::cout << "Enter option: ";
        std::cin >> option;

        switch (option) {
            case '0': {
                //Parse dataset
                std::cout << "Enter dataset path: ";
                std::string folder_path;
                std::cin >> folder_path;

                parser.parse(folder_path);

                article_tree = parser.build_AVL_tree();
                break;
            }

            case '1': {
                article_tree.form_persistent_file();
                break;
            }

            case '2': {
                article_tree.clear_persistent_file();
                break;
            }

            case '3': {
                //Display statistics
                std::cout << "\nTotal articles indexed is: " << article_tree.get_total_articles() << '\n';
                std::cout << "Unique Organizations: " << parser.orgs_map.insertion_count << '\n';
                std::cout << "Unique Persons: " << parser.person_map.insertion_count << '\n';
                std::cout << "Word-Article Ratio (Stop words excluded): " << article_tree.get_word_article_ratio()
                          << '\n';
                std::cout << "TOP 25 Most frequent words (Descending): \n";
                article_tree.proposition_279();
                break;
            }

            case '4': {
                //Search Dataset
                std::cout << "Enter search request: ";
                std::string search_request;
                std::cin.ignore();
                std::getline(std::cin, search_request);

                Query query(search_request);
                std::set<Article *> articles = query.get_elements(article_tree, parser.person_map, parser.orgs_map);
                std::cout << "\n---Search performed in: " << query.get_query_processing_time() << " second(s)---\n";

                std::vector<ArticlePair> pairs;
                for (Article *article: articles) {
                    pairs.push_back({.article = article, .weight = query.frequency(article->tokens)});
                }
                std::sort(pairs.begin(), pairs.end());
                for (const ArticlePair &pair: pairs) {
                    std::cout << pair.article->id << '\n';
                }
                break;
            }

            case '5': {
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