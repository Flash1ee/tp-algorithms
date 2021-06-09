#include <vector>
#include <set>
#include <cassert>
#include <queue>
#include <iostream>
#include <climits>

struct IGraph {
    virtual ~IGraph() = default;

    // Добавление ребра от from к to.
    virtual void AddEdge(size_t from, size_t to, size_t weight) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<std::pair<int, int>> GetNextVertices(int vertex) const = 0;

    virtual std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const = 0;
};

class ListGraph : public IGraph {
public:
    explicit ListGraph(IGraph &graph);

    explicit ListGraph(int vertices);

    ~ListGraph() override = default;

    // Добавление ребра от from к to.
    void AddEdge(size_t from, size_t to, size_t weight) override;

    int VerticesCount() const override;

    std::vector<std::pair<int, int>> GetNextVertices(int vertex) const override;

    std::vector<std::pair<int, int>> GetPrevVertices(int vertex) const override;

    friend int getShortPath(const IGraph &graph, size_t from, size_t to);

private:
    std::vector<std::vector<std::pair<int, int>>> lists;
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

std::vector<std::pair<int, int>> ListGraph::GetNextVertices(int vertex) const {
    assert(vertex >= 0 && vertex < lists.size());
    return lists[vertex];
}

std::vector<std::pair<int, int>> ListGraph::GetPrevVertices(int vertex) const {
    assert(vertex >= 0 && vertex < lists.size());
    std::vector<std::pair<int, int>> res;
    for (int from = 0; from < lists.size(); ++from) {
        for (int to = 0; to < lists[from].size(); ++to) {
            if (lists[from][to].first == vertex) {
                res.emplace_back(from, lists[from][to].second);
            }
        }
    }
    return res;
}

void ListGraph::AddEdge(size_t from, size_t to, size_t weight) {
    assert(from < lists.size());
    assert(to < lists.size());

    lists[from].push_back({to, weight});

}

bool relax(std::vector<int> &pi, std::vector<int> &d, int u, std::pair<int, int> &v) {
    if (d[v.first] > d[u] + v.second) {
        d[v.first] = d[u] + v.second;
        pi[v.first] = u;
        return true;
    }
    return false;
}

int getShortPath(const IGraph &graph, size_t from, size_t to) {
    std::vector<int> parents(graph.VerticesCount(), -1);
    std::vector<int> distance(graph.VerticesCount(), INT_MAX);

    distance[from] = 0;
    std::set<std::pair<int, int>, std::greater<std::pair<int, int>>> priority_q;

    priority_q.insert(std::make_pair(from, 0));

    while (!priority_q.empty()) {
        auto u = *priority_q.begin();
        auto tmp = priority_q.erase(u);
        auto next = graph.GetNextVertices(u.first);
        for (auto &v : next) {
            if (distance[v.first] == INT_MAX) {
                distance[v.first] = distance[u.first] + v.second;
                parents[v.first] = u.first;
                priority_q.insert(std::make_pair(v.first, distance[v.first]));
            } else {
                tmp = distance[v.first];
                if (relax(parents, distance, u.first, v)) {
                    priority_q.erase({v.first, tmp});
                    priority_q.insert(std::make_pair(v.first, distance[v.first]));
                }
            }
        }
    }
    if (distance[to] == INT_MAX) {
        return -1;
    }

    return distance[to];
}

int main() {
    size_t n = 0;
    size_t m = 0;
    std::cin >> n >> m;
    ListGraph graph(n);

    size_t from = 0;
    size_t to = 0;
    size_t weight = 0;

    for (size_t i = 0; i < m; ++i) {
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
        graph.AddEdge(to, from, weight);

    }

    std::cin >> from >> to;
    int res = getShortPath(graph, from, to);
    std::cout << res;

    return 0;
}