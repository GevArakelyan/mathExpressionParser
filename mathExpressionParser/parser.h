#pragma once

#include <vector>
#include <boost/variant/variant.hpp>
#include "token.h"
#include <stack>


namespace mex
{

	struct Context
	{
		int value;
	};

	/*
	 * parsing simple math expressions 
	 * example (x + 6 ) * sin ( x * 9 ) 
	 * currently only sin cos tan functions are supported.
	 */
	class Parser
	{
	public:

		explicit Parser(std::string input);

		explicit Parser();
		//TODO extract calculate to separate class. class should have single responsibility.
		boost::variant<int, double> calculate();
		boost::variant<int, double> parse();
		boost::variant<int, double> parseMulDiv();
		boost::variant<int, double> parseAddSub();


		void set_input(std::string input_string);

		Context& get_context();

	private:

		tok::Token current() const;
		tok::Token next() const;
		void move_next();

		void reset_internal_state();

		void set_tokens(std::vector<tok::Token> tokens);


		std::string expression_;			//input expression to parse and calculate value.
		Context context_{};					// used to store identifier->value pair information.
		size_t position_;					//current position within tokens_
		std::vector<tok::Token> tokens_;	//tokens produced by lexer.
		std::stack<tok::Token> functions_;  //use to see how deep in call stack you are.
		bool expecting_l_paren_{};			//true if we are expecting next token to be left parentheses.
	};

	
}
