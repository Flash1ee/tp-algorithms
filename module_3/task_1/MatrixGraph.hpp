//
// Created by flashie on 24.05.2021.
//
#pragma once
#include "IGraph.hpp"
#include "ListGraph.hpp"

class MatrixGraph : public IGraph {
public:
    explicit MatrixGraph(IGraph &graph);

    explicit MatrixGraph(int vertices);

    ~MatrixGraph() = default;

    // Добавление ребра от from к to.
    void AddEdge(int from, int to) override;

    int VerticesCount() const override;

    std::vector<int> GetNextVertices(int vertex) const override;

    std::vector<int> GetPrevVertices(int vertex) const override;

private:
    std::vector<std::vector<int>> matrix;
};