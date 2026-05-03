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

big_int_t a = big_int_from_string("1000000000000000000000000000000");
big_int_t b = big_int_from_string("1");
big_int_t c = big_int_sub(a, b);

big_int_print(c); /* 999999999999999999999999999999 */
```

Run tests with:

```sh
make test
```
