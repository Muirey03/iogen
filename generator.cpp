#include "generator.hpp"
#include <string>
#include <sstream>
#include "parser.hpp"
#include <fstream>

using namespace std;

Generator::Generator(std::string outputPath, std::string headerPath)
{
	this->outputPath = outputPath;
	this->headerPath = headerPath;
	ss = std::stringstream();
	hss = std::stringstream();
	//add includes:
	ss << "#include <mach/mach.h>\n";
	ss << "#include <IOKit/IOKitLib.h>\n" << endl;
	hss << "#include <mach/mach.h>\n";
	hss << "#include <IOKit/IOKitLib.h>\n" << endl;
}

bool Generator::generateWrapper(IOExternalMethodDispatch& method)
{
	//function signature:
	std::stringstream tmpss;
	tmpss << "kern_return_t " << method.function << "(io_connect_t client";
	if (method.scalarInputCount != 0xffffffff)
		for (int i = 0; i < method.scalarInputCount; i++)
			tmpss << ", uint64_t input" << i;
	else
		tmpss << ", uint64_t* input, uint32_t inputCnt";
	if (method.structureInputSize)
		tmpss << ", void* inputStruct, size_t inputStructSize";
	if (method.scalarOutputCount != 0xffffffff)
		for (int i = 0; i < method.scalarOutputCount; i++)
			tmpss << ", uint64_t* output" << i;
	else
		tmpss << ", uint64_t* output, uint32_t* outputCnt";
	if (method.structureOutputSize)
		tmpss << ", void* outputStruct, size_t* outputStructSize";

	ss << tmpss.str();
	hss << tmpss.str();

	ss << ")\n{" << endl;
	hss << ");" << endl;

	//function definition:
	//scalar input:
	if (method.scalarInputCount)
	{
		if (method.scalarInputCount == 0xffffffff)
		{
			ss << "\tif (inputCnt && !input)\n";
			ss << "\t\treturn kIOReturnBadArgument;" << endl;
		}
		else
		{
			ss << "\tconst uint64_t input[] = {\n";
			for (int i = 0; i < method.scalarInputCount; i++)
			{
				ss << "\t\tinput" << i << ",\n";
			}
			ss << "\t};" << endl;
		}
	}
	//structure input:
	if (method.structureInputSize)
	{
		ss << "\tif (inputStructSize && !inputStruct)\n";
		ss << "\t\treturn kIOReturnBadArgument;" << endl;
	}
	//scalar output:
	if (method.scalarOutputCount)
	{
		if (method.scalarOutputCount == 0xffffffff)
		{
			ss << "\tif (!output || !outputCnt)\n";
			ss << "\t\treturn kIOReturnBadArgument;" << endl;
		}
		else
		{
			ss << "\tuint32_t outputCnt = " << method.scalarOutputCount << ";\n";
			ss << "\tuint64_t output[" << method.scalarOutputCount << "];" << endl;
		}
	}
	//structure output:
	if (method.structureOutputSize)
	{
		ss << "\tif (!outputStruct || !outputStructSize)\n";
		ss << "\t\treturn kIOReturnBadArgument;" << endl;
	}

	//call IOConnectCallMethod:
	ss << "\tkern_return_t kr = IOConnectCallMethod(client, ";
	ss << method.selector << ", ";
	ss <<( method.scalarInputCount ? "input, " : "NULL, ");
	if (method.scalarInputCount != 0xffffffff)
		ss << method.scalarInputCount << ", ";
	else
		ss << "inputCnt, ";
	ss << (method.structureInputSize ? "inputStruct, " : "NULL, ");
	ss << (method.structureInputSize ? "inputStructSize, " : "0, ");
	ss << (method.scalarOutputCount ? "output, " : "NULL, ");
	if (method.scalarOutputCount != 0xffffffff)
		ss << (method.scalarOutputCount ? "&outputCnt, " : "NULL, ");
	else
		ss << "outputCnt, ";
	ss << (method.structureOutputSize ? "outputStruct, " : "NULL, ");
	ss << (method.structureOutputSize ? "outputStructSize);" : "NULL);") << endl;
	//write outputs back:
	if (method.scalarOutputCount != 0xffffffff)
	{
		for (int i = 0; i < method.scalarOutputCount; i++)
		{
			ss << "\tif (output" << i << ")\n";
			ss << "\t\t*output" << i << " = output[" << i << "];\n";
		}
	}
	//return:
	ss << "\treturn kr;\n";
	ss << "}\n" << endl;
	return true;
}

bool Generator::write(void)
{
	std::ofstream outFile(outputPath, std::ofstream::out | std::ofstream::trunc);
	outFile << ss.rdbuf();
	std::ofstream outHeader(headerPath, std::ofstream::out | std::ofstream::trunc);
	outHeader << hss.rdbuf();
	return !outFile.fail() && !outHeader.fail();
}