//
// Created by rostam on 10.06.25.
//

#ifndef GENERATORS_HPP
#define GENERATORS_HPP

#include "graph.hpp"

template <size_t N>
consteval auto generate_complete_graph() {
    constexpr size_t MaxEdges = N * (N - 1) / 2;
    std::array<Edge, MaxEdges> edges{};
    size_t k = 0;
    for (size_t i = 0; i < N; ++i) {
        for (size_t j = i + 1; j < N; ++j) {
            edges[k++] = {i, j};
        }
    }
    return Graph<N, MaxEdges, true>(edges);
}

consteval auto generate_petersen_graph() {
    constexpr std::array<Edge, 15> edges{{
        {0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, // outer pentagon
        {5, 7}, {7, 9}, {9, 6}, {6, 8}, {8, 5}, // inner star
        {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9}  // connecting spokes
    }};
    return Graph<10, 15, true>(edges);
}


#endif //GENERATORS_HPP
