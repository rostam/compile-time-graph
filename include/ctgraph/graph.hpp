#pragma once
#include <array>


struct Edge {
    size_t from;
    size_t to;
};

template <size_t NumNodes, size_t NumEdges, bool Undirected = false>
struct Graph {
    std::array<std::array<size_t, NumNodes>, NumNodes> adj{};
    std::array<size_t, NumNodes> sizes{};

    consteval explicit Graph(std::array<Edge, NumEdges> edges) {
        for (const auto& [from, to] : edges) {
            adj[from][sizes[from]++] = to;
            if (Undirected && from != to)
                adj[to][sizes[to]++] = from;
        }
    }

    constexpr const auto& neighbors(size_t node) const {
        return adj[node];
    }

    [[nodiscard]] constexpr size_t neighbor_count(size_t node) const {
        return sizes[node];
    }
};
