
//
// Created by flashie on 04.06.2021.
//
#include <iostream>
#include <vector>
#include <cassert>
#include <climits>
#include <set>

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
int getShortPath(const IGraph &graph, int k, int start, int finish) {
    std::vector<int> parents(graph.VerticesCount(), -1);
    std::vector<int> distance(graph.VerticesCount(), INT_MAX);

    distance[start] = 0;
    std::set<std::pair<int, int>, std::greater<std::pair<int, int>>> priority_q;

    priority_q.insert(std::make_pair(start, 0));

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
long long BellmanFord(const IGraph &graph, int k, int start, int finish){
    std::vector<std::pair<int, int>> distance(graph.VerticesCount(), std::make_pair(INT_MAX, INT_MAX)); //длина пути не привышает k в k-ом столбце
    distance[start].first = 0;
    for (int i = 0; i < k; ++i) {
        int prev = i % 2;
        int cur = 1 - (i % 2);
        for(int j = 0; j < graph.VerticesCount(); ++j) {
            if (distance[graph.GetNextVertices(j)[0]][prev] < INT_MAX) { //RELAX
                if (distance[e[j].finish][current_step] > distance[e[j].start][previous_step] + e[j].weight) {
                    distance[e[j].finish][current_step] = distance[e[j].start][previous_step] + e[j].weight;
                }
            }
        }
        for (int vertex = 0; vertex < distance.size(); vertex++) {
            distance[vertex][current_step ] = std::min(distance[vertex][current_step], distance[vertex][previous_step]);
        }
    }
    return distance[finish][K % 2];
}

int main() {
    int n, m, k, beg, end, from, to, weight;
    std::cin >> n >> m >> k >> beg >> end;
    ListGraph graph(n);
    for (int i = 0; i < m; ++i) {
        std::cin >> from >> to >> weight;
        graph.AddEdge(from, to, weight);
    }

    long long answer = BellmanFord(e, N, K, Start - 1, Finish - 1);
    if(answer == INF){
        std::cout << -1 << '\n';
    } else {
        std::cout << answer << '\n';
    }
}
