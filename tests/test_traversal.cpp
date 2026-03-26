#include <iostream>
#include "../include/ctgraph/graph.hpp"
#include "../include/ctgraph/traversal.hpp"
#include "../include/ctgraph/generators.hpp"

int main() {
    // ── DFS on Petersen graph from node 0 ──
    {
        constexpr auto g = generate_petersen_graph();
        constexpr auto result = dfs(g, 0);
        // DFS should visit all 10 nodes (Petersen graph is connected)
        static_assert(result.count == 10, "DFS should visit all 10 nodes");
        // First visited node is the start node
        static_assert(result.order[0] == 0, "DFS starts at node 0");
        // All nodes marked visited
        static_assert(result.visited[0] && result.visited[1] && result.visited[2],
                      "All nodes visited");
    }

    // ── BFS on Petersen graph from node 0 ──
    {
        constexpr auto g = generate_petersen_graph();
        constexpr auto result = bfs(g, 0);
        static_assert(result.count == 10, "BFS should visit all 10 nodes");
        static_assert(result.order[0] == 0, "BFS starts at node 0");
    }

    // ── DFS on complete graph K5 ──
    {
        constexpr auto g = generate_complete_graph<5>();
        constexpr auto result = dfs(g, 0);
        static_assert(result.count == 5, "DFS visits all 5 nodes in K5");
    }

    // ── BFS on path graph P5 ──
    {
        constexpr auto p5 = generate_path_graph<5>();
        constexpr auto result = bfs(p5, 0);
        static_assert(result.count == 5, "BFS visits all 5 nodes in P5");
        // BFS from one end of a path visits nodes in order
        static_assert(result.order[0] == 0, "BFS order[0] = 0");
        static_assert(result.order[1] == 1, "BFS order[1] = 1");
        static_assert(result.order[2] == 2, "BFS order[2] = 2");
        static_assert(result.order[3] == 3, "BFS order[3] = 3");
        static_assert(result.order[4] == 4, "BFS order[4] = 4");
    }

    // ── DFS on cycle graph C6 ──
    {
        constexpr auto c6 = generate_cycle_graph<6>();
        constexpr auto result = dfs(c6, 0);
        static_assert(result.count == 6, "DFS visits all 6 nodes in C6");
        static_assert(result.order[0] == 0, "DFS starts at 0");
    }

    // ── BFS on star graph S5 ──
    {
        constexpr auto s5 = generate_star_graph<5>();
        constexpr auto result = bfs(s5, 0);
        static_assert(result.count == 5, "BFS visits all 5 nodes in S5");
        static_assert(result.order[0] == 0, "BFS starts at center");
    }

    // ── DFS on directed chain DAG ──
    {
        constexpr auto dag = generate_dag_chain<4>();
        constexpr auto result = dfs(dag, 0);
        static_assert(result.count == 4, "DFS visits all 4 nodes in chain DAG");
        static_assert(result.order[0] == 0, "DFS chain: start at 0");
        static_assert(result.order[1] == 1, "DFS chain: then 1");
        static_assert(result.order[2] == 2, "DFS chain: then 2");
        static_assert(result.order[3] == 3, "DFS chain: then 3");
    }

    std::cout << "✅ All traversal tests passed.\n";
    return 0;
}
