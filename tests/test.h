#ifndef TEST_H
#define TEST_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Simple test framework for mini-shell testing
 */

typedef struct {
    int total_tests;
    int passed_tests;
    int failed_tests;
} test_stats_t;

extern test_stats_t test_stats;

#define TEST_ASSERT(condition, message) \
    do { \
        if (condition) { \
            test_stats.passed_tests++; \
            printf("  ✓ %s\n", message); \
        } else { \
            test_stats.failed_tests++; \
            printf("  ✗ %s\n", message); \
        } \
        test_stats.total_tests++; \
    } while (0)

#define TEST_EQUAL(a, b, message) \
    do { \
        if ((a) == (b)) { \
            test_stats.passed_tests++; \
            printf("  ✓ %s\n", message); \
        } else { \
            test_stats.failed_tests++; \
            printf("  ✗ %s (expected %d, got %d)\n", message, (b), (a)); \
        } \
        test_stats.total_tests++; \
    } while (0)

#define TEST_STRING_EQUAL(a, b, message) \
    do { \
        if (strcmp((a), (b)) == 0) { \
            test_stats.passed_tests++; \
            printf("  ✓ %s\n", message); \
        } else { \
            test_stats.failed_tests++; \
            printf("  ✗ %s (expected \"%s\", got \"%s\")\n", message, (b), (a)); \
        } \
        test_stats.total_tests++; \
    } while (0)

#define TEST_NOT_NULL(ptr, message) \
    do { \
        if ((ptr) != NULL) { \
            test_stats.passed_tests++; \
            printf("  ✓ %s\n", message); \
        } else { \
            test_stats.failed_tests++; \
            printf("  ✗ %s (pointer is NULL)\n", message); \
        } \
        test_stats.total_tests++; \
    } while (0)

#define TEST_NULL(ptr, message) \
    do { \
        if ((ptr) == NULL) { \
            test_stats.passed_tests++; \
            printf("  ✓ %s\n", message); \
        } else { \
            test_stats.failed_tests++; \
            printf("  ✗ %s (pointer is not NULL)\n", message); \
        } \
        test_stats.total_tests++; \
    } while (0)

#define RUN_TEST_SUITE(suite_name, test_func) \
    do { \
        printf("\n=== %s ===\n", suite_name); \
        test_func(); \
    } while (0)

void print_test_results(void);

#endif
