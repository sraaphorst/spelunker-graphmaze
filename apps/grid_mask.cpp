/**
 * grid_mask.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <cassert>
#include <iostream>
#include <fstream>
#include <tuple>
#include <vector>

#include <graphmaze/GraphUtils.h>
#include <graphmaze/MazeGraph.h>
#include <graphmaze/MazeGenerator.h>
//#include <graphmaze/BinaryTreeMazeGenerator.h>
#include <graphmaze/DFSMazeGenerator.h>

using namespace spelunker::graphmaze;

/**
 * NOTE: This example fails to generate a binary tree maze correctly.
 */
int main() {
    std::ifstream str{"mask.txt"};
    const auto graph = GraphUtils::makeGridFromMask(str);
    //const auto maze = BinaryTreeMazeGenerator{}.generate(graph);
    const auto maze = DFSMazeGenerator{}.generate(graph);
    GraphUtils::outputGraph(std::cout, maze);
}