#include "utils.hpp"

#include <iostream>
#include <fstream>
#include <sstream>

#include "../core/constants.hpp"

// Using declarations for std
using std::string, std::ifstream, std::cerr, std::endl;
using std::pair;

// Read a file from a provided path
string readFile(const string& path) {
    ifstream file(path);
    if (!file.is_open()) {
        cerr << "Failed to open file: " << path << endl;
        exit(EXIT_FAILURE);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

// Get the two file paths in a shader combination string
pair<string, string> splitKey(const string& key) {
    size_t index = key.find("|");
    if (index == std::string::npos) {
        cerr << "Failed to find pipe in provided key: " << key << endl;
        exit(EXIT_FAILURE);
    }

    string path1 = key.substr(0, index);
    string path2 = key.substr(index + 1);

    return pair<string, string>(path1, path2);
}

// Create the key for the function above
string createKey(const string& vertPath, const string& fragPath) {
    return vertPath + (string)"|" + fragPath;
}