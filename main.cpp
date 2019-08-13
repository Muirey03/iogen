#include <string>
#include <iostream>
#include "main.hpp"
#include "parseargs.hpp"
#include "utils.hpp"
#include "parser.hpp"
#include "generator.hpp"

void usage()
{
	std::cout << "Usage: iogen input_file [-o output_file] [-h header_output_file]" << std::endl;
	exit(EXIT_FAILURE);
}

int main(int argc, char** argv)
{
	//get args
	std::string inputFile;
	std::string outputFile;
	std::string headerFile;
	parse_args(argc, argv, inputFile, outputFile, headerFile);
	
	//read input file
	std::string input = readFile(inputFile);

	//parse input:
	Parser parser(input);
	std::vector<IOExternalMethodDispatch> methods = parser.start();
	
	//generate output:
	Generator gen(outputFile, headerFile);	
	for (IOExternalMethodDispatch method : methods)
	{
		bool success = gen.generateWrapper(method);
		if (!success)
		{
			std::cout << "Error: Unable to generate wrapper" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
	bool success = gen.write();
	if (!success)
	{
		std::cout << "Error: Unable to write to output file" << std::endl;
		exit(EXIT_FAILURE);
	}
	return 0;
}