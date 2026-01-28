# C Systems Programming Portfolio

A collection of from-scratch implementations demonstrating systems programming skills in C. Built as part of a structured learning path for junior C developer roles.

## 📋 Projects

### 1. [my_wc - Unix wc Clone](wc-clone/)
**Features:**
- Complete `wc` (word count) implementation with character, word, and line counting
- Supports all standard flags: `-l` (lines), `-w` (words), `-c` (bytes)
- Handles multiple files and standard input
- POSIX-compliant line counting (files ending without newline)
- Clean command-line parsing with short/long options
- **Skills demonstrated:** File I/O, state machines, character processing, command-line parsing, modular design

### 2. [my_grep - Unix grep Clone](grep-clone/)
**Features:**
- Complete grep implementation with substring search
- Extended regular expressions via POSIX APIs
- All major flags: `-i`, `-n`, `-v`, `-c`, `-E`, `--color`
- Terminal-aware color highlighting
- 40+ comprehensive tests, professional Makefile

**Skills demonstrated:** String algorithms, POSIX APIs, command-line parsing, file I/O, memory management, testing

### 3. [vector_int - Dynamic Array Library](vector-library/)
**Features:**
- Complete dynamic array implementation for integers
- Amortized O(1) push/pop with doubling strategy
- Full API: creation, access, search, memory management
- Opaque pointer pattern for encapsulation
- 50+ test assertions, example programs

**Skills demonstrated:** Data structures, algorithm analysis, API design, memory management, software engineering patterns

## 🛠️ Technologies

- **Language**: C99 with POSIX extensions
- **Build**: GNU Make with professional targets
- **Testing**: Comprehensive test suites with assertions
- **Tools**: GCC, Valgrind, Git
- **Platform**: Linux/Unix compatible

## 📈 What These Projects Demonstrate

1. **Systems Programming**: File I/O, process control, POSIX APIs
2. **Algorithm Design**: String search, dynamic array growth, amortized analysis
3. **Software Engineering**: Clean architecture, testing, documentation
4. **Memory Management**: Proper allocation/deallocation, no leaks
5. **Professional Practices**: Build systems, version control, API design

## 👨💻 Author

**[Антон Иванов]**  
- GitHub: [@juniorcdev](https://github.com/juniorcdev)
- Telegram: [tg](https://t.me/ylosss)
- Email: helloworld202101@proton.me

*Open to junior C developer roles. Currently building a fourth project (HTTP server).*