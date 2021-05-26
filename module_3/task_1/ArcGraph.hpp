//
// Created by flashie on 26.05.2021.
//
#pragma once

#include "IGraph.hpp"

class ArcGraph : public IGraph {
public:
    explicit ArcGraph(IGraph &graph);

    explicit ArcGraph(int vertices);

    ~ArcGraph() override = default;

    // Добавление ребра от from к to.
    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::pair<int, int>> lists;
};



