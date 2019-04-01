#include "parser.h"
#include "lexer.h"

#include <boost/variant.hpp>

namespace mex
{
	using namespace tok;
	using result_type = boost::variant<int, double>;

	template <typename FunctionType>
	result_type eval(FunctionType func,
	                 boost::variant<int, double> v1,
	                 boost::variant<int, double> v2)
	{
		boost::variant<int, double> result;
		if (int* i1 = boost::get<int>(&v1))
		{
			if (int* i2 = boost::get<int>(&v2))
			{
				result = func(*i1, *i2);
			}
			else
			{
				result = func(*i1, boost::get<double>(v2));
			}
		}
		else
		{
			if (int* i2 = boost::get<int>(&v2))
			{
				result = func(*i2, boost::get<double>(v1));
			}
			else
			{
				result = func(boost::get<double>(v1), boost::get<double>(v2));
			}
		}
		return result;
	}

	struct FuncVisitor : public boost::static_visitor<>
	{
		double operator()(int i) const
		{
			return static_cast<double>(i);
		}

		double operator()(double d) const
		{
			return d;
		}
	};


	boost::variant<int, double> eval(Token token, boost::variant<int, double> arg)
	{
		boost::variant<int, double> res;
		auto visitor = [](auto val) { return static_cast<double>(val); };
		if (token.view() == "sin")
		{
			auto d = static_cast<double>(
				boost::apply_visitor(visitor, arg));
			res = std::sin(d);
			return res;
		}
		if (token.view() == "cos")
		{
			res = std::cos(static_cast<double>(boost::apply_visitor(visitor, arg)));
			return res;
		}
		if (token.view() == "tan")
		{
			res = std::tan(static_cast<double>(boost::apply_visitor(visitor, arg)));
			return res;
		}
		assert(false && "This function not supported.");
	}

	Parser::Parser(std::string input): expression_(std::move(input)), position_(0)
	{
		set_tokens(Lexer{expression_}.generate_tokens());
	}

	Parser::Parser(): position_(0)
	{
		expecting_l_paren_ = false;
	}

	void Parser::reset_internal_state()
	{
		position_ = 0;
		expecting_l_paren_ = false;
	}

	boost::variant<int, double> Parser::calculate()
	{
		reset_internal_state();
		return parseAddSub();
	}

	boost::variant<int, double> Parser::parse()
	{
		auto cur = current();
		if (expecting_l_paren_ && !cur.is(l_paren))
		{
			throw lex::InvalidInput("expecting (...");
		}
		switch (cur.getKind())
		{
		case x:
			{
				int number = context_.value;
				move_next();
				return number;
			}
		case function:
			{
				functions_.push(cur);
				expecting_l_paren_ = true;
				move_next();
				result_type res = mex::eval(cur, parseAddSub());
				move_next();
				return res;
			}
		case numeric_constant:
			{
				int number = boost::lexical_cast<int>(cur.to_string());
				move_next();
				return number;
			}
		case minus:
			{
				result_type res = parse();
				boost::apply_visitor([](auto& val) { val = -val; }, res);
				return res;
			}
		case l_paren:
			{
				
				move_next();
				const bool expecting_l_paren = expecting_l_paren_;
				expecting_l_paren_ = false;
				result_type res = parseAddSub();
				if (expecting_l_paren && !functions_.empty())
				{
					functions_.pop();
					return res;
				}
				if (!current().is(r_paren))
				{
					throw lex::InvalidInput("expected )");
				}
				move_next();
				return res;
			}
		case eof:
			throw lex::InvalidInput("empty input.");
		default:
			throw lex::InvalidInput("Error: expected expression...");
		}
	}

	result_type Parser::parseMulDiv()
	{
		result_type res = parse();
		while (true)
		{
			switch (current().getKind())
			{
			case star:
				move_next();
				res = mex::eval(std::multiplies{}, res, parse());
				break;
			case divide:
				{
					move_next();
					const auto v2 = parse();
					res = mex::eval([](auto& a, auto& b) { return a / b; },
					                res, v2);
					break;
				}
			default:
				return res;
			}
		}
	}

	result_type Parser::parseAddSub()
	{
		auto res = parseMulDiv();
		while (true)
		{
			switch (current().getKind())
			{
			case plus:
				{
					move_next();
					auto v2 = parseMulDiv();
					res = mex::eval(std::plus{}, res, v2);
					break;
				}
			case minus:
				{
					move_next();
					auto v2 = parseMulDiv();
					res = mex::eval(std::minus{}, res, v2);
					break;
				}
			default:
				return res;
			}
		}
	}

	void Parser::set_input(std::string input_string)
	{
		expression_ = input_string;
	}

	Context& Parser::get_context()
	{
		return context_;
	}

	Token Parser::current() const
	{
		return tokens_[position_];
	}

	Token Parser::next() const
	{
		return tokens_[position_ + 1];
	}

	void Parser::move_next()
	{
		++position_;
		if(position_ >= tokens_.size())
			throw lex::InvalidInput("missing )...");
	}
}
