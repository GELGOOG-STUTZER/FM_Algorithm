#include "Gain_container.h"
#include "Hypergraph.h"
#include "Partitions.h"

// Constructor of GainContainer
// Calculates gain for each vertice
Gain_container::Gain_container(Hypergraph const &HG, Partitions const &Prt) {
    unsigned const VertSize = Prt.get_part().size();
    VertGain.resize(VertSize);

    for (unsigned i = 0; i != VertSize; ++i) {
        int CurrentGain = 0;
        bool CurrentPart = Prt.get_part().at(i);

        for (unsigned EdgeId : HG.get_vertices().at(i)) {
            bool IsAlone = true;
            bool IsEntirely = true;

            for (unsigned VertId : HG.get_edges().at(EdgeId)) {
                if (CurrentPart != Prt.get_part().at(VertId))
                    IsEntirely = false;
                if (CurrentPart == Prt.get_part().at(VertId) && VertId != i)
                    IsAlone = false;
            }

            if (IsAlone)
                ++CurrentGain;
            if (IsEntirely)
                --CurrentGain;
        }

        auto &SizeToUpd = get_need_side(CurrentPart);
        SizeToUpd[CurrentGain].insert(i);
        VertGain[i] = CurrentGain;
    }
}

// Method to get the whole side of a partition
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

// Creates a pair showing the best possible move of a vertice at the moment
Gain_container::Move Gain_container::best_feasible_move(int Side) {
    bool IsSideR;

    if (Side == 1) {
        IsSideR = true;
    }
    else if (Side == 0) {
        IsSideR = false;
    }

    auto &SizeToUpd = IsSideR ? Right : Left;

    auto &&[Gain, Vertices] = *SizeToUpd.rbegin();
    unsigned Vertex = *Vertices.begin();
    Vertices.erase(Vertex);

    if (Vertices.empty())
        SizeToUpd.erase(Gain);

    return std::make_pair(Vertex, Gain);
}

// Method to update Gain_container
void Gain_container::update(unsigned Vertex, bool Side, int Value) {
    if (Deleted.count(Vertex))
        return;
    erase(Vertex, Side);
    VertGain[Vertex] += Value;
    auto &SizeToUpd = get_need_side(Side);
    SizeToUpd[VertGain[Vertex]].insert(Vertex);
}

// Removes vertice from gain container
void Gain_container::erase(unsigned Vertex, bool Side) {
    auto &SizeToUpd = get_need_side(Side);
    int const Gain = VertGain.at(Vertex);
    SizeToUpd[Gain].erase(Vertex);
    if (SizeToUpd[Gain].empty())
        SizeToUpd.erase(Gain);
}

// Update the list of deleted vertices
void Gain_container::update_deleted(unsigned Vertex) { Deleted.insert(Vertex); }
