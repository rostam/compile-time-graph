//
// Created by rostam on 10.06.25.
//

#ifndef UTILS_HPP
#define UTILS_HPP

template <size_t NumNodes, size_t NumEdges, bool Undirected = false>
std::string generate_dot_graph(
    const Graph<NumNodes, NumEdges, Undirected>& graph,
    const ColorResult<NumNodes>& coloring
) {
    const std::array<const char*, 10> colors = {
        "red", "blue", "green", "orange", "purple", "cyan", "magenta", "gold", "grey", "brown"
    };

    std::string dot = "graph G {\n";

    for (size_t i = 0; i < NumNodes; ++i) {
        dot += "  " + std::to_string(i) + " [style=filled, fillcolor=" +
               colors[coloring.color[i] % colors.size()] + "];\n";
    }

    std::array<bool, NumNodes * NumNodes> printed{};
    for (size_t u = 0; u < NumNodes; ++u) {
        for (size_t i = 0; i < graph.sizes[u]; ++i) {
            size_t v = graph.adj[u][i];
            if (!printed[u * NumNodes + v] && !printed[v * NumNodes + u]) {
                dot += "  " + std::to_string(u) + " -- " + std::to_string(v) + ";\n";
                printed[u * NumNodes + v] = true;
                printed[v * NumNodes + u] = true;
            }
        }
    }

    dot += "}\n";
    return dot;
}


#endif //UTILS_HPP
