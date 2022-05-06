#include <iostream>
#include "Parser.h"
#include "Query.h"
#include "Article.h"

int main(int argc, char **argv) {

    char option;
    AvlTree<std::string, Article *> article_tree;
    Parser parser;
    std::vector<ArticlePair> pairs;

    do {
        std::cout << "\n---GOOGLEYES SEARCH ENGINE---\n";

        std::cout << "0 - Parse Dataset" << '\n';
        std::cout << "1 - Form Persistent File" << '\n';
        std::cout << "2 - Clear Persistent File" << '\n';
        std::cout << "3 - Display Engine Statistics" << '\n';
        std::cout << "4 - Search Dataset" << '\n';
        std::cout << "5 - Display Article Info" << '\n';
        std::cout << "6 - Quit" << '\n';

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
                std::cout << "Total Unique Words (Excluding Stop Words): " << article_tree.size() << '\n';
                std::cout << "Unique Organizations: " << parser.k1.size() << '\n';
                std::cout << "Unique Persons: " << parser.k2.size() << '\n';
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
                pairs = {};
                Query query(search_request);
                std::set<Article *> articles = query.get_elements(article_tree, parser.person_map, parser.orgs_map);
                std::cout << "\n---Search performed in: " << query.get_query_processing_time() << " second(s)---\n";
                for (Article *article: articles) {
                    pairs.push_back({.article = article, .weight = query.frequency(article->tokens)});
                }
                std::sort(pairs.begin(), pairs.end());
                int n = 0;
                for (const ArticlePair &pair: pairs) {
                    std::cout << pair.article->id << ": " << pair.article->title << '\n';
                    if (n == 25) break;
                    ++n;
                }
                if (n == 25) {
                    std::cout << "Limited to the top 25 Results\n";
                }
                break;
            }

            case '5': {
                std::cout << "Enter Article ID: ";
                std::string id;
                std::cin >> id;
                for (const ArticlePair &pair: pairs) {
                    if (pair.article->id == id) {
                        std::cout << "\nTitle: " << pair.article->title << '\n';
                        std::cout << "\nText: " << pair.article->text << '\n';
                        break;
                    }
                }
                break;
            }

            case '6': {
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
