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
		void reset_internal_state();
		boost::variant<int, double> calculate();
		boost::variant<int, double> parse();
		boost::variant<int, double> parseMulDiv();
		boost::variant<int, double> parseAddSub();


		void set_input(std::string input_string);

		Context& get_context();

		tok::Token current() const;
		tok::Token next() const;
		void move_next();

	private:

		void set_tokens(std::vector<tok::Token> tokens)
		{
			this->tokens_ = std::move(tokens);
		}


		std::string expression_;
		size_t position_;
		Context context_{};
		std::vector<tok::Token> tokens_;
		std::stack<tok::Token> functions_;
		bool expecting_l_paren_;
		
	};

	
}
