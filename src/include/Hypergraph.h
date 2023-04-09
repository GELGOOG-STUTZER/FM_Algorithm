#pragma once

#include <fstream>
#include <iostream>
#include <vector>

class Hypergraph {
    std::vector<std::vector<unsigned>> Vertices;
    std::vector<std::vector<unsigned>> Edges;
public:
    explicit Hypergraph(std::ifstream &file_input);
    std::vector<std::vector<unsigned>> const &get_vertices() const { return Vertices; }
    std::vector<std::vector<unsigned>> const &get_edges() const { return Edges; }
};
