#!/bin/bash

# Test runner for OrionCC compiler
# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

ORIONCC="./bin/orioncc"
TEST_DIR="tests"
PASSED=0
FAILED=0
TOTAL=0

echo "Running OrionCC Test Suite"
echo "=========================="
echo ""

# Function to run a test case
run_test() {
    local test_name="$1"
    local input="$2"
    local expected="$3"
    local should_fail="$4"
    
    TOTAL=$((TOTAL + 1))
    
    # Create temporary test file
    echo "$input" > /tmp/test_input.oc
    
    # Run the compiler
    output=$($ORIONCC /tmp/test_input.oc 2>&1)
    exit_code=$?
    
    if [ "$should_fail" = "true" ]; then
        # Test should fail
        if [ $exit_code -ne 0 ]; then
            echo -e "${GREEN}✓${NC} $test_name (correctly failed)"
            PASSED=$((PASSED + 1))
        else
            echo -e "${RED}✗${NC} $test_name (should have failed but succeeded)"
            echo "  Output: $output"
            FAILED=$((FAILED + 1))
        fi
    else
        # Test should succeed
        if [ $exit_code -eq 0 ]; then
            # Extract result value
            result=$(echo "$output" | grep "Result:" | awk '{print $2}')
            if [ "$result" = "$expected" ]; then
                echo -e "${GREEN}✓${NC} $test_name"
                PASSED=$((PASSED + 1))
            else
                echo -e "${RED}✗${NC} $test_name"
                echo "  Expected: $expected"
                echo "  Got: $result"
                FAILED=$((FAILED + 1))
            fi
        else
            echo -e "${RED}✗${NC} $test_name (failed to execute)"
            echo "  Error: $output"
            FAILED=$((FAILED + 1))
        fi
    fi
}

# Basic arithmetic tests
echo "Basic Arithmetic Tests:"
run_test "Simple addition" "1 + 2" "3" "false"
run_test "Simple subtraction" "10 - 3" "7" "false"
run_test "Simple multiplication" "4 * 5" "20" "false"
run_test "Simple division" "20 / 4" "5" "false"
run_test "Single number" "42" "42" "false"
echo ""

# Operator precedence tests
echo "Operator Precedence Tests:"
run_test "Multiplication before addition" "1 + 2 * 3" "7" "false"
run_test "Division before subtraction" "10 - 6 / 2" "7" "false"
run_test "Multiple operations" "2 + 3 * 4 - 5" "9" "false"
run_test "Left-to-right addition" "1 + 2 + 3" "6" "false"
run_test "Left-to-right multiplication" "2 * 3 * 4" "24" "false"
echo ""

# Parentheses tests
echo "Parentheses Tests:"
run_test "Simple parentheses" "(1 + 2) * 3" "9" "false"
run_test "Nested parentheses" "((1 + 2) * 3) + 4" "13" "false"
run_test "Multiple parentheses" "(1 + 2) * (3 + 4)" "21" "false"
run_test "Complex expression" "1 + 2 * (3 + 4)" "15" "false"
run_test "Division with parentheses" "(1 + 2 + 3) / 2" "3" "false"
echo ""

# Unary minus tests
echo "Unary Minus Tests:"
run_test "Negative number" "-5" "-5" "false"
run_test "Negative with addition" "-5 + 10" "5" "false"
run_test "Negative with multiplication" "-5 * 2" "-10" "false"
run_test "Double negative" "--5" "5" "false"
run_test "Negative in parentheses" "(-5) * 2" "-10" "false"
run_test "Complex negative" "10 - -5" "15" "false"
echo ""

# Edge cases
echo "Edge Case Tests:"
run_test "Zero" "0" "0" "false"
run_test "Large number" "999999" "999999" "false"
run_test "Complex calculation" "(10 + 5) * (20 - 8) / 3" "60" "false"
run_test "Many operations" "1 + 2 - 3 + 4 - 5 + 6" "5" "false"
echo ""

# Error handling tests
echo "Error Handling Tests:"
run_test "Division by zero" "5 / 0" "" "true"
run_test "Empty input" "" "" "true"
run_test "Incomplete expression" "1 + " "" "true"
run_test "Missing closing paren" "(1 + 2" "" "true"
run_test "Missing opening paren" "1 + 2)" "" "true"
run_test "Invalid character" "1 + @ + 2" "" "true"
echo ""

# Summary
echo "=========================="
echo "Test Results:"
echo -e "  Passed: ${GREEN}$PASSED${NC}"
echo -e "  Failed: ${RED}$FAILED${NC}"
echo "  Total:  $TOTAL"
echo ""

if [ $FAILED -eq 0 ]; then
    echo -e "${GREEN}All tests passed!${NC}"
    exit 0
else
    echo -e "${RED}Some tests failed.${NC}"
    exit 1
fi
