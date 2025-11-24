#include "managers/config.hpp"
#include <string>

std::string ASSETS_CONFIG = "assets.ini";
std::map<std::string, std::string> asset_config;

inline void ltrim(std::string &s) {
    s.erase(s.begin(), std::find_if(s.begin(), s.end(), [](unsigned char ch) {
                return !std::isspace(ch);
            }));
}

inline void rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(),
                         [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
}

void print_map(const std::map<std::string, std::string> &m) {
    for (auto &[key, value] : m) {
        std::string msg = '[' + key + "] = " + value;
    }
}

void LoadConfig(const char *fileName) {
    char *fileContent = LoadFileText(fileName);
    std::string delimiter = "=";

    if (fileContent == NULL) {
        printf("Failed to load config file: %s\n", fileName);
        return;
    }

    int lineCount;
    std::string section = "";
    char **lines = TextSplit(fileContent, '\n', &lineCount);

    for (int i = 0; i < lineCount; i++) {
        std::string line = lines[i];
        if (line.length() == 0 || line[0] == '#' || line[0] == '\n') {
            continue;
        }

        // sections
        if (line[0] == '[') {
            section = line.substr(1, line.length() - 2);
            continue;
        }

        long split_pos = line.find(delimiter);

        std::string key = line.substr(0, split_pos);
        rtrim(key);
        key = section + "." + key;

        std::string value = line.substr(split_pos + 1, line.length());
        ltrim(value);

        asset_config.insert({key, value});
    }

    print_map(asset_config);

    UnloadFileText(fileContent);
}

void UnloadConfig() {}
