# Simple Line Editor (C)

A command-line line editor built for the "Build a Simple Line Editor in C"
studio competition. It holds a small text document in memory as a dynamic
array of strings and lets you insert, delete, view, search, and save/load
lines through short typed commands — no GUI.

## Team

> Fill in before submitting:

- Name 1 — role/what they worked on
- Name 2 — role/what they worked on
- Name 3 — role/what they worked on

## Features implemented

**Core (required 2–3):**
- [x] Insert a line
- [x] Delete a line
- [x] Display the document

**Bonus (extra credit):**
- [x] Save / load a file
- [x] Search
- [x] Find & replace (single line and whole-document)
- [x] Line count / word count

(Undo was intentionally left out — the problem statement flags it as the
most implementation-heavy bonus, and we prioritized polishing the other
five features and the documentation instead.)

## Project structure

```
line-editor/
├── src/
│   ├── main.c      # command-line loop: reads input, dispatches to editor.c
│   ├── editor.c    # the actual line-editor logic (insert/delete/etc.)
│   └── editor.exe    # The compilation folder
├── Makefile
├── DESIGN.md         # paper design: data structure choice + pseudocode
├── HELP.md            # full command reference with examples
└── README.md          # this file
```

## How to compile and run

Requires `gcc` and `make` (standard on Linux/macOS; on Windows, use WSL or
MinGW).

```bash
make        # builds the 'editor' executable
./editor    # run it
```

or in one step:

```bash
make run
```

To clean up build artifacts:

```bash
make clean
```

## Quick start

```
$ ./editor
Simple Line Editor -- type 'h' for help, 'q' to quit.
> i 1 Hello world
Inserted line 1.
> i 2 This is a line editor
Inserted line 2.
> p
   1 | Hello world
   2 | This is a line editor
> s mydoc.txt
Saved to mydoc.txt.
> q
Goodbye!
```

See [`HELP.md`](HELP.md) for the full command list and [`DESIGN.md`](DESIGN.md)
for the data structure justification and paper-design pseudocode.

## Design notes

We chose a **dynamic array of strings** over a linked list because every
command in this editor addresses a specific line number, and an array
gives O(1) access to "line N" instead of an O(n) walk. The full trade-off
discussion is in `DESIGN.md`.

## Known limitations

- `r`/`ra` treat `<old>` and `<new>` as single words (no spaces), to keep
  command parsing simple within the two-hour scope.
- Lines longer than 1023 characters are truncated when loaded from a file.
- No undo — see "Features implemented" above.
