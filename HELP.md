# Help — Simple Line Editor

Start the editor with `./editor`. You'll see a `>` prompt. Type one
command per line and press Enter. Line numbers are **1-indexed** (the
first line of the document is line 1).

---

### `i <line#> <text>` — insert a line

Inserts `<text>` as the given line number. Every existing line at or
after that position shifts down by one. Use `<line#>` equal to
"last line + 1" to append at the end.

```
> i 1 Hello world
Inserted line 1.
> i 2 This is the second line
Inserted line 2.
```

### `d <line#>` — delete a line

Removes the given line number. Every line after it shifts up by one.

```
> d 2
Deleted line 2.
```

### `p` — display (print) the document

Shows every current line with its line number.

```
> p
   1 | Hello world
   2 | Another line
```

### `f <word>` — find / search

Reports every line number that contains the given word or phrase.

```
> f world
   1 | Hello world
(1 line matched)
```

### `r <line#> <old> <new>` — replace on one line

Replaces `<old>` with `<new>` on a single line. `<old>` and `<new>` are
each a single word (no spaces).

```
> r 1 Hello Goodbye
Replaced "Hello" with "Goodbye" on line 1.
```

### `ra <old> <new>` — replace everywhere

Same as `r`, but applies to every line in the document.

```
> ra the THE
Replaced "the" with "THE" on 3 lines.
```

### `wc` — word count / line count

Prints how many lines and words the document currently has.

```
> wc
Lines: 2
Words: 5
```

### `s <filename>` — save

Writes the current document to a plain-text file, one line per row.

```
> s notes.txt
Saved to notes.txt.
```

### `l <filename>` — load

Reads a text file into the editor, **replacing** whatever document is
currently open. Each line of the file becomes one line of the document.

```
> l notes.txt
Loaded notes.txt (2 lines).
```

### `h` — help

Prints this command list from inside the program.

### `q` — quit

Exits the editor. Remember to `s <filename>` first if you want to keep
your changes!

---

## Notes on invalid input

The editor won't crash on bad input — it prints an error and returns you
to the prompt. For example:

```
> d 99
Error: line 99 does not exist.
> i 50 too far ahead
Error: line 50 is out of range (valid: 1 to 1).
```
