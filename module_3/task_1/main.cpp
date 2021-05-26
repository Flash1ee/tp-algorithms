//
// Created by flashie on 24.05.2021.
//
#include "ListGraph.hpp"
#include "MatrixGraph.hpp"
#include "SetGraph.hpp"
#include "ArcGraph.hpp"

#include <iostream>


int main() {
//0---->1--->5
//|\   /
//| \ /
//2  3--->4

    int vertices = 6;

    ListGraph list_graph(vertices);
    list_graph.AddEdge(0, 1);
    list_graph.AddEdge(0, 3);
    list_graph.AddEdge(0, 2);
    list_graph.AddEdge(1, 3);
    list_graph.AddEdge(1, 5);
    list_graph.AddEdge(3, 4);


    BFS(list_graph, 0, [](int vertex) -> void { std::cout << vertex << " "; });

    std::cout << std::endl;
    MatrixGraph matrix_graph(vertices);
    matrix_graph.AddEdge(0, 1);
    matrix_graph.AddEdge(0, 3);
    matrix_graph.AddEdge(0, 2);
    matrix_graph.AddEdge(1, 3);
    matrix_graph.AddEdge(1, 5);
    matrix_graph.AddEdge(3, 4);


    BFS(matrix_graph, 0, [](int vertex) -> void { std::cout << vertex << " "; });

    std::cout << std::endl;

    SetGraph set_graph(vertices);
    set_graph.AddEdge(0, 1);
    set_graph.AddEdge(0, 3);
    set_graph.AddEdge(0, 2);
    set_graph.AddEdge(1, 3);
    set_graph.AddEdge(1, 5);
    set_graph.AddEdge(3, 4);


    BFS(set_graph, 0, [](int vertex) -> void { std::cout << vertex << " "; });

    std::cout << std::endl;

    ArcGraph arc_graph(vertices);
    arc_graph.AddEdge(0, 1);
    arc_graph.AddEdge(0, 3);
    arc_graph.AddEdge(0, 2);
    arc_graph.AddEdge(1, 3);
    arc_graph.AddEdge(1, 5);
    arc_graph.AddEdge(3, 4);


    BFS(arc_graph, 0, [](int vertex) -> void { std::cout << vertex << " "; });

    std::cout << std::endl;



    return 0;
}
