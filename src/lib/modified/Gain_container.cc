#include "Gain_container.h"
#include "Hypergraph.h"
#include "Partitions.h"

using namespace AlgMod;

Gain_container::Gain_container(Hypergraph const &HG, Partitions const &Prt) {
    unsigned const VertSize = Prt.get_part().size();
    VertGain.resize(VertSize);
    Deleted.resize(VertSize);
    Deltas.resize(VertSize);
    Iterators.resize(VertSize);

    unsigned SkipFirst = 0;
    for (auto &&VecEdges : HG.get_edges()) {
        if (!SkipFirst++)
            continue;
        int VLeft = 0, VRight = 0;
        unsigned VLeftId = 0, VRightId = 0;
        for (unsigned Vertex : VecEdges)
            if (!Prt.get_part().at(Vertex)) {
                VLeft++;
                VLeftId = Vertex;
            } else {
                VRight++;
                VRightId = Vertex;
            }

        if ((VRight == 0 && VLeft != 1) || (VLeft == 0 && VRight != 1))
            for (unsigned Vertex : VecEdges)
                --VertGain[Vertex];

        if (VLeft == 1 && VRight != 0)
            ++VertGain[VLeftId];
        if (VRight == 1 && VLeft != 0)
            ++VertGain[VRightId];
    }

    for (unsigned i = Prt.get_part().size() - 1; i != 0; --i) {
        auto &&Side = get_need_side(Prt.get_part().at(i));
        auto &Item = Side[VertGain[i]];
        Item.push_front(i);
        Iterators[i] = Item.begin();
    }
}

Gain_container::RLSide &Gain_container::get_need_side(bool Side) {
    return Side ? Right : Left;
}

Gain_container::RLSide const &Gain_container::get_need_side(bool Side) const {
    return Side ? Right : Left;
}

bool Gain_container::is_empty(bool Side) const {
    auto const &SizeToUpd = get_need_side(Side);
    return SizeToUpd.empty();
}

Gain_container::Move Gain_container::best_feasible_move(int Displacement) {
    const bool IsSideR = Displacement > 0 ? true : false;

    auto &SizeToUpd = get_need_side(IsSideR);

    auto &&[Gain, Vertices] = *SizeToUpd.rbegin();
    unsigned Vertex = *Vertices.begin();
    Vertices.pop_front();
    Deleted[Vertex] = true;
    if (Vertices.empty())
        SizeToUpd.erase(Gain);

    return std::make_pair(Vertex, Gain);
}

void Gain_container::update(unsigned Vertex, bool Side, int Value) {
    if (Deleted[Vertex])
        return;
    erase(Vertex, Side);
    VertGain[Vertex] += Value;
    auto &SizeToUpd = get_need_side(Side);
    SizeToUpd[VertGain[Vertex]].push_front(Vertex);
    Iterators[Vertex] = SizeToUpd[VertGain[Vertex]].begin();
}

void Gain_container::erase(unsigned Vertex, bool Side) {
    if (Deleted[Vertex])
        return;
    auto &SizeToUpd = get_need_side(Side);
    int const Gain = VertGain.at(Vertex);
    SizeToUpd[Gain].erase(Iterators[Vertex]);
    if (SizeToUpd[Gain].empty())
        SizeToUpd.erase(Gain);
}

void Gain_container::update_deleted(unsigned Vertex) {
    Deleted[Vertex] = true;
}

std::vector<int> &Gain_container::get_deltas() {
    return Deltas;
}
