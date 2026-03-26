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

// Cycle graph C_n  (undirected)
template <size_t N>
consteval auto generate_cycle_graph() {
    std::array<Edge, N> edges{};
    for (size_t i = 0; i < N; ++i)
        edges[i] = {i, (i + 1) % N};
    return Graph<N, N, true>(edges);
}

// Path graph P_n  (undirected, N nodes, N-1 edges)
template <size_t N>
consteval auto generate_path_graph() {
    constexpr size_t E = N - 1;
    std::array<Edge, E> edges{};
    for (size_t i = 0; i < E; ++i)
        edges[i] = {i, i + 1};
    return Graph<N, E, true>(edges);
}

// Star graph S_n  (undirected, node 0 is the center, N nodes total)
template <size_t N>
consteval auto generate_star_graph() {
    constexpr size_t E = N - 1;
    std::array<Edge, E> edges{};
    for (size_t i = 0; i < E; ++i)
        edges[i] = {0, i + 1};
    return Graph<N, E, true>(edges);
}

// Complete bipartite graph K_{M,N}  (undirected, M+N nodes, M*N edges)
// Nodes 0..M-1 form one partition, M..M+N-1 form the other.
template <size_t M, size_t N>
consteval auto generate_complete_bipartite_graph() {
    constexpr size_t Nodes = M + N;
    constexpr size_t E = M * N;
    std::array<Edge, E> edges{};
    size_t k = 0;
    for (size_t i = 0; i < M; ++i)
        for (size_t j = 0; j < N; ++j)
            edges[k++] = {i, M + j};
    return Graph<Nodes, E, true>(edges);
}

// Directed acyclic graph (DAG): a chain 0 → 1 → 2 → ... → N-1
template <size_t N>
consteval auto generate_dag_chain() {
    constexpr size_t E = N - 1;
    std::array<Edge, E> edges{};
    for (size_t i = 0; i < E; ++i)
        edges[i] = {i, i + 1};
    return Graph<N, E, false>(edges);
}

#endif //GENERATORS_HPP
