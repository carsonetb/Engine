#pragma once
#include <string>
#include <vector>

std::string ltrim(std::string s);
std::string rtrim(std::string s);
std::string trim(std::string s);
std::string trimChar(std::string s, char toTrim);
std::string readFile(std::string fileName);
std::vector<std::string> splitString(std::string toSplit, std::string character);
bool startsWith(const std::string &str, const std::string &prefix);
std::vector<std::string> insertVectorAtIndString(int i, std::vector<std::string> addedInto, std::vector<std::string> toAdd);
std::string removeAllFirstChars(std::string input, std::string character);
