# my_grep - A grep Implementation in C

A feature-rich, from-scratch implementation of the Unix `grep` command written in C. Built as a portfolio project to demonstrate systems programming skills for junior C developer roles.

## 🚀 Features

### Core Functionality
- **Pattern Matching**: Basic substring search with naive O(n*m) algorithm
- **Extended Regular Expressions**: POSIX regex support via `regcomp()`/`regexec()` 
- **Multiple File Support**: Process multiple files with proper filename prefixes
- **Standard Input**: Read from stdin when no files provided
- **Exit Codes**: GNU grep-compatible (0=match, 1=no match, 2=error)

### Command-line Flags
| Flag | Long Form | Description |
|------|-----------|-------------|
| `-i` | `--ignore-case` | Case-insensitive matching |
| `-n` | `--line-number` | Print line numbers with output |
| `-v` | `--invert-match` | Select non-matching lines |
| `-c` | `--count` | Print only count of matching lines |
| `-E` | `--regex` | Interpret pattern as extended regex |
| `--color` | | Highlight matches with ANSI color codes |

### Technical Highlights
- **POSIX Compliance**: Uses only standard C libraries and POSIX APIs
- **Memory Safe**: Proper `free()` calls, no memory leaks (verified with Valgrind)
- **Error Handling**: Comprehensive error checking and reporting
- **Terminal Awareness**: Colors auto-disable when output is piped (`isatty()` check)
- **Clean Architecture**: Separation of parsing, matching, and output logic

## 📦 Installation

```bash
# Clone the repository
git clone <repository-url>
cd my_grep

# Build the project
make

# Optional: Install system-wide
sudo make install
```

## 🛠️ Build System

The project includes a professional Makefile with:
- Standard build targets (all, clean, install, uninstall)
- Comprehensive test suite (make test)
- Development quick-test (make quick-test)
- Warning flags for code quality (-Wall -Wextra -pedantic)

## 🧪 Testing

Run the comprehensive test suite:
```bash
make test
```

The test suite includes 40+ tests covering:
- Basic functionality and edge cases
- All command-line flags and combinations
- Regular expression support
- Error handling and exit codes
- Output formatting

## 📖 Usage Examples

```bash
# Basic pattern search
./my_grep "pattern" file.txt

# Case-insensitive search with line numbers
./my_grep -i -n "hello" file1.txt file2.txt

# Count matches in multiple files
./my_grep -c "error" *.log

# Regular expression search
./my_grep -E "[0-9]+" data.txt

# Highlight matches in color (terminal only)
./my_grep --color "TODO" src/*.c

# Invert match (lines NOT containing pattern)
./my_grep -v "debug" app.log

# Read from standard input
cat large_file.txt | ./my_grep "search_term"
```

## 🏗️ Architecture

```text
main.c
├── parse_args() - Command-line argument parsing
├── process_file() - File I/O and line processing
├── raw_string_match() - Naive substring search
├── regex_match() - POSIX regex matching (regcomp/regexec)
├── print_match() - Output formatting
└── print_colored_line() - ANSI color highlighting
```

### Key Design Decisions
1. Line-by-line Processing: Uses getline() for efficient memory allocation
1. Early Regex Compilation: Compiles regex once per line (could be optimized)
1. Terminal Detection: Checks isatty(STDOUT_FILENO) before using colors
1. Match Position Tracking: Returns match positions for color highlighting
1. Error Code Standardization: Follows GNU grep exit code conventions

## 🔍 Performance Notes

- **Substring Search**: Uses naive O(n*m) algorithm - could be optimized with Boyer-Moore
- **Regex Matching**: Recompiles pattern for each line - could cache compiled regex
- **Memory Usage**: getline() dynamically allocates buffers, reused across lines
- **File I/O**: Buffered I/O with FILE* streams for efficiency

## 📊 Comparison with GNU grep

|Feature|my_grep|GNU grep|
|:------|:-----:|:------:|
|Basic substring search	|✅	|✅|
|Extended regex (-E)	|✅	|✅|
|Case-insensitive (-i)	|✅	|✅|
|Line numbers (-n)	|✅	|✅|
|Invert match (-v)	|✅	|✅|
|Count only (-c)	|✅	|✅|
|Color highlighting	|✅	|✅|
|Recursive search (-r)	|❌	|✅|
|Binary file support	|❌	|✅|
|Performance	|Good	|Excellent|

## 📄 License
This project is licensed under the MIT License - see the LICENSE file for details.

## 🎯 Learning Outcomes

This project demonstrates proficiency in:
1. **Systems Programming:** File I/O, process control, POSIX APIs
1. **String Algorithms:** Pattern matching, regex implementation
1. **Memory Management:** Dynamic allocation, cleanup, Valgrind verification
1. **Error Handling:** Comprehensive error checking and reporting
1. **Software Engineering:** Clean architecture, testing, documentation
1. **Build Systems:** Makefile creation and maintenance