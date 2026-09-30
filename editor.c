/*
 * Simple Line Editor in C
 * Portfolio Building - Studio Course (3rd Semester Coding Competition)
 *
 * Data structure: Dynamic array of strings (char **)
 * Why this choice?
 *   - O(1) access by line number
 *   - Easy to implement insert/delete with shifting
 *   - Sufficient for small documents
 *   - Clear and easy to explain in a viva
 *
 * Core features:
 *   1. Insert a line
 *   2. Delete a line
 *   3. Display the document
 *   4. Save / Load a file
 *
 * Bonus features:
 *   - Search
 *   - Line count / Word count
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define MAX_LINE_LEN   512
#define INITIAL_CAP    8
#define CMD_BUF_SIZE   1024

/* ===================== Global Document State ===================== */
char **lines = NULL;     /* Array of lines (each line is a heap-allocated string) */
int line_count = 0;      /* Current number of lines */
int capacity = 0;        /* Allocated size of the lines array */

/* ===================== Utility Functions ===================== */

/* Trim leading and trailing whitespace from a string (in-place) */
void trim(char *s) {
    if (s == NULL) return;

    /* Trim leading */
    char *start = s;
    while (*start && isspace((unsigned char)*start)) start++;
    if (start != s) memmove(s, start, strlen(start) + 1);

    /* Trim trailing */
    size_t len = strlen(s);
    while (len > 0 && isspace((unsigned char)s[len - 1])) {
        s[len - 1] = '\0';
        len--;
    }
}

/* Make sure the dynamic array has space for at least one more line */
int ensure_capacity(void) {
    if (line_count < capacity) return 1;

    int new_cap = (capacity == 0) ? INITIAL_CAP : capacity * 2;
    char **tmp = realloc(lines, new_cap * sizeof(char *));
    if (tmp == NULL) {
        fprintf(stderr, "Error: Out of memory!\n");
        return 0;
    }
    lines = tmp;
    capacity = new_cap;
    return 1;
}

/* Free all memory used by the document */
void free_document(void) {
    for (int i = 0; i < line_count; i++) {
        free(lines[i]);
    }
    free(lines);
    lines = NULL;
    line_count = 0;
    capacity = 0;
}

/* ===================== Core Feature: Display ===================== */
void display_document(void) {
    if (line_count == 0) {
        printf("Document is empty.\n");
        return;
    }
    printf("----- Document (%d lines) -----\n", line_count);
    for (int i = 0; i < line_count; i++) {
        printf("%3d | %s\n", i + 1, lines[i]);
    }
    printf("--------------------------------\n");
}

/* ===================== Core Feature: Insert ===================== */
/* Insert text at position line_num (1-based). Existing lines shift down. */
int insert_line(int line_num, const char *text) {
    /* Allow inserting at the end (line_num == line_count + 1) */
    if (line_num < 1 || line_num > line_count + 1) {
        printf("Error: Line number must be between 1 and %d.\n", line_count + 1);
        return 0;
    }

    if (!ensure_capacity()) return 0;

    /* Shift lines down to make space */
    for (int i = line_count; i >= line_num; i--) {
        lines[i] = lines[i - 1];
    }

    /* Allocate and copy the new line */
    lines[line_num - 1] = malloc(strlen(text) + 1);
    if (lines[line_num - 1] == NULL) {
        fprintf(stderr, "Error: Out of memory!\n");
        /* Shift back to restore consistency */
        for (int i = line_num - 1; i < line_count; i++) {
            lines[i] = lines[i + 1];
        }
        return 0;
    }
    strcpy(lines[line_num - 1], text);
    line_count++;
    printf("Inserted at line %d.\n", line_num);
    return 1;
}

/* ===================== Core Feature: Delete ===================== */
int delete_line(int line_num) {
    if (line_count == 0) {
        printf("Error: Document is empty. Nothing to delete.\n");
        return 0;
    }
    if (line_num < 1 || line_num > line_count) {
        printf("Error: Line number must be between 1 and %d.\n", line_count);
        return 0;
    }

    free(lines[line_num - 1]);

    /* Shift lines up */
    for (int i = line_num - 1; i < line_count - 1; i++) {
        lines[i] = lines[i + 1];
    }
    line_count--;
    printf("Deleted line %d.\n", line_num);
    return 1;
}

/* ===================== Core Feature: Save ===================== */
int save_file(const char *filename) {
    FILE *fp = fopen(filename, "w");
    if (fp == NULL) {
        printf("Error: Could not open file '%s' for writing.\n", filename);
        return 0;
    }

    for (int i = 0; i < line_count; i++) {
        fprintf(fp, "%s\n", lines[i]);
    }
    fclose(fp);
    printf("Document saved to '%s' (%d lines).\n", filename, line_count);
    return 1;
}

/* ===================== Core Feature: Load ===================== */
int load_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error: Could not open file '%s' for reading.\n", filename);
        return 0;
    }

    /* Clear current document */
    free_document();

    char buffer[MAX_LINE_LEN];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        /* Remove trailing newline if present */
        size_t len = strlen(buffer);
        if (len > 0 && buffer[len - 1] == '\n') {
            buffer[len - 1] = '\0';
        }

        if (!ensure_capacity()) {
            fclose(fp);
            return 0;
        }

        lines[line_count] = malloc(strlen(buffer) + 1);
        if (lines[line_count] == NULL) {
            fprintf(stderr, "Error: Out of memory while loading!\n");
            fclose(fp);
            return 0;
        }
        strcpy(lines[line_count], buffer);
        line_count++;
    }

    fclose(fp);
    printf("Loaded '%s' (%d lines).\n", filename, line_count);
    return 1;
}

/* ===================== Bonus: Search ===================== */
void search_text(const char *word) {
    if (line_count == 0) {
        printf("Document is empty.\n");
        return;
    }

    int found = 0;
    printf("Searching for \"%s\":\n", word);
    for (int i = 0; i < line_count; i++) {
        if (strstr(lines[i], word) != NULL) {
            printf("  Line %d: %s\n", i + 1, lines[i]);
            found = 1;
        }
    }
    if (!found) {
        printf("  No matches found.\n");
    }
}

/* ===================== Bonus: Count ===================== */
void show_counts(void) {
    int words = 0;
    for (int i = 0; i < line_count; i++) {
        char *p = lines[i];
        int in_word = 0;
        while (*p) {
            if (isspace((unsigned char)*p)) {
                in_word = 0;
            } else if (!in_word) {
                in_word = 1;
                words++;
            }
            p++;
        }
    }
    printf("Lines: %d | Words: %d\n", line_count, words);
}

/* ===================== Help ===================== */
void print_help(void) {
    printf("\n========== Simple Line Editor - Commands ==========\n");
    printf("  insert <n> <text>   Insert text at line n (shifts existing lines down)\n");
    printf("  delete <n>          Delete line n\n");
    printf("  display             Show the whole document with line numbers\n");
    printf("  save <filename>     Save document to a text file\n");
    printf("  load <filename>     Load document from a text file (replaces current)\n");
    printf("  search <word>       Find lines containing the given word/phrase\n");
    printf("  count               Show line count and word count\n");
    printf("  help                Show this help message\n");
    printf("  quit / exit         Exit the editor\n");
    printf("===================================================\n");
    printf("Notes:\n");
    printf("  - Line numbers start from 1.\n");
    printf("  - To insert at the end, use n = current_line_count + 1.\n");
    printf("  - Text after the line number is taken as the line content.\n");
    printf("===================================================\n\n");
}

/* ===================== Command Parser & Main Loop ===================== */
void process_command(char *input) {
    trim(input);
    if (strlen(input) == 0) return;

    char cmd[64];
    char *rest = NULL;

    /* Extract first word (command) */
    if (sscanf(input, "%63s", cmd) != 1) return;

    /* Pointer to the rest of the line after the command */
    rest = input + strlen(cmd);
    while (*rest && isspace((unsigned char)*rest)) rest++;

    if (strcmp(cmd, "insert") == 0) {
        int n;
        char text[MAX_LINE_LEN];
        if (sscanf(rest, "%d", &n) != 1) {
            printf("Usage: insert <line_number> <text>\n");
            return;
        }
        /* Skip the number and any spaces after it */
        char *p = rest;
        while (*p && !isspace((unsigned char)*p)) p++;   /* skip number */
        while (*p && isspace((unsigned char)*p)) p++;    /* skip spaces */
        if (*p == '\0') {
            printf("Usage: insert <line_number> <text>\n");
            return;
        }
        strncpy(text, p, MAX_LINE_LEN - 1);
        text[MAX_LINE_LEN - 1] = '\0';
        insert_line(n, text);
    }
    else if (strcmp(cmd, "delete") == 0) {
        int n;
        if (sscanf(rest, "%d", &n) != 1) {
            printf("Usage: delete <line_number>\n");
            return;
        }
        delete_line(n);
    }
    else if (strcmp(cmd, "display") == 0 || strcmp(cmd, "list") == 0 || strcmp(cmd, "print") == 0) {
        display_document();
    }
    else if (strcmp(cmd, "save") == 0) {
        if (strlen(rest) == 0) {
            printf("Usage: save <filename>\n");
            return;
        }
        save_file(rest);
    }
    else if (strcmp(cmd, "load") == 0) {
        if (strlen(rest) == 0) {
            printf("Usage: load <filename>\n");
            return;
        }
        load_file(rest);
    }
    else if (strcmp(cmd, "search") == 0) {
        if (strlen(rest) == 0) {
            printf("Usage: search <word or phrase>\n");
            return;
        }
        search_text(rest);
    }
    else if (strcmp(cmd, "count") == 0) {
        show_counts();
    }
    else if (strcmp(cmd, "help") == 0 || strcmp(cmd, "?") == 0) {
        print_help();
    }
    else if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0) {
        printf("Goodbye!\n");
        free_document();
        exit(0);
    }
    else {
        printf("Unknown command: '%s'. Type 'help' for available commands.\n", cmd);
    }
}

int main(int argc, char *argv[]) {
    char input[CMD_BUF_SIZE];

    printf("========================================\n");
    printf("   Simple Line Editor (C)\n");
    printf("   Type 'help' for commands\n");
    printf("========================================\n");

    /* Optional: load a file given on the command line */
    if (argc >= 2) {
        load_file(argv[1]);
    }

    while (1) {
        printf("editor> ");
        if (fgets(input, sizeof(input), stdin) == NULL) {
            /* Handle Ctrl+D / EOF */
            printf("\n");
            break;
        }
        /* Remove trailing newline */
        size_t len = strlen(input);
        if (len > 0 && input[len - 1] == '\n') {
            input[len - 1] = '\0';
        }
        process_command(input);
    }

    free_document();
    return 0;
}
