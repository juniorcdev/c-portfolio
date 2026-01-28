#!/bin/bash
echo "=== Basic wc Tests ==="

# Clean up any old test files
rm -f test_*.txt

# Create test files
echo -e "hello world\nsecond line" > test1.txt
echo -e "line1\nline2\nline3" > test2.txt

echo ""
echo "1. Default output (all counts):"
./my_wc test1.txt

echo ""
echo "2. Multiple files with totals:"
./my_wc test1.txt test2.txt

echo ""
echo "3. Long options:"
./my_wc --lines test1.txt
./my_wc --words test1.txt
./my_wc --bytes test1.txt

echo ""
echo "4. Short options:"
./my_wc -l test1.txt
./my_wc -w test1.txt
./my_wc -c test1.txt

echo ""
echo "5. Mixed options:"
./my_wc -l --words test1.txt

echo ""
echo "6. Stdin via pipe:"
echo "hello world" | ./my_wc

echo ""
echo "7. Stdin via redirect:"
./my_wc < test1.txt

echo ""
echo "8. Help and version:"
./my_wc --help | head -8
./my_wc --version

echo ""
echo "9. Error handling (nonexistent file):"
./my_wc nonexistent.txt 2>&1 | head -1

# Cleanup
rm -f test1.txt test2.txt
echo ""
echo "=== All tests completed ==="