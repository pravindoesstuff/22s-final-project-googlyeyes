//
// Created by pravin on 4/6/22.
//

#include <filesystem>
#include <iostream>
#include <fstream>
#include "Parser.h"
#include "rapidjson/document.h"

using namespace rapidjson;
using std::ifstream;

void write_data(const std::filesystem::directory_entry& entry) {
    ifstream file(entry.path());
    string json_string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());\
    file.close();
    Document document;
    document.Parse(json_string.c_str());
}

void Parser::build_data(string folderpath) {
    for (const auto &entry: std::filesystem::directory_iterator(folderpath)) {
        write_data(entry);
    }
}
