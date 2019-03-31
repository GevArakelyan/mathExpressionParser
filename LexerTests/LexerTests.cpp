#include <catch2/catch.hpp>
#include "../mathExpressionParser/lexer.h"

SCENARIO("LexerTests")
{
	GIVEN("Lexer with an empty string")
	{
		mex::Lexer lexer{ "" };
		THEN("Will generate 1 token. (eof)")
		{
			REQUIRE(lexer.generate_tokens().size() == 1);
		}
		WHEN("input is 1234")
		{
			lexer.set_expression_to_parse("1234 ");
			THEN("output contains 2 elements")
			{
				REQUIRE(lexer.generate_tokens().size() == 2);
			}
		}
		WHEN("input is 1234. ")
		{
			lexer.set_expression_to_parse("1234. ");
			THEN("InvalidInput exception raised.")
			{
				REQUIRE_THROWS_AS(lexer.generate_tokens(), lex::InvalidInput);
			}
		}
		WHEN("input is sin")
		{
			lexer.set_expression_to_parse("sin");
			THEN("output contains 2 elements")
			{
				REQUIRE(lexer.generate_tokens().size() == 2 );
			}
		}
		WHEN("input is sing")
		{
			lexer.set_expression_to_parse("sing");
			THEN("InvalidInput exception raised.")
			{
				REQUIRE_THROWS_AS(lexer.generate_tokens(), lex::InvalidInput);
			}
		}
		WHEN("input is san")
		{
			lexer.set_expression_to_parse("san");
			THEN("InvalidInput exception raised.")
			{
				REQUIRE_THROWS_AS(lexer.generate_tokens(), lex::InvalidInput);
			}
		}
		WHEN("input is (1+1)")
		{
			lexer.set_expression_to_parse("(1+1)");
			THEN("output contains 6 elements")
			{
				REQUIRE(lexer.generate_tokens().size() == 6);
			}
		}
		WHEN("input is 1+1")
		{
			lexer.set_expression_to_parse("1 + 1 ");
			THEN("output contains 4 elements")
			{
				REQUIRE(lexer.generate_tokens().size() == 4);
			}
		}
		WHEN("input is (52+990)")
		{
			lexer.set_expression_to_parse("(52+990)");
			THEN("output contains 6 elements")
			{
				REQUIRE(lexer.generate_tokens().size() == 6);
			}
		}
		
	}
}

