/**
 * binary_tree.cpp
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
#include <graphmaze/BinaryTreeMazeGenerator.h>

using namespace spelunker::graphmaze;

constexpr auto R = 21;

int main() {
    const auto circ = GraphUtils::makeSpherical(R);
    const auto cmaze = BinaryTreeMazeGenerator{}.generate(circ);
    GraphUtils::outputGraph(std::cout, cmaze);
}