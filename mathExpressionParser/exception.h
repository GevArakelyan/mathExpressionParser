#pragma once
#include <exception>
#include <string>
#include "token.h"

namespace lex {
	class InvalidFunction final : public std::exception
	{
	public:

		char const* what() const override
		{
			return  error_.data();
		}

		explicit InvalidFunction(unsigned pos)
		{
			error_.append("unknown string at pos == ");
			error_.append(std::to_string(pos));
		}
	private:
		std::string error_;
	};
	class InvalidNumber final : public std::exception
	{
	public:

		char const* what() const override
		{
			return  error_.data();
		}

		explicit InvalidNumber(unsigned pos)
		{
			error_.append("unknown string at pos == ");
			error_.append(std::to_string(pos));
		}
	private:
		std::string error_;
	};
	class InvalidToken final : public std::exception
	{
	public:

		char const* what() const override
		{
			return  error_.data();
		}

		explicit InvalidToken(mex::tok::Token t)
		{
			token_ = t;
			error_.append("Invalid sequence");
		}
	private:
		mex::tok::Token token_;
		std::string error_;
	};

	class InvalidInput final : public std::exception
	{
	public:

		char const* what() const override
		{
			return  error_.data();
		}
		explicit InvalidInput(const char* error_string)
		{
			error_ = error_string;
		}
		explicit InvalidInput(unsigned pos)
		{
			error_.append("unknown character at pos == ");
			error_.append(std::to_string(pos));
		}
	private:
		std::string error_;
	};

}
