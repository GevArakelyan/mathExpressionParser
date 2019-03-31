#ifndef TOKEN_H
#define TOKEN_H

#include <boost/utility/string_view.hpp>
#include <boost/any.hpp>

namespace mex::tok
{
	enum TokenKind
	{
		unknown,
		eof,
		x,
		numeric_constant,
		star,
		plus,
		minus,
		percent,
		function,
		l_paren,
		r_paren,
		divide,
		num_tokens
	};

	static const char* TokNames[13]
	{
		"unknown",
		"eof",
		"x",
		"numeric_constant",
		"star",
		"plus",
		"minus",
		"percent",
		"function",
		"l_paren",
		"r_paren",
		"divide",
		nullptr
	};

	inline tok::TokenKind token_for(char c)
	{
		switch (c)
		{
		case '(':
			return l_paren;
		case ')':
			return r_paren;
		case '*':
			return star;
		case '/':
			return divide;
		case '%':
			return percent;
		case '+':
			return plus;
		case '-':
			return minus;
		default:
			return unknown;
		}
	}

	inline const char* getTokenName(TokenKind Kind)
	{
		if (Kind < tok::num_tokens)
			return TokNames[Kind];
		return nullptr;
	}

	inline bool isIdentifier(TokenKind k)
	{
		return k == TokenKind::x;
	}

	inline bool isNumeric(TokenKind k)
	{
		return k == TokenKind::numeric_constant;
	}


	class Token
	{
		

	public:
		Token(const Token& other)
			: loc_(other.loc_),
			  length_(other.length_),
			  kind_(other.kind_)
		{
		}
		Token(tok::TokenKind kind)
			: loc_(nullptr),
			length_(0),
			kind_(kind)
		{
		}

		Token(Token&& other) noexcept
			: loc_(other.loc_),
			  length_(other.length_),
			  kind_(other.kind_)
		{
		}

		Token& operator=(const Token& other)
		{
			if (this == &other)
				return *this;
			loc_ = other.loc_;
			length_ = other.length_;
			kind_ = other.kind_;
			return *this;
		}

		Token& operator=(Token&& other) noexcept
		{
			if (this == &other)
				return *this;
			loc_ = other.loc_;
			length_ = other.length_;
			kind_ = other.kind_;
			return *this;
		}
		
		Token(): loc_(nullptr), length_(0), kind_(tok::TokenKind::unknown)
		{
		}

		Token(const char* location, size_t length, tok::TokenKind kind)
			: loc_(location), length_(length), kind_(kind)
		{
		}


		operator boost::string_view() const
		{
			return boost::string_view(loc_, length_);
		}

		tok::TokenKind getKind() const { return kind_; }
		void setKind(tok::TokenKind k) { kind_ = k; }

		bool is(tok::TokenKind k) const { return kind_ == k; }
		bool isNot(tok::TokenKind k) const { return kind_ != k; }

		bool isOneOf(tok::TokenKind k1, tok::TokenKind k2) const
		{
			return is(k1) || is(k2);
		}

		template <typename... Ts>
		bool isOneOf(tok::TokenKind K1, tok::TokenKind K2, Ts ... Ks) const
		{
			return is(K1) || isOneOf(K2, Ks...);
		}

		bool isIdentifier() const
		{
			return tok::isIdentifier(getKind());
		}

		size_t getLength() const
		{
			return length_;
		}

		void setLength(size_t len)
		{
			length_ = len;
		}
		std::string to_string() const
		{
			return std::string(loc_, length_);
		}

		boost::string_view view() const
		{
			return boost::string_view(loc_, length_);
		}
		
		friend bool operator==(const Token& lhs, const Token& rhs)
		{
			return lhs.loc_ == rhs.loc_
				&& lhs.length_ == rhs.length_
				&& lhs.kind_ == rhs.kind_;
		}

		friend bool operator!=(const Token& lhs, const Token& rhs)
		{
			return !(lhs == rhs);
		}

		const char* getName() const { return tok::getTokenName(kind_); }
	private:
		///token location
		const char* loc_;

		///this holds the length of the token.
		size_t length_;


		/// Kind - The actual flavor of token this is.
		tok::TokenKind kind_;
	};
}


#endif // TOKEN_H
