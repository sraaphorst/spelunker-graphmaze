/**
 * sidewinder.cpp
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
#include <graphmaze/SidewinderMazeGenerator.h>
#include <graphmaze/StringGridMazeRenderer.h>

using namespace spelunker::graphmaze;

constexpr auto W = 50;
constexpr auto H = 50;

int main() {
    MazeGraph grid = GraphUtils::makeGrid(W, H);
    const auto maze = SidewinderMazeGenerator{}.generate(grid);
    GraphUtils::outputGraph(std::cout, maze);

    StringGridMazeRenderer r{std::cout};
    r.render(maze);
}