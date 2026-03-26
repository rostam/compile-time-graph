#include <iostream>
#include "../include/ctgraph/graph.hpp"
#include "../include/ctgraph/generators.hpp"
#include "../include/ctgraph/properties.hpp"

using namespace ctgraph;

int main() {
    // ── Complete graph K5 generators ──
    {
        constexpr auto k5 = generate_complete_graph<5>();
        static_assert(count_edges(k5) == 10, "K5 has 10 edges");
        for (size_t i = 0; i < 5; ++i)
            if (k5.neighbor_count(i) != 4)
                return 1;
    }

    // ── Complete graph K3 ──
    {
        constexpr auto k3 = generate_complete_graph<3>();
        static_assert(count_edges(k3) == 3, "K3 has 3 edges");
        static_assert(k3.neighbor_count(0) == 2, "K3 degree = 2");
    }

    // ── Petersen graph ──
    {
        constexpr auto p = generate_petersen_graph();
        static_assert(count_edges(p) == 15, "Petersen has 15 edges");
        // Petersen graph is 3-regular
        static_assert(p.neighbor_count(0) == 3, "Petersen is 3-regular");
        static_assert(p.neighbor_count(5) == 3, "Petersen is 3-regular");
    }

    // ── Cycle graph C6 ──
    {
        constexpr auto c6 = generate_cycle_graph<6>();
        static_assert(count_edges(c6) == 6, "C6 has 6 edges");
        static_assert(c6.neighbor_count(0) == 2, "C6 is 2-regular");
        static_assert(c6.neighbor_count(3) == 2, "C6 is 2-regular");
    }

    // ── Path graph P5 ──
    {
        constexpr auto p5 = generate_path_graph<5>();
        static_assert(count_edges(p5) == 4, "P5 has 4 edges");
        static_assert(p5.neighbor_count(0) == 1, "P5 endpoint degree 1");
        static_assert(p5.neighbor_count(2) == 2, "P5 internal degree 2");
        static_assert(p5.neighbor_count(4) == 1, "P5 endpoint degree 1");
    }

    // ── Star graph S6 ──
    {
        constexpr auto s6 = generate_star_graph<6>();
        static_assert(count_edges(s6) == 5, "S6 has 5 edges");
        static_assert(s6.neighbor_count(0) == 5, "S6 center degree = N-1");
        static_assert(s6.neighbor_count(1) == 1, "S6 leaf degree 1");
    }

    // ── Complete bipartite K3,3 ──
    {
        constexpr auto k33 = generate_complete_bipartite_graph<3, 3>();
        static_assert(count_edges(k33) == 9, "K3,3 has 9 edges");
        // Nodes 0-2 each connect to 3-5
        static_assert(k33.neighbor_count(0) == 3, "K3,3 partition A degree = 3");
        static_assert(k33.neighbor_count(3) == 3, "K3,3 partition B degree = 3");
    }

    // ── Complete bipartite K2,4 ──
    {
        constexpr auto k24 = generate_complete_bipartite_graph<2, 4>();
        static_assert(count_edges(k24) == 8, "K2,4 has 8 edges");
        static_assert(k24.neighbor_count(0) == 4, "K2,4: node in A has degree 4");
        static_assert(k24.neighbor_count(2) == 2, "K2,4: node in B has degree 2");
    }

    // ── DAG chain ──
    {
        constexpr auto dag = generate_dag_chain<5>();
        static_assert(count_edges(dag) == 4, "DAG chain 5 has 4 edges");
        static_assert(dag.neighbor_count(0) == 1, "DAG chain: node 0 out-degree 1");
        static_assert(dag.neighbor_count(4) == 0, "DAG chain: last node out-degree 0");
    }

    std::cout << "✅ All generator tests passed.\n";
    return 0;
}
