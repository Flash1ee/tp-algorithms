//
// Created by flashie on 24.05.2021.
//
#include <cassert>
#include "ListGraph.hpp"

int ListGraph::VerticesCount() const {
    return this->lists.size();
}

ListGraph::ListGraph(int vertices) : lists(vertices) {

}

ListGraph::ListGraph(IGraph &graph) : lists(graph.VerticesCount()) {
    for (int i = 0; i < lists.size(); ++i) {
        lists[i] = graph.GetNextVertices(i);
    }
}

std::vector<int> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < lists.size());
    return lists[vertex];
}

std::vector<int> ListGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < lists.size());
    std::vector<int> res;
    for (int from = 0; from < lists.size(); ++from) {
        for (int to = 0; to < lists[from].size(); ++to) {
            if (lists[from][to] == vertex) {
                res.push_back(from);
            }
        }
    }
    return res;
}

void ListGraph::AddEdge(int from, int to) {
    assert(from >= 0 && from < lists.size());
    assert(to >= 0 && to < lists.size());

    lists[from].push_back(to);

}


