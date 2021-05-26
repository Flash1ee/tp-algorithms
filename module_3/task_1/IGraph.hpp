//
// Created by flashie on 24.05.2021.
//

#pragma once

#include <vector>
#include <cassert>
#include <queue>

struct IGraph {
    virtual ~IGraph() {};

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;

    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};
void BFS(IGraph &graph, int vertex, void (*visit)(int));



