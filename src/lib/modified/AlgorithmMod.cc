#include "AlgorithmMod.h"
#include "Gain_container.h"
#include "Partitions.h"

using namespace AlgMod;

static int pass(Gain_container &GC, Partitions &Prt, Hypergraph const &HG);
static void move_apply(Gain_container &GC, Partitions &Prt, Hypergraph const &HG, unsigned MoveVertex);

namespace AlgMod {

unsigned FM(Hypergraph const &HG, Partitions &Prt) {
    unsigned Iteration = 0;
    while (true) {
        ++Iteration;
        Gain_container GC{HG, Prt};
        int const Best = pass(GC, Prt, HG);
        if (Best == Prt.get_cost())
            break;

        Prt.set_cost(Best);
    }
    return Iteration;
}

}

int pass(Gain_container &GC, Partitions &Prt, Hypergraph const &HG) {
    int Cost = Prt.get_cost();
    int BestCost = Cost;
    std::vector<unsigned> VertToChange;

    while (!GC.is_empty()) {
        auto [Vertex, Gain] = GC.best_feasible_move(Prt.get_side());
        VertToChange.push_back(Vertex);
        Cost -= Gain;
        if (Cost < BestCost) {
            BestCost = Cost;
            VertToChange.clear();
        }
        move_apply(GC, Prt, HG, Vertex);
    }

    for (auto Vertex : VertToChange)
        Prt.update(Vertex);

    return BestCost;
}

void move_apply(Gain_container &GC, Partitions &Prt, Hypergraph const &HG, unsigned MoveVertex) {
    for (auto Edge : HG.get_vertices().at(MoveVertex)) {
        bool IsNoVInDst = true;
        bool IsOneVInSrc = true;

        int CountInDst = 0;
        unsigned VertDst = 0;

        int CountInSrc = 0;
        unsigned VertSrc = 0;

        for (auto Vertex : HG.get_edges().at(Edge)) {
            if (Prt.get_part().at(MoveVertex)) {
                if (!Prt.get_part().at(Vertex)) {
                    IsNoVInDst = false;
                    CountInDst++;
                    VertDst = Vertex;
                } else if (Vertex != MoveVertex) {
                    CountInSrc++;
                    VertSrc = Vertex;
                }
            } else {
                if (Prt.get_part().at(Vertex)) {
                    IsNoVInDst = false;
                    CountInDst++;
                    VertDst = Vertex;
                } else if (Vertex != MoveVertex) {
                    CountInSrc++;
                    VertSrc = Vertex;
                }
            }

            if (Prt.get_part().at(MoveVertex) == Prt.get_part().at(Vertex) && Vertex != MoveVertex)
                IsOneVInSrc = false;
        }

        int const UpdateVal = IsNoVInDst ? 1 : IsOneVInSrc ? -1 : 0;

        if (UpdateVal)
            for (auto Vertex : HG.get_edges().at(Edge))
                GC.get_deltas().at(Vertex) += UpdateVal;

        if (CountInSrc == 1)
            ++GC.get_deltas().at(VertSrc);
        if (CountInDst == 1)
            --GC.get_deltas().at(VertDst);
    }

    GC.erase(MoveVertex, Prt.get_part().at(MoveVertex));
    GC.update_deleted(MoveVertex);

    for (unsigned Edge : HG.get_vertices().at(MoveVertex))
        for (unsigned Vertex : HG.get_edges().at(Edge))
            if (GC.get_deltas().at(Vertex)) {
                GC.update(Vertex, Prt.get_part().at(Vertex), GC.get_deltas().at(Vertex));
                GC.get_deltas().at(Vertex) = 0;
            }

    Prt.update(MoveVertex);

}
