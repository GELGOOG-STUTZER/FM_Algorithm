#include "Partitions.h"
#include <algorithm>

// Calculates cost (number of nets which are cut)
void Partitions::calculate_cost() {
    unsigned Skip = 0;
    for (auto &&EdgesVec : Graph.get_edges()) {
        if (!Skip++)
            continue;
        bool const CurrentPartition = VertPartitions[EdgesVec[0]];
        bool const Inc =
                std::any_of(EdgesVec.begin() + 1, EdgesVec.end(),
                            [&CurrentPartition, this](int Edge) {
                    return VertPartitions[Edge] != CurrentPartition;});
        if (Inc)
            ++Cost;
    }
}

Partitions::Partitions(Hypergraph const GraphIn) : Graph{GraphIn} {
    unsigned const Size = Graph.get_vertices().size() - 1;
    VertPartitions.resize(Size + 1);

    for (unsigned i = 1; i <= Size / 2; ++i)
        VertPartitions[i] = false;
    for (unsigned i = Size / 2 + 1; i <= Size; ++i)
        VertPartitions[i] = true;

    Side = Size % 2;
    calculate_cost();
}

void Partitions::update(unsigned Vertex) {
    int const Inc = VertPartitions.at(Vertex) ? -1 : 1;
    Side += Inc;
    VertPartitions.at(Vertex) = !VertPartitions.at(Vertex);
}

void Partitions::out(std::ostream &Out) const {
    unsigned Skip = 0;
    for (auto &&Part : VertPartitions) {
        if (!Skip++)
            continue;
        Out << Part << '\n';
    }
}