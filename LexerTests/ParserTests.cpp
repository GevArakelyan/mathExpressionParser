#include <catch2/catch.hpp>
#include "../mathExpressionParser/parser.h"
#include "../mathExpressionParser/lexer.h"


#include <boost/variant.hpp>

using namespace mex;


SCENARIO("ParserTests")
{
	GIVEN("an empty input.")
	{
		THEN("Parser will return 0.")
		{
			Parser parser{""};
			REQUIRE_THROWS_AS(parser.calculate(),lex::InvalidInput);
		}
	}
	GIVEN("Input is number")
	{
		THEN("Parser return is number")
		{
			Parser parser{"2"};
			REQUIRE(boost::get<int>(parser.calculate()) == 2);
		}
	}
	GIVEN("Input is x")
	{
		THEN("Parser return is x")
		{
			int x = 324;

			Parser parser{std::to_string(x)};
			REQUIRE(boost::get<int>(parser.calculate()) == x);
		}
	}
	GIVEN("Input is x+1")
	{
		THEN("Parser return is x+1")
		{
			int x = 324;

			Parser parser{std::to_string(x) + "+1"};
			REQUIRE(boost::get<int>(parser.calculate()) == x+1);
		}
	}
	GIVEN("Input is x*2")
	{
		THEN("Parser return is x*2")
		{
			int x = 324;

			Parser parser{"x * 2"};
			parser.get_context().value = x;
			REQUIRE(boost::get<int>(parser.calculate()) == x*2);
		}
	}
	GIVEN("Input is 34*2")
	{
		THEN("Parser return is 68")
		{
			Parser parser{"34 * 2"};
			REQUIRE(boost::get<int>(parser.calculate()) == 68);
		}
	}
	GIVEN("Input is (1+1)*(3)")
	{
		THEN("Parser return is 6")
		{
			Parser parser{"(1+1)*(3)"};
			REQUIRE(boost::get<int>(parser.calculate()) == 6);
		}
	}
	GIVEN("Input is (1+1)* 3")
	{
		THEN("Parser return is 6")
		{
			Parser parser{"(1+1)* 3"};
			REQUIRE(boost::get<int>(parser.calculate()) == 6);
		}
	}
	GIVEN("Input is (1*2)* 3")
	{
		THEN("Parser return is 6")
		{
			Parser parser{"(1*2)* 3"};
			REQUIRE(boost::get<int>(parser.calculate()) == 6);
		}
	}
	GIVEN("Input is (1*2)* (2*1)")
	{
		THEN("Parser return is 6")
		{
			Parser parser{"(1*2)* (2*1)"};
			REQUIRE(boost::get<int>(parser.calculate()) == 4);
		}
	}
	GIVEN("Input is (1*2) + (2*1)")
	{
		THEN("Parser return is 6")
		{
			Parser parser{"(1*2) + (2*2)"};
			REQUIRE(boost::get<int>(parser.calculate()) == 6);
		}
	}
	GIVEN("Input is (x*2) + (x * 3) and x == 1")
	{
		THEN("Parser return is 5")
		{
			Parser parser{"(x*2) + (x * 3)"};
			parser.get_context().value = 1;
			REQUIRE(boost::get<int>(parser.calculate()) == 5);
		}
	}

}