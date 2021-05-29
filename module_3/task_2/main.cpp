#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <memory>
#include <climits>
#include <algorithm>

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

class ShortestWay {
public:
    ShortestWay() = delete;

    ShortestWay(const ShortestWay &) = delete;

    ShortestWay &operator=(const ShortestWay &) = delete;

    explicit ShortestWay(int vertexes) : _graph(std::make_shared<ListGraph>(vertexes)) {}

    long GetCountWays(int from, int to) {
        BFS(from, to);
        return _ways_count[to];
    }

    std::shared_ptr<IGraph> _graph;

private:
    void BFS(int from, int to);

    std::vector<int> _ways_count;
    std::vector<int> _distance;
};

void ShortestWay::BFS(int from, int to) {
    _distance.assign(_graph->VerticesCount(), INT_MAX);
    _ways_count.assign(_graph->VerticesCount(), 0);

    std::queue<int> q;
    std::vector<bool> visit(_graph->VerticesCount(), false);

    _distance[from] = 0;
    _ways_count[from] = 1;
    visit[from] = true;
    q.push(from);

    while (!q.empty()) {
        int u = q.front();
        q.pop();
        for (auto &v : _graph->GetNextVertices(u)) {
            if (!visit[v]) {
                q.push(v);
                visit[v] = true;
            }
            if (_distance[u] + 1 < _distance[v]) {
                _ways_count[v] = _ways_count[u];
                _distance[v] = _distance[u] + 1;
            } else if (_distance[u] + 1 == _distance[v]) {
                _ways_count[v] += _ways_count[u];
            }
        }
        if (u == to) {break;}
    }


}

int main() {
    /*
     * 0--1
     *  \ | \
     *    2--3
     */
    int vertexes = 0;
    int edges = 0;
    std::cin >> vertexes >> edges;
    ShortestWay ways(vertexes);

    int from, to;
    for (auto i = 0; i < edges; ++i) {
        std::cin >> from >> to;
        ways._graph->AddEdge(from, to);
        ways._graph->AddEdge(to, from);
    }

    std::cin >> from >> to;

    std::cout << ways.GetCountWays(from, to);
    return 0;

}