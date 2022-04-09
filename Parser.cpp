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


/*
 * @Params:         "json_file" a representation for a json file within the filesystem to retrieve Article data from.
 * @return:         "Article" with attributes derived from the JSON file being read
 * @description:    Read a json_file (usually from a directory_iterator) and returns an
 *                  'Article' object with the attributes derived from the JSON file pointed by the json_file
 */
Article parse_data(const std::filesystem::directory_entry &json_file) {

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
        throw "RapidJSON couldn't parse :-(";
    }

    //6-
    Article article;
    auto &entities = JSON_document["entities"]; //an element of the JSON file containing arrays
    auto &persons = entities["persons"]; //an array
    if (!persons.IsNull()) {
        for (const auto &person: persons.GetArray()) {
            //add person names to article vector named "persons"
            article.persons.emplace_back(person["name"].GetString());
        }
    }
    auto &organizations = entities["organizations"]; //an array
    if (!organizations.IsNull()) {
        for (const auto &organization: organizations.GetArray()) {
            //add organization names to article vector named "organizations"
            article.organizations.emplace_back(organization["name"].GetString());
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
