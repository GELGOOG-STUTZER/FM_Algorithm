#pragma once

#include "Hypergraph.h"
#include <iostream>
#include <vector>

class Partitions {
    std::vector<bool> VertPartitions;
    Hypergraph const &Graph;

    int Cost{0};
    int Side{0};

    void calculate_cost();
public:
    explicit Partitions(Hypergraph GraphIn);
    void out(std::ostream &Out = std::cout) const;
    int get_cost() const { return Cost; }
    void set_cost(int CostIn) { Cost = CostIn; }
    int get_side() const { return Side; }
    void update(unsigned Vertex);

    std::vector<bool> const &get_part() const { return VertPartitions; }
};
