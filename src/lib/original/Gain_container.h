#pragma once

#include <iostream>
#include <map>
#include <set>
#include <vector>

class Hypergraph;
class Partitions;

class Gain_container {

    using RLSide = std::map<int, std::set<int>>; // Map with gains and vertices (set is used)
    using Move = std::pair<unsigned, int>;       // Information about a move of a vertice

    RLSide Left;  // Left side of partition
    RLSide Right; // Right side of partition

    std::vector<int> VertGain;
    std::set<unsigned> Deleted; // set containing deleted vertices

    RLSide &get_need_side(bool Side);
    RLSide const &get_need_side(bool Side) const;

public:
    Gain_container(Hypergraph const &HG, Partitions const &Prt);
    bool is_empty(bool Check) const;
    Move best_feasible_move(bool Side);
    void update(unsigned Vertex, bool Side, int Value);
    void erase(unsigned Vertex, bool Side);
    void update_deleted(unsigned Vertex);

};
