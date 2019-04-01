#pragma once

#include "lexer.h"
#include <vector>
#include <boost/variant/variant.hpp>


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

		Parser();
		boost::variant<int, double> calculate();
		boost::variant<int, double> parse();
		boost::variant<int, double> parseMulDiv();
		boost::variant<int, double> parseAddSub();

		void set_tokens(std::vector<tok::Token> tokens)
		{
			this->tokens_ = std::move(tokens);
		}

		Context& get_context()
		{
			return context_;
		}

		tok::Token current() const;
		tok::Token next() const;
		void move_next();

	private:
		size_t position_ = 0;
		Context context_{};
		std::vector<tok::Token> tokens_;
		tok::Token current_;
		tok::Token next_;
		tok::Token prev_;

	};

	
}
