template <size_t NumNodes>
struct DFSResult {
    std::array<bool, NumNodes> visited{};
    std::array<size_t, NumNodes> order{};
    size_t count = 0;
};

template <size_t NumNodes, size_t NumEdges, bool Undirected = false>
consteval DFSResult<NumNodes> dfs(const Graph<NumNodes, NumEdges, Undirected>& graph, size_t start) {
    DFSResult<NumNodes> result{};
    std::array<size_t, NumNodes> stack{};
    size_t stack_top = 0;
    stack[stack_top++] = start;

    while (stack_top > 0) {
        size_t node = stack[--stack_top];
        if (result.visited[node]) continue;

        result.visited[node] = true;
        result.order[result.count++] = node;

        for (size_t i = 0; i < graph.sizes[node]; ++i) {
            size_t neighbor = graph.adj[node][graph.sizes[node] - 1 - i]; // reverse for DFS order
            if (!result.visited[neighbor]) {
                stack[stack_top++] = neighbor;
            }
        }
    }

    return result;
}
