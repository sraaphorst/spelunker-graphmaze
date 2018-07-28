#include <cassert>
#include <iostream>
#include <tuple>
#include <vector>

#include <graphmaze/GraphUtils.h>
#include <graphmaze/MazeGraph.h>
#include <graphmaze/MazeGenerator.h>
#include <graphmaze/BinaryTreeMazeGenerator.h>

using namespace spelunker::graphmaze;

constexpr auto W = 100;
constexpr auto H = 100;
constexpr auto R = 10;

int main() {
//    const auto grid = GraphUtils::makeProjectivePlane(W, H);
//    const auto maze = BinaryTreeMazeGenerator{}.generate(grid);
//    GraphUtils::outputGraph(std::cout, maze);

    const auto circ = GraphUtils::makeCircular(R);
    const auto cmaze = BinaryTreeMazeGenerator{}.generate(circ);
    GraphUtils::outputGraph(std::cout, cmaze);
}