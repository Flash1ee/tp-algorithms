//
// Created by flashie on 24.05.2021.
//
#include "IGraph.hpp"

void BFS(IGraph &graph, int vertex, void (*visit)(int)) {
    std::vector<bool> visited(graph.VerticesCount(), false);
    std::queue<int> queue;
    queue.push(vertex);
    visited[vertex] = true;

    while (!queue.empty()) {
        int cur = queue.front();
        queue.pop();
        visit(cur);
        std::vector<int> relations = graph.GetNextVertices(cur);
        for (int i = 0; i < relations.size(); ++i) {
            if (!visited[relations[i]]) {
                queue.push(relations[i]);
                visited[relations[i]] = true;
            }
        }
    }
}

