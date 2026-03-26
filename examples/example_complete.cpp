#include <iostream>
#include "ctgraph/generators.hpp"
#include "ctgraph/coloring.hpp"
#include "ctgraph/properties.hpp"

int main() {
    constexpr auto k5 = generate_complete_graph<5>();
    constexpr auto coloring = ctgraph::color_graph_welsh_powell(k5);
    static_assert(ctgraph::is_valid_coloring(k5, coloring), "Invalid coloring!");
    static_assert(coloring.num_colors == 5, "K5 needs exactly 5 colors");

    constexpr auto stats = ctgraph::degree_stats(k5);
    static_assert(stats.min_degree == 4 && stats.max_degree == 4, "K5 is 4-regular");
    static_assert(ctgraph::is_connected(k5), "K5 is connected");
    static_assert(ctgraph::diameter(k5) == 1, "K5 diameter is 1");

    std::cout << "Complete graph K5:\n";
    std::cout << "  Edges:      " << ctgraph::count_edges(k5) << "\n";
    std::cout << "  Diameter:   " << ctgraph::diameter(k5) << "\n";
    std::cout << "  Colors:     " << coloring.num_colors << "\n";
    for (size_t i = 0; i < coloring.color.size(); ++i)
        std::cout << "  Node " << i << " -> Color " << coloring.color[i] << "\n";
}
