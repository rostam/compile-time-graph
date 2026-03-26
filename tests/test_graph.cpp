#include <iostream>
#include "../include/ctgraph/graph.hpp"
#include "../include/ctgraph/generators.hpp"

// ── Basic Graph structure tests ────────────────────────────────────────

// Directed triangle: 0→1, 1→2, 2→0
constexpr auto make_triangle_directed() {
    constexpr std::array<Edge, 3> edges{{{0,1},{1,2},{2,0}}};
    return Graph<3, 3, false>(edges);
}

// Undirected triangle
constexpr auto make_triangle_undirected() {
    constexpr std::array<Edge, 3> edges{{{0,1},{1,2},{2,0}}};
    return Graph<3, 3, true>(edges);
}

int main() {
    // ── Directed graph tests ──
    {
        constexpr auto g = make_triangle_directed();
        // Each node has exactly 1 outgoing edge
        static_assert(g.neighbor_count(0) == 1, "node 0 out-degree should be 1");
        static_assert(g.neighbor_count(1) == 1, "node 1 out-degree should be 1");
        static_assert(g.neighbor_count(2) == 1, "node 2 out-degree should be 1");
        // Check adjacency
        static_assert(g.neighbors(0)[0] == 1, "0 → 1");
        static_assert(g.neighbors(1)[0] == 2, "1 → 2");
        static_assert(g.neighbors(2)[0] == 0, "2 → 0");
    }

    // ── Undirected graph tests ──
    {
        constexpr auto g = make_triangle_undirected();
        // Each node has exactly 2 neighbors
        static_assert(g.neighbor_count(0) == 2, "node 0 degree should be 2");
        static_assert(g.neighbor_count(1) == 2, "node 1 degree should be 2");
        static_assert(g.neighbor_count(2) == 2, "node 2 degree should be 2");
    }

    // ── Complete graph K4 structure ──
    {
        constexpr auto k4 = generate_complete_graph<4>();
        static_assert(k4.neighbor_count(0) == 3, "K4: every node has degree 3");
        static_assert(k4.neighbor_count(1) == 3, "K4: every node has degree 3");
        static_assert(k4.neighbor_count(2) == 3, "K4: every node has degree 3");
        static_assert(k4.neighbor_count(3) == 3, "K4: every node has degree 3");
    }

    // ── Path graph P5 structure ──
    {
        constexpr auto p5 = generate_path_graph<5>();
        static_assert(p5.neighbor_count(0) == 1, "P5: endpoint degree 1");
        static_assert(p5.neighbor_count(1) == 2, "P5: internal degree 2");
        static_assert(p5.neighbor_count(2) == 2, "P5: internal degree 2");
        static_assert(p5.neighbor_count(3) == 2, "P5: internal degree 2");
        static_assert(p5.neighbor_count(4) == 1, "P5: endpoint degree 1");
    }

    // ── Self-loop handling (directed) ──
    {
        constexpr std::array<Edge, 2> edges{{{0,0},{0,1}}};
        constexpr auto g = Graph<2, 2, false>(edges);
        static_assert(g.neighbor_count(0) == 2, "self-loop counts as neighbor");
    }

    std::cout << "✅ All graph structure tests passed.\n";
    return 0;
}
