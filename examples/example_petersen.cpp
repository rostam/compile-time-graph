#include <iostream>

#include "ctgraph/generators.hpp"
#include "ctgraph/coloring.hpp"

int main() {
    constexpr auto petersen = generate_petersen_graph();
    constexpr auto coloring = ctgraph::color_graph_welsh_powell(petersen);
    static_assert(ctgraph::is_valid_coloring(petersen, coloring), "Invalid coloring!");

    std::cout << "Welsh-Powell Coloring:\n";
    for (size_t i = 0; i < coloring.color.size(); ++i)
        std::cout << "Node " << i << " → Color " << coloring.color[i] << "\n";

    std::cout << "Colors used: " << coloring.num_colors << "\n\n";

    //std::string dot = ctgraph::generate_dot_graph(petersen, coloring);
    //std::cout << "DOT output:\n" << dot;
}
