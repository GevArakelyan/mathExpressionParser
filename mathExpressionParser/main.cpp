#include "parser.h"
#include "lexer.h"
#include <iostream>

int main(int argc, char* argv[])
{
	std::string expression;
	std::cin >> expression;
	mex::Parser parser(expression);
	int start = 0;
	int end = 0;
	std::cin >> start;
	std::cin >> end;

	std::cout << "\n";
	std::cout << " x    y\n";

	for (int x = start; x  <= end; ++x)
	{
		parser.get_context().value = x;
		std::cout << x << "    " << parser.calculate();
	}



	return 0;
}
