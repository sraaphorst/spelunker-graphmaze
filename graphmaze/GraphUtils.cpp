/**
 * GraphUtils.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>

#include "MazeGraph.h"
#include "GraphUtils.h"

namespace spelunker::graphmaze {

    MazeGraph GraphUtils::makeTorus(const int width, const int height) {
        auto ranker = [width, height](int x, int y) {
            x = (x + width) % width;
            y = (y + height) % height;
            return x + y * width;
        };

        auto unranker = [width](int v) {
            return std::make_pair(v % width, v / width);
        };

        MazeGraph g;
        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x) {
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

        return g;
    }

    MazeSeed GraphUtils::makeSeed(const MazeGraph &tmplt) noexcept {
        return MazeSeed {
            tmplt,
            createInitialMaze(tmplt),
            numVertices(tmplt),
            initializeUnvisitedVertices(tmplt)
        };
    }

    void GraphUtils::outputGraph(std::ostream &out, MazeGraph graph) {
        for (auto[viter, vend] = boost::vertices(graph); viter != vend; ++viter)
            std::cout << "Vertex " << *viter << std::endl;
        for (auto[eiter, eend] = boost::edges(graph); eiter != eend; ++eiter)
            std::cout << "Edge " << *eiter << std::endl;
    }

    // TODO: Fix this up.
    vertex GraphUtils::randomStartVertex(const MazeGraph &maze) noexcept {
        return 0;
    }

    VertexCollection GraphUtils::unvisitedNeighbours(const MazeSeed &seed, const vertex &v) {
        VertexCollection unvisitedNeighbours;
        for (auto [eiter, eEnd] = boost::out_edges(v, seed.tmplt); eiter != eEnd; ++eiter) {
            const vertex t = boost::target(*eiter, seed.tmplt);
            if (seed.unvisited.at(t))
                unvisitedNeighbours.emplace_back(t);
        }
        return unvisitedNeighbours;
    }

    VertexCollection GraphUtils::neighbours(const MazeSeed &seed, const vertex &v) {
        VertexCollection neighbours;
        for (auto [eiter, eEnd] = boost::out_edges(v, seed.tmplt); eiter != eEnd; ++eiter) {
            const auto t = boost::target(*eiter, seed.tmplt);
            neighbours.emplace_back(t);
        }
        return neighbours;
    }

    MazeGraph GraphUtils::createInitialMaze(const MazeGraph &tmplt) noexcept {
        MazeGraph out;
        for (auto[vIter, vEnd] = boost::vertices(tmplt); vIter != vEnd; ++vIter)
            boost::add_vertex(out);
        return out;
    }

    UnvisitedVertices GraphUtils::initializeUnvisitedVertices(const MazeGraph &tmplt) noexcept {
        UnvisitedVertices unvisited;
        for (auto[vIter, vEnd] = boost::vertices(tmplt); vIter != vEnd; ++vIter)
            unvisited[*vIter] = true;
        return unvisited;
    }

    int GraphUtils::numVertices(const MazeGraph &maze) {
        auto [vIter, vEnd] = boost::vertices(maze);
        return std::distance(vIter, vEnd);
    }
}