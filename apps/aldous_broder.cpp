#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

#include <graphmaze/GraphUtils.h>
#include <graphmaze/MazeGraph.h>
#include <graphmaze/MazeGenerator.h>
#include <graphmaze/AldousBroderMazeGenerator.h>

using namespace spelunker::graphmaze;

constexpr auto W = 50;
constexpr auto H = 40;

int main() {
    const auto torus = GraphUtils::makeCylinder(W, H);
    const auto maze = AldousBroderMazeGenerator{}.generate(torus);
    GraphUtils::outputGraph(std::cout, maze);
}