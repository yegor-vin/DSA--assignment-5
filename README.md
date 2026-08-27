# Assignment 5 — Data Structures and Algorithms

**Author:** Vinnyk Yehor

Implementation and performance comparison of four data structures: an **AVL tree**, a **Splay tree**, a **hash table with separate chaining**, and a **hash table with double hashing**. Each structure supports insert, delete, and find, and all four are benchmarked against each other for increasing input sizes.

## Project layout

| File | Description |
|---|---|
| `avl.h` / `avl.c` | AVL (self-balancing binary search) tree |
| `splay.h` / `splay.c` | Splay tree |
| `hash-chaining.h` / `hash-chaining.c` | Hash table using separate chaining for collision resolution |
| `hash-double.h` / `hash-double.c` | Hash table using double hashing (open addressing) for collision resolution |
| `testing.c` | `main()` entry point; benchmarks all structures and prints timing results |
| `names.txt` | 30 sample first names, used to generate unique string keys for the hash table benchmarks |
| `program.exe` | Prebuilt Windows binary |
| `Data structures and algorithms.pdf` | Original assignment write-up with design notes and measured results |

> **Platform note:** the source uses `<windows.h>` and the `QueryPerformanceCounter` / `QueryPerformanceFrequency` API for high-resolution timing, so this project is Windows-only as written (or requires MinGW-w64 for cross-compiling).

## Building

On Windows with MinGW/gcc:

```bash
gcc -O2 -o program.exe testing.c avl.c splay.c hash-chaining.c hash-double.c
```

`names.txt` must be in the same directory as the executable at run time, since `testing.c` opens it with a relative path.

## Running

```bash
./program.exe
```

The program reads `names.txt`, then runs each benchmark for N = 1 000 / 10 000 / 100 000 / 1 000 000, printing insert/search/delete timings (in seconds) to stdout. In `testing.c`'s `main()`, the AVL/Splay tree benchmark calls are commented out by default — only the two hash table benchmarks (chaining and double hashing) run out of the box. Uncomment the `runBenchmarkTrees(...)` calls to include tree benchmarks as well.

## Data structures

### 1. AVL tree (`avl.c` / `avl.h`)
A self-balancing BST keyed on `int`. After every insert/delete, each node's balance factor `BF = height(left) - height(right)` is checked; if `|BF| > 1`, left/right rotations rebalance the subtree, walking from the modified leaf back up to the root. All operations are **O(log n)**.

- `avlInsertNode` / `avlDeleteNode` — standard BST insert/delete plus bottom-up rebalancing (delete uses in-order successor replacement when a node has two children).
- `avlFindNode` — iterative BST search.
- `avlInorderSearch` / `avlPreorderSearch` / `avlPostorderSearch` — traversal-based search, included for comparison but far less efficient than direct BST search (see benchmarks below).
- `avlFreeTree` — recursively frees all nodes.

### 2. Splay tree (`splay.c` / `splay.h`)
A BST where the most recently accessed node is rotated up to the root ("splaying") via zig / zig-zig / zig-zag rotations. Amortized **O(log n)** per operation, with recently/frequently accessed keys becoming cheaper to reach.

- `splayInsertNode` — BST insert, then splay the new node to the root.
- `splayDeleteNode` — splay the target node to the root, detach it, then splay the maximum of the left subtree to become the new root (joining the two subtrees).
- `splayFindNode` — search, splaying the found node to the root as a side effect.
- Traversal-based searches (`Inorder`/`Preorder`/`Postorder`) are also provided; unlike the AVL case, these are relatively efficient for splay trees per the assignment's own measurements.
- `splayFreeTree` — recursively frees all nodes.

### 3. Hash table — separate chaining (`hash-chaining.c` / `hash-chaining.h`)
String-keyed (`char key[30]`) hash table backed by an array of linked-list buckets. Uses the djb2 hash function. Average-case **O(1)** operations.

- `chainingCreateTable` — allocates the table and initializes all buckets to `NULL`.
- `chainingInsertNode` — resizes (doubles capacity) when load factor exceeds 0.75, then inserts at the head of the target bucket's list.
- `chainingDeleteNode` / `chainingFindNode` — walk the target bucket's linked list.
- `chainingFreeTable` — frees every node and the bucket array.

### 4. Hash table — double hashing (`hash-double.c` / `hash-double.h`)
String-keyed hash table using open addressing with two hash functions: `h1` (djb2) for the initial index and `h2` for the probe step. Each slot tracks a `status`: `0` = empty, `1` = occupied, `2` = deleted (tombstone), which lets probing skip over deleted slots correctly.

- `doubleCreateTable` — allocates the table as a flat array (`calloc`, so all slots start empty).
- `doubleInsertNode` — resizes (`2*capacity + 1`, kept odd to help second-hash coverage) past load factor 0.75; probes for the key or an empty/tombstone slot, reusing the first tombstone seen if the key isn't already present.
- `doubleDeleteNode` / `doubleFindNode` — probe using `h1`/`h2` until the key or an empty (never-used) slot is found.
- `doubleFreeTable` — frees the table array and the struct.

## Benchmark methodology (`testing.c`)

For each structure, `n` integer keys (trees) or `n` unique name-derived string keys (hash tables, built as `<name><index>`) are inserted, then searched, then deleted, with `QueryPerformanceCounter` wrapping each phase. Results are printed as `[<name>] N = <n> | Time taken to <op> <seconds>`.

## Measured results (from the assignment report)

All times in seconds.

**Trees vs. hash tables**

| Operation | Structure | 1 000 | 10 000 | 100 000 | 1 000 000 |
|---|---|---|---|---|---|
| insert | AVL tree | 0.000310 | 0.002581 | 0.036864 | 0.391722 |
| insert | Splay tree | 0.000061 | 0.000581 | 0.007438 | 0.064864 |
| insert | Hash (chaining) | 0.000250 | 0.002611 | 0.042017 | 0.369231 |
| insert | Hash (double) | 0.000263 | 0.003526 | 0.048833 | 0.511038 |
| delete | AVL tree | 0.000197 | 0.001897 | 0.027855 | 0.289316 |
| delete | Splay tree | 0.000009 | 0.000099 | 0.000452 | 0.011927 |
| delete | Hash (chaining) | 0.000186 | 0.002211 | 0.035224 | 0.320670 |
| delete | Hash (double) | 0.000180 | 0.005752 | 0.032602 | 0.323434 |
| search | AVL tree | 0.000049 | 0.000530 | 0.007383 | 0.074640 |
| search | Splay tree | 0.000025 | 0.000286 | 0.004686 | 0.058763 |
| search | Hash (chaining) | 0.000161 | 0.002895 | 0.028802 | 0.288554 |
| search | Hash (double) | 0.000179 | 0.002285 | 0.041256 | 0.321002 |

**Trees only (smaller N)**

| Operation | Structure | 100 | 1 000 | 10 000 |
|---|---|---|---|---|
| insert | AVL tree | 0.000021 | 0.000291 | 0.002722 |
| insert | Splay tree | 0.000014 | 0.000067 | 0.000605 |
| delete | AVL tree | 0.000019 | 0.000160 | 0.002012 |
| delete | Splay tree | 0.000002 | 0.000009 | 0.000046 |
| search | AVL tree | 0.000004 | 0.000045 | 0.000632 |
| search | Splay tree | 0.000003 | 0.000040 | 0.000524 |

**Traversal-based search (inorder/preorder/postorder) vs. direct search**

- AVL tree, 10 000 nodes: ~2.60 s for all three traversal orders — orders of magnitude slower than direct BST search, since traversal ignores sortedness.
- AVL tree, 100 000 nodes: ~26.5–26.7 s for all three orders.
- Splay tree, 100 000 nodes: ~0.0011–0.0013 s for all three orders — roughly 4x faster than the equivalent AVL traversal case, with inorder the best of the three.

## Key takeaways

- **Splay trees** consistently outperform AVL trees on this workload (sequential 1..n insert/search/delete), especially for delete, because the accessed pattern keeps recently-touched nodes near the root.
- **Hash tables** (both variants) are competitive with AVL trees for insert/search/delete on random-ish string keys, though double hashing showed higher variance and generally isn't faster than chaining here.
- Traversal-based search (inorder/preorder/postorder) is a poor substitute for direct BST search on both tree types, but is comparatively far less harmful for splay trees than for AVL trees.
- Per the report, 1,000,000-element hash table runs may fail on constrained hardware; if so, run the hash table benchmarks in isolation.
