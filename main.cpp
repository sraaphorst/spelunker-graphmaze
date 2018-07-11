#include "graphmaze/MazeGraph.h"
#include "graphmaze/MazeGenerator.h"
#include "graphmaze/BFSMazeGenerator.h"
#include <cassert>
#include <tuple>
#include <vector>
#include <iostream>

using namespace spelunker::graphmaze;

// Let's make a torus.
constexpr auto W = 6;
constexpr auto H = 4;
constexpr auto V = W * H;

int ranker(int x, int y) {
    x = (x + W) % W;
    y = (y + H) % H;
    return x + y * W;
}

std::pair<int, int> unranker(int v) {
    return std::make_pair(v % W, v / W);
}

int main() {
    std::cout << "Starting..." << std::endl;

    MazeGraph g;
    for (auto y = 0; y < H; ++y)
        for (auto x = 0; x < W; ++x) {
            const auto v = ranker(x, y);
            const auto w = boost::add_vertex(g);
            assert(v == w);

            std::cout << "Added vertex " << v << "(" << x << ", " << y << ")" << std::endl;
        }

    // Create the torus.
    for (auto[viter, vend] = boost::vertices(g); viter != vend; ++viter) {
        // Add all four directions.
        const auto v = *viter;
        const auto [vx, vy] = unranker(v);
        const auto xm1 = ranker(vx-1, vy);
        const auto xp1 = ranker(vx+1, vy);
        const auto ym1 = ranker(vx, vy-1);
        const auto yp1 = ranker(vx, vy+1);
        std::cout << "Adding edge to (" << v << "," << xm1 << ")" << std::endl;
        boost::add_edge(v, xm1, g);
        std::cout << "Adding edge to (" << v << "," << xp1 << ")" << std::endl;
        boost::add_edge(v, xp1, g);
        std::cout << "Adding edge to (" << v << "," << ym1 << ")" << std::endl;
        boost::add_edge(v, ym1, g);
        std::cout << "Adding edge to (" << v << "," << yp1 << ")" << std::endl;
        boost::add_edge(v, yp1, g);
    }

    BFSMazeGenerator rng(W, H);
    const auto maze = rng.generate(g);

    for (auto[viter, vend] = boost::vertices(maze); viter != vend; ++viter)
        std::cout << "Vertex " << *viter << std::endl;
    for (auto[eiter, eend] = boost::edges(maze); eiter != eend; ++eiter)
        std::cout << "Edge " << *eiter << std::endl;
}
