/**
 * bfs.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include <graphmaze/GraphUtils.h>
#include <graphmaze/MazeGraph.h>
#include <graphmaze/BFSMazeGenerator.h>
#include <graphmaze/StringGridMazeRenderer.h>

using namespace spelunker::graphmaze;

constexpr auto W = 50;
constexpr auto H = 50;

int main() {
    const auto torus = GraphUtils::makeTorus(W, H);
    const auto [maze, start] = BFSMazeGenerator{}.generate(torus);
    GraphUtils::outputGraph(std::cout, maze);

    StringGridMazeRenderer r{std::cout};
    r.render(maze);
}
