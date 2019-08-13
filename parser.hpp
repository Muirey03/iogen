#pragma once
#include <string>
#include <vector>

struct IOExternalMethodDispatch
{
	uint32_t selector;
    std::string function;
    uint32_t scalarInputCount;
    uint32_t structureInputSize;
    uint32_t scalarOutputCount;
    uint32_t structureOutputSize;
};

class Parser
{
protected:
	std::string input;
	static bool findMatching(std::string str, int index, int& outIndex);
	static bool nextOccurence(std::string str, int starting, char needle, int& outIndex);
	static bool parseMethod(std::string str, uint32_t selector, IOExternalMethodDispatch& outMethod);
public:
	Parser(std::string input);
	std::vector<IOExternalMethodDispatch> start();
};