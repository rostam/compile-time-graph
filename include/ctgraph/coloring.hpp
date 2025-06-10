#pragma once
namespace ctgraph
{
    template <size_t NumNodes>
    struct ColorResult
    {
        std::array<size_t, NumNodes> color{}; // color[i] = color assigned to node i
        size_t num_colors = 0;
    };


    template <size_t NumNodes, size_t NumEdges, bool Undirected = false>
    consteval bool is_valid_coloring(
        const Graph<NumNodes, NumEdges, Undirected>& graph,
        const ColorResult<NumNodes>& coloring
    )
    {
        for (size_t u = 0; u < NumNodes; ++u)
        {
            for (size_t i = 0; i < graph.sizes[u]; ++i)
            {
                size_t v = graph.adj[u][i];
                if (u < v && coloring.color[u] == coloring.color[v])
                    return false;
            }
        }
        return true;
    }

    template <size_t N>
    consteval void sort_by_degree(std::array<size_t, N>& nodes, const std::array<size_t, N>& degrees)
    {
        for (size_t i = 0; i < N - 1; ++i)
        {
            for (size_t j = i + 1; j < N; ++j)
            {
                if (degrees[nodes[j]] > degrees[nodes[i]])
                    std::swap(nodes[i], nodes[j]);
            }
        }
    }

    template <size_t NumNodes, size_t NumEdges, bool Undirected = false>
    consteval ColorResult<NumNodes> color_graph_welsh_powell(
        const Graph<NumNodes, NumEdges, Undirected>& graph
    )
    {
        ColorResult<NumNodes> result{};
        result.color.fill(static_cast<size_t>(-1));

        std::array<size_t, NumNodes> degrees{};
        for (size_t i = 0; i < NumNodes; ++i)
            degrees[i] = graph.sizes[i];

        std::array<size_t, NumNodes> order{};
        for (size_t i = 0; i < NumNodes; ++i)
            order[i] = i;

        sort_by_degree(order, degrees);

        for (size_t idx = 0; idx < NumNodes; ++idx)
        {
            size_t u = order[idx];
            std::array<bool, NumNodes> used_colors{};
            used_colors.fill(false);

            for (size_t i = 0; i < graph.sizes[u]; ++i)
            {
                size_t v = graph.adj[u][i];
                size_t color = result.color[v];
                if (color != static_cast<size_t>(-1))
                    used_colors[color] = true;
            }

            for (size_t c = 0; c < NumNodes; ++c)
            {
                if (!used_colors[c])
                {
                    result.color[u] = c;
                    if (c + 1 > result.num_colors)
                        result.num_colors = c + 1;
                    break;
                }
            }
        }

        return result;
    }


    template <size_t NumNodes, size_t NumEdges, bool Undirected = false>
    consteval ColorResult<NumNodes> color_graph(const Graph<NumNodes, NumEdges, Undirected>& graph)
    {
        ColorResult<NumNodes> result{};
        result.color.fill(static_cast<size_t>(-1)); // initially uncolored

        for (size_t u = 0; u < NumNodes; ++u)
        {
            std::array<bool, NumNodes> used_colors{};
            used_colors.fill(false);

            // Mark used colors by neighbors
            for (size_t i = 0; i < graph.sizes[u]; ++i)
            {
                size_t neighbor = graph.adj[u][i];
                size_t color = result.color[neighbor];
                if (color != static_cast<size_t>(-1))
                {
                    used_colors[color] = true;
                }
            }

            // Assign the smallest available color
            for (size_t c = 0; c < NumNodes; ++c)
            {
                if (!used_colors[c])
                {
                    result.color[u] = c;
                    if (c + 1 > result.num_colors)
                        result.num_colors = c + 1;
                    break;
                }
            }
        }

        return result;
    }
}
