# PG_Projects

Coursework from my studies in Data Engineering at **Gdańsk University of Technology** (WETI),
2024–2026.

The originals are in Polish. The summaries below are in English.

| Folder | Course | Language | What it is |
|---|---|---|---|
| [`projekt_procesor_stosowy`](projekt_procesor_stosowy) | Algorithms and Data Structures | C++ | Emulator for a symbolic-object stack processor |
| [`projekt_wyciagami_i_gorami`](projekt_wyciagami_i_gorami) | Algorithms and Data Structures | C++ | Fastest route across terrain with scheduled one-way lifts |
| [`dynamic_sparse_table`](dynamic_sparse_table) | Algorithms and Data Structures | C++ | n×k trie with insert, lookup, delete and preorder traversal |
| [`Car_dealership_DB`](Car_dealership_DB) | Databases | SQL (SQL Server) | 12-table schema, 8 analytical queries, transaction analysis |

---

## 1. Stack processor emulator — `projekt_procesor_stosowy`

**Task.** Emulate a processor designed for a fictional space agency. It has a program memory,
an instruction pointer and a stack; each instruction is a *single character*. Stack elements
are lists of characters, and a number is stored as its digits least-significant-first with an
optional trailing minus (so `-1234` is the list `4321-`). There are about twenty instructions:
push empty list, duplicate, swap, indirect stack access, read a character from input, print,
logical negation, comparisons, conditional jump, negation/absolute value, split, concatenate,
addition, ASCII conversion, and dump the stack.

**Constraints.**
- No STL except input/output.
- Arrays may only hold the program and the input — everything else must be dynamic.
- At most **one** `for`/`while`/`do-while` in the whole program; iteration must be done with
  **recursion**.

**Approach.** The stack is a chain of nodes allocated by hand, each holding a linked list of
characters, so list operations are pointer surgery rather than indexed access. The instruction
cycle is driven by recursion, which is why the program has exactly one `for` statement and no
`while` at all. Numbers are handled as character lists, with digit-wise arithmetic written out
by hand instead of relying on numeric limits.

**If you build this on Windows:** deep recursion will exhaust the default 1 MB stack. The task
itself notes the judge uses 10 MB; in Visual Studio that is
*Project → Properties → Linker → System → Stack Reserve Size*.

**File:** `Źródło.cpp`

---

## 2. Fastest route across terrain with lifts — `projekt_wyciagami_i_gorami`

**Task.** A map of rectangular cells, each with a height. Moving to a neighbouring cell costs
`A - B + 1` minutes going uphill (height A from height B) and 1 minute otherwise. On top of
that, the map has one-way lifts: each lift has a start, a destination, a ride time, and a
departure interval — it only leaves on minutes that are multiples of that interval. So arriving
at a lift at minute 8 when it runs every 5 minutes means waiting 2 minutes. Find the minimum
time from start to destination.

**Why it is not plain Dijkstra.** The cost of a lift edge depends on when you arrive, so the
"distance to a cell" is not enough state. The search runs over (cell, arrival time) with waiting
time computed as the next multiple of the interval — i.e. Dijkstra over an augmented state space,
where the terrain moves are the ordinary edges and the lifts are the time-dependent ones.

**Approach.** A binary min-heap is implemented by hand, with a position index so a node's key
can be lowered in place instead of pushing duplicates. Adjacency for lifts is a linked list per
cell. No `std::priority_queue`, no `std::vector`.

**Complexity.** `O((V + E) log V)`, as standard for Dijkstra with a binary heap.

**File:** `Źródło.cpp`; sample inputs and expected outputs are in `wyc_i_gor/`.

---

## 3. Dynamic sparse table — `dynamic_sparse_table`

**Task.** Implement the `n × k` trie from the sparse-table description: the root has `n`
children, every other node has `k`. A key `x` descends by `x mod n` at the root and `x mod k`
below it. Insert and lookup follow that path; delete is harder, because deleting an internal
node means promoting a leaf — the task fixes the rule: take the **leftmost external
descendant**. The output must be the preorder listing of the tree, which is exactly why the
deletion rule is specified so precisely.

**Approach.** Recursive insert, lookup, delete and preorder printing, with child arrays allocated
on demand and freed by hand. Two independent iterations are kept, which is the clearest way to
show the design trade-off:

| File | Design |
|---|---|
| `Źródło.cpp` | Separate root and branch node types, with an explicit "key present" flag |
| `source_3.cpp` | A single node type, using a sentinel key to mark an empty slot |
| `source_2.cpp` | Earlier iteration of the root/branch design |

`Źródło.cpp` is what the Visual Studio project compiles (it is the submission); the other two
are earlier experiments kept for comparison.

**Complexity.** A lookup takes a fixed number of steps — one hop of width `n`, then one of width
`k` — which is the property that makes this structure predictable where a hash table has a
worst case.

**Files:** `*.cpp`; reference inputs and outputs in `1/`, `2/`, `3/`, `4/`.

---

## 4. Car dealership database — `Car_dealership_DB`

**Task.** Design and implement a database for a car dealership: schema with integrity
constraints, seed data, business queries, and an analysis of transaction isolation.

**Schema — 12 tables.** `Location`, `Producers`, `Engine`, `Tires`, `Trim`, `Person`, `Models`,
`Salesperson`, `Customer`, `Version`, `Orders`, `Cars`.

Design decisions worth pointing at:
- `Person` is the identity table; `Salesperson` and `Customer` are 1:1 extensions keyed by
  `PESEL` so a person cannot be a customer twice.
- `Cars` is keyed by `VIN`, constrained to exactly 17 characters.
- Business rules are enforced in the schema, not only in the application: valid `PESEL`
  (digits only), customer must be 18+, positive prices and horsepower, enumerations for
  order status, car status, fuel type, tyre season and trim material via `CHECK`.
- Foreign keys cascade on update where the referenced value may change, and are `NO ACTION`
  where history matters (orders).

**Queries (`SQL/`).** Eight analytical queries, each with a written business justification:
master inventory view (replacing raw IDs with readable names), salesperson performance
leaderboard, luxury stock above average price, unsold high-end models, revenue by brand,
winter-tyre marketing list, best performers against the management average commission, and a
monthly sales report.

**Transactions (`SQL/Transactions.docx`).** Three concurrency cases, each with the anomaly it
risks and the isolation level that prevents it:

| Case | Risk | Isolation level |
|---|---|---|
| The same car sold twice | Lost update | `SERIALIZABLE` |
| Monthly revenue report while orders are inserted | Phantoms | `SERIALIZABLE` |
| Bulk price update while customers browse | Dirty read | `READ COMMITTED` |

Each case ships with a correct script and a deliberately broken one (`Transaction_good*.sql`,
`Transaction_bad*.sql`) to demonstrate the failure.

**Also here:** `erd.png` (entity-relationship diagram), `Databases_corrected.docx` (full report),
`SQL/screenshots.docx` (query results).

---

## Building and running the C++ projects

Each is a single translation unit with no dependencies beyond the standard headers for I/O.

```bash
# GCC / Clang
g++ -O2 -std=c++17 Źródło.cpp -o program
./program < input.txt > output.txt

# Windows, MSVC
cl /O2 /EHsc Źródło.cpp
```

For the stack processor, raise the stack reserve if you build with MSVC as mentioned before.

---

## About

Written by **Igor Koziński** — Data Engineering, Gdańsk University of Technology.
