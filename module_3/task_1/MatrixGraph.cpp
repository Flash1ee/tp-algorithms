//
// Created by flashie on 24.05.2021.
//
#include <algorithm>
#include "MatrixGraph.hpp"

MatrixGraph::MatrixGraph(int vertices) :  matrix(vertices, std::vector<int>(vertices, 0)) {

}

MatrixGraph::MatrixGraph(IGraph &graph):matrix(graph.VerticesCount(), std::vector<int>(graph.VerticesCount(), 0)) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        auto relate = graph.GetNextVertices(from);
        for (auto &v : relate) {
            ++matrix[from][v];
        }
    }

}

void MatrixGraph::AddEdge(int from, int to) {
    ++matrix[from][to];
}

int MatrixGraph::VerticesCount() const {
    return matrix.size();
}

std::vector<int> MatrixGraph::GetNextVertices(int vertex) const {
    std::vector<int> res;
    for (int to = 0; to < matrix.size(); ++to) {
        int count = matrix[vertex][to];
        for (int j = 0; j < count; ++j) {
            res.push_back(to);
        }
    }
    return res;
}

std::vector<int> MatrixGraph::GetPrevVertices(int vertex) const {
    std::vector<int> res;
    for (int from = 0; from < matrix.size(); ++from) {
        int count = matrix[from][vertex];
        for (int j = 0; j < count; ++j) {
            res.push_back(from);
        }
    }
    return res;
}
