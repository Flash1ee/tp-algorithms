//
// Created by flashie on 24.05.2021.
//
#include "SetGraph.hpp"


SetGraph::SetGraph(int vertices) : lists(vertices) {

}

SetGraph::SetGraph(IGraph &graph) : lists(graph.VerticesCount()) {
    for (int from = 0; from < graph.VerticesCount(); ++from) {
        auto v = graph.GetNextVertices(from);
        for (auto i:v) {
            lists[from].insert(i);
        }
    }
}

void SetGraph::AddEdge(int from, int to) {
    assert(from < lists.size());
    assert(this->lists[from].find(to) == lists[from].end());
    this->lists[from].insert(to);
}

int SetGraph::VerticesCount() const {
    return this->lists.size();
}

std::vector<int> SetGraph::GetNextVertices(int vertex) const {
    std::vector<int> res{};
    for (auto i : this->lists[vertex]) {
        res.push_back(i);
    }
    return res;
}

std::vector<int> SetGraph::GetPrevVertices(int vertex) const {
    std::vector<int> res{};
    for (int i = 0; i < this->lists.size(); ++i) {
        for (auto &v : lists[i]) {
            if (v == vertex) {
                res.push_back(i);
            }
        }
    }
    return res;
}
