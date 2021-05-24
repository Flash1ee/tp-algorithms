//
// Created by flashie on 24.05.2021.
//
#pragma once

#include "IGraph.hpp"

class ListGraph : public IGraph {
public:
    explicit ListGraph(IGraph &graph);

    explicit ListGraph(int vertices);

    ~ListGraph() override = default;

    // Добавление ребра от from к to.
    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> lists;
};

