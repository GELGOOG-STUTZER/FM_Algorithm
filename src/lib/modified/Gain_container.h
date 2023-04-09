#pragma once

#include <iostream>
#include <list>
#include <map>
#include <vector>

class Hypergraph;
class Partitions;

namespace AlgMod {

class Gain_container {

    using RLSide = std::map<int, std::list<int>>; // Map with gains and vertices (list is used)
    using Move = std::pair<unsigned, int>;        // Information about a move of a vertice

    RLSide Left;  // Left side of partition
    RLSide Right; // Right side of partition

    std::vector<int> VertGain;   // Vector containing gain of every vertice
    std::vector<bool> Deleted; // vector containing deleted vertices
    std::vector<int> Deltas;     // Vector with costs of vertices
    std::vector<std::list<int>::iterator> Iterators;

    RLSide &get_need_side(bool Side);

public:
    Gain_container(Hypergraph const &HG, Partitions const &Prt);
    bool is_empty() const;
    Move best_feasible_move(int Displacement);
    void update(unsigned Vertex, bool Side, int Value);
    void erase(unsigned Vertex, bool Side);
    void update_deleted(unsigned Vertex);
    std::vector<int> &get_deltas();
};

}
