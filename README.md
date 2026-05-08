# big_int

Single-header C library for basic big integer arithmetic.

Currently supports:
- creation from `int64_t` and decimal strings
- comparison
- addition
- subtraction

Quick example:

```c

#define BIG_INT_IMPLEMENTATION
#include "big_int.h"

int main(void) {
    big_int_t a = big_int_from_string("1000000000000000000000000000000");
    big_int_t b = big_int_from_string("1");
    big_int_t c = big_int_sub(a, b);

    big_int_print(c); /* 999999999999999999999999999999 */

    big_int_delete(&a);
    big_int_delete(&b);
    big_int_delete(&c);
    return 0;
}
```

Run tests with:

```sh
make test
```

TODO:

MVP:
- [ ] Multiplication support:
  Add `big_int_mul(big_int_t a, big_int_t b)` for full arbitrary-precision multiplication.
- [ ] Division and remainder:
  Add `big_int_div(big_int_t a, big_int_t b)` and `big_int_mod(big_int_t a, big_int_t b)`.
- [ ] Combined division API:
  Add `big_int_div_mod(big_int_t a, big_int_t b, big_int_t *quotient, big_int_t *remainder)` to avoid duplicated work.
- [ ] Decimal string export:
  Add `char *big_int_to_string(big_int_t n)` that returns a heap-allocated string the caller frees.
- [ ] Copy helper:
  Add `big_int_copy(big_int_t n)` so callers can safely duplicate heap-backed values.
- [ ] Unary helpers:
  Add `big_int_neg(big_int_t n)` and `big_int_abs(big_int_t n)` for sign operations.
- [ ] Parsing validation:
  Add `bool big_int_try_from_string(const char *str, big_int_t *out)` for validated parsing with error reporting.
- [ ] Better parse rules:
  Support optional leading whitespace, explicit `+`, strict rejection of invalid trailing characters, and predictable handling of leading zeros.
- [ ] Zero and sign predicates:
  Add `bool big_int_is_zero(big_int_t n)`, `bool big_int_is_negative(big_int_t n)`, and `bool big_int_is_positive(big_int_t n)` as public helpers.
- [ ] Canonical invariants:
  Ensure one normalized representation for zero, sign, and leading digits across every constructor and operation.
- [ ] Error reporting strategy:
  Define consistent behavior for allocation failure and invalid input, either via return codes, status enums, or documented fail-fast semantics.
- [ ] Overflow-safe size handling:
  Audit all capacity growth and allocation math for `size_t` overflow safety.
- [ ] More tests:
  Cover overflow boundaries, invalid strings, leading zeros, `INT64_MIN`, `INT64_MAX`, zero normalization, and large division cases.
- [ ] Warning-clean build:
  Make the library compile cleanly under strict warnings such as `-Wall -Wextra -Wpedantic`.
- [ ] Documentation pass:
  Document complexity, ownership rules, normalization guarantees, and edge-case behavior for every public API.

Convenience / v1.0:
- [ ] Small integer helpers:
  Add `big_int_add_i64(big_int_t a, int64_t b)`, `big_int_sub_i64(big_int_t a, int64_t b)`, and `big_int_mul_i64(big_int_t a, int64_t b)`.
- [ ] More comparison helpers:
  Add `big_int_eq`, `big_int_ne`, `big_int_lt`, `big_int_le`, `big_int_gt`, and `big_int_ge` wrappers around `big_int_cmp(...)`.
- [ ] Even/odd predicates:
  Add `bool big_int_is_even(big_int_t n)` and `bool big_int_is_odd(big_int_t n)`.
- [ ] Better formatting helpers:
  Add `void big_int_fprint(FILE *stream, big_int_t n)` so callers can print to arbitrary streams.
- [ ] Formatting buffers:
  Add `size_t big_int_format(char *buf, size_t buf_size, big_int_t n)` so callers can print without extra heap allocation.
- [ ] Ownership-friendly constructors:
  Add init/destroy style APIs such as `big_int_init`, `big_int_init_from_int`, and `big_int_swap` for easier lifecycle management in larger C codebases.
- [ ] In-place operations:
  Add `big_int_add_inplace`, `big_int_sub_inplace`, `big_int_mul_inplace`, etc. to reduce temporary allocations.
- [ ] Exponentiation:
  Add `big_int_pow(big_int_t base, uint64_t exp)` for integer powers.
- [ ] GCD and LCM:
  Add `big_int_gcd(big_int_t a, big_int_t b)` and `big_int_lcm(big_int_t a, big_int_t b)`.
- [ ] Examples:
  Add small example programs for calculator-style usage, factorial/power computations, and modular arithmetic.

Extended Math:
- [ ] Square root:
  Add `big_int_sqrt(big_int_t n)` or `bool big_int_sqrt_exact(big_int_t n, big_int_t *out)` for common integer-math workflows.
- [ ] Modular arithmetic:
  Add `big_int_pow_mod(...)`, `big_int_mul_mod(...)`, and `big_int_mod_inverse(...)` for number-theory use cases.
- [ ] Non-decimal conversion:
  Add import/export helpers for hexadecimal and binary strings.
- [ ] Stream input helpers:
  Add APIs for reading/parsing big integers from `FILE *` streams.

Performance:
- [ ] Internal base upgrade:
  Consider moving from base-10 digits to a larger internal base such as `10^9` or `2^32` for much better performance.
- [ ] Memory reuse:
  Reduce repeated `malloc`/`realloc` churn with smarter capacity growth and reusable temporary buffers.
- [ ] Faster algorithms:
  Add thresholds for Karatsuba/Toom-Cook multiplication and better long-division algorithms once the simple versions are stable.
- [ ] Benchmarks:
  Measure performance for parsing, formatting, addition, multiplication, and division on realistic operand sizes.

Quality / Release:
- [ ] Property-based tests:
  Cross-check results against Python `int` or another arbitrary-precision reference implementation on randomized inputs.
- [ ] Fuzzing:
  Add fuzz targets for string parsing and arithmetic operations to catch crashes and invariant violations.
- [ ] Portability pass:
  Verify clean builds on GCC, Clang 64-bit targets.
- [ ] Public configuration section:
  Allow users to customize assertions, allocator hooks, and optional debug instrumentation with macros.
