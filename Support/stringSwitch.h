#ifndef SUP_ADT_STRINGSWITCH_H
#define SUP_ADT_STRINGSWITCH_H

#include "../Support/compiler.h"
#include "../Support/stringRef.h"
#include <cassert>
#include <cstring>

namespace sup {

	/// A switch()-like statement whose cases are string literals.
	///
	/// The StringSwitch class is a simple form of a switch() statement that
	/// determines whether the given string matches one of the given string
	/// literals. The template type parameter \p T is the type of the value that
	/// will be returned from the string-switch expression. For example,
	/// the following code switches on the name of a color in \c argv[i]:
	///
	/// \code
	/// Color color = StringSwitch<Color>(argv[i])
	///   .Case("red", Red)
	///   .Case("orange", Orange)
	///   .Case("yellow", Yellow)
	///   .Case("green", Green)
	///   .Case("blue", Blue)
	///   .Case("indigo", Indigo)
	///   .Cases("violet", "purple", Violet)
	///   .Default(UnknownColor);
	/// \endcode
	template<typename T, typename R = T>
	class StringSwitch {
		/// The string we are matching.
		const StringRef Str;

		/// The pointer to the result of this switch statement, once known,
		/// null before that.
		std::optional<T> Result;

	public:
		explicit StringSwitch(StringRef S)
			: Str(S), Result() { }

		// StringSwitch is not copyable.
		StringSwitch(const StringSwitch&) = delete;

		// StringSwitch is not assignable due to 'Str' being 'const'.
		void operator=(const StringSwitch&) = delete;
		void operator=(StringSwitch&& other) = delete;

		StringSwitch(StringSwitch&& other)
			: Str(other.Str), Result(std::move(other.Result)) { }

		~StringSwitch() = default;

		// Case-sensitive case matchers
		StringSwitch& Case(StringLiteral S, T Value) {
			if (!Result && Str == S) {
				Result = std::move(Value);
			}
			return *this;
		}

		StringSwitch& EndsWith(StringLiteral S, T Value) {
			if (!Result && Str.endswith(S)) {
				Result = std::move(Value);
			}
			return *this;
		}

		StringSwitch& StartsWith(StringLiteral S, T Value) {
			if (!Result && Str.startswith(S)) {
				Result = std::move(Value);
			}
			return *this;
		}

		StringSwitch& Cases(StringLiteral S0, StringLiteral S1, T Value) {
			return Case(S0, Value).Case(S1, Value);
		}

		StringSwitch& Cases(StringLiteral S0, StringLiteral S1, StringLiteral S2,
			T Value) {
			return Case(S0, Value).Cases(S1, S2, Value);
		}

		StringSwitch& Cases(StringLiteral S0, StringLiteral S1, StringLiteral S2,
			StringLiteral S3, T Value) {
			return Case(S0, Value).Cases(S1, S2, S3, Value);
		}

		StringSwitch& Cases(StringLiteral S0, StringLiteral S1, StringLiteral S2,
			StringLiteral S3, StringLiteral S4, T Value) {
			return Case(S0, Value).Cases(S1, S2, S3, S4, Value);
		}

		StringSwitch& Cases(StringLiteral S0, StringLiteral S1, StringLiteral S2,
			StringLiteral S3, StringLiteral S4, StringLiteral S5,
			T Value) {
			return Case(S0, Value).Cases(S1, S2, S3, S4, S5, Value);
		}

		StringSwitch& Cases(StringLiteral S0, StringLiteral S1, StringLiteral S2,
			StringLiteral S3, StringLiteral S4, StringLiteral S5,
			StringLiteral S6, T Value) {
			return Case(S0, Value).Cases(S1, S2, S3, S4, S5, S6, Value);
		}

		StringSwitch& Cases(StringLiteral S0, StringLiteral S1, StringLiteral S2,
			StringLiteral S3, StringLiteral S4, StringLiteral S5,
			StringLiteral S6, StringLiteral S7, T Value) {
			return Case(S0, Value).Cases(S1, S2, S3, S4, S5, S6, S7, Value);
		}

		StringSwitch& Cases(StringLiteral S0, StringLiteral S1, StringLiteral S2,
			StringLiteral S3, StringLiteral S4, StringLiteral S5,
			StringLiteral S6, StringLiteral S7, StringLiteral S8,
			T Value) {
			return Case(S0, Value).Cases(S1, S2, S3, S4, S5, S6, S7, S8, Value);
		}

		StringSwitch& Cases(StringLiteral S0, StringLiteral S1, StringLiteral S2,
			StringLiteral S3, StringLiteral S4, StringLiteral S5,
			StringLiteral S6, StringLiteral S7, StringLiteral S8,
			StringLiteral S9, T Value) {
			return Case(S0, Value).Cases(S1, S2, S3, S4, S5, S6, S7, S8, S9, Value);
		}

		// Case-insensitive case matchers.
		StringSwitch& CaseLower(StringLiteral S, T Value) {
			if (!Result && Str.equals_lower(S))
				Result = std::move(Value);

			return *this;
		}

		StringSwitch& EndsWithLower(StringLiteral S, T Value) {
			if (!Result && Str.endswith_lower(S))
				Result = Value;

			return *this;
		}

		StringSwitch& StartsWithLower(StringLiteral S, T Value) {
			if (!Result && Str.startswith_lower(S))
				Result = std::move(Value);

			return *this;
		}

		StringSwitch& CasesLower(StringLiteral S0, StringLiteral S1, T Value) {
			return CaseLower(S0, Value).CaseLower(S1, Value);
		}

		StringSwitch& CasesLower(StringLiteral S0, StringLiteral S1, StringLiteral S2,
			T Value) {
			return CaseLower(S0, Value).CasesLower(S1, S2, Value);
		}

		StringSwitch& CasesLower(StringLiteral S0, StringLiteral S1, StringLiteral S2,
			StringLiteral S3, T Value) {
			return CaseLower(S0, Value).CasesLower(S1, S2, S3, Value);
		}

		StringSwitch& CasesLower(StringLiteral S0, StringLiteral S1, StringLiteral S2,
			StringLiteral S3, StringLiteral S4, T Value) {
			return CaseLower(S0, Value).CasesLower(S1, S2, S3, S4, Value);
		}

		SUP_NODISCARD
			R Default(T Value) {
			if (Result)
				return std::move(*Result);
			return Value;
		}

		SUP_NODISCARD
			operator R() {
			assert(Result && "Fell off the end of a string-switch");
			return std::move(*Result);
		}
	};


#endif // SUP_STRINGSWITCH_H