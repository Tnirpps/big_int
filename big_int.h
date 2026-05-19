#ifndef BIG_INT_H
#define BIG_INT_H

#include <stdlib.h>
#include <assert.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX(X, Y) ((X) > (Y) ? (X) : (Y))
#define MIN(X, Y) ((X) < (Y) ? (X) : (Y))

typedef struct {
    size_t sz;
    size_t cp; // = 0 -> as_num
    union {
        int32_t *data;
        int64_t num;
    } as;
    bool negative;
} big_int_t;

big_int_t big_int_create();
big_int_t big_int_from_int(int64_t x);
big_int_t big_int_from_string(const char *str);
big_int_t big_int_sum(big_int_t a, big_int_t b);
big_int_t big_int_sub(big_int_t a, big_int_t b);

int big_int_cmp(big_int_t a, big_int_t b);
int big_int_cmp_abs(big_int_t a, big_int_t b);

void big_int_delete(big_int_t *n);
void big_int_print(big_int_t n);

big_int_t big_int_copy(big_int_t n);




// internal functions
bool safe_add(int64_t a, int64_t b, int64_t *res);
bool big_int_is_zero(big_int_t n);
bool big_int_is_negative(big_int_t n);
bool big_int_is_positive(big_int_t n);

void convert_to_big(big_int_t *n);
void big_int_normalize(big_int_t *n);
void big_int_resize(big_int_t *n, size_t cp);
void big_int_debug(big_int_t n);

#ifdef BIG_INT_IMPLEMENTATION
big_int_t big_int_create() {
    big_int_t n = {0};
    return n;
}

big_int_t big_int_from_int(int64_t x) {
    big_int_t n = big_int_create();
    n.as.num = x;
    return n;
}

big_int_t big_int_from_string(const char *str) {
    big_int_t n = big_int_create();
    
    size_t len = strlen(str);
    if (len == 0) {
        return n;
    }
    
    size_t start = 0;
    bool negative = false;
    if (str[0] == '-') {
        negative = true;
        start = 1;
    } else if (str[0] == '+') {
        start = 1;
    }
    
    size_t digit_count = len - start;
    
    if (digit_count <= 18) {
        int64_t val = 0;
        for (size_t i = start; i < len; ++i) {
            if (str[i] >= '0' && str[i] <= '9') {
                val = val * 10 + (str[i] - '0');
            }
        }
        n.as.num = negative ? -val : val;
        return n;
    }
    
    n.negative = negative;
    n.cp = digit_count;
    n.as.data = (int32_t*)malloc(sizeof(int32_t) * n.cp);
    n.sz = 0;
    
    for (size_t i = start; i < len; ++i) {
        if (str[i] >= '0' && str[i] <= '9') {
            n.as.data[n.sz++] = str[i] - '0';
        }
    }
    
    for (size_t i = 0; i < n.sz / 2; ++i) {
        int32_t tmp = n.as.data[i];
        n.as.data[i] = n.as.data[n.sz - 1 - i];
        n.as.data[n.sz - 1 - i] = tmp;
    }

    big_int_normalize(&n);
    
    return n;
}

bool safe_add(int64_t a, int64_t b, int64_t *res) {
    if (b >= 0) {
        if (a <= INT64_MAX - b) {
            *res = a + b;
            return true;
        }
        return false;
    } else {
        // (a - |b| < INT64_MAX )
        // (a > INT64_MIN + |b|) -> (a > INT64_MIN - b)
        if (a >= INT64_MIN - b) {
            *res = a + b;
            return true;
        }
        return false;
    }
}

bool big_int_is_zero(big_int_t n) {
    if (n.cp == 0) {
        return n.as.num == 0;
    }

    for (size_t i = 0; i < n.sz; ++i) {
        if (n.as.data[i] != 0) {
            return false;
        }
    }

    return true;
}

void convert_to_big(big_int_t *n) {
    if (n->cp != 0) {
        return;
    }

    int64_t value = n->as.num;
    if (value < 0) {
        n->negative = true;
    } else {
        n->negative = false;
    }

    uint64_t x = (value < 0) ? (uint64_t)(-(value + 1)) + 1 : (uint64_t)value;

    n->cp = 20;
    n->as.data = (int32_t*)malloc(sizeof(int32_t) * n->cp);
    n->sz = 0;

    if (x == 0) {
        n->as.data[n->sz++] = 0;
        return;
    }

    while (x > 0) {
        n->as.data[n->sz++] = x % 10;
        x /= 10;
    }
}

void big_int_normalize(big_int_t *n) {
    if (n->cp == 0) {
        if (n->as.num == 0) {
            n->negative = false;
        }
        return;
    }

    while (n->sz > 1 && n->as.data[n->sz - 1] == 0) {
        n->sz--;
    }

    if (n->sz == 0) {
        n->sz = 1;
        n->as.data[0] = 0;
    }

    if (n->sz == 1 && n->as.data[0] == 0) {
        n->negative = false;
    }
}

void big_int_resize(big_int_t *n, size_t cp) {
    if (n->cp == 0) {
        convert_to_big(n);
    }

    n->as.data = (int32_t *)realloc(n->as.data, sizeof(int32_t) * cp);
    n->cp = cp;

    if (n->sz > cp) {
        n->sz = cp;
    }
}

void big_int_delete(big_int_t *n) {
    if (n->cp != 0) {
        free(n->as.data);
    }
    *n = (big_int_t){0};
}

big_int_t big_int_sum(big_int_t a, big_int_t b) {
    big_int_t res = big_int_create();

    bool short_a = (a.cp == 0);
    bool short_b = (b.cp == 0);

    if (short_a && short_b) {
        if (safe_add(a.as.num, b.as.num, &res.as.num)) {
            return res;
        }
    }

    convert_to_big(&a);
    convert_to_big(&b);

    if (a.negative == b.negative) {
        size_t sz = MAX(a.sz, b.sz);
        big_int_resize(&res, sz + 1);

        int carry = 0;

        for (size_t i = 0; i < sz; ++i) {
            int32_t da = (i < a.sz ? a.as.data[i] : 0);
            int32_t db = (i < b.sz ? b.as.data[i] : 0);

            int sum = da + db + carry;
            res.as.data[i] = sum % 10;
            carry = sum / 10;
        }

        res.as.data[sz] = carry;
        res.sz = sz + (carry != 0);
        res.negative = a.negative;
    }
    else {
        int cmp = big_int_cmp_abs(a, b);

        big_int_t *big = (cmp >= 0 ? &a : &b);
        big_int_t *small = (cmp >= 0 ? &b : &a);

        big_int_resize(&res, big->sz);

        int borrow = 0;

        for (size_t i = 0; i < big->sz; ++i) {
            int32_t da = big->as.data[i];
            int32_t db = (i < small->sz ? small->as.data[i] : 0);

            int diff = da - db - borrow;

            if (diff < 0) {
                diff += 10;
                borrow = 1;
            } else {
                borrow = 0;
            }

            res.as.data[i] = diff;
        }

        res.sz = big->sz;

        while (res.sz > 1 && res.as.data[res.sz - 1] == 0) {
            res.sz--;
        }

        res.negative = big->negative;
    }

    big_int_normalize(&res);

    if (short_a) big_int_delete(&a);
    if (short_b) big_int_delete(&b);

    return res;
}

big_int_t big_int_sub(big_int_t a, big_int_t b) {
    if (b.cp == 0) {
        if (b.as.num == INT64_MIN) {
            convert_to_big(&b);
            b.negative = !b.negative;
        } else {
            b.as.num = -b.as.num;
        }
    } else if (!big_int_is_zero(b)) {
        b.negative = !b.negative;
    }

    return big_int_sum(a, b);
}

void big_int_debug(big_int_t n) {
    if (n.cp == 0) {
        printf("[stack]: %ld\n", n.as.num);
    } else {
        printf("data = %p, sz = %zu, cp = %zu, neg = %d\n", n.as.data, n.sz, n.cp, n.negative);
        printf("num = ");
        for (size_t i = 0; i < n.sz; ++i) {
            printf("%d", n.as.data[i]);
        }
        printf("\n");
    }
}

void big_int_print(big_int_t n) {
    if (n.cp == 0) {
        printf("%ld\n", n.as.num);
    } else {
        if (n.negative) {
            printf("-");
        }
        for (size_t i = 0; i < n.sz; ++i) {
            printf("%d", n.as.data[n.sz - i - 1]);
        }
        printf("\n");
    }
}

int big_int_cmp_abs(big_int_t a, big_int_t b) {
    if (a.cp == 0 && b.cp == 0) {
        uint64_t ua = (a.as.num < 0) ? (uint64_t)(-(a.as.num + 1)) + 1 : (uint64_t)a.as.num;
        uint64_t ub = (b.as.num < 0) ? (uint64_t)(-(b.as.num + 1)) + 1 : (uint64_t)b.as.num;
        if (ua == ub) return 0;
        return (ua > ub) ? 1 : -1;
    }
    
    if (a.cp == 0) convert_to_big(&a);
    if (b.cp == 0) convert_to_big(&b);
    
    if (a.sz != b.sz) {
        return (a.sz > b.sz) ? 1 : -1;
    }

    for (size_t i = a.sz; i > 0; --i) {
        if (a.as.data[i - 1] != b.as.data[i - 1]) {
            return (a.as.data[i - 1] > b.as.data[i - 1]) ? 1 : -1;
        }
    }
    return 0;
}

int big_int_cmp(big_int_t a, big_int_t b) {
    bool short_a = (a.cp == 0);
    bool short_b = (b.cp == 0);
    
    if (short_a && short_b) {
        if (a.as.num == b.as.num) return 0;
        return (a.as.num > b.as.num) ? 1 : -1;
    }
    
    if (short_a) convert_to_big(&a);
    if (short_b) convert_to_big(&b);
    
    if (a.negative != b.negative) {
        return a.negative ? -1 : 1;
    }
    
    int cmp = big_int_cmp_abs(a, b);
    
    if (a.negative) {
        return -cmp;
    }
    return cmp;
}
bool big_int_is_negative(big_int_t n) {
    if (big_int_is_zero(n)) return false;
    if (n.cp == 0) return n.as.num < 0;
    return n.negative;
}

bool big_int_is_positive(big_int_t n) {
    if (big_int_is_zero(n)) return false;
    if (n.cp == 0) return n.as.num > 0;
    return !n.negative;
}

big_int_t big_int_copy(big_int_t n) {
    if (n.cp == 0) return n;
    big_int_t c;
    c.sz       = n.sz;
    c.cp       = n.cp;
    c.negative = n.negative;
    c.as.data  = (int32_t *)malloc(sizeof(int32_t) * n.cp);
    assert(c.as.data != NULL);
    memcpy(c.as.data, n.as.data, sizeof(int32_t) * n.sz);
    return c;
}   

#endif // BIG_INT_IMPLEMENTATION

#endif // !BIG_INT_H
