/**
 * GraphUtils.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <iostream>
#include <tuple>

#include "MazeGraph.h"
#include "GraphUtils.h"

namespace spelunker::graphmaze {

    // Ranking / unranking functions needed to work with the underlying grid nature.
    static int ranker(const int x, const int y, const int width, const int height) {
        const auto xp = (x + width) % width;
        const auto yp = (y + height) % height;
        return xp + yp * width;
    }

    static std::pair<int, int> unranker(const int v, const int width) {
        return std::make_pair(v % width, v / width);
    }

    MazeGraph GraphUtils::makeGrid(const int width, const int height) {
        MazeGraph g;

        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x) {
                const auto v = ranker(x, y, width, height);
                const auto w = boost::add_vertex(g);
                assert(v == w);
            }

        // Now add all right and down edges.
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                // Check if we can add right.
                if (x < width - 1)
                    boost::add_edge(ranker(x, y, width, height), ranker(x+1, y, width, height), g);
                // Check if we can add down.
                if (y < height - 1)
                    boost::add_edge(ranker(x, y, width, height), ranker(x, y+1, width, height), g);
            }
        }

        return g;
    }

    MazeGraph GraphUtils::makeCylinder(const int width, const int height) {
        MazeGraph g = makeGrid(width, height);

        // Add the additional edges.
        for (auto y = 0; y < height; ++y)
            boost::add_edge(ranker(0, y, width, height), ranker(width-1, y, width, height), g);
        return g;
    }

    MazeGraph GraphUtils::makeTorus(const int width, const int height) {
        MazeGraph g = makeCylinder(width, height);

        // Add the additional edges.
        for (auto x = 0; x < width; ++ x)
            boost::add_edge(ranker(x, 0, width, height), ranker(x, height-1, width, height), g);

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

        // These are perfect mazes, so |edges| = #vertices - 1.
        auto [ceIter, ceEnd] = boost::edges(graph);
        const auto numEdges = std::distance(ceIter, ceEnd);
        assert(numEdges == numVertices(graph) - 1);
    }

    // TODO: Fix this up.
    vertex GraphUtils::randomStartVertex(const MazeGraph &maze) noexcept {
        return 0;
    }

    static VertexCollection nbrs(const MazeSeed &seed, const vertex &v, const bool visited) {
        VertexCollection unvisitedNeighbours;
        for (auto [eiter, eEnd] = boost::out_edges(v, seed.tmplt); eiter != eEnd; ++eiter) {
            const vertex t = boost::target(*eiter, seed.tmplt);
            if (seed.unvisited.at(t) != visited)
                unvisitedNeighbours.emplace_back(t);
        }
        return unvisitedNeighbours;
    }

    VertexCollection GraphUtils::unvisitedNeighbours(const MazeSeed &seed, const vertex &v) {
        return nbrs(seed, v, false);
    }

    VertexCollection GraphUtils::visitedNeighbours(const MazeSeed &seed, const vertex &v) {
        return nbrs(seed, v, true);
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