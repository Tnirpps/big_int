#include <limits.h>
#include <string.h>

#include "htest.h"

#define BIG_INT_IMPLEMENTATION
#include "../big_int.h"

TEST_FN(test_addition_simple) {
    big_int_t a = big_int_from_int(3);
    big_int_t b = big_int_from_int(1);
    big_int_t c = big_int_sum(a, b);
    big_int_t expected = big_int_from_int(4);

    TEST_CHECK(big_int_cmp(c, expected) == 0, "3 + 1 == 4");
    
    big_int_delete(&a);
    big_int_delete(&b);
    big_int_delete(&c);
    big_int_delete(&expected);
}

TEST_FN(test_from_string_small) {
    big_int_t n = big_int_from_string("12345");
    big_int_t expected = big_int_from_int(12345);
    
    TEST_CHECK(big_int_cmp(n, expected) == 0, "from_string('12345') == 12345");
    
    big_int_delete(&n);
    big_int_delete(&expected);
}

TEST_FN(test_from_string_negative) {
    big_int_t n = big_int_from_string("-999");
    big_int_t expected = big_int_from_int(-999);
    
    TEST_CHECK(big_int_cmp(n, expected) == 0, "from_string('-999') == -999");
    
    big_int_delete(&n);
    big_int_delete(&expected);
}

TEST_FN(test_addition_large_numbers) {
    big_int_t a = big_int_from_string("999999999999999999999999999999");
    big_int_t b = big_int_from_string("1");
    big_int_t c = big_int_sum(a, b);
    big_int_t expected = big_int_from_string("1000000000000000000000000000000");
    
    TEST_CHECK(big_int_cmp(c, expected) == 0, "999...999 + 1 == 1000...000");
    
    big_int_delete(&a);
    big_int_delete(&b);
    big_int_delete(&c);
    big_int_delete(&expected);
}

TEST_FN(test_addition_large_same_sign) {
    big_int_t a = big_int_from_string("123456789012345678901234567890");
    big_int_t b = big_int_from_string("987654321098765432109876543210");
    big_int_t c = big_int_sum(a, b);
    big_int_t expected = big_int_from_string("1111111110111111111011111111100");
    
    TEST_CHECK(big_int_cmp(c, expected) == 0, "large positive addition");
    
    big_int_delete(&a);
    big_int_delete(&b);
    big_int_delete(&c);
    big_int_delete(&expected);
}

TEST_FN(test_addition_mixed_sign) {
    big_int_t a = big_int_from_string("1000");
    big_int_t b = big_int_from_string("-300");
    big_int_t c = big_int_sum(a, b);
    big_int_t expected = big_int_from_string("700");
    
    TEST_CHECK(big_int_cmp(c, expected) == 0, "1000 + (-300) == 700");
    
    big_int_delete(&a);
    big_int_delete(&b);
    big_int_delete(&c);
    big_int_delete(&expected);
}

TEST_FN(test_addition_negative_result) {
    big_int_t a = big_int_from_string("-1000");
    big_int_t b = big_int_from_string("300");
    big_int_t c = big_int_sum(a, b);
    big_int_t expected = big_int_from_string("-700");
    
    TEST_CHECK(big_int_cmp(c, expected) == 0, "-1000 + 300 == -700");
    
    big_int_delete(&a);
    big_int_delete(&b);
    big_int_delete(&c);
    big_int_delete(&expected);
}

TEST_FN(test_addition_large_negative) {
    big_int_t a = big_int_from_string("-999999999999999999999999999999");
    big_int_t b = big_int_from_string("-1");
    big_int_t c = big_int_sum(a, b);
    big_int_t expected = big_int_from_string("-1000000000000000000000000000000");
    
    TEST_CHECK(big_int_cmp(c, expected) == 0, "large negative addition");
    
    big_int_delete(&a);
    big_int_delete(&b);
    big_int_delete(&c);
    big_int_delete(&expected);
}

TEST_FN(test_addition_large_mixed_sign) {
    big_int_t a = big_int_from_string("-999999999999999999999999999999");
    big_int_t b = big_int_from_string("1000000000000000000000000000000");
    big_int_t c = big_int_sum(a, b);
    big_int_t expected = big_int_from_string("1");
    
    TEST_CHECK(big_int_cmp(c, expected) == 0, "large mixed sign addition");
    
    big_int_delete(&a);
    big_int_delete(&b);
    big_int_delete(&c);
    big_int_delete(&expected);
}

TEST_FN(test_subtraction_simple) {
    big_int_t a = big_int_from_int(10);
    big_int_t b = big_int_from_int(3);
    big_int_t c = big_int_sub(a, b);
    big_int_t expected = big_int_from_int(7);

    TEST_CHECK(big_int_cmp(c, expected) == 0, "10 - 3 == 7");

    big_int_delete(&a);
    big_int_delete(&b);
    big_int_delete(&c);
    big_int_delete(&expected);
}

TEST_FN(test_subtraction_large_borrow) {
    big_int_t a = big_int_from_string("1000000000000000000000000000000");
    big_int_t b = big_int_from_string("1");
    big_int_t c = big_int_sub(a, b);
    big_int_t expected = big_int_from_string("999999999999999999999999999999");

    TEST_CHECK(big_int_cmp(c, expected) == 0, "1000...000 - 1 == 999...999");

    big_int_delete(&a);
    big_int_delete(&b);
    big_int_delete(&c);
    big_int_delete(&expected);
}

TEST_FN(test_subtraction_negative_result) {
    big_int_t a = big_int_from_int(3);
    big_int_t b = big_int_from_int(10);
    big_int_t c = big_int_sub(a, b);
    big_int_t expected = big_int_from_int(-7);

    TEST_CHECK(big_int_cmp(c, expected) == 0, "3 - 10 == -7");

    big_int_delete(&a);
    big_int_delete(&b);
    big_int_delete(&c);
    big_int_delete(&expected);
}

TEST_FN(test_subtraction_to_zero) {
    big_int_t a = big_int_from_string("-999999999999999999999999999999");
    big_int_t b = big_int_from_string("-999999999999999999999999999999");
    big_int_t c = big_int_sub(a, b);
    big_int_t expected = big_int_from_int(0);

    TEST_CHECK(big_int_cmp(c, expected) == 0, "x - x == 0");

    big_int_delete(&a);
    big_int_delete(&b);
    big_int_delete(&c);
    big_int_delete(&expected);
}

int main() {
    RUN_TESTS();
}
