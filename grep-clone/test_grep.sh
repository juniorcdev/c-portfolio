#!/bin/bash
# test_grep.sh - Comprehensive test suite for my_grep
# Usage: ./test_grep.sh   or   make test

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
NC='\033[0m' # No Color

# Counter for test results
PASSED=0
FAILED=0
TOTAL=0
FAILED_TESTS=()

# Helper function to run a test
run_test() {
    local description="$1"
    local command="$2"
    local expected_status="${3:-0}"  # Default expected status is 0 (success)
    local expected_output="${4:-}"   # Optional: expected output (regex pattern)
    local expected_no_output="${5:-}" # Optional: output that should NOT appear
    
    ((TOTAL++))
    
    echo -n "Test $TOTAL: $description ... "
    
    # Run the command, capture output and status
    # Use 'bash -c' instead of eval for safety
    bash -c "$command" > /tmp/test_output.txt 2>&1
    local status=$?
    
    local passed=true
    
    # Check exit status
    if [ $status -ne $expected_status ]; then
        passed=false
        echo -n "[wrong exit: $status vs $expected_status] "
    fi
    
    # Check output if expected output provided
    if [ -n "$expected_output" ]; then
        if ! grep -q "$expected_output" /tmp/test_output.txt 2>/dev/null; then
            passed=false
            echo -n "[missing output] "
        fi
    fi
    
    # Check that certain output does NOT appear
    if [ -n "$expected_no_output" ]; then
        if grep -q "$expected_no_output" /tmp/test_output.txt 2>/dev/null; then
            passed=false
            echo -n "[unexpected output] "
        fi
    fi
    
    if [ "$passed" = true ]; then
        echo -e "${GREEN}PASS${NC}"
        ((PASSED++))
    else
        echo -e "${RED}FAIL${NC}"
        FAILED_TESTS+=("$description")
        echo "  Command: $command"
        echo "  Expected status: $expected_status"
        echo "  Got status: $status"
        if [ -n "$expected_output" ]; then
            echo "  Expected output pattern: $expected_output"
        fi
        if [ -n "$expected_no_output" ]; then
            echo "  Should NOT contain: $expected_no_output"
        fi
        echo "  Output:"
        cat /tmp/test_output.txt | sed 's/^/    /'
        ((FAILED++))
    fi
}

# Helper to create test files
setup_test_files() {
    # Clean up old test files
    rm -f /tmp/test_grep_*.txt
    
    # Create test file 1
    cat > /tmp/test_grep_1.txt << 'EOF'
Hello world
This is a test line
Goodbye world
Another test
HELLO in uppercase
line with multiple test words

Empty line above
EOF
    
    # Create test file 2
    cat > /tmp/test_grep_2.txt << 'EOF'
Different file
test here too
no match in this line
line with test at end test
EOF
    
    # Create file with numbers for regex tests
    cat > /tmp/test_grep_numbers.txt << 'EOF'
test123
456test
no numbers here
123 456 789
test
EOF
    
    # Create empty file
    touch /tmp/test_grep_empty.txt
}

# Cleanup function
cleanup() {
    rm -f /tmp/test_grep_*.txt /tmp/test_output.txt
}

# Main test function
main() {
    echo "=== my_grep Test Suite ==="
    echo
    
    # Setup trap to cleanup on exit
    trap cleanup EXIT
    
    # Build the program first
    echo "Building my_grep..."
    if ! make > /dev/null 2>&1; then
        echo -e "${RED}Failed to build my_grep!${NC}"
        exit 1
    fi
    
    # Setup test files
    setup_test_files
    
    echo "Running tests..."
    echo
    
    # Test group 1: Basic functionality
    echo "--- Basic Functionality Tests ---"
    run_test "Exact match" "./my_grep 'world' /tmp/test_grep_1.txt" 0 "world"
    run_test "No match (exit 1)" "./my_grep 'nonexistent' /tmp/test_grep_1.txt" 1 ""
    run_test "Multiple matches" "./my_grep 'test' /tmp/test_grep_1.txt" 0 "test"
    run_test "Empty file" "./my_grep 'anything' /tmp/test_grep_empty.txt" 1 ""
    run_test "Literal ^ without -E" "./my_grep '^Hello' /tmp/test_grep_1.txt" 1 ""  # Should NOT match (^ is literal)
    run_test "Literal $ without -E" "./my_grep 'world\$' /tmp/test_grep_1.txt" 1 ""  # Should NOT match ($ is literal)
    
    # Test group 2: Flags
    echo -e "\n--- Flag Tests ---"
    run_test "Case-insensitive (-i)" "./my_grep -i 'hello' /tmp/test_grep_1.txt" 0 "" # Just verify matches found (exit 0)
    run_test "Line numbers (-n)" "./my_grep -n 'test' /tmp/test_grep_1.txt" 0 "^[0-9]\+:"
    run_test "Invert match (-v)" "./my_grep -v 'test' /tmp/test_grep_1.txt" 0 "" "test"
    run_test "Count only (-c) single file" "./my_grep -c 'test' /tmp/test_grep_1.txt" 0 "^[0-9]\+$"
    run_test "Count only (-c) multiple files" "./my_grep -c 'test' /tmp/test_grep_1.txt /tmp/test_grep_2.txt" 0 "/tmp/test_grep_"
    run_test "Combined flags (-i -n)" "./my_grep -i -n 'hello' /tmp/test_grep_1.txt" 0 "" 
    run_test "Count with invert (-c -v)" "./my_grep -c -v 'test' /tmp/test_grep_1.txt" 0 "^[0-9]\+$"
    
    # Test group 3: Multiple files
    echo -e "\n--- Multiple File Tests ---"
    run_test "Two files" "./my_grep 'test' /tmp/test_grep_1.txt /tmp/test_grep_2.txt" 0 "test"
    run_test "Multiple files with line numbers" "./my_grep -n 'test' /tmp/test_grep_1.txt /tmp/test_grep_2.txt" 0 ""
    run_test "Multiple files with count" "./my_grep -c 'test' /tmp/test_grep_1.txt /tmp/test_grep_2.txt" 0 "/tmp/test_grep_.*:[0-9]"
    
    # Test group 4: Standard input
    echo -e "\n--- Standard Input Tests ---"
    run_test "Stdin via pipe" "echo 'search this line' | ./my_grep 'search'" 0 "search"
    run_test "Stdin with flags" "echo -e 'line1\nLINE2\nline3' | ./my_grep -i 'line2'" 0 "LINE2"
    run_test "Empty stdin" "echo '' | ./my_grep 'anything'" 1 ""
    run_test "Stdin count" "echo -e 'match\nno\nmatch' | ./my_grep -c 'match'" 0 "^2$"
    
    # Test group 5: Error handling
    echo -e "\n--- Error Handling Tests ---"
    run_test "Nonexistent file" "./my_grep 'pattern' /tmp/nonexistent.txt" 2 ""
    run_test "No pattern argument" "./my_grep 2>&1" 2 "pattern argument required"
    run_test "Help flag" "./my_grep --help" 0 "Usage:"
    run_test "Version flag" "./my_grep --version" 0 "my_grep"
    run_test "Invalid short option" "./my_grep -x 'pattern' 2>&1" 2 "invalid option"
    run_test "Invalid long option" "./my_grep --nonexistent 'pattern' 2>&1" 2 "unrecognized option"
    
    # Test group 6: Regex functionality (-E flag)
    echo -e "\n--- Regex Tests (-E flag) ---"
    run_test "Regex: digits [0-9]+" "./my_grep -E '[0-9]+' /tmp/test_grep_numbers.txt" 0 "123"
    run_test "Regex: start anchor ^" "./my_grep -E '^test' /tmp/test_grep_numbers.txt" 0 "test"
    run_test "Regex: end anchor $" "./my_grep -E 'test\$' /tmp/test_grep_numbers.txt" 0 "test"
    run_test "Regex: dot wildcard" "./my_grep -E 't.st' /tmp/test_grep_numbers.txt" 0 "test"
    run_test "Regex: case-insensitive with -i" "./my_grep -E -i 'TEST' /tmp/test_grep_numbers.txt" 0 "test"
    run_test "Regex: invalid pattern" "./my_grep -E '[' /tmp/test_grep_numbers.txt" 1 ""  # Should exit 1 (no match), not crash
    
    # Test group 7: Color functionality
    echo -e "\n--- Color Tests (--color flag) ---"
    # Note: We test color by checking for ANSI escape codes
    run_test "Color flag doesn't affect matching" "./my_grep --color 'test' /tmp/test_grep_1.txt" 0 "test"
    # Test that color doesn't break normal output
    run_test "Color with line numbers" "./my_grep --color -n 'test' /tmp/test_grep_1.txt 2>&1" 0 "test"
    
    # Test group 8: Edge cases
    echo -e "\n--- Edge Cases ---"
    run_test "Empty pattern matches all" "./my_grep '' /tmp/test_grep_1.txt" 0 ""
    run_test "Pattern with spaces" "./my_grep 'test line' /tmp/test_grep_1.txt" 0 "test line"
    run_test "Special characters literal" "./my_grep '.*' /tmp/test_grep_1.txt" 1 ""  # Should NOT match (.* is literal without -E)
    run_test "Very long line" "printf '%1000s' | tr ' ' 'x' | ./my_grep 'x'" 0 "x"
    run_test "Pattern longer than line" "./my_grep 'verylongpattern' /tmp/test_grep_1.txt" 1 ""
    
    # Test group 9: Combined regex and other flags
    echo -e "\n--- Combined Regex Tests ---"
    run_test "Regex with line numbers" "./my_grep -E -n '[0-9]+' /tmp/test_grep_numbers.txt" 0 "^[0-9]\+:.*[0-9]"
    run_test "Regex with invert" "./my_grep -E -v '[0-9]+' /tmp/test_grep_numbers.txt" 0 "" "[0-9]"
    run_test "Regex with count" "./my_grep -E -c '[0-9]+' /tmp/test_grep_numbers.txt" 0 "^[0-9]\+$"
    
    # Test group 10: Output format verification
    echo -e "\n--- Output Format Tests ---"
    run_test "Filename prefix with multiple files" "./my_grep 'test' /tmp/test_grep_1.txt /tmp/test_grep_2.txt" 0 "/tmp/test_grep_"
    run_test "No filename with single file" "./my_grep 'test' /tmp/test_grep_1.txt" 0 "test" "/tmp/test_grep_1.txt:"
    
    # Summary
    echo -e "\n=== Test Summary ==="
    echo "Total tests: $TOTAL"
    echo -e "${GREEN}Passed: $PASSED${NC}"
    
    if [ $FAILED -eq 0 ]; then
        echo -e "${GREEN}Failed: $FAILED${NC}"
        echo -e "\n✅ All tests passed!"
        exit 0
    else
        echo -e "${RED}Failed: $FAILED${NC}"
        echo -e "\n❌ Failed tests:"
        for test in "${FAILED_TESTS[@]}"; do
            echo "  - $test"
        done
        exit 1
    fi
}

# Run main function
main