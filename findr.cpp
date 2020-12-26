//
// Created by 3top1a on 25/12/2020.
//

//Program structure:
// 1) get the path to scan
// 2) loop thru all files
// 3) look for paterns
// 4) return any findings
//

#include <iostream>
#include <string>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <vector>
#include <regex>
#include "findr.h"
#include <json/json.h>

bool hasEnding(std::string full, std::string end);

int main() {
    // Get home path
    std::string path;
    //https://stackoverflow.com/questions/2552416/how-can-i-find-the-users-home-dir-in-a-cross-platform-manner-using-c
#ifdef unix
    path = getenv("HOME");
#elif defined(_WIN32)
    HomeDirectory = getenv("HOMEDRIVE");
    const char*Homepath = getenv("HOMEPATH");
    path = malloc(strlen(HomeDirectory)+strlen(Homepath)+1);
    strcat(HomeDirectory, Homepath);
#endif

    //TODO
    //Argument parser
    path = "/home/not-a-weeb/Documents/GitHub/file-findr/demo_files";

    // Get all matchers
    int count = 0;
    std::vector<std::string> matchers;
    using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
    for (auto dir_Entry: std::filesystem::recursive_directory_iterator("./matchers/")) {
        if (hasEnding(dir_Entry.path().string(), ".json")) {
            std::ifstream ifs(dir_Entry.path().string());
            std::string content((std::istreambuf_iterator<char>(ifs)),
                                (std::istreambuf_iterator<char>()));
            matchers.push_back(content);
            count++;
        }
    }
    std::cout << "Found " << count << " matcher(s)!" << std::endl;

    // Match all files
    try {
        using recursive_directory_iterator = std::filesystem::recursive_directory_iterator;
        for (auto dir_Entry: std::filesystem::recursive_directory_iterator(path)) {
            const auto filenameStr = dir_Entry.path().string();

            std::ifstream ifs(filenameStr);
            std::string content((std::istreambuf_iterator<char>(ifs)),
                                (std::istreambuf_iterator<char>()));

            //For all matchers, check them
            for (int i = 0; i < matchers.size(); ++i) {
                Json::CharReaderBuilder builder{};
                auto reader = std::unique_ptr<Json::CharReader>(builder.newCharReader());
                Json::Value root{};
                std::string errors{};
                const auto is_parsed = reader->parse(matchers[i].c_str(),
                                                     matchers[i].c_str() + matchers[i].length(),
                                                     &root,
                                                     &errors);

                std::smatch sm;
                std::regex r = (std::regex) root["regex"].asString();
                if (std::regex_search(content, sm, r)) {
                    for (int i = 0; i < sm.size(); i++) {
                        //Match!
                        std::cout << "\033[4;32m " << filenameStr << " \033[0m" << std::endl;
                        std::cout << sm[i] << std::endl;
                    }
                }
            }
        }
    }
    catch (const std::filesystem::__cxx11::filesystem_error &e) {
        std::cout << "\033[1;31m Error! probably insufficient privileges \033[0m" << std::endl;
        std::cout << e.what() << std::endl;
        return 1;
    }

    return 0;
}

bool hasEnding(std::string fullString, std::string ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}