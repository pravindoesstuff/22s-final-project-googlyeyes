#include <iostream>
#include "Parser.h"

int main(int argc, char **argv) {
    char option;

    do {
        std::cout << "---GOOGLEYES SEARCH ENGINE---" << std::endl;

        std::cout << "0 - Parse Dataset" << '\n';
        std::cout << "1 - Display Engine Statistics" << '\n';
        std::cout << "2 - Search Dataset" << '\n';
        std::cout << "3 - Quit" << '\n';

        std::cin >> option;

        switch (option) {
            case '0': {
                //Parse dataset
                std::cout << "Dataset parsed" << '\n';
                break;
            }

            case '1': {
                //Display statistics
                std::cout << "Statistics" << '\n';
                break;
            }

            case '2': {
                //Search Datasets
                std::cout << "Search" << '\n';
                break;
            }

            case '3': {
                std::cout << "Bye!" << std::endl;
                return 0;
            }

            default: {
                std::cout << "Bad option, Try again" << std::endl;
                break;
            }
        }
    } while (true);
}