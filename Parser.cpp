//
// Created by pravin on 4/6/22.
//

#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

#include <future>
#include <queue>

#include <vector>
#include "Parser.h"
#include "Article.h"
#include "rapidjson/document.h"
#include "thread_pool.h"

#include <exception>

class InvalidFile {

};


/*
 * @Params entry is the stl representation for the json file within the filesystem to retrieve Article data from.
 * @return Article with attributes derived from the JSON file being read
 * @description Intakes a constant directory entry (usually from a directory_iterator) and returns an 'Article' with the attributes derived from the JSON file pointed by the entry
 */
Article parse_data(const std::filesystem::directory_entry &entry) {
    std::ifstream file(entry.path());
    string json_string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    Article article;
    rapidjson::Document document;
    document.Parse(json_string.c_str());
    if (!document.IsObject()) {
        std::cerr << "RapidJSON couldn't parse " << entry.path() << '\n';
        return article;
    }
    auto &entities = document["entities"];
    auto &persons = entities["persons"];
    if (!persons.IsNull()) {
        for (const auto &person: persons.GetArray()) {
            article.persons.emplace_back(person["name"].GetString());
        }
    }
    auto &organizations = entities["organizations"];
    if (!organizations.IsNull()) {
        for (const auto &organization: organizations.GetArray()) {
            article.persons.emplace_back(organization["name"].GetString());
        }
    }
    return article;
}

std::vector<Article> parse_folder(const std::filesystem::directory_entry &folder) {
    std::vector<Article> articles;
    for (const auto &entry: std::filesystem::directory_iterator(folder)) {
        articles.emplace_back(parse_data(entry));
    }
    return articles;
}

void Parser::build_data(const string &kaggle_path) {
    auto kaggle_data_dir = std::filesystem::directory_iterator(kaggle_path);
    std::queue<std::future<std::vector<Article>>> future_queue;
    std::vector<Article> articles;
    for (const auto &year_dir: kaggle_data_dir) {
        std::future<vector<Article>> future_article_vec = std::async(parse_folder, year_dir);
        future_queue.push(std::move(future_article_vec));
    }
    while (!future_queue.empty()) {
        std::vector<Article> queued_articles = future_queue.front().get();
        articles.insert(articles.end(), queued_articles.begin(), queued_articles.end());
        future_queue.pop();
    }
}
