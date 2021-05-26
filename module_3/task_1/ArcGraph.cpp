//
// Created by flashie on 26.05.2021.
//
#include "ArcGraph.hpp"


ArcGraph::ArcGraph(int vertices) : lists(vertices) {

}

int ArcGraph::VerticesCount() const {
    return static_cast<int>(this->lists.size());
}

ArcGraph::ArcGraph(IGraph &graph) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        auto to = graph.GetNextVertices(from);
        for (auto &v: to) {
            this->lists.emplace_back(from, v);
        }
    }
}

void ArcGraph::AddEdge(int from, int to) {
    assert(from >= 0 && to >= 0);
    this->lists.emplace_back(from, to);
}

std::vector<int> ArcGraph::GetNextVertices(int vertex) const {
    std::vector<int> res{};
    for (auto v : this->lists) {
        if (v.first == vertex) {
            res.push_back(v.second);
        }
    }
    return res;
}

std::vector<int> ArcGraph::GetPrevVertices(int vertex) const {
    std::vector<int> res{};
    for (auto v : this->lists) {
        if (v.second == vertex) {
            res.push_back(v.first);
        }
    }
    return res;
}
