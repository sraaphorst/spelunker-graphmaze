/**
 * octagonal.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

#include <graphmaze/GraphUtils.h>
#include <graphmaze/MazeGraph.h>
#include <graphmaze/MazeGenerator.h>
#include <graphmaze/AldousBroderMazeGenerator.h>

using namespace spelunker::graphmaze;

constexpr auto W = 7;
constexpr auto H = 7;

int main() {
    const auto grid = GraphUtils::makeOctagonalGrid(W, H);
    const auto maze = AldousBroderMazeGenerator{}.generate(grid);
    GraphUtils::outputGraph(std::cout, maze);
}