#pragma once
#include <string>
#include <vector>

std::string readFile(std::string filePath);
std::vector<std::string> splitString(std::string str, std::string delim);
bool stringToInt(std::string str, uint32_t& outInt);