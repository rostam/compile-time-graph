# compile-time-graph

A header-only C++23 library for **compile-time graph algorithms**. All computations run at compile time using `consteval`, producing zero-overhead constants embedded directly into your binary.

## Features

- **Fully compile-time** — every algorithm is `consteval`; results are available as `constexpr` constants verified with `static_assert`
- **Header-only** — just add `include/` to your include path
- **Directed & undirected** graphs via a template parameter
- **Built-in generators** for common graph families (complete, cycle, path, star, bipartite, Petersen, DAG chain)
- **GraphViz export** for visualization

## Quick Start

```cpp
#include "ctgraph/generators.hpp"
#include "ctgraph/properties.hpp"
#include "ctgraph/coloring.hpp"

constexpr auto k5 = generate_complete_graph<5>();

// All results computed at compile time
static_assert(ctgraph::is_connected(k5));
static_assert(ctgraph::diameter(k5) == 1);
static_assert(ctgraph::count_edges(k5) == 10);

constexpr auto coloring = ctgraph::color_graph_welsh_powell(k5);
static_assert(ctgraph::is_valid_coloring(k5, coloring));
static_assert(coloring.num_colors == 5);
```

## Building

```bash
cmake -B build -DCMAKE_CXX_STANDARD=23
cmake --build build
ctest --test-dir build
```

## Algorithm Comparison

### Time Complexity

| Algorithm | Function | Time Complexity | Method |
|---|---|---|---|
| Edge query | `has_edge(graph, u, v)` | O(deg(u)) | Linear scan of adjacency list |
| Edge count | `count_edges(graph)` | O(V) | Sum of degree array |
| Degree statistics | `degree_stats(graph)` | O(V) | Single pass over degree array |
| Connectivity | `is_connected(graph)` | O(V + E) | BFS from node 0 |
| Cycle detection | `has_cycle(graph)` | O(V + E) | DFS with 3-state coloring |
| Bipartiteness | `is_bipartite(graph)` | O(V + E) | BFS 2-coloring |
| Shortest path | `shortest_path_length(graph, src, dst)` | O(V + E) | BFS (unweighted) |
| Diameter | `diameter(graph)` | O(V · (V + E)) | BFS from every node |
| Topological sort | `topological_sort(graph)` | O(V + E) | Kahn's algorithm |
| Connected components | `connected_components(graph)` | O(V + E) | BFS per component |
| DFS traversal | `dfs(graph, start)` | O(V + E) | Iterative with explicit stack |
| BFS traversal | `bfs(graph, start)` | O(V + E) | Iterative with queue |
| Greedy coloring | `color_graph(graph)` | O(V · deg) | Sequential node order |
| Welsh-Powell coloring | `color_graph_welsh_powell(graph)` | O(V² + V · deg) | Degree-sorted greedy |

### Graph Property Results Across Graph Types

| Property | K₅ | Petersen | C₆ | P₅ | S₆ | K₃,₃ |
|---|---|---|---|---|---|---|
| Nodes | 5 | 10 | 6 | 5 | 6 | 6 |
| Edges | 10 | 15 | 6 | 4 | 5 | 9 |
| Min degree | 4 | 3 | 2 | 1 | 1 | 3 |
| Max degree | 4 | 3 | 2 | 2 | 5 | 3 |
| Connected | ✓ | ✓ | ✓ | ✓ | ✓ | ✓ |
| Has cycle | ✓ | ✓ | ✓ | ✗ | ✗ | ✓ |
| Bipartite | ✗ | ✗ | ✓ | ✓ | ✓ | ✓ |
| Diameter | 1 | 2 | 3 | 4 | 2 | 2 |

### Shortest Path Comparison

| Graph | Source → Target | Distance |
|---|---|---|
| K₅ | 0 → 4 | 1 |
| P₅ | 0 → 4 | 4 |
| P₅ | 1 → 3 | 2 |
| C₆ | 0 → 3 | 3 |
| C₆ | 0 → 1 | 1 |
| Petersen | 0 → 1 | 1 |

### Traversal: DFS vs BFS

Both algorithms visit all reachable nodes but in different orders:

| Graph | Algorithm | Visit Count | Exploration Strategy |
|---|---|---|---|
| Petersen (10 nodes) | DFS | 10 | Explores deep branches first via explicit stack |
| Petersen (10 nodes) | BFS | 10 | Explores level-by-level via queue |
| K₅ (5 nodes) | DFS | 5 | All neighbors adjacent — explores one deep path |
| K₅ (5 nodes) | BFS | 5 | All neighbors adjacent — explores all at depth 1 |
| P₅ (5 nodes) | BFS | 5 | Visits in order: 0 → 1 → 2 → 3 → 4 |
| DAG chain (4 nodes) | DFS | 4 | Visits in order: 0 → 1 → 2 → 3 |
| C₆ (6 nodes) | DFS | 6 | Follows one direction around the cycle |
| S₅ (star, 5 nodes) | BFS | 5 | Center first, then all leaves at depth 1 |

### Coloring: Greedy vs Welsh-Powell

Both algorithms assign the smallest available color to each node, but differ in the order nodes are processed:

| Aspect | Greedy (`color_graph`) | Welsh-Powell (`color_graph_welsh_powell`) |
|---|---|---|
| Node ordering | Sequential (0, 1, 2, …) | Sorted by degree (descending) |
| Sorting step | None | O(V²) bubble sort |
| Coloring step | O(V · deg) | O(V · deg) |
| Overall complexity | O(V · deg) | O(V² + V · deg) |
| Color quality | May use more colors | Usually fewer colors on irregular graphs |
| Best for | Regular graphs (all degrees equal) | Graphs with varying node degrees |

Both produce valid colorings — no two adjacent nodes share the same color. Welsh-Powell's degree-based ordering tends to color high-degree nodes first, leaving more color choices for lower-degree nodes.

**Example — Petersen graph** (3-regular, χ = 3):

| Algorithm | Colors used | Valid |
|---|---|---|
| Welsh-Powell | ≤ 4 | ✓ |

**Example — K₅** (4-regular, χ = 5):

| Algorithm | Colors used | Valid |
|---|---|---|
| Welsh-Powell | 5 | ✓ |

On regular graphs (like K₅ where every node has the same degree), the node ordering has no effect, so both algorithms produce the same number of colors.

## Graph Generators

| Generator | Function | Nodes | Edges | Undirected |
|---|---|---|---|---|
| Complete graph K_N | `generate_complete_graph<N>()` | N | N·(N−1)/2 | ✓ |
| Petersen graph | `generate_petersen_graph()` | 10 | 15 | ✓ |
| Cycle graph C_N | `generate_cycle_graph<N>()` | N | N | ✓ |
| Path graph P_N | `generate_path_graph<N>()` | N | N−1 | ✓ |
| Star graph S_N | `generate_star_graph<N>()` | N | N−1 | ✓ |
| Complete bipartite K_{M,N} | `generate_complete_bipartite_graph<M, N>()` | M+N | M·N | ✓ |
| DAG chain | `generate_dag_chain<N>()` | N | N−1 | ✗ |

## Requirements

- C++23 compiler (GCC 13+, Clang 17+, or MSVC 19.36+)
- CMake 3.20+
