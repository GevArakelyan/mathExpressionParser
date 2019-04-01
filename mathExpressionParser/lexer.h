#ifndef LEXER_H
#define LEXER_H

#include "token.h"
#include <string>
#include <vector>


namespace mex
{
	class CharSequence
	{
	public:
		CharSequence(const char* begin, size_t size);

		char move_next(size_t count = 1);

		const char* next() const;

		unsigned pos_from_begin() const;

		size_t size() const;

		const char* pos() const;

		const char* begin() const;

		const char* end() const;

		operator boost::basic_string_view<char>() const;

	private:

		///the begin of the sequence
		const char* begin_;
		///current pos_from_begin within sequence
		const char* pos_;
		///remaining size of the character sequence 
		size_t size_;
	};

	class Lexer
	{
	public:

		explicit Lexer(const char* expression);

		explicit Lexer(const std::string& expression);

		void set_expression_to_parse(std::string_view expression_view);

		std::vector<tok::Token> generate_tokens() const;

		friend std::ostream& operator<<(std::ostream& os, const Lexer& obj);

	private:
		std::string_view expression_view_;
	};
}
#endif // LEXER_H
