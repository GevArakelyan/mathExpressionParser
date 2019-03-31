#pragma once

/// LLVM_NODISCARD - Warn if a type or return value is discarded.
#if __cplusplus > 201402L && __has_cpp_attribute(nodiscard)
#define SUP_NODISCARD [[nodiscard]]
#else
#define SUP_NODISCARD
#endif
#pragma once
