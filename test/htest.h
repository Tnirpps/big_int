#ifndef HTEST_H
#define HTEST_H

#include <stdio.h>
#include <stdbool.h>
#include <dlfcn.h>
#include <string.h>
#include <stdlib.h>

/* === Colors for terminal === */
#define TEST_COLOR_RED     "\033[31m"
#define TEST_COLOR_GREEN   "\033[32m"
#define TEST_COLOR_YELLOW  "\033[33m"
#define TEST_COLOR_RESET   "\033[0m"

/* === Run statistics === */
static int htest_total  = 0;
static int htest_passed = 0;
static int htest_failed = 0;

#define TEST_PRINT(result, fmt, ...)                                       \
    do {                                                                   \
        const char* _color = TEST_COLOR_YELLOW;                            \
        const char* _label = "INFO";                                       \
        if ((result) > 0)  { _color = TEST_COLOR_GREEN; _label = "OK"; }   \
        if ((result) < 0)  { _color = TEST_COLOR_RED;   _label = "FAIL"; } \
        printf("   %s[%s]%s ", _color, _label, TEST_COLOR_RESET);          \
        printf(fmt "\n", ##__VA_ARGS__);                                   \
    } while (0)

/* === Макросы проверок === */
#define TEST_CHECK(cond, msg)                              \
    do {                                                   \
        if (cond)                                          \
            TEST_PRINT(+1, "%s", msg);                     \
        else {                                             \
            TEST_PRINT(-1, "%s", msg);                     \
            htest_failed++;                                \
        }                                                  \
    } while (0)

#define TEST_EQ(a, b)                                      \
    do {                                                   \
        if ((a) == (b)) {                                  \
           /* TEST_PRINT(+1, "%s == %s", #a, #b); */          \
        } else {                                             \
            TEST_PRINT(-1, "%s != %s (got %lld vs %lld)", #a, #b, \
                       (long long)(a), (long long)(b));    \
            htest_failed++;                                \
        }                                                  \
    } while (0)


/* === Core macros === */
#define CONCAT_IMPL(a,b) a##b
#define CONCAT(a,b) CONCAT_IMPL(a,b)


#define TEST_FN(name) \
    static void name(void); \
    void CONCAT(help_, __COUNTER__)(void) { \
        printf(TEST_COLOR_RESET "[%s]: running...\n" TEST_COLOR_RESET, #name); \
        htest_total++; \
        int fails_before = htest_failed; \
        name(); \
        if (htest_failed == fails_before) { \
            printf(TEST_COLOR_GREEN "[%s]: PASS\n" TEST_COLOR_RESET, #name); \
            htest_passed++; \
        } else { \
            printf(TEST_COLOR_RED "[%s]: FAIL\n" TEST_COLOR_RESET, #name); \
        } \
        printf("======================\n"); \
    } \
    static void name(void)

#define RUN_TESTS() \
    void* handler = dlopen(NULL, RTLD_NOW); \
    if (!handler) { \
        fprintf(stderr, "dlopen failed\n"); \
        return 1; \
    } \
 \
    int cnt = __COUNTER__; \
    char help_name[16] = {0}; \
 \
    for (int i = 0; i < cnt; ++i) { \
        snprintf(help_name, 16, "help_%d", i); \
        void (*help)(void) = (void(*)(void)) dlsym(handler, help_name); \
        if (!help) continue; \
        help(); \
    } \
 \
    printf("\n==== Test summary ====\n"); \
    if (htest_failed == 0) { \
        printf(TEST_COLOR_GREEN "All tests passed (%d/%d)\n" TEST_COLOR_RESET, htest_passed, htest_total); \
    } else { \
        printf(TEST_COLOR_RED "%d/%d tests failed\n" TEST_COLOR_RESET, htest_failed, htest_total); \
    } \
 \
    return htest_failed == 0 ? 0 : 1

#endif // HTEST_H

