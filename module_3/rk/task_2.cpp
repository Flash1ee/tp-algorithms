//
// Created by flashie on 04.06.2021.
//
// Проверить, является ли неориентированный граф деревом.
// Первая строка содержит число N – количество вершин.
// Вторая строка содержит число M - количество ребер.


#include <vector>
#include <cassert>
#include <queue>
#include <iostream>

#define IS_TREE 1

struct IGraph {
    virtual ~IGraph() {};

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;

    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

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

void dfs(const IGraph &graph, std::vector<bool> &visited, int vertex) {
    visited[vertex] = true;
    for (auto &to : graph.GetNextVertices(vertex)) {
        if (!visited[to]) {
            dfs(graph, visited, to);
        }
    }
}


int main() {
    int n = 0;
    int m = 0;

    std::cin >> n >> m;
    int from = 0;
    int to = 0;

    ListGraph graph(n);
    for (int i = 0; i < m; ++i) {
        std::cin >> from >> to;
        graph.AddEdge(from, to);
        graph.AddEdge(to, from);
    }

    bool is_tree = IS_TREE;
    if (n - 1 !=  m) {
        is_tree = !IS_TREE;
    } else {
        std::vector<bool> visited(n, false);
        for (int i = 0; i < n; ++i) {
            if (!visited[i]) {
                dfs(graph,visited, i);
            }
        }
        for (auto &&i : visited) {
            if (!i) {
                is_tree = !IS_TREE;
            }
        }
    }

    std::cout << is_tree << std::endl;

    return 0;
}