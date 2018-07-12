#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

#include "graphmaze/GraphUtils.h"
#include "graphmaze/MazeGraph.h"
#include "graphmaze/MazeGenerator.h"
#include "graphmaze/BFSMazeGenerator.h"

using namespace spelunker::graphmaze;

constexpr auto W = 50;
constexpr auto H = 40;

int main() {
    MazeGraph torus = GraphUtils::makeTorus(W, H);
    const auto maze = BFSMazeGenerator{}.generate(torus);
    GraphUtils::outputGraph(std::cout, maze);
}
