#include "Parser.h"

Article Parser::parse_json(const std::filesystem::directory_entry &json_file) {

    //1- Open stream to file
    std::ifstream file(json_file.path());

    //2- Read the file into "json_string"
    std::string json_string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    //3- Close the stream
    file.close();

    //4- Parsing "json_string" into "JSON_document" using rapidjson
    rapidjson::Document JSON_document;
    JSON_document.Parse(json_string.c_str());

    //5- if "json_string" is invalid, throw exception
    if (!JSON_document.IsObject()) {
        throw std::exception();
    }

    //6- Store "persons name" into Article
    Article article;
    auto &entities = JSON_document["entities"]; //an element of the JSON file containing arrays
    auto &persons = entities["persons"]; //an array
    if (!persons.IsNull()) {
        for (const auto &person: persons.GetArray()) {
            //add person names to article vector named "persons"
            article.persons.emplace_back(person["name"].GetString());
        }
    }

    //7- Store "organizations name" into Article
    auto &organizations = entities["organizations"]; //an array
    if (!organizations.IsNull()) {
        for (const auto &organization: organizations.GetArray()) {
            //add organization names to article vector named "organizations"
            article.organizations.emplace_back(organization["name"].GetString());
        }
    }

    //8- Store text
    article.text = JSON_document["text"].GetString();

    //9- Tokenize, lowercase, and stemming
    std::istringstream ss (JSON_document["text"].GetString());
    std::string token;
    while(std::getline(ss, token, ' ')){
        //if stop-word, ignore.
        if(stop_words.find(token) != stop_words.end()) continue;
        //punctuation removal
        token.erase(std::remove_if(token.begin(), token.end(), ::ispunct), token.end());
        //lowering. I used std::transform to mutate "token" directly
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        //stem token
        Porter2Stemmer::stem(token);
        //add token to list of tokens in the article.
        article.tokens.emplace_back(token);
    }

    return article;
}

std::vector<Article> Parser::parse_folder(const std::filesystem::directory_entry &folder) {
    std::vector<Article> articles;
    for (const auto &entry: std::filesystem::directory_iterator(folder)) {
        try {
            articles.emplace_back(parse_json(entry));
        } catch (std::exception &e) {
            std::cerr << e.what() << '\n';
        }
    }
    return articles;
}

void Parser::parse(const std::string &kaggle_path) {
    auto kaggle_data_dir = std::filesystem::directory_iterator(kaggle_path);
    std::queue<std::future<std::vector<Article>>> future_queue;
    std::vector<Article> articles;

    //For EACH folder within kaggle_data_dir
    for (const auto &year_dir: kaggle_data_dir) {
        //1- asynchronously call "parse_folder" on each folder. NOTE: std::future will store "parse_folder" return value once available.
        std::future<std::vector<Article>> future_article_vec = std::async(parse_folder, year_dir);

        //2- Add future object to queue.
        future_queue.push(std::move(future_article_vec));
    }

    //What happen here? Ask Pravin to help you understand.
    while (!future_queue.empty()) {
        std::vector<Article> queued_articles = future_queue.front().get();
        articles.insert(articles.end(), queued_articles.begin(), queued_articles.end());
        future_queue.pop();
    }
}
