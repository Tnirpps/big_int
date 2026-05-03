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
- [ ] Small integer helpers:
  Add `big_int_add_i64(big_int_t a, int64_t b)`, `big_int_sub_i64(big_int_t a, int64_t b)`, and `big_int_mul_i64(big_int_t a, int64_t b)`.
- [ ] Parsing validation:
  Add `bool big_int_try_from_string(const char *str, big_int_t *out)` for validated parsing with error reporting.
- [ ] Zero and sign predicates:
  Add `bool big_int_is_zero(big_int_t n)`, `bool big_int_is_negative(big_int_t n)`, and `bool big_int_is_positive(big_int_t n)` as public helpers.
- [ ] Better formatting helpers:
  Add `void big_int_fprint(FILE *stream, big_int_t n)` so callers can print to arbitrary streams.
- [ ] More tests:
  Cover overflow boundaries, invalid strings, leading zeros, `INT64_MIN`, `INT64_MAX`, zero normalization, and large division cases.
