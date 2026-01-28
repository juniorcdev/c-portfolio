#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include <string.h>

/* Configuration flags */
struct config {
	bool show_lines;
	bool show_words;
	bool show_chars;
};

struct file_stats {
	int lines;
	int words;
	int chars;
};

/* Function prototypes */
int parse_args(int argc, const char* argv[], struct config *cfg);
struct file_stats count_stream(FILE *fp);
void print_stats(const struct file_stats *stats, const struct config *cfg);
void print_help(const char *prog_name);
void print_version(void);

/*
 * Print help message
 */
void print_help(const char *prog_name) {
    printf("Usage: %s [OPTION]... [FILE]...\n", prog_name);
    printf("Print newline, word, and byte counts for each FILE.\n\n");
    printf("With no FILE read standard input.\n\n");
    printf("Options:\n");
    printf("  -l, --lines            print the line count\n");
    printf("  -w, --words            print the word count\n");
    printf("  -c, --bytes            print the byte count\n");
    printf("      --help             display this help and exit\n");
    printf("      --version          output version information and exit\n\n");
    printf("Examples:\n");
    printf("  %s file.txt            # Count lines, words, bytes\n", prog_name);
    printf("  %s -l file.txt         # Count lines only\n", prog_name);
    printf("  cat file.txt | %s      # Count from stdin\n", prog_name);
}

/*
 * Print version information
 */
void print_version(void) {
    printf("my_wc 1.0\n");
    printf("A wc (word count) implementation for learning C.\n");
    printf("Features: -l, -w, -c flags, multiple files, stdin support.\n");
}

/*
 * Parse command-line arguments.
 * Returns index of first non-option argument (first filename).
 * Sets flags in config struct.
 */
int parse_args(int argc, const char* argv[], struct config *cfg) {
	int i = 1;

	// Initialize all to false
	cfg->show_chars = false;
	cfg->show_words = false;
	cfg->show_lines = false;

	// Parse options
	while (i < argc && argv[i][0] == '-') {
		// Check for long options (--option)
		if (argv[i][1] == '-') {
			if (strcmp(argv[i], "--lines") == 0) {
                cfg->show_lines = true;
            } else if (strcmp(argv[i], "--words") == 0) {
                cfg->show_words = true;
            } else if (strcmp(argv[i], "--bytes") == 0) {
                cfg->show_chars = true;
            } else if (strcmp(argv[i], "--help") == 0) {
                print_help(argv[0]);
                exit(EXIT_SUCCESS);
            } else if (strcmp(argv[i], "--version") == 0) {
                print_version();
                exit(EXIT_SUCCESS);
            } else {
                fprintf(stderr, "%s: unrecognized option '%s'\n", argv[0], argv[i]);
                fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
                exit(EXIT_FAILURE);
            }
		} 
		// Check for short options
		else {
			// Skip the '-' and parse each character
			const char *opt = argv[i] + 1;
	
			if (opt[0] == '\0') {
				// Handle single dash (treat as filename starting with '-')
				break;
			}
	
			for (int j = 0; opt[j] != '\0'; j++) {
				switch (opt[j]) {
					case 'l': cfg->show_lines = true; break;
					case 'w': cfg->show_words = true; break;
					case 'c': cfg->show_chars = true; break;
					default:
						fprintf(stderr, "%s: invalid option -- '%c'\n", argv[0], opt[j]);
		                fprintf(stderr, "Try '%s --help' for more information.\n", argv[0]);
						exit(EXIT_FAILURE);
				}
			}
		}


		
		i++; // Move to next argument
	}

	// If no flags specified, show everything (default behavior)
	if (!cfg->show_lines && !cfg->show_words && !cfg->show_chars) {
				cfg->show_lines = cfg->show_words = cfg->show_chars = true;
	}
	
	return i;  // Return index of first filename
}

/*
 * count_stream - Count characters, words, and lines in a FILE stream
 * 
 * Arguments:
 *   fp - FILE pointer open for reading (must be opened with "r")
 * 
 * Returns:
 *   file_stats struct with character, word, and line counts
 * 
 * Design decisions:
 *   - Uses fgetc() for clarity over performance
 *   - State machine word counting algorithm
 *   - POSIX-compliant line counting
 * 
 * Note: Caller is responsible for closing fp.
 */
struct file_stats count_stream(FILE *fp) {
    struct file_stats stats = {0, 0, 0};
    bool looking_for_word_start = true; // Start in "looking for word" state
    int ch;
	int last_char = 0;
    
	while ((ch = fgetc(fp)) != EOF) {
		stats.chars++;
		last_char = ch;

		if (ch == '\n') {
			stats.lines++;
		}

		if (isspace(ch)) {
			looking_for_word_start = true; // Next non-space starts new word
		} else if (looking_for_word_start) {
			// Found first letter of a word
			stats.words++;
			looking_for_word_start = false; // Now inside a word, stop looking
		}
		// If we're inside a word and see non-whitespace: do nothing

	}

	// Count last line if file doesn't end with newline (POSIX wc behavior)
	if ((stats.chars > 0) && (last_char != '\n')) {
		stats.lines++;
	}

	return stats;
}

/*
 * Print stats of a single file according to configuration.
 */
void print_stats(const struct file_stats *stats, const struct config *cfg) {
	bool first = true;

	if (cfg->show_lines) {
	/*
	* Using fixed width %7d for simplicity.
	* Real wc uses dynamic width based on maximum counts,
	* but that requires two-pass processing or estimation.
	* %7d supports files up to 9,999,999 lines/words/chars.
	*/
		printf("%7d", stats->lines);
		first = false;
	}
	
	if (cfg->show_words) {
		if (!first) printf(" ");
		printf("%7d", stats->words);
		first = false;
	}
	if (cfg->show_chars) {
		if (!first) printf(" ");
		printf("%7d", stats->chars);
	}
}

int main(int argc, const char *argv[]){
	struct config cfg;
	int file_start = parse_args(argc, argv, &cfg);

	// Check if we have any files to process
	if (file_start >= argc) {
		struct file_stats stats = count_stream(stdin);
        print_stats(&stats, &cfg);
        printf("\n");
        return EXIT_SUCCESS;

	}
    
	// Process each file
	struct file_stats total = {0, 0, 0};
	int file_count = 0;

	for (int i = file_start; i < argc; i++) {
		FILE *fp = fopen(argv[i], "r");
		if (fp == NULL) {
			fprintf(stderr, "%s: cannot open '%s'\n", argv[0], argv[i]);
			continue; // Skip to next file
		}

		struct file_stats stats = count_stream(fp);
		fclose(fp);
		
		print_stats(&stats, &cfg);
		printf(" %s\n", argv[i]); // Print filename

		// Accumulate total
		total.lines += stats.lines;
		total.words += stats.words;
		total.chars += stats.chars;
		file_count++;
	}

	// Print total if multiple files
	if (file_count > 1) {
		print_stats(&total, &cfg);
		printf(" total\n");
	}

	return EXIT_SUCCESS;
}
