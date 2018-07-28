/**
 * GraphUtils.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

// For M_PI.
#define _USE_MATH_DEFINES

#include <algorithm>
#include <cmath>
#include <iostream>
#include <map>
#include <set>
#include <tuple>

#include <math/DefaultRNG.h>
#include <types/AxialOrientation.h>

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

    /**
     * Create candidate functions for a binary tree over a grid.
     * We want to be able to use this function for all grid-like objects, i.e. grids, cylinders, toruses, mobius strips,
     * Klein bottles, and projective planes.
     *
     * The question in each case is whether or not we allow edges at the various boundaries, which
     * is dictated by the parameters to the function.
     *
     * Note that we NEED each row and each column to contain at least one cell of each direction or we will not be
     * connected.
     *
     * @param width the width of the grid
     * @param height the height of the grid
     * @param xlooped true if the graph wraps around the x-axis, be it reversed or not
     * @param ylooped true if the graph wraps around the y-axis, be it reversed or not
     * @return
     */
    static BTCandidateFunction makeGridFunction(const int width, const int height,
                                                const bool xlooped, const bool ylooped) {
        return [width, height, xlooped, ylooped](vertex v) {
            const auto x = v % height;
            const auto y = v / height;
            std::set<types::Direction> dirs;

            if (xlooped || x < width - 1)  dirs.insert(types::Direction::EAST);
            if (ylooped || y < height - 1) dirs.insert(types::Direction::SOUTH);
            return dirs;
        };
    }

    MazeGraph GraphUtils::makeGrid(const int width, const int height) {
        return makeGrid(width, height, types::AxialOrientation::DISCONNECTED, types::AxialOrientation::DISCONNECTED);
    }

    MazeGraph GraphUtils::makeCylinder(const int width, const int height) {
        return makeGrid(width, height, types::AxialOrientation::LOOPED, types::AxialOrientation::DISCONNECTED);
    }

    MazeGraph GraphUtils::makeTorus(const int width, const int height) {
        return makeGrid(width, height, types::AxialOrientation::LOOPED, types::AxialOrientation::LOOPED);
    }

    MazeGraph GraphUtils::makeMobiusStrip(const int width, const int height) {
        return makeGrid(width, height, types::AxialOrientation::REVERSE_LOOPED, types::AxialOrientation::DISCONNECTED);
    }

    MazeGraph GraphUtils::makeKleinBottle(const int width, const int height) {
        return makeGrid(width, height, types::AxialOrientation::REVERSE_LOOPED, types::AxialOrientation::LOOPED);
    }

    MazeGraph GraphUtils::makeProjectivePlane(const int width, const int height) {
        return makeGrid(width, height, types::AxialOrientation::REVERSE_LOOPED, types::AxialOrientation::REVERSE_LOOPED);
    }

    MazeGraph GraphUtils::makeGrid(const int width, const int height,
                                   const types::AxialOrientation xorientation,
                                   const types::AxialOrientation yorientation) {

        MazeGraph g{GraphInfo{true, makeGridFunction(width, height,
                                                           xorientation != types::AxialOrientation::DISCONNECTED,
                                                           yorientation != types::AxialOrientation::DISCONNECTED)}};

        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x) {
                const auto v = ranker(x, y, width, height);
                const auto w = boost::add_vertex(g);
                assert(v == w);
            }

        // Now add all east and south edges.
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                // Check if we can add east.
                if (x < width - 1) {
                    const vertex v1 = ranker(x, y, width, height);
                    const vertex v2 = ranker(x + 1, y, width, height);
                    const EdgeInfo ei { v1, types::Direction::EAST, v2, types::Direction::WEST };
                    boost::add_edge(v1, v2, ei, g);
                }
                // Check if we can add south.
                if (y < height - 1) {
                    const vertex v1 = ranker(x, y, width, height);
                    const vertex v2 = ranker(x, y + 1, width, height);
                    const EdgeInfo ei { v1, types::Direction::SOUTH, v2, types::Direction::NORTH };
                    boost::add_edge(v1, v2, ei, g);
                }
            }
        }

        // If we are x-looped, add the additional edges.
        if (xorientation != types::AxialOrientation::DISCONNECTED) {
            bool flip = xorientation == types::AxialOrientation::REVERSE_LOOPED;
            for (auto y = 0; y < height; ++y) {
                const vertex v1 = ranker(0, y, width, height);
                const vertex v2 = ranker(width - 1, flip ? (height - y - 1) : y, width, height);
                const EdgeInfo ei { v1, types::Direction::WEST, v2, types::Direction::EAST };
                boost::add_edge(v1, v2, ei, g);
            }
        }

        // If we are y-looped, add the additional edges.
        if (yorientation != types::AxialOrientation::DISCONNECTED) {
            bool flip = yorientation == types::AxialOrientation::REVERSE_LOOPED;
            for (auto x = 0; x < width; ++x) {
                const vertex v1 = ranker(x, 0, width, height);
                const vertex v2 = ranker(flip ? (width - x - 1) : x, height - 1, width, height);
                const EdgeInfo ei { v1, types::Direction::NORTH, v2, types::Direction::SOUTH };
                boost::add_edge(v1, v2, ei, g);
            }
        }

        return g;
    }

    static BTCandidateFunction makeCircularFunction(const std::vector<int> &sizes) {
        // We always allow OUT and CLOCKWISE unless not possible.
        // Determine the number of cells not in the last row.
        int cellCount = std::accumulate(sizes.cbegin(), sizes.cend() - 1, 0);
        return [sizes, cellCount](vertex v) {
            std::set<types::Direction> dirs;
            if (v <= cellCount) dirs.insert(types::Direction::OUT);
            if (v > 0) dirs.insert(types::Direction::CLOCKWISE);
            return dirs;
        };
    }

    /**
     * For a circular graph, calculate the number of cells in each ring.
     * If we have rings i and i+1 with r_{i} and r_{i+1} cells in each, then
     * each cell in ring i has r_{i+1} / r_{i} children in ring i+1.
     * @param radius
     * @return vector of cells per ring
     */
    static std::vector<int> calculateRingSizes(const int radius) {
        const auto rowHeight = 1.0 / radius;
        std::vector<int> sizes { 1 };

        for (auto row = 1; row < radius; ++row) {
            // Determine the number of cells we want in this row. Their size should be as close as possible to the
            // number of cells in the previous row: we branch if it is not.
            const auto innerRadius = row * rowHeight;
            const auto innerCircumference = 2 * M_PI * innerRadius;
            const auto estimatedCellWidth = innerCircumference / sizes.back();
            const auto ratio = static_cast<int>(std::round(estimatedCellWidth / rowHeight));
            const auto cols = sizes.back() * ratio;
            sizes.emplace_back(cols);
        }

        return sizes;
    }

    MazeGraph GraphUtils::makeCircular(int radius) {
        // Calculate the ring sizes and create the binary tree function.
        const auto ringSizes = calculateRingSizes(radius);
        const auto circularFunction = makeCircularFunction(ringSizes);
        // A circular graph has more of a parent-sibling relationship when it comes to rank rather than the standard

        MazeGraph g{GraphInfo{false, circularFunction}};

        // We still want a map from row x column to vertex number, i.e. a ranking function, but more complex
        // (ha) than in the case of anything grid-like, so we just use a map.
        std::map<std::pair<int, int>, vertex> ranker;

        // Add the (0,0) vertex.
        ranker[std::make_pair(0,0)] = boost::add_vertex(g);

        for (auto row = 1; row < radius; ++row) {
            // Add the cells for this row.
            const int cols = ringSizes[row];
            for (int col = 0; col < cols; ++col)
                ranker[std::make_pair(row, col)] = boost::add_vertex(g);

            // Now add the adjacencies.
            // The ratio, which is integral, dictates the number of columns in this row per column of the previous
            // row, so we use this to dictate parental adjacency.
            const auto ratio = cols / ringSizes[row-1];

            for (int col = 0; col < cols; ++col) {
                const vertex v = ranker[std::make_pair(row, col)];

                // As we are undirected, we only need to connect to our clockwise neighbour.
                int nextCol = (col + 1) % cols;
                const vertex vn = ranker[std::make_pair(row, nextCol)];
                const EdgeInfo ein { v, types::Direction::CLOCKWISE, vn, types::Direction::COUNTERCLOCKWISE };
                boost::add_edge(v, vn, ein, g);

                // Connect to the parent.
                const vertex vp = ranker[std::make_pair(row - 1, col / ratio)];
                const EdgeInfo eip { v, types::Direction::IN, vp, types::Direction::OUT };
                boost::add_edge(v, vp, eip, g);
            }
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

    void GraphUtils::outputGraph(std::ostream &out, const MazeGraph &graph) {
        for (auto[viter, vend] = boost::vertices(graph); viter != vend; ++viter)
            std::cout << "Vertex " << *viter << std::endl;
        for (auto[eiter, eend] = boost::edges(graph); eiter != eend; ++eiter)
            std::cout << "Edge " << *eiter << std::endl;

        // These are perfect mazes, so |edges| = #vertices - 1.
        auto [ceIter, ceEnd] = boost::edges(graph);
        const auto numEdges = std::distance(ceIter, ceEnd);
        assert(numEdges == numVertices(graph) - 1);
    }

    vertex GraphUtils::randomStartVertex(const MazeGraph &maze) noexcept {
        return math::DefaultRNG::randomRange(numVertices(maze));
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

    void GraphUtils::addEdge(vertex v1, vertex v2, MazeSeed &seed) {
        // Get the edge properties from the template.
        auto edge = boost::edge(v1, v2, seed.tmplt);
        auto edgeInfo = boost::get(EdgeInfoPropertyTag(), seed.tmplt, edge.first);
        std::cout << "Adding edge!" << std::endl;
        boost::add_edge(v1, v2, edgeInfo, seed.maze);
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