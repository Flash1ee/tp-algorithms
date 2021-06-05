//
// Created by flashie on 04.06.2021.
//
#include <vector>

/* Нужно написать функцию, проверяющую, является ли граф Эйлеровым.
 * На вход функции подается граф IGraph с интерфейсом, как в задаче 1 модуля 3.
*/
struct IGraph {
    virtual ~IGraph() {};

    // Добавление ребра от from к to.
    virtual void AddEdge(int from, int to) = 0;

    virtual int VerticesCount() const = 0;

    virtual std::vector<int> GetNextVertices(int vertex) const = 0;

    virtual std::vector<int> GetPrevVertices(int vertex) const = 0;
};

void dfs(const IGraph &graph, std::vector<bool> &visited, int vertex) {
    visited[vertex] = true;
    for (auto &to : graph.GetNextVertices(vertex)) {
        if (!visited[to]) {
            dfs(graph, visited, to);
        }
    }
}


bool is_eulerian_graph(const IGraph &graph) {
    int oddVertex = 0;
    for (int v = 0; v <graph.VerticesCount(); ++v) {
        size_t deg = graph.GetNextVertices(v).size();
        if (deg % 2) {
            ++oddVertex;
        }
    }
    // вершин с нечетной степенью больше двух -> граф не эйлеров
    if (oddVertex > 2) {
        return false;
    }

    std::vector<bool> visited(graph.VerticesCount(), false);
    for (int v = 0; v <graph.VerticesCount(); ++v) {
        if (!graph.GetNextVertices(v).empty()) {
            dfs(graph, visited, v);
            break;
        }
    }
    for (int v = 0; v <graph.VerticesCount(); ++v) {
        // если количество компонент связности, содержащие ребра, > 1, то граф не эйлеров
        if (!graph.GetNextVertices(v).empty() && !visited[v]) {
            return false;
        }
    }
    return true;
}
