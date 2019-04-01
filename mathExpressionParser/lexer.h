#ifndef LEXER_H
#define LEXER_H

#include "exception.h"
#include "token.h"
#include <string>
#include <string>
#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/lexical_cast.hpp>
#include <ostream>
#include <iomanip>

namespace mex
{
	class CharSequence
	{
	public:
		CharSequence(const char* begin, size_t size)
			: begin_(begin), pos_(begin), size_(size)
		{
		}

		char move_next(size_t count = 1)
		{
			char elem = *pos_;
			pos_ += count;
			size_ -= count;
			return elem;
		}

		const char* next() const
		{
			return pos_;
		}

		unsigned pos_from_begin() const
		{
			return static_cast<unsigned>(std::distance(pos_, begin_));
		}

		size_t size() const
		{
			return size_;
		}

		const char* pos() const
		{
			return pos_;
		}

		const char* begin() const
		{
			return pos_;
		}

		const char* end() const
		{
			return pos_ + size_;
		}

		operator boost::string_view() const
		{
			return boost::string_view{pos_, size_};
		}

	private:

		///the begin of the sequence
		const char* begin_{nullptr};
		///current pos_from_begin within sequence
		const char* pos_;
		///remaining size of the character sequence 
		size_t size_{0};
	};

	inline boost::string_view extract_symbols(const CharSequence& chars)
	{
		const auto symbol_end = std::find_if_not(chars.begin(), chars.end(), &isalnum);
		boost::string_view symbols{
			chars.pos(),
			static_cast<size_t>(std::distance(chars.pos(), symbol_end))
		};
		if (symbols == "sin"
			|| symbols == "cos"
			|| symbols == "tan"
			|| symbols == "x")
		{
		}
		else
		{
			throw lex::InvalidInput(chars.pos_from_begin());
		}
		return symbols;
	}


	inline size_t extract_number(CharSequence& chars)
	{
		boost::string_view sv{chars};
		auto end_of_number = sv.find_first_not_of("0123456789");
		end_of_number = end_of_number == boost::string_view::npos
			                ? chars.size()
			                : end_of_number;
		boost::string_view number_view(sv.data(), end_of_number);
		const std::string number_string(number_view);
		size_t len = 0;
		try
		{
			auto i = std::stoi(number_string, &len);
		}
		catch (...)
		{
		}

		if (!len)
			throw lex::InvalidNumber(chars.pos_from_begin());
		else
			chars.move_next(end_of_number);
		return end_of_number;
	}

	class Lexer
	{
		std::string expression_;
	public:
		explicit Lexer(const char* expression) : expression_(expression){}
		explicit  Lexer(std::string expression) : expression_(std::move(expression)){}

		/**
		 * \brief
		 * \param expression may contain X,+,-,*,/, and sin,cos tan functions)
		 */
		void set_expression_to_parse(std::string expression)
		{
			this->expression_ = std::move(expression);
		}

		std::vector<tok::Token> generate_tokens() const
		{
			namespace ba = boost::algorithm;
			using namespace std::string_literals;

			std::vector<tok::Token> result;

			const auto end = (expression_.c_str() + expression_.size());
			CharSequence chars{expression_.data(), expression_.size()};
			while (chars.next() != end)
			{
				char c = *chars.next();
				const char* cur_position = chars.pos();
				if (ba::any_of_equal(" \n"s, c))
				{
					chars.move_next();
					continue;
				}
				if (ba::any_of_equal("+-*/"s, c))
				{
					result.emplace_back(cur_position, 1, tok::token_for(c));
					chars.move_next();
					continue;
				}
				if (ba::any_of_equal("()"s, c))
				{
					result.emplace_back(cur_position, 1, tok::token_for(c));
					chars.move_next();
					continue;
				}
				if (ba::any_of_equal("xcst"s, c)) //cos sin tan functions.
				{
					boost::string_view symbols = extract_symbols(chars);
					chars.move_next(symbols.size());
					result.emplace_back(cur_position, symbols.size(),
						symbols.size() == 1 ? tok::TokenKind::x
						: tok::TokenKind::function
						);
				}
				else if (c >= '0' && c <= '9')
				{
					result.emplace_back(cur_position, extract_number(chars), tok::TokenKind::numeric_constant);
				}
				else
				{
					throw lex::InvalidInput(chars.pos_from_begin());
				}
			}
			result.emplace_back(chars.next(), 0, tok::TokenKind::eof);
			return result;
		}

		friend std::ostream& operator<<(std::ostream& os, const Lexer& obj)
		{
			for (auto& token : obj.generate_tokens())
				os << std::quoted(token.getName()) << " : " << std::quoted(std::string(token.view())) << "\n";
			return os;
		}
	};
}
#endif // LEXER_H
