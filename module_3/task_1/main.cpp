//
// Created by flashie on 24.05.2021.
//
#include "ListGraph.hpp"
#include <iostream>


int main() {
//0---->1
//|\   /
//| \ /
//2  3--->4

    int vertices = 5;

    ListGraph list_graph(vertices);
    list_graph.AddEdge(0, 1);
    list_graph.AddEdge(0, 3);
    list_graph.AddEdge(0, 2);
    list_graph.AddEdge(1, 3);
    list_graph.AddEdge(3, 4);

    BFS(list_graph, 0, [](int vertex)->void{std::cout << vertex << " ";});



    return 0;
}
