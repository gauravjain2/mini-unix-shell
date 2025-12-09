# Mini Unix Shell - Test Suite Documentation

## Overview

This document describes the comprehensive test suite for the Mini Unix Shell project. The test suite contains 12 test cases (exceeding the minimum requirement of 10) covering parser functionality, utility functions, and command structure validation.

## Test Framework

The test suite uses a custom lightweight test framework defined in `tests/test.h` with the following macros:

- `TEST_ASSERT(condition, message)` - Assert a boolean condition
- `TEST_EQUAL(a, b, message)` - Assert equality of two integers
- `TEST_STRING_EQUAL(a, b, message)` - Assert equality of two strings
- `TEST_NOT_NULL(ptr, message)` - Assert pointer is not NULL
- `TEST_NULL(ptr, message)` - Assert pointer is NULL

## Test Cases

### Test Suite 1: Parser - Simple Command

**File:** `tests/test_suite.c` - `test_parser_simple_command()`

Tests basic command parsing functionality.

**Cases:**

1. Parser creates command structure
2. Command argv is allocated
3. First argument is 'ls'
4. Second argument is '-la'
5. Arguments list is NULL-terminated
6. No pipe continuation
7. Not a background command

**Input:** `ls -la`

---

### Test Suite 2: Parser - Input Redirection

**File:** `tests/test_suite.c` - `test_parser_input_redirection()`

Tests input redirection parsing with `<` operator.

**Cases:**

1. Parser handles input redirection
2. Command name is 'grep'
3. Argument is 'pattern'
4. Input file is 'input.txt'
5. No output redirection set

**Input:** `grep pattern < input.txt`

---

### Test Suite 3: Parser - Output Redirection

**File:** `tests/test_suite.c` - `test_parser_output_redirection()`

Tests output redirection parsing with `>` operator.

**Cases:**

1. Parser handles output redirection
2. Command name is 'echo'
3. Argument is 'hello'
4. Output file is 'output.txt'
5. No input redirection set

**Input:** `echo hello > output.txt`

---

### Test Suite 4: Parser - Background Execution

**File:** `tests/test_suite.c` - `test_parser_background_execution()`

Tests background execution flag parsing with `&` operator.

**Cases:**

1. Parser handles background execution
2. Command name is 'sleep'
3. Argument is '100'
4. Background flag is set
5. Arguments properly terminated

**Input:** `sleep 100 &`

---

### Test Suite 5: Parser - Pipeline

**File:** `tests/test_suite.c` - `test_parser_pipeline()`

Tests multi-stage pipeline parsing with `|` operator.

**Cases:**

1. Parser handles pipelines
2. First command is 'cat'
3. First command argument is 'file.txt'
4. Pipeline has continuation
5. Second command is 'grep'
6. Second command argument is 'pattern'
7. Pipeline has third stage
8. Third command is 'wc'
9. Third command argument is '-l'
10. Pipeline terminates properly

**Input:** `cat file.txt | grep pattern | wc -l`

---

### Test Suite 6: Parser - Complex Command

**File:** `tests/test_suite.c` - `test_parser_complex_command()`

Tests complex command combining pipelines and redirections.

**Cases:**

1. Parser handles complex commands
2. First command is 'cat'
3. Input redirection is 'input.txt'
4. Pipeline exists
5. Piped command is 'sort'
6. Output redirection on piped command is 'output.txt'

**Input:** `cat input.txt | sort > output.txt`

---

### Test Suite 7: Parser - Empty Input Handling

**File:** `tests/test_suite.c` - `test_parser_empty_input()`

Tests edge case handling for empty or whitespace-only input.

**Cases:**

1. Empty input returns NULL
2. Whitespace-only input returns NULL

**Input:** `""` and `"   "`

---

### Test Suite 8: Parser - Multiple Arguments

**File:** `tests/test_suite.c` - `test_parser_multiple_arguments()`

Tests parsing commands with many arguments and flags.

**Cases:**

1. Parser handles many arguments
2. Command name is 'gcc'
3. First flag is '-Wall'
4. Second flag is '-O2'
5. Third flag is '-o'
6. Output argument is 'output'
7. Input file is 'file.c'
8. Library flag is '-lm'
9. Arguments terminate properly

**Input:** `gcc -Wall -O2 -o output file.c -lm`

---

### Test Suite 9: Parser - Both Input and Output Redirection

**File:** `tests/test_suite.c` - `test_parser_both_redirections()`

Tests simultaneous input and output redirection.

**Cases:**

1. Parser handles dual redirections
2. Command name is 'tr'
3. First argument is 'a-z'
4. Second argument is 'A-Z'
5. Input file is 'lowercase.txt'
6. Output file is 'uppercase.txt'

**Input:** `tr a-z A-Z < lowercase.txt > uppercase.txt`

---

### Test Suite 10: Parser - Flags and Options

**File:** `tests/test_suite.c` - `test_parser_flags_and_options()`

Tests parsing complex find command with multiple flags and options.

**Cases:**

1. Parser handles complex find command
2. Command name is 'find'
3. First argument is '.'
4. Flag name is '-name'
5. Pattern is '\*.c'
6. Second flag is '-type'
7. Type argument is 'f'

**Input:** `find . -name '*.c' -type f`

---

### Test Suite 11: Parser - Pipeline with Redirections

**File:** `tests/test_suite.c` - `test_parser_pipeline_with_redirections()`

Tests complex combination of pipelines and redirections.

**Cases:**

1. Parser handles pipeline with complex redirections
2. First command is 'cat'
3. Input on first command is 'input.txt'
4. Second command exists and is 'grep'
5. Third command exists and is 'tee'
6. Tee target is 'errors.log'
7. Output redirection is '/dev/null'

**Input:** `cat input.txt | grep error | tee errors.log > /dev/null`

---

### Test Suite 12: Utility - PWD Functions

**File:** `tests/test_suite.c` - `test_utility_pwd_functions()`

Tests utility functions for working directory management.

**Cases:**

1. get_home() returns non-NULL
2. get_pwd() returns non-NULL
3. get_cwd() returns non-NULL

---

## Running the Tests

### Build the test suite:

```bash
make test
```

This command will:

1. Compile all necessary source files
2. Compile the test suite
3. Link and create the test runner executable
4. Execute all tests

### Run only the shell:

```bash
make shell
./shell
```

### Clean build artifacts:

```bash
make clean
```

## Test Results

The test suite generates a summary report showing:

- Total number of tests run
- Number of tests passed
- Number of tests failed
- Overall pass rate percentage

### Example Output:

```
Mini Unix Shell - Comprehensive Test Suite
==========================================

=== Test 1: Parser - Simple Command ===
  ✓ Parser creates command structure
  ✓ Command argv is allocated
  ✓ First argument is 'ls'
  ...

========================================
Test Results Summary
========================================
Total Tests:  XX
Passed:       XX
Failed:       X
Pass Rate:    XX.X%
========================================
```

## Coverage Summary

The test suite covers:

### Parser Functionality (11 test suites)

- ✓ Simple command parsing
- ✓ Input redirection
- ✓ Output redirection
- ✓ Background execution
- ✓ Pipelines (single and multi-stage)
- ✓ Complex command combinations
- ✓ Edge cases (empty input)
- ✓ Multiple arguments and flags
- ✓ Dual redirections
- ✓ Complex flag parsing
- ✓ Pipeline with redirections

### Utility Functions (1 test suite)

- ✓ Working directory functions (get_home, get_pwd, get_cwd)

## Future Test Enhancements

Potential areas for expansion:

- Integration tests for actual command execution
- Executor functionality tests
- Builtin command tests (cd, exit)
- Performance/stress testing
- Memory leak detection
- Signal handling tests
- Child process management tests
- Error handling and recovery tests
