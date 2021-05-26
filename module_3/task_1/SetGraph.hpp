//
// Created by flashie on 24.05.2021.
//
#pragma once

#include <unordered_set>
#include "IGraph.hpp"

class SetGraph: public IGraph {
public:
    explicit SetGraph(IGraph &graph);

    explicit SetGraph(int vertices);

    ~SetGraph() = default;

    // Добавление ребра от from к to.
    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::unordered_set<int>> lists;
};