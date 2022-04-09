/**
 * @Author(s):      Pravin and Kassi
 * @filename:       Parser.cpp
 * @date:           04-05-2022
 * @description:    This class is the parser of the project
 *                  It is responsible for:
 *                      - reading and processing JSON files asynchronously
 *                      - Storing processed JSON file (Article) into std::vector
 */

#ifndef INC_22S_FINAL_PROJ_PARSER_H
#define INC_22S_FINAL_PROJ_PARSER_H

#include <filesystem>
#include <fstream>
#include <iostream>
#include <thread>

#include <future>
#include <queue>
#include <vector>
#include <exception>
#include <string>

#include "Article.h"
#include "rapidjson/document.h"
#include "thread_pool.h"

class Parser {

private:
    /// \param json_file    -> Path to JSON file within the filesystem
    /// \return Article     -> An article object
    /// \description        -> Reads, parses, extracts, and process data (persons, organizations,
    ///                     text) from raw JSON file
    static Article parse_data(const std::filesystem::directory_entry &json_file);

    ///
    /// \param folder       -> Path to a folder withing the filesystem
    /// \return Vector      -> A set of processed JSON (Article objects)
    /// \description        -> Call "parse_data" on EACH JSON file within "folder"
    static std::vector<Article> parse_folder(const std::filesystem::directory_entry &folder);

public:
    ///
    /// \param kaggle_path  -> Path the kaggle folder (data set folder)
    /// \return void        -> N.A.
    /// \description        -> asynchronously call "parse_folder" on EACH folder within
    ///                     "kaggle_path"
    static void build_data(const std::string& kaggle_path);
};


#endif //INC_22S_FINAL_PROJ_PARSER_H
