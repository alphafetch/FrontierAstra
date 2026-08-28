#ifndef UTILS_UTILS_HPP
#define UTILS_UTILS_HPP

#include <string>
#include <utility>

// Read a file from a provided path
std::string readFile(const std::string& path);

// Get the two file paths in a shader combonation string
std::pair<std::string, std::string> splitKey(const std::string& key);

// Create the key for the function above
std::string createKey(const std::string& vertPath, const std::string& fragPath);

#endif