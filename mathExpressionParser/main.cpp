#include "parser.h"
#include "lexer.h"
#include <iostream>

int main(int argc, char* argv[])
{
	std::string expression = "(x+sin(x*cos(x))+(234*23)";
	std::cin >> expression;
	mex::Parser parser(expression);
	int start = 0;
	int end = 0;
	std::cin >> start;
	std::cin >> end;

	std::cout << "\n";
	std::cout << " x    y\n";
	try
	{
		
	
	for (int x = start; x  <= end; ++x)
	{
		parser.get_context().value = x;
		std::cout << x << "    " << parser.calculate() << "\n";
	}
	}catch (std::exception& e)
	{
		std::cerr << std::endl;
		std::cerr << "Error: " <<  e.what() << std::endl;
	}


	return 0;
}
