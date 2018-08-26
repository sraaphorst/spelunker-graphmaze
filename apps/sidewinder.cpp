/**
 * sidewinder.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include <graphmaze/GraphUtils.h>
#include <graphmaze/MazeGraph.h>
#include <graphmaze/SidewinderMazeGenerator.h>

using namespace spelunker::graphmaze;

constexpr auto W = 50;
constexpr auto H = 50;

int main() {
    MazeGraph grid = GraphUtils::makeSpherical(11);
    const auto maze = SidewinderMazeGenerator{}.generate(grid);
    GraphUtils::outputGraph(std::cout, maze);

    //StringGridMazeRenderer r{std::cout};
    //r.render(maze);
}