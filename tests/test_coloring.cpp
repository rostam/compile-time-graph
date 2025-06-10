#include <iostream>
#include "../include/ctgraph/graph.hpp"
#include "../include/ctgraph/coloring.hpp"
#include "../include/ctgraph/generators.hpp"

using namespace ctgraph;

int main() {
    constexpr auto g = generate_petersen_graph();
    constexpr auto coloring = color_graph_welsh_powell(g);

    static_assert(is_valid_coloring(g, coloring), "❌ Invalid coloring!");
    static_assert(coloring.num_colors <= 4, "❌ Petersen graph should color with ≤ 4 colors.");

    std::cout << "✅ Compile-time coloring is valid.\n";
    std::cout << "Used colors: " << coloring.num_colors << "\n";

    for (size_t i = 0; i < coloring.color.size(); ++i) {
        std::cout << "Node " << i << " → Color " << coloring.color[i] << "\n";
    }

    return 0;
}
