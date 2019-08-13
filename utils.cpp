#include "utils.hpp"
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>

std::string readFile(std::string filePath)
{
	std::ifstream fStream(filePath);
	if (fStream.fail())
	{
		std::cout << "Error: Failed to open file \"" << filePath << "\"" << std::endl;
		exit(EXIT_FAILURE);
	}
	std::stringstream ss;
	ss << fStream.rdbuf();
	return ss.str();
}

std::vector<std::string> splitString(std::string str, std::string delim)
{
	std::vector<std::string> vec;
	std::size_t current, previous = 0;
    current = str.find(delim);
    while (current != std::string::npos)
	{
        vec.push_back(str.substr(previous, current - previous));
        previous = current + delim.size();
        current = str.find(delim, previous);
    }
    vec.push_back(str.substr(previous, current - previous));
	return vec;
}

bool stringToInt(std::string str, uint32_t& outInt)
{
	//parse string:
	bool useHex = str.find("0x") != std::string::npos;
	std::istringstream iss(str);
	if (useHex)
		iss >> std::hex >> outInt;
	else
		iss >> outInt;
	return !iss.fail();
}