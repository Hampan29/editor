# Paper Design

This is the design we worked out **before** opening VS Code, as required by
the 0:10–1:05 portion of the two-hour schedule. It's transcribed here for
readability, but represents work that should genuinely happen on paper
first — grab a printout of this file (or your own handwritten version) as
your "photo/scan" deliverable if your team used paper.

## 1. Data structure choice

**Choice: dynamic array of strings** (`char **lines`, plus a `count` and a
`capacity`), rather than a linked list.

| | Dynamic array (chosen) | Linked list |
|---|---|---|
| Access "line N" | O(1) — direct index | O(n) — walk from head |
| Insert/delete at line N | O(n) — shift elements | O(n) to *find* line N, then O(1) to splice |
| Memory layout | Contiguous, cache-friendly | Scattered, extra pointer overhead per node |
| Code complexity | Lower (array indexing) | Higher (pointer bookkeeping) |

Line editor commands are almost always phrased "act on line N" — `p`
(display), `f` (find), `r` (replace) and `wc` (stats) all want to jump
straight to a specific line or scan every line in order. A linked list
would need an O(n) walk just to *locate* line N before doing anything,
which erases the advantage it has for insert/delete. For a small
in-memory document (a few dozen lines, well within a two-hour project's
scope), the O(n) shifting cost of array insert/delete is negligible, so we
get fast reads for "free" in exchange for a cost we barely pay.

The array grows by doubling its capacity whenever it fills up (starting at
4 slots), which is the standard amortized-O(1) append strategy.

## 2. Command set

| Command | Meaning |
|---|---|
| `i <line#> <text>` | Insert `text` as line `<line#>`, shifting later lines down |
| `d <line#>` | Delete line `<line#>`, shifting later lines up |
| `p` | Print/display the whole document with line numbers |
| `f <word>` | Search: report every line containing `<word>` |
| `r <line#> <old> <new>` | Replace `<old>` with `<new>` on one line |
| `ra <old> <new>` | Replace `<old>` with `<new>` on every line |
| `wc` | Show line count and word count |
| `s <filename>` | Save the document to a text file |
| `l <filename>` | Load a document from a text file |
| `h` | Show help |
| `q` | Quit |

## 3. Functions we planned to need

- `doc_init` / `doc_free` — set up and tear down the array
- `doc_insert(doc, line_num, text)` — core feature
- `doc_delete(doc, line_num)` — core feature
- `doc_display(doc)` — core feature
- `doc_save(doc, filename)` / `doc_load(doc, filename)` — bonus
- `doc_search(doc, word)` — bonus
- `doc_replace_line` / `doc_replace_all` — bonus
- `doc_stats(doc)` — bonus

## 4. Hand-written core logic (pseudocode)

This is the logic we sketched by hand for the three required features,
before writing any real C.

```
INSERT(doc, line_num, text):
    if line_num < 1 or line_num > doc.count + 1:
        return FAILURE                # out of range

    if doc.count == doc.capacity:
        grow the array (double capacity)

    index = line_num - 1              # convert to 0-indexed slot
    for i from doc.count down to index + 1:
        doc.lines[i] = doc.lines[i - 1]   # shift right, back-to-front

    doc.lines[index] = copy_of(text)
    doc.count = doc.count + 1
    return SUCCESS


DELETE(doc, line_num):
    if line_num < 1 or line_num > doc.count:
        return FAILURE                # out of range

    index = line_num - 1
    free(doc.lines[index])

    for i from index to doc.count - 2:
        doc.lines[i] = doc.lines[i + 1]   # shift left, front-to-back

    doc.count = doc.count - 1
    return SUCCESS


DISPLAY(doc):
    if doc.count == 0:
        print "(document is empty)"
        return

    for i from 0 to doc.count - 1:
        print (i + 1), doc.lines[i]   # +1 to show 1-indexed line numbers
```

Note the two shift loops run in **opposite directions** on purpose:
- Insert shifts *back-to-front* (highest index first) so we don't
  overwrite a value before we've copied it out of the way.
- Delete shifts *front-to-back* (lowest index first) for the same reason,
  mirrored.

This pseudocode maps almost line-for-line onto the final C in
`src/editor.c` — the paper design step paid off by the time we got to
VS Code.
