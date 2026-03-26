#include <iostream>
#include "../include/ctgraph/graph.hpp"
#include "../include/ctgraph/generators.hpp"
#include "../include/ctgraph/properties.hpp"

using namespace ctgraph;

int main() {
    // ═══════════════════════════════════════════════════════════════════
    // has_edge
    // ═══════════════════════════════════════════════════════════════════
    {
        constexpr auto k4 = generate_complete_graph<4>();
        static_assert(has_edge(k4, 0, 1), "K4 has edge 0-1");
        static_assert(has_edge(k4, 1, 0), "K4 undirected: 1-0 too");
        static_assert(has_edge(k4, 2, 3), "K4 has edge 2-3");
    }
    {
        constexpr auto p3 = generate_path_graph<3>();
        static_assert(has_edge(p3, 0, 1), "P3 has 0-1");
        static_assert(has_edge(p3, 1, 2), "P3 has 1-2");
        static_assert(!has_edge(p3, 0, 2), "P3 does NOT have 0-2");
    }

    // ═══════════════════════════════════════════════════════════════════
    // count_edges
    // ═══════════════════════════════════════════════════════════════════
    {
        constexpr auto k5 = generate_complete_graph<5>();
        static_assert(count_edges(k5) == 10, "K5 has C(5,2)=10 edges");
    }
    {
        constexpr auto dag = generate_dag_chain<4>();
        static_assert(count_edges(dag) == 3, "DAG chain of 4 has 3 edges");
    }

    // ═══════════════════════════════════════════════════════════════════
    // degree_stats
    // ═══════════════════════════════════════════════════════════════════
    {
        constexpr auto k5 = generate_complete_graph<5>();
        constexpr auto stats = degree_stats(k5);
        static_assert(stats.min_degree == 4, "K5 is 4-regular: min=4");
        static_assert(stats.max_degree == 4, "K5 is 4-regular: max=4");
    }
    {
        constexpr auto s5 = generate_star_graph<5>();
        constexpr auto stats = degree_stats(s5);
        static_assert(stats.min_degree == 1, "S5 leaves have degree 1");
        static_assert(stats.max_degree == 4, "S5 center has degree 4");
    }
    {
        constexpr auto p = generate_petersen_graph();
        constexpr auto stats = degree_stats(p);
        static_assert(stats.min_degree == 3, "Petersen 3-regular");
        static_assert(stats.max_degree == 3, "Petersen 3-regular");
    }

    // ═══════════════════════════════════════════════════════════════════
    // is_connected
    // ═══════════════════════════════════════════════════════════════════
    {
        constexpr auto k5 = generate_complete_graph<5>();
        static_assert(is_connected(k5), "K5 is connected");
    }
    {
        constexpr auto p = generate_petersen_graph();
        static_assert(is_connected(p), "Petersen is connected");
    }
    {
        constexpr auto c8 = generate_cycle_graph<8>();
        static_assert(is_connected(c8), "C8 is connected");
    }
    {
        constexpr auto p5 = generate_path_graph<5>();
        static_assert(is_connected(p5), "P5 is connected");
    }
    {
        // Two isolated nodes connected by nothing (directed, no edges between them)
        constexpr std::array<Edge, 1> edges{{{0, 1}}};
        constexpr auto g = Graph<3, 1, false>(edges);
        static_assert(!is_connected(g), "3-node graph with 1 directed edge is not connected");
    }

    // ═══════════════════════════════════════════════════════════════════
    // has_cycle
    // ═══════════════════════════════════════════════════════════════════
    {
        constexpr auto c6 = generate_cycle_graph<6>();
        static_assert(has_cycle(c6), "C6 has a cycle");
    }
    {
        constexpr auto p5 = generate_path_graph<5>();
        static_assert(!has_cycle(p5), "P5 (path) has no cycle");
    }
    {
        constexpr auto k5 = generate_complete_graph<5>();
        static_assert(has_cycle(k5), "K5 has cycles");
    }
    {
        constexpr auto dag = generate_dag_chain<5>();
        static_assert(!has_cycle(dag), "DAG chain has no cycle");
    }
    {
        constexpr auto s4 = generate_star_graph<4>();
        static_assert(!has_cycle(s4), "Star graph (tree) has no cycle");
    }

    // ═══════════════════════════════════════════════════════════════════
    // is_bipartite
    // ═══════════════════════════════════════════════════════════════════
    {
        constexpr auto k33 = generate_complete_bipartite_graph<3, 3>();
        static_assert(is_bipartite(k33), "K3,3 is bipartite");
    }
    {
        constexpr auto k3 = generate_complete_graph<3>();
        static_assert(!is_bipartite(k3), "K3 (triangle) is NOT bipartite");
    }
    {
        constexpr auto c6 = generate_cycle_graph<6>();
        static_assert(is_bipartite(c6), "Even cycle C6 is bipartite");
    }
    {
        constexpr auto c5 = generate_cycle_graph<5>();
        static_assert(!is_bipartite(c5), "Odd cycle C5 is NOT bipartite");
    }
    {
        constexpr auto p5 = generate_path_graph<5>();
        static_assert(is_bipartite(p5), "Path P5 is bipartite");
    }
    {
        constexpr auto s6 = generate_star_graph<6>();
        static_assert(is_bipartite(s6), "Star S6 is bipartite");
    }

    // ═══════════════════════════════════════════════════════════════════
    // shortest_path_length
    // ═══════════════════════════════════════════════════════════════════
    {
        constexpr auto k5 = generate_complete_graph<5>();
        static_assert(shortest_path_length(k5, 0, 4) == 1, "K5: any pair at distance 1");
        static_assert(shortest_path_length(k5, 0, 0) == 0, "distance to self is 0");
    }
    {
        constexpr auto p5 = generate_path_graph<5>();
        static_assert(shortest_path_length(p5, 0, 4) == 4, "P5: endpoints at distance 4");
        static_assert(shortest_path_length(p5, 1, 3) == 2, "P5: 1→3 at distance 2");
    }
    {
        constexpr auto c6 = generate_cycle_graph<6>();
        static_assert(shortest_path_length(c6, 0, 3) == 3, "C6: opposite nodes at distance 3");
        static_assert(shortest_path_length(c6, 0, 1) == 1, "C6: adjacent nodes at distance 1");
    }
    {
        constexpr auto p = generate_petersen_graph();
        static_assert(shortest_path_length(p, 0, 0) == 0, "Petersen: self distance 0");
        static_assert(shortest_path_length(p, 0, 1) == 1, "Petersen: adjacent distance 1");
    }

    // ═══════════════════════════════════════════════════════════════════
    // diameter
    // ═══════════════════════════════════════════════════════════════════
    {
        constexpr auto k5 = generate_complete_graph<5>();
        static_assert(diameter(k5) == 1, "K5 diameter = 1");
    }
    {
        constexpr auto p5 = generate_path_graph<5>();
        static_assert(diameter(p5) == 4, "P5 diameter = 4");
    }
    {
        constexpr auto c6 = generate_cycle_graph<6>();
        static_assert(diameter(c6) == 3, "C6 diameter = 3");
    }
    {
        constexpr auto p = generate_petersen_graph();
        static_assert(diameter(p) == 2, "Petersen diameter = 2");
    }
    {
        constexpr auto s6 = generate_star_graph<6>();
        static_assert(diameter(s6) == 2, "Star S6 diameter = 2");
    }

    // ═══════════════════════════════════════════════════════════════════
    // topological_sort (directed)
    // ═══════════════════════════════════════════════════════════════════
    {
        constexpr auto dag = generate_dag_chain<5>();
        constexpr auto topo = topological_sort(dag);
        static_assert(topo.count == 5, "Topo sort visits all 5 nodes in DAG chain");
        // Chain 0→1→2→3→4 has unique topological order
        static_assert(topo.order[0] == 0, "Topo: first is 0");
        static_assert(topo.order[1] == 1, "Topo: second is 1");
        static_assert(topo.order[2] == 2, "Topo: third is 2");
        static_assert(topo.order[3] == 3, "Topo: fourth is 3");
        static_assert(topo.order[4] == 4, "Topo: fifth is 4");
    }
    {
        // Diamond DAG: 0→1, 0→2, 1→3, 2→3
        constexpr std::array<Edge, 4> edges{{{0,1},{0,2},{1,3},{2,3}}};
        constexpr auto dag = Graph<4, 4, false>(edges);
        constexpr auto topo = topological_sort(dag);
        static_assert(topo.count == 4, "Diamond DAG: all 4 nodes sorted");
        static_assert(topo.order[0] == 0, "Diamond DAG: 0 comes first");
        static_assert(topo.order[3] == 3, "Diamond DAG: 3 comes last");
    }

    // ═══════════════════════════════════════════════════════════════════
    // connected_components
    // ═══════════════════════════════════════════════════════════════════
    {
        constexpr auto k5 = generate_complete_graph<5>();
        constexpr auto cc = connected_components(k5);
        static_assert(cc.num_components == 1, "K5 has 1 component");
    }
    {
        // Two disconnected edges: 0-1 and 2-3 (4 nodes, 2 undirected edges)
        constexpr std::array<Edge, 2> edges{{{0,1},{2,3}}};
        constexpr auto g = Graph<4, 2, true>(edges);
        constexpr auto cc = connected_components(g);
        static_assert(cc.num_components == 2, "Two disconnected edges = 2 components");
        static_assert(cc.component[0] == cc.component[1], "0 and 1 same component");
        static_assert(cc.component[2] == cc.component[3], "2 and 3 same component");
        static_assert(cc.component[0] != cc.component[2], "components differ");
    }

    // ═══════════════════════════════════════════════════════════════════
    // Compile-time computed metrics (printed at runtime for demonstration)
    // ═══════════════════════════════════════════════════════════════════
    {
        constexpr auto petersen = generate_petersen_graph();
        constexpr size_t pet_edges = count_edges(petersen);
        constexpr size_t pet_diameter = diameter(petersen);
        constexpr bool pet_connected = is_connected(petersen);
        constexpr bool pet_bipartite = is_bipartite(petersen);
        constexpr bool pet_has_cycle = has_cycle(petersen);
        constexpr auto pet_stats = degree_stats(petersen);

        std::cout << "=== Petersen graph compile-time metrics ===\n";
        std::cout << "  Edges:       " << pet_edges << "\n";
        std::cout << "  Diameter:    " << pet_diameter << "\n";
        std::cout << "  Connected:   " << pet_connected << "\n";
        std::cout << "  Bipartite:   " << pet_bipartite << "\n";
        std::cout << "  Has cycle:   " << pet_has_cycle << "\n";
        std::cout << "  Min degree:  " << pet_stats.min_degree << "\n";
        std::cout << "  Max degree:  " << pet_stats.max_degree << "\n";

        constexpr auto k6 = generate_complete_graph<6>();
        constexpr size_t k6_edges = count_edges(k6);
        constexpr size_t k6_diameter = diameter(k6);
        constexpr auto k6_stats = degree_stats(k6);

        std::cout << "\n=== K6 compile-time metrics ===\n";
        std::cout << "  Edges:       " << k6_edges << "\n";
        std::cout << "  Diameter:    " << k6_diameter << "\n";
        std::cout << "  Min degree:  " << k6_stats.min_degree << "\n";
        std::cout << "  Max degree:  " << k6_stats.max_degree << "\n";
    }

    std::cout << "\n✅ All properties tests passed.\n";
    return 0;
}
