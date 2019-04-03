#include <catch2/catch.hpp>
#include "../mathExpressionParser/parser.h"
#include "../mathExpressionParser/exception.h"


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
	GIVEN("Input is -number")
	{
		THEN("Parser return is -number")
		{
			Parser parser{ "-2" };
			REQUIRE(boost::get<int>(parser.calculate()) == -2);
		}
	}
	GIVEN("Input is -x+1")
	{
		THEN("Parser return is -x+1")
		{
			Parser parser{ "-x + 1" };
			parser.get_context().value = 1;
			REQUIRE(boost::get<int>(parser.calculate()) == 0);
		}
	}
	GIVEN("Input is ((number))")
	{
		THEN("Parser return is number")
		{
			Parser parser{"((2))"};
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

SCENARIO("ParseFunctionsTests")
{
	GIVEN("input is sin(2)")
	{
		THEN("the output is sin(2)")
		{
			Parser parser{"sin(2)"};
			REQUIRE(boost::get<double>(parser.calculate()) == Approx(sin(2)));
		}
	}
	GIVEN("input is sinsin(2))")
	{
		THEN("the output is sinsin(2))")
		{
			REQUIRE_THROWS_AS(Parser{"sinsin(2))"},lex::InvalidInput);
		}
	}
	GIVEN("input is sin(sin(2))")
	{
		THEN("the output is sin(sin(2))")
		{
			Parser parser{"sin(sin(2))"};
			REQUIRE(boost::get<double>(parser.calculate()) == Approx(sin(sin(2))));
		}
	}
	GIVEN("input is cos(2)")
	{
		THEN("the output is cos(2)")
		{
			Parser parser{"cos(2)"};
			REQUIRE(boost::get<double>(parser.calculate()) == Approx(cos(2)));
		}
	}
	GIVEN("input is tan(2)")
	{
		THEN("the output is tan(2)")
		{
			Parser parser{"tan(2)"};
			REQUIRE(boost::get<double>(parser.calculate()) == Approx(tan(2)));
		}
	}
	GIVEN("input is sin(x) + 1")
	{
		THEN("the output is sin(x) + 1")
		{
			Parser parser{"sin(x) + 1"};
			parser.get_context().value = 0;
			double x = 0.;
			REQUIRE(boost::get<double>(parser.calculate()) == Approx(sin(x) + 1));
		}
	}
	GIVEN("input is sin(x + (1))")
	{
		THEN("the output is sin(x + (1))")
		{
			Parser parser{"sin(x + (1))"};
			parser.get_context().value = 0;
			double x = 0.;
			REQUIRE(boost::get<double>(parser.calculate()) == Approx(sin(x + (1))));
		}
	}
	GIVEN("input is sin(x + sin(1))")
	{
		THEN("the output is sin(x + sin(1))")
		{
			Parser parser{"sin(x + sin(1))"};
			parser.get_context().value = 0;
			double x = 0.;
			REQUIRE(boost::get<double>(parser.calculate()) == Approx(sin(x + sin(1))));
		}
	}
	GIVEN("input is sin(x + (1)) + 2")
	{
		THEN("the output is sin(x + (1)) + 2")
		{
			Parser parser{"sin(x + (1)) + 2"};
			parser.get_context().value = 0;
			double x = 0.;
			REQUIRE(boost::get<double>(parser.calculate()) == Approx(sin(x + (1)) + 2));
		}
	}
	GIVEN("input is sin(x) + cos(x*4)")
	{
		THEN("the output is sin(x) + cos(x*4)")
		{
			Parser parser{"sin(x) + cos(x*4)"};
			parser.get_context().value = 0;
			double x = 0.;
			REQUIRE(boost::get<double>(parser.calculate()) == Approx(sin(x) + cos(x*4)));
		}
	}
}
