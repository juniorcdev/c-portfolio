# my_wc - A wc Implementation in C

A complete implementation of the Unix `wc` (word count) utility in C, built for learning systems programming.

## Features

- ✅ Count lines, words, and bytes
- ✅ Command-line flags: `-l`, `-w`, `-c`
- ✅ Long options: `--lines`, `--words`, `--bytes`
- ✅ Multiple file support with totals
- ✅ Standard input support (`cat file.txt | ./my_wc`)
- ✅ Help (`--help`) and version (`--version`) flags
- ✅ POSIX-compliant line counting (counts incomplete last line)
- ✅ GNU-style error messages

## Building

```bash
make
```

## Usage

```bash
# Basic usage
./my_wc file.txt

# Count only lines
./my_wc -l file.txt
./my_wc --lines file.txt

# Count from stdin
cat file.txt | ./my_wc
./my_wc < file.txt

# Multiple files
./my_wc file1.txt file2.txt

# Help and version
./my_wc --help
./my_wc --version
```

## Testing

```bash
make test
```

Or run individual tests:

```bash
./test_basic.sh
```

## Design decisions

1. Algorithm: State machine for word counting (detects transitions from whitespace)
1. I/O: Uses fgetc() for clarity over performance
1. Formatting: Fixed-width columns (%7d) supporting files up to 9,999,999 lines
1. Error handling: Graceful failure on file open errors, continues with other files
1. POSIX compliance: Counts final line even without trailing newline

## Project Structure

- `my_wc.c` - Main implementation (≈200 lines)
- `Makefile` - Build system
- `test_basic.sh` - Comprehensive test suite
- `README.md` - This documentation

## Learning outcomes

This project demonstrates:
- File I/O in C using FILE* streams
- Command-line argument parsing (short and long options)
- State machine implementation for text processing
- Memory management (stack allocation only)
- Error handling and graceful degradation
- POSIX compliance considerations
- Modular code organization

## Comparison with real wc

|Feature|my_wc|GNU wc|
|:------|:---:|:----:|
|Basic counting|✅|✅|
|Short options (-l, -w, -c)|✅|✅|
|Long options (--lines, etc.)|✅|✅|
|Dynamic column width|❌ (fixed %7d)|✅|
|UTF-8 character count (-m)|❌|✅|
|Maximum line length (-L)|❌|✅|
|Performance|⚠️ (fgetc())|✅ (optimized)|

## License

MIT License - see LICENSE file for details.