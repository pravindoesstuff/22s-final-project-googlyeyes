#include "Parser.h"

Article *Parser::parse_json(const std::filesystem::directory_entry &json_file) {

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

    return article;
}

void Parser::parse(const std::filesystem::path &root_folder_path) {
    auto data_dir = std::filesystem::directory_iterator(root_folder_path);
    for (const auto &element: data_dir) {
        if (element.is_directory()) {
            // Recursively enter the next directory
            parse(element.path());
        } else if (element.path().extension() == ".json") {
            //1- Submit parse_json routine to pool and move it into the future_queue
            future_queue.push_back(thread_pool.enqueue(&Parser::parse_json, this, element));
        }
    }
}

std::vector<Article *>
Parser::search_article_trees(const std::string &token, std::vector<AvlTree<std::string, Article *>> trees) {
    std::vector<std::future<std::vector<Article *> * >> future_tokens;
    for (AvlTree<std::string, Article *> &articles: trees) {
        future_tokens.emplace_back(thread_pool.enqueue(&AvlTree<std::string, Article *>::search, &articles, token));
    }
    std::vector<Article *> matching_articles;
    for (std::future<std::vector<Article *> *> &future_articles: future_tokens) {
        std::vector<Article *> *articles = future_articles.get();
        if (articles != nullptr) {
            for (Article *article: *articles) {
                matching_articles.emplace_back(article);
            }
        }
    }
    return matching_articles;
}

AvlTree<std::string, Article *> Parser::make_tree(const std::vector<Article *> &articles) {
    AvlTree<std::string, Article *> article_tree;
    for (Article *article: articles) {
        for (const std::string &token: article->tokens) {
            article_tree.insert(token, article);
        }
    }
    return article_tree;
}

std::vector<AvlTree<std::string, Article *>> Parser::build_AVL_trees() {
    uint nthreads = std::thread::hardware_concurrency();
    size_t queue_len = future_queue.size();
    std::vector<std::vector<Article *>> segmented_articles(nthreads);
    for (size_t i = 0; i < queue_len; ++i) {
        segmented_articles[i * nthreads / queue_len].emplace_back(future_queue[i].get());
    }
    future_queue.clear();
    std::vector<std::future<AvlTree<std::string, Article *>>> future_trees;
    for (const std::vector<Article *> &articles: segmented_articles) {
        future_trees.emplace_back(thread_pool.enqueue(make_tree, articles));
    }
    std::vector<AvlTree<std::string, Article *>> trees;
    for (auto &future_tree: future_trees) {
        trees.emplace_back(future_tree.get());
    }
    return trees;
}
