#pragma once
#include "graph.hpp"

namespace ctgraph
{
    // ── Edge query ──────────────────────────────────────────────────────
    template <size_t NumNodes, size_t NumEdges, bool Undirected>
    consteval bool has_edge(const Graph<NumNodes, NumEdges, Undirected>& graph,
                           size_t u, size_t v)
    {
        for (size_t i = 0; i < graph.sizes[u]; ++i)
            if (graph.adj[u][i] == v) return true;
        return false;
    }

    // ── Edge count (for undirected graphs each edge stored twice) ───────
    template <size_t NumNodes, size_t NumEdges, bool Undirected>
    consteval size_t count_edges(const Graph<NumNodes, NumEdges, Undirected>& graph)
    {
        size_t total = 0;
        for (size_t u = 0; u < NumNodes; ++u)
            total += graph.sizes[u];
        if constexpr (Undirected)
            total /= 2;
        return total;
    }

    // ── Degree statistics ───────────────────────────────────────────────
    template <size_t NumNodes>
    struct DegreeStats {
        size_t min_degree = 0;
        size_t max_degree = 0;
    };

    template <size_t NumNodes, size_t NumEdges, bool Undirected>
    consteval DegreeStats<NumNodes> degree_stats(
        const Graph<NumNodes, NumEdges, Undirected>& graph)
    {
        DegreeStats<NumNodes> stats{};
        stats.min_degree = graph.sizes[0];
        stats.max_degree = graph.sizes[0];
        for (size_t i = 1; i < NumNodes; ++i) {
            if (graph.sizes[i] < stats.min_degree) stats.min_degree = graph.sizes[i];
            if (graph.sizes[i] > stats.max_degree) stats.max_degree = graph.sizes[i];
        }
        return stats;
    }

    // ── Connectivity (BFS-based) ────────────────────────────────────────
    template <size_t NumNodes, size_t NumEdges, bool Undirected>
    consteval bool is_connected(const Graph<NumNodes, NumEdges, Undirected>& graph)
    {
        std::array<bool, NumNodes> visited{};
        std::array<size_t, NumNodes> queue{};
        size_t front = 0, back = 0;

        queue[back++] = 0;
        visited[0] = true;

        while (front < back) {
            size_t node = queue[front++];
            for (size_t i = 0; i < graph.sizes[node]; ++i) {
                size_t nb = graph.adj[node][i];
                if (!visited[nb]) {
                    visited[nb] = true;
                    queue[back++] = nb;
                }
            }
        }

        for (size_t i = 0; i < NumNodes; ++i)
            if (!visited[i]) return false;
        return true;
    }

    // ── Cycle detection (DFS-based, works for directed and undirected) ──
    namespace detail
    {
        template <size_t NumNodes, size_t NumEdges, bool Undirected>
        consteval bool has_cycle_from(
            const Graph<NumNodes, NumEdges, Undirected>& graph,
            size_t start,
            std::array<int, NumNodes>& state)   // 0 = white, 1 = gray, 2 = black
        {
            // Iterative DFS with explicit stack storing (node, neighbor-index, parent)
            struct Frame { size_t node; size_t idx; size_t parent; };
            std::array<Frame, NumNodes * NumNodes> stack{};
            size_t top = 0;

            state[start] = 1;
            stack[top++] = {start, 0, static_cast<size_t>(-1)};

            while (top > 0) {
                auto& f = stack[top - 1];
                if (f.idx < graph.sizes[f.node]) {
                    size_t nb = graph.adj[f.node][f.idx];
                    ++f.idx;
                    if constexpr (Undirected) {
                        if (nb == f.parent) continue;
                        if (state[nb] == 1) return true;  // back-edge
                        if (state[nb] == 0) {
                            state[nb] = 1;
                            stack[top++] = {nb, 0, f.node};
                        }
                    } else {
                        if (state[nb] == 1) return true;  // back-edge in directed graph
                        if (state[nb] == 0) {
                            state[nb] = 1;
                            stack[top++] = {nb, 0, f.node};
                        }
                    }
                } else {
                    state[f.node] = 2;
                    --top;
                }
            }
            return false;
        }
    }

    template <size_t NumNodes, size_t NumEdges, bool Undirected>
    consteval bool has_cycle(const Graph<NumNodes, NumEdges, Undirected>& graph)
    {
        std::array<int, NumNodes> state{};  // all 0 (white)
        for (size_t i = 0; i < NumNodes; ++i) {
            if (state[i] == 0) {
                if (detail::has_cycle_from(graph, i, state))
                    return true;
            }
        }
        return false;
    }

    // ── Bipartiteness check (BFS 2-coloring) ───────────────────────────
    template <size_t NumNodes, size_t NumEdges, bool Undirected>
    consteval bool is_bipartite(const Graph<NumNodes, NumEdges, Undirected>& graph)
    {
        // side[i]: 0 = unvisited, 1 = side-A, 2 = side-B
        std::array<int, NumNodes> side{};
        std::array<size_t, NumNodes> queue{};

        for (size_t s = 0; s < NumNodes; ++s) {
            if (side[s] != 0) continue;
            size_t front = 0, back = 0;
            side[s] = 1;
            queue[back++] = s;
            while (front < back) {
                size_t node = queue[front++];
                for (size_t i = 0; i < graph.sizes[node]; ++i) {
                    size_t nb = graph.adj[node][i];
                    if (side[nb] == 0) {
                        side[nb] = (side[node] == 1) ? 2 : 1;
                        queue[back++] = nb;
                    } else if (side[nb] == side[node]) {
                        return false;
                    }
                }
            }
        }
        return true;
    }

    // ── BFS shortest path length (unweighted) ──────────────────────────
    template <size_t NumNodes, size_t NumEdges, bool Undirected>
    consteval size_t shortest_path_length(
        const Graph<NumNodes, NumEdges, Undirected>& graph,
        size_t src, size_t dst)
    {
        if (src == dst) return 0;

        constexpr size_t INF = static_cast<size_t>(-1);
        std::array<size_t, NumNodes> dist{};
        dist.fill(INF);
        std::array<size_t, NumNodes> queue{};
        size_t front = 0, back = 0;

        dist[src] = 0;
        queue[back++] = src;

        while (front < back) {
            size_t node = queue[front++];
            for (size_t i = 0; i < graph.sizes[node]; ++i) {
                size_t nb = graph.adj[node][i];
                if (dist[nb] == INF) {
                    dist[nb] = dist[node] + 1;
                    if (nb == dst) return dist[nb];
                    queue[back++] = nb;
                }
            }
        }
        return INF;  // unreachable
    }

    // ── Diameter (longest shortest path, for connected graphs) ──────────
    template <size_t NumNodes, size_t NumEdges, bool Undirected>
    consteval size_t diameter(const Graph<NumNodes, NumEdges, Undirected>& graph)
    {
        size_t max_dist = 0;
        for (size_t u = 0; u < NumNodes; ++u) {
            // BFS from u
            constexpr size_t INF = static_cast<size_t>(-1);
            std::array<size_t, NumNodes> dist{};
            dist.fill(INF);
            std::array<size_t, NumNodes> queue{};
            size_t front = 0, back = 0;

            dist[u] = 0;
            queue[back++] = u;
            while (front < back) {
                size_t node = queue[front++];
                for (size_t i = 0; i < graph.sizes[node]; ++i) {
                    size_t nb = graph.adj[node][i];
                    if (dist[nb] == INF) {
                        dist[nb] = dist[node] + 1;
                        queue[back++] = nb;
                    }
                }
            }
            for (size_t v = 0; v < NumNodes; ++v) {
                if (dist[v] != INF && dist[v] > max_dist)
                    max_dist = dist[v];
            }
        }
        return max_dist;
    }

    // ── Topological sort (Kahn's algorithm, directed acyclic graphs) ────
    template <size_t NumNodes>
    struct TopoResult {
        std::array<size_t, NumNodes> order{};
        size_t count = 0;  // number of nodes in topological order (< NumNodes if cycle exists)
    };

    template <size_t NumNodes, size_t NumEdges, bool Undirected>
    consteval TopoResult<NumNodes> topological_sort(
        const Graph<NumNodes, NumEdges, Undirected>& graph)
    {
        TopoResult<NumNodes> result{};

        // Compute in-degrees
        std::array<size_t, NumNodes> in_degree{};
        for (size_t u = 0; u < NumNodes; ++u)
            for (size_t i = 0; i < graph.sizes[u]; ++i)
                ++in_degree[graph.adj[u][i]];

        // Enqueue nodes with in-degree 0
        std::array<size_t, NumNodes> queue{};
        size_t front = 0, back = 0;
        for (size_t i = 0; i < NumNodes; ++i)
            if (in_degree[i] == 0)
                queue[back++] = i;

        while (front < back) {
            size_t node = queue[front++];
            result.order[result.count++] = node;
            for (size_t i = 0; i < graph.sizes[node]; ++i) {
                size_t nb = graph.adj[node][i];
                if (--in_degree[nb] == 0)
                    queue[back++] = nb;
            }
        }
        return result;
    }

    // ── Connected components (BFS-based, undirected) ────────────────────
    template <size_t NumNodes>
    struct ComponentsResult {
        std::array<size_t, NumNodes> component{};  // component[i] = component id of node i
        size_t num_components = 0;
    };

    template <size_t NumNodes, size_t NumEdges, bool Undirected>
    consteval ComponentsResult<NumNodes> connected_components(
        const Graph<NumNodes, NumEdges, Undirected>& graph)
    {
        ComponentsResult<NumNodes> result{};
        result.component.fill(static_cast<size_t>(-1));

        std::array<size_t, NumNodes> queue{};

        for (size_t s = 0; s < NumNodes; ++s) {
            if (result.component[s] != static_cast<size_t>(-1)) continue;
            size_t comp_id = result.num_components++;
            size_t front = 0, back = 0;
            result.component[s] = comp_id;
            queue[back++] = s;
            while (front < back) {
                size_t node = queue[front++];
                for (size_t i = 0; i < graph.sizes[node]; ++i) {
                    size_t nb = graph.adj[node][i];
                    if (result.component[nb] == static_cast<size_t>(-1)) {
                        result.component[nb] = comp_id;
                        queue[back++] = nb;
                    }
                }
            }
        }
        return result;
    }
}
