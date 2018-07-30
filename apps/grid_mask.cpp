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
#include <graphmaze/AldousBroderMazeGenerator.h>
#include <graphmaze/BFSMazeGenerator.h>
#include <graphmaze/BinaryTreeMazeGenerator.h>
#include <graphmaze/DFSMazeGenerator.h>
#include <graphmaze/HuntAndKillMazeGenerator.h>
#include <graphmaze/StringGridMazeRenderer.h>

using namespace spelunker::graphmaze;

/**
 * NOTE: This example fails to generate a binary tree maze correctly.
 * Generated by:
 *    http://patorjk.com/software/taag/#p=display&f=Doh&t=MAZE
 */
int main() {
    std::ifstream str{"mask2.txt"};
    const auto graph = GraphUtils::makeGridFromMask(str);
    //const auto maze = AldousBroderMazeGenerator{}.generate(graph);
    //const auto maze = BFSMazeGenerator{}.generate(graph);
    //const auto maze = BinaryTreeMazeGenerator{}.generate(graph);
    //const auto maze = DFSMazeGenerator{}.generate(graph);
    const auto maze = HuntAndKillMazeGenerator{}.generate(graph);

    StringGridMazeRenderer r{std::cout};
    r.render(maze);
}