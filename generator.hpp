#include <string>
#include <sstream>
#include "parser.hpp"

class Generator
{
	protected:
		std::string outputPath;
		std::string headerPath;
		std::stringstream hss;
		std::stringstream ss;
	public:
		Generator(std::string outputPath, std::string headerPath);
		bool generateWrapper(IOExternalMethodDispatch& method);
		bool write(void);
};