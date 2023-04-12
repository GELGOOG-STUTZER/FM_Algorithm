#include "Algorithm.h"
#include "AlgorithmMod.h"
#include "Hypergraph.h"
#include "Partitions.h"
#include <chrono>

int main(int argc, char *argv[]) {
    using namespace std::literals;

    std::string const argument{argv[1]};
    bool const is_modified = argument == "-m";
    int const file_index = is_modified ? 2 : argument.empty() ? 2 : 1;

    std::string const input_file{argv[file_index]};
    std::ifstream File{input_file};
    std::ofstream Output_File{input_file + ".part.2"};

    Hypergraph Graph{File};
    Partitions Prt{Graph};

    auto const start = std::chrono::steady_clock::now();
    unsigned const iterations = is_modified ? AlgMod::FM(Graph, Prt) : Alg::FM(Graph, Prt);
    auto const finish = std::chrono::steady_clock::now();



    Prt.out(Output_File);

    return 0;
}
