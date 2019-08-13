#include "parser.hpp"
#include "utils.hpp"
#include <string>
#include <iostream>
#include <vector>
#include <sstream>

Parser::Parser(std::string input)
{
	this->input = input;
}

std::vector<IOExternalMethodDispatch> Parser::start()
{
	std::vector<IOExternalMethodDispatch> vec;
	bool success;
	std::vector<std::string> strMethods;
	int head;

	//find first occurence of '{'
	int startIndex;
	success = nextOccurence(input, -1, '{', startIndex);
	if (!success) goto fail;

	//find matching '}'
	int lastIndex;
	success = findMatching(input, startIndex, lastIndex);
	if (!success) goto fail;

	//loop through methods:
	head = startIndex;
	do
	{
		//find next occurence of '{'
		success = nextOccurence(input, head, '{', head);
		if (!success) break;
		//find matching '}'
		int matching;
		success = findMatching(input, head, matching);
		if (!success) goto fail;
		//truncate string and add it to the vector:
		std::string sub = input.substr(head + 1, matching - head - 1);
		strMethods.push_back(sub);
		//update head:
		head = matching;
	} while (head < lastIndex);

	//parse each method:
	for (int i = 0; i < strMethods.size(); i++)
	{
		IOExternalMethodDispatch method;
		success = parseMethod(strMethods[i], i, method);
		if (!success) goto fail;
		vec.push_back(method);
	}
	return vec;
fail:
	std::cout << "Error: Unable to parse input" << std::endl;
	exit(EXIT_FAILURE);
}

bool Parser::nextOccurence(std::string str, int starting, char needle, int& outIndex)
{
	for (int i = starting + 1; i < str.length() && i >= 0; i++)
	{
		if (str[i] == needle)
		{
			outIndex = i;
			return true;
		}
	}
	return false;
}

bool Parser::findMatching(std::string str, int index, int& outIndex)
{
	int indentation = 1;
	size_t strLen = str.length();
	for (int i = index + 1; i < strLen; i++)
	{
		char c = str[i];
		if (c == '{')
			indentation++;
		if (c == '}')
		{
			indentation--;
			if (indentation == 0)
			{
				outIndex = i;
				return true;
			}
		}
	}
	return false;
}

//(not perfect but should usually work)
bool Parser::parseMethod(std::string str, uint32_t selector, IOExternalMethodDispatch& outMethod)
{
	//split into components:
	std::vector<std::string> components = splitString(str, ",");
	if (components.size() != 5)
		return false;
	outMethod.selector = selector;

	//parse function name:
	std::vector<std::string> nameComps = splitString(components[0], "::");
	outMethod.function = nameComps[nameComps.size() - 1];
	
	//parse scalarInputCount:
	uint32_t scalarInputCount;
	if (!stringToInt(components[1], scalarInputCount))
		return false;
	outMethod.scalarInputCount = scalarInputCount;
	
	//parse structureInputSize:
	uint32_t structureInputSize;
	if (!stringToInt(components[2], structureInputSize))
		return false;
	outMethod.structureInputSize = structureInputSize;

	//parse scalarOutputCount:
	uint32_t scalarOutputCount;
	if (!stringToInt(components[3], scalarOutputCount))
		return false;
	outMethod.scalarOutputCount = scalarOutputCount;

	//parse structureOutputSize:
	uint32_t structureOutputSize;
	if (!stringToInt(components[4], structureOutputSize))
		return false;
	outMethod.structureOutputSize = structureOutputSize;

	return true;
}