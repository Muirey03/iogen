#include "parseargs.hpp"
#include <string>
#include <iostream>
#include <sstream>
#include "main.hpp"

std::string lastPathComponent(std::string path)
{
	std::stringstream ss(path);
	std::string token;
	while (std::getline(ss, token, '/')) {}
	return token;
}

std::string removeFileExtension(std::string fileName)
{
	size_t index = fileName.find_last_of(".");
	if (index == std::string::npos)
		return fileName;
	return fileName.substr(0, index);
}

void parse_args(int argc, char** argv, std::string& input, std::string& output, std::string& header)
{
	std::string inputFile;
	std::string outputFile;
	std::string headerFile;
	bool foundOutput = false;
	bool foundHeader = false;

	if (argc < 2)
		usage();

	inputFile = argv[1];
	for (int i = 2; i < argc; i++)
	{
		if (std::string(argv[i]) == "-o")
		{
			if (i + 1 < argc)
			{
				outputFile = argv[i+1];
				foundOutput = true;
			}
			else
			{
				usage();
			}
			//don't need to check next arg
			i++;
		}
		else if (std::string(argv[i]) == "-h")
		{
			if (i + 1 < argc)
			{
				headerFile = argv[i+1];
				foundHeader = true;
			}
			else
			{
				usage();
			}
			//don't need to check next arg
			i++;
		}
		else
		{
			if (argv[i][0] == '-')
				std::cout << "Unrecognised option \"" << argv[i] << "\"" << std::endl;
			usage();
		}
	}

	if (!foundOutput)
	{
		std::string inputName = removeFileExtension(lastPathComponent(inputFile));
		outputFile = inputName + ".c";
	}
	if (!foundHeader)
	{
		std::string outputName = removeFileExtension(outputFile);
		headerFile = outputName + ".h";
	}

	input = inputFile;
	output = outputFile;
	header = headerFile;
}