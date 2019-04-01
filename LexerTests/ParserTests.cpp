#include <catch2/catch.hpp>
#include "../mathExpressionParser/parser.h"


#include <boost/variant.hpp>

using namespace mex;


SCENARIO("ParserTests")
{
	GIVEN("an empty input.")
	{
		THEN("Parser will return 0.")
		{
			Parser parser;
			Lexer lexer{""};
			parser.set_tokens(lexer.generate_tokens());
			REQUIRE_THROWS_AS(parser.calculate(),lex::InvalidInput);
		}
	}
}