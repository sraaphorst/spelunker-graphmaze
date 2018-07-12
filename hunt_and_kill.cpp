#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

#include "graphmaze/GraphUtils.h"
#include "graphmaze/MazeGraph.h"
#include "graphmaze/MazeGenerator.h"
#include "graphmaze/HuntAndKillMazeGenerator.h"

using namespace spelunker::graphmaze;

constexpr auto W = 50;
constexpr auto H = 40;

int main() {
    MazeGraph torus = GraphUtils::makeGrid(W, H);
    const auto maze = HuntAndKillMazeGenerator{}.generate(torus);
    GraphUtils::outputGraph(std::cout, maze);
}