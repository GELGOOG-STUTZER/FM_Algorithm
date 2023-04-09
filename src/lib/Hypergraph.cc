#include "Hypergraph.h"
#include <cassert>
#include <sstream>
#include <string>

Hypergraph::Hypergraph(std::ifstream &file_input) {
    assert(file_input.is_open());

    unsigned edges_number = 0;
    unsigned vertices_number = 0;
    file_input >> edges_number >> vertices_number;

    Vertices.resize(vertices_number + 1);
    Edges.resize(edges_number + 1);

    unsigned vertice_current{0};
    std::string Line;
    unsigned edge_current{0};

    while (getline(file_input, Line)) {
        std::istringstream Stream(Line);
        while (Stream >> vertice_current) {
            Vertices[vertice_current].push_back(edge_current);
            Edges[edge_current].push_back(vertice_current);
        }
        ++edge_current;
    }
}
