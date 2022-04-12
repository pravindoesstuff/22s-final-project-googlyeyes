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


    //5- Store "persons name" into Article
    Article article;
    auto &entities = JSON_document["entities"]; //an element of the JSON file containing arrays
    auto &persons = entities["persons"]; //an array
    if (!persons.IsNull()) {
        for (const auto &person: persons.GetArray()) {
            //add person names to article vector named "persons"
            article.persons.emplace_back(person["name"].GetString());
        }
    }

    //6- Store "organizations name" into Article
    auto &organizations = entities["organizations"]; //an array
    if (!organizations.IsNull()) {
        for (const auto &organization: organizations.GetArray()) {
            //add organization names to article vector named "organizations"
            article.organizations.emplace_back(organization["name"].GetString());
        }
    }

    //7- Store text
    article.text = JSON_document["text"].GetString();

    //8- Tokenize, lowercase, and stemming
    std::istringstream ss(article.text);
    std::string token;
    while (std::getline(ss, token, ' ')) {
        //if stop-word, ignore.
        if (stop_words.find(token) != stop_words.end()) continue;
        //punctuation removal
        token.erase(std::remove_if(token.begin(), token.end(), ::ispunct), token.end());
        //lowering. I used std::transform to mutate "token" directly
        std::transform(token.begin(), token.end(), token.begin(), ::tolower);
        //stem token
        Porter2Stemmer::stem(token);
        //add token to list of tokens in the article.
        article.tokens.emplace_back(token);
    }

    //9- Store ID
    article.id = JSON_document["uuid"].GetString();

    //10- Get Title
    article.title = JSON_document["title"].GetString();

    return article;
}

std::vector<Article> Parser::parse(const std::filesystem::path &root_folder_path) {
    std::vector<Article> articles;

    auto data_dir = std::filesystem::directory_iterator(root_folder_path);
    std::queue<std::future<Article>> future_queue;

    //For EACH folder within data_dir
    for (const auto &element: data_dir) {
        if (element.is_directory()) {
            // Stores articles from the recursive call
            std::vector<Article> temp_articles = parse(element.path());
            // Adds the articles into the current 'articles' vector
            articles.insert(articles.begin(), temp_articles.begin(), temp_articles.end());
        } else {
            //1- Submit parse_json routine to pool and store the future
            std::future<Article> future_article = thread_pool.Submit(parse_json, element);
            //2- Add future object to queue.
            future_queue.push(std::move(future_article));
        }
    }

    //What happen here? Ask Pravin to help you understand.
    while (!future_queue.empty()) {
        Article article = future_queue.front().get();
        articles.emplace_back(article);
        future_queue.pop();
    }

    return articles;
}
