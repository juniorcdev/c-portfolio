#define _POSIX_C_SOURCE 200809L // for getline() from <stdio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <errno.h>
#include <regex.h>
#include <unistd.h>

/* Configuration structure */
struct grep_config {
    bool case_insensitive;   // -i flag
    bool show_line_numbers;  // -n flag
    bool invert_match;       // -v flag
    bool count_only;         // -c flag
    bool use_color;          // --color flag (for later)
    bool use_regex;          // -E flag
    const char *pattern;     // The search pattern
};

/* Function prototypes */
void print_help(const char *prog_name);
void print_version(void);
int parse_args(int argc, const char *argv[], struct grep_config *cfg);
static bool raw_string_match(const char *line, const char *pattern, bool case_insensitive, int *match_start, int *match_len);
static bool regex_match(const char *line, const char *pattern, bool case_insensitive, int *match_start, int *match_len);
void print_match(const char *line, int line_num, const char *filename, const struct grep_config *cfg);
int process_file(FILE *fp, const char *filename, struct grep_config *cfg);
void process_stdin(struct grep_config *cfg);
static void print_colored_line(const char *line, int match_start, int match_len, int line_num, const char *filename, const struct grep_config *cfg);

/*
 * Print help message
 */
void print_help(const char *prog_name) {
    printf("Usage: %s [OPTION]... PATTERN [FILE]...\n", prog_name);
    printf("Search for PATTERN in each FILE or standard input.\n\n");
    printf("Options:\n");
    printf("  -i, --ignore-case   ignore case distinctions\n");
    printf("  -n, --line-number   print line number with output lines\n");
    printf("  -v, --invert-match  select non-matching lines\n");
    printf("  -c, --count         print only a count of matching lines\n");
    printf("  -E, --regex         interpret PATTERN as an extended regular expression\n");
    printf("      --color         use colors to highlight matching text\n");
    printf("      --help          display this help and exit\n");
    printf("      --version       output version information and exit\n\n");
    printf("Examples:\n");
    printf("  %s 'hello' file.txt          # Search for 'hello' in file.txt\n", prog_name);
    printf("  %s -i 'HELLO' file.txt       # Case-insensitive search\n", prog_name);
    printf("  cat file.txt | %s 'hello'    # Search stdin\n", prog_name);
}

/*
 * Print version information
 */
void print_version(void) {
    printf("my_grep 1.0\n");
    printf("A grep implementation for learning C.\n");
    printf("Features: basic pattern matching, case-insensitive search, line numbers.\n");
}

/*
 * Parse command-line arguments
 * Returns index of first filename argument, or -1 if error
 */
int parse_args(int argc, const char *argv[], struct grep_config *cfg) {
    // Initialize configuration with defaults
    cfg->case_insensitive = false;
    cfg->show_line_numbers = false;
    cfg->invert_match = false;
    cfg->count_only = false;
    cfg->use_regex = false;
    cfg->use_color = false;
    cfg->pattern = NULL;
    
    int i = 1;
    
    // Parse options
    while (i < argc && argv[i][0] == '-') {
        // Long options
        if (argv[i][1] == '-') {
            if (strcmp(argv[i], "--help") == 0) {
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
            } else if (strcmp(argv[i], "--version") == 0) {
                print_version();
                exit(EXIT_SUCCESS);
            } else if (strcmp(argv[i], "--ignore-case") == 0) {
                cfg->case_insensitive = true;
            } else if (strcmp(argv[i], "--line-number") == 0) {
                cfg->show_line_numbers = true;
            } else if (strcmp(argv[i], "--invert-match") == 0) {
                cfg->invert_match = true;
            } else if (strcmp(argv[i], "--count") == 0) {
                cfg->count_only = true;
            } else if (strcmp(argv[i], "--regex") == 0) {
                cfg->use_regex = true;
            } else if (strcmp(argv[i], "--color") == 0) {
                cfg->use_color = true;
            } else {
                fprintf(stderr, "%s: unrecognized option '%s'\n", argv[0], argv[i]);
                fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
                return -1;
            }
        } 
        // Short options
        else {
            const char *opt = argv[i] + 1;
            
            // Handle combined short options like -inv
            for (int j = 0; opt[j] != '\0'; j++) {
                switch (opt[j]) {
                    case 'i': cfg->case_insensitive = true; break;
                    case 'n': cfg->show_line_numbers = true; break;
                    case 'v': cfg->invert_match = true; break;
                    case 'E': cfg->use_regex = true; break;
                    case 'c': cfg->count_only = true; break;
                    default:
                        fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], opt[j]);
                        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
                        return -1;
                }
            }
        }
        i++;
    }
    
    // Next argument should be the pattern
    if (i >= argc) {
        fprintf(stderr, "%s: pattern argument required\n", argv[0]);
        fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
        return -1;
    }
    
    cfg->pattern = argv[i];
    i++;  // Move past pattern to first filename
    
    return i;  // Index of first filename, or argc if no files
}

/*
 * Case-insensitive character comparison
 */
static inline int char_equal(char a, char b, bool case_insensitive) {
    if (case_insensitive) {
        return tolower((unsigned char)a) == tolower((unsigned char)b);
    }
    return a == b;
}

/*
 * Check if line matches pattern using POSIX extended regular expressions
 * Returns match position via pointers, returns true if match found
 */
static bool regex_match(const char *line, const char *pattern, bool case_insensitive, int *match_start, int *match_len) {
    regex_t regex;
    regmatch_t match;  // This stores match position information
    int flags = REG_EXTENDED | REG_NEWLINE;  // Removed REG_NOSUB - we need match positions!
    
    if (case_insensitive) {
        flags |= REG_ICASE;
    }
    
    // Compile the regular expression
    if (regcomp(&regex, pattern, flags) != 0) {
        return false;
    }
    
    // Execute the regular expression - now we get match position in 'match'
    int result = regexec(&regex, line, 1, &match, 0);
    
    // Free the compiled regex
    regfree(&regex);
    
    if (result == 0) {
        // Match found! match.rm_so = start offset, match.rm_eo = end offset
        *match_start = match.rm_so;
        *match_len = match.rm_eo - match.rm_so;
        return true;
    }
    
    return false;
}

/*
 * Check if line contains pattern (case-sensitive or insensitive)
 * Returns match position via pointers, returns true if match found
 */
static bool raw_string_match(const char *line, const char *pattern, bool case_insensitive, int *match_start, int *match_len) {
    size_t pattern_len = strlen(pattern);
    if (pattern_len == 0) {
        *match_start = 0;
        *match_len = 0;
        return true;  // Empty pattern matches everything at position 0
    }
    
    size_t line_len = strlen(line);
    if (line_len < pattern_len) {
        return false;
    }
    
    // Naive string search - return first match position
    for (size_t i = 0; i <= line_len - pattern_len; i++) {
        bool match = true;
        for (size_t j = 0; j < pattern_len; j++) {
            if (!char_equal(line[i + j], pattern[j], case_insensitive)) {
                match = false;
                break;
            }
        }
        if (match) {
            *match_start = i;
            *match_len = pattern_len;
            return true;
        }
    }
    
    return false;
}

/*
 * Print a matching line with appropriate formatting
 */
void print_match(const char *line, int line_num, const char *filename, const struct grep_config *cfg) {
    // Skip printing if in count-only mode
    if (cfg->count_only) {
        return;
    }
    
    // For invert match, we just print the whole line (no highlighting)
    if (cfg->invert_match) {
        // Print without color since invert means "lines that DON'T match"
        const char *line_end = line + strlen(line);
        if (line_end > line && *(line_end - 1) == '\n') {
            line_end--;
        }
        
        if (filename != NULL) {
            printf("%s:", filename);
        }
        if (cfg->show_line_numbers) {
            printf("%d:", line_num);
        }
        printf("%.*s\n", (int)(line_end - line), line);
        return;
    }
    
    // Find the match position
    int match_start, match_len;
    bool found;
    
    if (cfg->use_regex) {
        found = regex_match(line, cfg->pattern, cfg->case_insensitive, 
                           &match_start, &match_len);
    } else {
        found = raw_string_match(line, cfg->pattern, cfg->case_insensitive,
                                &match_start, &match_len);
    }
    
    if (!found) {
        return;  // Shouldn't happen since print_match is only called for matches
    }
    
    // Check if we should use color and terminal supports colored output
    if (cfg->use_color && isatty(STDOUT_FILENO)) {
        print_colored_line(line, match_start, match_len, line_num, filename, cfg);
    } else {
        // Print without color
        const char *line_end = line + strlen(line);
        if (line_end > line && *(line_end - 1) == '\n') {
            line_end--;
        }
        
        if (filename != NULL) {
            printf("%s:", filename);
        }
        if (cfg->show_line_numbers) {
            printf("%d:", line_num);
        }
        printf("%.*s\n", (int)(line_end - line), line);
    }
}

/*
 * Print a line with the matched portion highlighted in color
 */
static void print_colored_line(const char *line, int match_start, int match_len, int line_num, const char *filename, const struct grep_config *cfg) {
    const char *line_end = line + strlen(line);
    
    // Remove trailing newline if present
    if (line_end > line && *(line_end - 1) == '\n') {
        line_end--;
    }
    
    int line_length = line_end - line;
    
    // Print filename if provided
    if (filename != NULL) {
        printf("%s:", filename);
    }
    
    // Print line number if requested
    if (cfg->show_line_numbers) {
        printf("%d:", line_num);
    }
    
    // Check if match is within bounds (safety check)
    if (match_start < 0 || match_start >= line_length || match_len <= 0) {
        // No valid match position, print entire line normally
        printf("%.*s\n", line_length, line);
        return;
    }
    
    // Ensure we don't go past end of line
    if (match_start + match_len > line_length) {
        match_len = line_length - match_start;
    }
    
    // Print part before match
    if (match_start > 0) {
        printf("%.*s", match_start, line);
    }
    
    // Print match in color (bold red)
    printf("\033[1;31m");  // Start color: bold red
    printf("%.*s", match_len, line + match_start);
    printf("\033[0m");     // Reset color
    
    // Print part after match
    int after_start = match_start + match_len;
    if (after_start < line_length) {
        printf("%.*s", line_length - after_start, line + after_start);
    }
    
    printf("\n");
}

/*
 * Process a single file
 */
int process_file(FILE *fp, const char *filename, struct grep_config *cfg) {
    char *line = NULL;
    size_t buffer_size = 0;
    int line_num = 0;
    int file_matches = 0;

    // Read file line by line
    while ((getline(&line, &buffer_size, fp)) != (ssize_t) -1) {
        line_num++;

        bool matches;
        if (cfg->invert_match) {
            // For invert match, we need the opposite
            if (cfg->use_regex) {
                int dummy1, dummy2;
                matches = !regex_match(line, cfg->pattern, cfg->case_insensitive, &dummy1, &dummy2);
            } else {
                int dummy1, dummy2;
                matches = !raw_string_match(line, cfg->pattern, cfg->case_insensitive, &dummy1, &dummy2);
            }
        } else {
            // Normal match
            if (cfg->use_regex) {
                int dummy1, dummy2;
                matches = regex_match(line, cfg->pattern, cfg->case_insensitive, &dummy1, &dummy2);
            } else {
                int dummy1, dummy2;
                matches = raw_string_match(line, cfg->pattern, cfg->case_insensitive, &dummy1, &dummy2);
            }
        }

        if (matches) {
            file_matches++;
            print_match(line, line_num, filename, cfg);
        }
    }
    
    // If count-only mode
    if (cfg->count_only) {
        if (filename != NULL) {
            printf("%s:", filename);
        }
        printf("%d\n", file_matches);
    }

    // Cleanup
    free(line);

    return file_matches;
}

/*
 * Main function
 */
int main(int argc, const char *argv[]) {
    struct grep_config cfg;
    bool any_matches = false;
    bool any_errors = false;
    
    // Parse arguments
    int file_start = parse_args(argc, argv, &cfg);
    if (file_start == -1) {
        return 2;
    }
    
    // Handle stdin if no files provided
    if (file_start >= argc) {
        int matches = process_file(stdin, NULL, &cfg);
        return matches > 0 ? 0 : 1;
    }
    
    // Track if we have multiple files for filename printing
    bool multiple_files = (argc - file_start) > 1;
    
    // Process each file
    for (int i = file_start; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "%s: cannot open '%s': %s\n", argv[0], argv[i], strerror(errno));
            any_errors = true;
            continue;
        }
        
        // For count-only mode with multiple files, we need filename context
        // We'll handle this inside process_file
        
        int matches = process_file(fp, multiple_files ? argv[i] : NULL, &cfg);
        if (matches > 0) {
            any_matches = true;
        }
        fclose(fp);
    }
    
    // If count-only mode with stdin (already handled) or single file
    // The count is printed inside process_file for files
    
    // After processing all files:
    if (any_errors) return 2;
    if (any_matches) return 0;
    return 1;
}