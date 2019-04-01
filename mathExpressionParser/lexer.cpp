#include "lexer.h"
#include "exception.h"

#include <boost/algorithm/cxx11/any_of.hpp>
#include <boost/utility/string_view.hpp>
#include <boost/lexical_cast.hpp>
#include <iomanip>


namespace mex
{
	boost::string_view extract_symbols(const CharSequence& chars)
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
			throw ::lex::InvalidInput(chars.pos_from_begin());
		}
		return symbols;
	}

	size_t extract_number(CharSequence& chars)
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
			throw ::lex::InvalidNumber(chars.pos_from_begin());

		chars.move_next(end_of_number);
		return end_of_number;
	}

	void Lexer::set_expression_to_parse(std::string_view expression_view)
	{
		this->expression_view_ = expression_view;
	}

	std::vector<tok::Token> Lexer::generate_tokens() const
	{
		namespace ba = boost::algorithm;
		using namespace std::string_literals;

		std::vector<tok::Token> result;

		const auto end = (expression_view_.data() + expression_view_.size());
		CharSequence chars{expression_view_.data(), expression_view_.size()};
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
				                    symbols.size() == 1
					                    ? tok::TokenKind::x
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

	std::ostream& operator<<(std::ostream& os, const Lexer& obj)
	{
		for (auto& token : obj.generate_tokens())
			os << std::quoted(token.getName()) << " : " << std::quoted(std::string(token.view())) << "\n";
		return os;
	}


	char mex::CharSequence::move_next(size_t count)
	{
		char elem = *pos_;
		pos_ += count;
		size_ -= count;
		return elem;
	}


	mex::CharSequence::CharSequence(const char* begin, size_t size) : begin_(begin), pos_(begin), size_(size)
	{
	}

	const char* mex::CharSequence::next() const
	{
		return pos_;
	}

	unsigned mex::CharSequence::pos_from_begin() const
	{
		return static_cast<unsigned>(std::distance(pos_, begin_));
	}

	size_t mex::CharSequence::size() const
	{
		return size_;
	}

	const char* mex::CharSequence::pos() const
	{
		return pos_;
	}

	const char* mex::CharSequence::begin() const
	{
		return pos_;
	}

	const char* mex::CharSequence::end() const
	{
		return pos_ + size_;
	}

	mex::CharSequence::operator boost::basic_string_view<char>() const
	{
		return boost::string_view{pos_, size_};
	}


	Lexer::Lexer(const char* expression): expression_view_(expression)
	{
	}

	Lexer::Lexer(const std::string& expression): expression_view_(expression)
	{
	}
}
