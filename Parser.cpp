#include "Parser.h"

AvlTree<std::string, Article *> Parser::parse_json(const std::filesystem::directory_entry &json_file) {

    //1- Open stream to file
    std::ifstream file(json_file.path());

    //2- Read the file into "json_string"
    std::string json_string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    //3- Close the stream
    file.close();

    //4- Parsing "json_string" into "JSON_document" using rapidjson
    rapidjson::Document JSON_document;
    JSON_document.Parse(json_string.c_str());


    //5- Store "persons name" into Article
    Article *article = new Article();
    auto &entities = JSON_document["entities"]; //an element of the JSON file containing arrays
    auto &persons = entities["persons"]; //an array
    for (const auto &person: persons.GetArray()) {
        //add person names to article vector named "persons"
        article->persons.emplace_back(person["name"].GetString());
    }

    //6- Store "organizations name" into Article
    auto &organizations = entities["organizations"]; //an array
    for (const auto &organization: organizations.GetArray()) {
        //add organization names to article vector named "organizations"
        article->organizations.emplace_back(organization["name"].GetString());
    }

    //7- Store text
    article->text = JSON_document["text"].GetString();

    //8- Tokenize, lowercase, and stemming
    std::istringstream ss(article->text);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        //if stop-word, ignore.
        if (stop_words.find(token) != stop_words.end()) {
            continue;
        }
        //punctuation removal
        char *token_ptr = token.data();

        // Remove all characters that are not alphabetic from token
        for (const char c: token) {
            if (isalpha(c)) {
                *token_ptr++ = c;
            }
        }

        // If the token_ptr hasn't changed, then no alphabetic characters were found, so we skip this token
        if (token.c_str() == token_ptr) {
            continue;
        }

        // Truncate punctuation that may appear at the end of the character
        token.resize(token_ptr - token.c_str());

        // Convert all alphabetic characters to lowercase
        for (char &c: token) {
            c = (char) tolower(c);
        }

        //stem token
        Porter2Stemmer::stem(token);
        //add token to list of tokens in the article.
        article->tokens.emplace_back(token);
    }

    //9- Store ID
    article->id = JSON_document["uuid"].GetString();

    //10- Get Title
    article->title = JSON_document["title"].GetString();

    //11- Insert individual words into AVL tree
    AvlTree<std::string, Article *> article_tree;
    for (const std::string &word: article->tokens) {
        article_tree.insert(word, article);
    }

    return article_tree;
}

void Parser::parse(const std::filesystem::path &root_folder_path) {
    auto data_dir = std::filesystem::directory_iterator(root_folder_path);
    for (const auto &element: data_dir) {
        if (element.is_directory()) {
            // Recursively enter the next directory
            parse(element.path());
        } else if (element.path().extension() == ".json") {
            //1- Submit parse_json routine to pool and move it into the future_queue
            future_queue.push_back(thread_pool.Submit(parse_json, element));
        }
    }
}

/*
 * NOTE! This could later on be replaced with a function similar to build_avl_tree(), since all elements are being
 * accessed here anyways
 */

std::vector<AvlTree<std::string, Article *>> Parser::build_AVL_tree() {
    //set of avl tree
    std::vector<AvlTree<std::string, Article *>> avl_trees;

    for (std::future<AvlTree<std::string, Article *>> &future_avl: future_queue) {
        // Get current avl_tree from the future_avl and place it into avl_trees
        avl_trees.push_back(future_avl.get());
    }
    future_queue.clear();
    return avl_trees;
}
