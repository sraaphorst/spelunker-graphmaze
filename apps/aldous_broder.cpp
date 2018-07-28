#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

#include <graphmaze/GraphUtils.h>
#include <graphmaze/MazeGraph.h>
#include <graphmaze/MazeGenerator.h>
#include <graphmaze/AldousBroderMazeGenerator.h>

using namespace spelunker::graphmaze;

constexpr auto W = 100;
constexpr auto H = 100;

int main() {
    const auto grid = GraphUtils::makeKleinBottle(W, H);
    const auto maze = AldousBroderMazeGenerator{}.generate(grid);
    GraphUtils::outputGraph(std::cout, maze);
}