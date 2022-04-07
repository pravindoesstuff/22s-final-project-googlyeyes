//
// Created by pravin on 4/6/22.
//

#include <filesystem>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <queue>
#include "Parser.h"
#include "Article.h"
#include "rapidjson/document.h"
#include "thread_pool.h"

using namespace rapidjson;
using std::ifstream, std::vector, std::thread, std::queue;

Article write_data(const std::filesystem::directory_entry &entry) {
    ifstream file(entry.path());
    string json_string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    file.close();
    Article article;
    Document document;
    document.Parse(json_string.c_str());
    auto &entities = document["entities"];
    auto &persons = entities["persons"];
    if (!persons.IsNull()) {
        for (auto &person: persons.GetArray()) {
            article.persons.emplace_back(person["name"].GetString());
        }
    }
    auto &organizations = entities["organizations"];
    if (!organizations.IsNull()) {
        for (auto &organization: organizations.GetArray()) {
            article.persons.emplace_back(organization["name"].GetString());
        }
    }
    return article;
}

void Parser::build_data(const string &folderpath) {
    ThreadPool pool;
    for (const auto &entry: std::filesystem::directory_iterator(folderpath)) {
        pool.Submit(write_data, entry);
    }
}
