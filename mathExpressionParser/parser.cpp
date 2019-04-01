#include "parser.h"

#include <boost/variant.hpp>
#include <iostream>

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

	Parser::Parser(): position_(0)
	{
	}

	boost::variant<int, double> Parser::calculate()
	{
		return parseAddSub();
	}

	boost::variant<int, double> Parser::parse()
	{
		auto cur = current();
		switch (cur.getKind())
		{
		case x:
			{
				int number = context_.value;
				move_next();
				return number;
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
				result_type res = parseMulDiv();
				if (current() != r_paren)
				{
					std::cerr << "Error: missing right bracket";
					exit(-1);
				}
				move_next();
				return res;
			}
		case eof:
			throw lex::InvalidInput("empty input.");
		default:
			std::cerr << "Error: expected expression...";
			exit(-1);
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
				res = mex::eval(std::multiplies{}, res, parse());
				break;
			case divide:
				{
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
	}
}
