/**
 * GraphUtils.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

// For M_PI.
#define _USE_MATH_DEFINES

#include <algorithm>
#include <cmath>
#include <deque>
#include <iostream>
#include <tuple>
#include <vector>

#include <math/DefaultRNG.h>
#include <types/AxialOrientation.h>
#include <types/Exceptions.h>

#include "MazeGraph.h"
#include "GraphUtils.h"

namespace spelunker::graphmaze {

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

    MazeGraph GraphUtils::makeGridFromMask(std::istream &str) {
        // Create a grid of booleans indicating if cells exist or not.
        std::vector<std::vector<bool>> cells;

        std::string line;
        while (std::getline(str, line)) {
            std::vector<bool> row;
            std::for_each(line.begin(), line.end(), [&row](char c) {
               row.emplace_back(c != ' ');
            });
            cells.emplace_back(row);
        }

        // Remove heading and trailing rows of length 0.
        while (cells.back().empty())
            cells.pop_back();

        // We want all rows to be the same length.
        auto maxwidth = std::max_element(cells.cbegin(), cells.cend(),
                [](const auto &r, const auto &s) { return r.size() < s.size(); })->size();
        std::for_each(cells.begin(), cells.end(), [maxwidth](auto &r) { r.resize(maxwidth, false); });

        const auto binaryTreeFunc = [](int) {
            return std::deque<types::Direction>{ types::Direction::EAST, types::Direction::SOUTH };
        };
        MazeGraph g{GraphInfo{true, binaryTreeFunc, {}}};

        GridRankerMap ranker;
        for (auto y = 0; y < cells.size(); ++y) {
            const auto row = cells[y];
            for (auto x = 0; x < row.size(); ++x) {
                if (!row[x]) continue;

                const auto v = boost::add_vertex(g);
                ranker[{x,y}] = v;

                // Create any WEST and NORTH edges.
                if (x > 0 && row[x-1]) {
                    const auto vw = ranker[{x-1, y}];
                    EdgeInfo ei{v, types::Direction::WEST, vw, types::Direction::EAST};
                    boost::add_edge(v, vw, ei, g);
                }

                if (y > 0 && cells[y-1][x]) {
                    const auto vn = ranker[{x, y-1}];
                    EdgeInfo ei{v, types::Direction::NORTH, vn, types::Direction::SOUTH};
                    boost::add_edge(v, vn, ei, g);
                }
            }
        }

        g.m_property.get()->m_value.gridRankerMap = ranker;
        return g;
    }

    MazeGraph GraphUtils::makeCircular(int radius) {
        // Calculate the ring sizes and create the binary tree function, which
        // always allows us to carve OUT and CLOCKWISE.
        const auto ringSizes = calculateRingSizes(radius);
        const auto circularFunction = [](int) {
            return std::deque<types::Direction>{types::Direction::CLOCKWISE, types::Direction::OUT};
        };

        MazeGraph g{GraphInfo{false, circularFunction, {}}};

        // We still want a map from row x column to vertex number, i.e. a ranking function, but more complex
        // (ha) than in the case of anything grid-like, so we just use a map.
        GridRankerMap ranker;

        // Add the (0,0) vertex.
        ranker[{0, 0}] = boost::add_vertex(g);

        for (auto row = 1; row < radius; ++row) {
            // Add the cells for this row.
            const int cols = ringSizes[row];
            for (int col = 0; col < cols; ++col)
                ranker[{row, col}] = boost::add_vertex(g);

            // Now add the adjacencies.
            // The ratio, which is integral, dictates the number of columns in this row per column of the previous
            // row, so we use this to dictate parental adjacency.
            const auto ratio = cols / ringSizes[row - 1];

            for (int col = 0; col < cols; ++col) {
                const vertex v = ranker[{row, col}];

                // As we are undirected, we only need to connect to our clockwise neighbour.
                int nextCol = (col + 1) % cols;
                const vertex vn = ranker[{row, nextCol}];
                const EdgeInfo ein { v, types::Direction::CLOCKWISE, vn, types::Direction::COUNTERCLOCKWISE };
                boost::add_edge(v, vn, ein, g);

                // Connect to the parent.
                const vertex vp = ranker[{row - 1, col / ratio}];
                const EdgeInfo eip { v, types::Direction::IN, vp, types::Direction::OUT };
                boost::add_edge(v, vp, eip, g);
            }
        }
        g.m_property.get()->m_value.gridRankerMap = ranker;
        return g;
    }

    /**
     * We could base this on makeCircular by joining two circular halves together, but:
     * 1. We want NSEW directions instead of IN / OUT / CLOCKWISE / COUNTERCLOCKWISE directions;
     * 2. We must handle the equator differently; and
     * 3. We want vertex numbers to be strictly increasing from north pole to south pole.
     * Thus, we recode this to achieve these specifications, despite the repetitiveness.
     */
    MazeGraph GraphUtils::makeSpherical(int diameter) {
        // Calculate the ring sizes. If radius is odd, then we want to include the equator in this calculation.
        const auto northernRows = diameter / 2 + diameter % 2;
        const auto ringSizes = calculateRingSizes(northernRows);

        // The binary tree spherical function allows us to always carve east and south.
        const auto sphericalFunction = [](int) {
            return std::deque<types::Direction>{types::Direction::EAST, types::Direction::SOUTH};
        };

        MazeGraph g{GraphInfo{false, sphericalFunction, {}}};

        // We still want a map from row x column to vertex number, i.e. a ranking function, but more complex
        // (ha) than in the case of anything grid-like, so we just use a map.
        GridRankerMap ranker;

        // Create the two halves of the sphere separately.
        // This will give us a nicer ordering on the vertices: they will continuously increase from
        // north pole to south pole.

        // Begin by adding the north pole.
        ranker[{0, 0}] = boost::add_vertex(g);

        // Now create the northern hemisphere, and if radius is odd, the equator.
        for (auto row = 1; row < northernRows; ++row) {
            // Add the cells for this ring of latitude.
            const int cols = ringSizes[row];
            for (int col = 0; col < cols; ++col)
                ranker[{row, col}] = boost::add_vertex(g);

            // Now add the adjacencies.
            // The ratio, as in the case of the circular maze, indicates the northern parent.
            const auto ratio = cols / ringSizes[row - 1];

            for (int col = 0; col < cols; ++col) {
                const vertex v = ranker[{row, col}];

                // Link east.
                int nextCol = (col + 1) % cols;
                const vertex ve = ranker[{row, nextCol}];
                const EdgeInfo eie { v, types::Direction::EAST, ve, types::Direction::WEST };
                boost::add_edge(v, ve, eie, g);

                // Link north.
                const vertex vn = ranker[{row - 1, col / ratio}];
                const EdgeInfo ein { v, types::Direction::NORTH, vn, types::Direction::SOUTH };
                boost::add_edge(v, vn, ein, g);
            }
        }

        // Now create the southern hemisphere, including the south pole.
        int curRow = ringSizes.size();
        int prevCols = ringSizes.back();
        for (int southRow = ringSizes.size() - 1 - diameter % 2; southRow >= 0; --southRow) {
            // Add the cells for this ring of latitude.
            const int cols = ringSizes[southRow];
            for (int col = 0; col < cols; ++col)
                ranker[{curRow, col}] = boost::add_vertex(g);

            // Now add the adjacencies.
            // The ratio will dictate how many cells in the north latitude correspond to each in this one.
            // This row will have the same or fewer.
            const auto ratio = prevCols / cols;

            int prevCt = 0;
            for (int col = 0; col < cols; ++col) {
                const vertex v = ranker[{curRow, col}];

                // Link east if we aren't at the south pole.
                if (southRow > 0) {
                    int nextCol = (col + 1) % cols;
                    const vertex ve = ranker[{curRow, nextCol}];
                    const EdgeInfo eie{v, types::Direction::EAST, ve, types::Direction::WEST};
                    boost::add_edge(v, ve, eie, g);
                }

                // Link north.
                for (int i = 0; i < ratio; ++i) {
                    const vertex vn = ranker[{curRow - 1, prevCt}];
                    const EdgeInfo ein { v, types::Direction::NORTH, vn, types::Direction::SOUTH };
                    boost::add_edge(v, vn, ein, g);
                    ++prevCt;
                }
            }
            assert(prevCt == prevCols);

            prevCols = cols;
            ++curRow;
        }
        assert(curRow == diameter);

        g.m_property.get()->m_value.gridRankerMap = ranker;
        return g;
    }

    MazeGraph GraphUtils::makeOctogonal(int width, int height,
            types::AxialOrientation xorientation, types::AxialOrientation yorientation) {
        // In the binary tree func, 0 is an octagon, and 1 is a diamond.
        const auto binaryTreeFunc = [](int type) {
            switch (type) {
                case 0:
                    return std::deque<types::Direction>{ types::Direction::EAST, types::Direction::SOUTH, types::Direction::SOUTHEAST};
                case 1:
                    return std::deque<types::Direction>{ types::Direction::SOUTHEAST };
            }
        };

        MazeGraph g{GraphInfo{true, binaryTreeFunc, {}}};
        GridRankerMap octagonalRanker;
        GridRankerMap diamondRanker;

        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x) {
                VertexInfo vi { 0 };
                octagonalRanker[{x, y}] = boost::add_vertex(vi, g);
            }

        const auto diamondWidth = width - (xorientation == types::AxialOrientation::DISCONNECTED ? 1 : 0);
        const auto diamondHeight = height - (yorientation == types::AxialOrientation::DISCONNECTED ? 1 : 0);
        for (auto y = 0; y < diamondWidth; ++y)
            for (auto x = 0; x < diamondHeight; ++x) {
                VertexInfo vi { 1 };
                diamondRanker[{x, y}] = boost::add_vertex(vi, g);
            }

        // Add all the octagonal - octagonal edges.
        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x) {
                const auto v1 = octagonalRanker[{x, y}];

                // NORTH
                if (y > 0 || yorientation != types::AxialOrientation::DISCONNECTED) {
                    vertex v2;
                    if (y - 1 >= 0) v2 = octagonalRanker[{x, y - 1}];
                    else v2 = octagonalRanker[{yorientation == types::AxialOrientation::LOOPED ? x : width - x - 1, height - 1}];
                    EdgeInfo ei{v1, types::Direction::NORTH, v2, types::Direction::SOUTH};
                    boost::add_edge(v1, v2, ei, g);
                }

                // SOUTH
                if (y < width || yorientation != types::AxialOrientation::DISCONNECTED) {
                    vertex v2;
                    if (y + 1 < height) v2 = octagonalRanker[{x, y + 1}];
                    else v2 = octagonalRanker[{yorientation == types::AxialOrientation::LOOPED ? x : width - x - 1, 0}];
                    EdgeInfo ei{v1, types::Direction::SOUTH, v2, types::Direction::NORTH};
                    boost::add_edge(v1, v2, ei, g);
                }

                // EAST
                if (x < width - 1 || xorientation != types::AxialOrientation::DISCONNECTED) {
                    vertex v2;
                    if (x + 1 < width) v2 = octagonalRanker[{x + 1, y}];
                    else v2 = octagonalRanker[{0, xorientation == types::AxialOrientation::LOOPED ? y : height - y - 1}];
                    EdgeInfo ei{v1, types::Direction::EAST, v2, types::Direction::WEST};
                    boost::add_edge(v1, v2, ei, g);
                }

                // WEST
                if (x > 0 || xorientation != types::AxialOrientation::DISCONNECTED) {
                    vertex v2;
                    if (x - 1 >= 0) v2 = octagonalRanker[{x - 1, y}];
                    else v2 = octagonalRanker[{width - 1, xorientation == types::AxialOrientation::LOOPED ? y : height - y - 1}];
                    EdgeInfo ei{v1, types::Direction::WEST, v2, types::Direction::EAST};
                    boost::add_edge(v1, v2, ei, g);
                }
            }

        // Add all the diamond - octagonal edges.
        // This is slightly more complicated, because:
        // 1. We can always add the edges; and
        // 2. The width and height of the diamonds change depending on the axial orientation, with an extra row on
        //    the east and the south if not disconnected.
        for (auto y = 0; y < diamondHeight; ++y)
            for (auto x = 0; x < diamondWidth; ++x) {
                const auto v1 = diamondRanker[{x, y}];

                // NORTHWEST
                const auto vnw = octagonalRanker[{x, y}];
                EdgeInfo einw{ v1, types::Direction::NORTHWEST, vnw, types::Direction::SOUTHEAST};

                // NORTHEAST
                vertex vne;
                if (x + 1 == diamondWidth && xorientation != types::AxialOrientation::DISCONNECTED)
                    vne = octagonalRanker[{0, xorientation == types::AxialOrientation::LOOPED ? y : height - y - 1}];
                else vne = octagonalRanker[{x + 1, y}];
                EdgeInfo eine{v1, types::Direction::NORTHEAST, vne, types::Direction::SOUTHWEST};

                // SOUTHWEST
                vertex vsw;
                if (y + 1 == diamondHeight && yorientation != types::AxialOrientation::DISCONNECTED)
                    vsw = octagonalRanker[{yorientation == types::AxialOrientation::LOOPED ? x : width - x - 1, y}];
            }
    }

    MazeSeed GraphUtils::makeSeed(const MazeGraph &tmplt) noexcept {
        return MazeSeed {
            tmplt,
            createInitialMaze(tmplt),
            numVertices(tmplt),
            initializeUnvisitedVertices(tmplt)
        };
    }

    vertex GraphUtils::randomStartVertex(const MazeGraph &maze) noexcept {
        return math::DefaultRNG::randomRange(numVertices(maze));
    }

    VertexCollection GraphUtils::unvisitedNeighbours(const MazeSeed &seed, const vertex &v) {
        return nbrs(seed, v, false);
    }

    VertexCollection GraphUtils::visitedNeighbours(const MazeSeed &seed, const vertex &v) {
        return nbrs(seed, v, true);
    }

    VertexCollection GraphUtils::neighbours(const MazeSeed &seed, const vertex &v) {
        VertexCollection neighbours;
        for (auto [eIter, eEnd] = boost::out_edges(v, seed.tmplt); eIter != eEnd; ++eIter) {
            const auto t = boost::target(*eIter, seed.tmplt);
            neighbours.emplace_back(t);
        }
        return neighbours;
    }

    void GraphUtils::addEdge(vertex v1, vertex v2, MazeSeed &seed) {
        // Get the edge properties from the template.
        auto edge = boost::edge(v1, v2, seed.tmplt);
        auto edgeInfo = boost::get(EdgeInfoPropertyTag(), seed.tmplt, edge.first);
        std::cout << "Adding edge: " << edgeInfo.v1 << " " << types::directionShortName(edgeInfo.d1) << " " << edgeInfo.v2 << " " << types::directionShortName(edgeInfo.d2) << std::endl;
        boost::add_edge(v1, v2, edgeInfo, seed.maze);
    }

    void GraphUtils::outputGraph(std::ostream &out, const MazeGraph &graph) {
        for (auto [eIter, eEnd] = boost::edges(graph); eIter != eEnd; ++eIter) {
            //const auto &ei = boost::get(EdgeInfoPropertyTag(), graph, *eIter);
            //std::cout << "Edge " << *eIter << ", " << types::directionShortName(ei.d1) << std::endl;
            std::cout << "Edge " << *eIter << std::endl;
        }

        // These are perfect mazes, so we should have |edges| = #vertices - 1.
        const auto v = numVertices(graph);

        auto [ceIter, ceEnd] = boost::edges(graph);
        const auto e = std::distance(ceIter, ceEnd);

        std::cout << v << " vertices, " << e << " edges" << std::endl;
        assert(e == v - 1);
    }

    std::optional<BTCandidateFunction> GraphUtils::getCandidateFunction(const MazeSeed &seed) {
        // TODO: There has to be a nicer way to access this, but I don't know what it is.
        return seed.tmplt.m_property.get()->m_value.binaryTreeCandidates;
    }

    VertexCollection GraphUtils::nbrs(const MazeSeed &seed, const vertex &v, const bool visited) {
        VertexCollection vc;
        for (auto [eIter, eEnd] = boost::out_edges(v, seed.tmplt); eIter != eEnd; ++eIter) {
            const vertex t = boost::target(*eIter, seed.tmplt);
            if (seed.unvisited.at(t) != visited)
                vc.emplace_back(t);
        }
        return vc;
    }

    std::vector<int> GraphUtils::calculateRingSizes(const int radius) {
        if (radius <= 0)
            throw std::invalid_argument("Radius must be positive.");

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

    MazeGraph GraphUtils::makeGrid(const int width, const int height,
                                   const types::AxialOrientation xorientation,
                                   const types::AxialOrientation yorientation) {
        const auto binaryTreeFunc = [](int) {
            return std::deque<types::Direction>{ types::Direction::EAST, types::Direction::SOUTH };
        };
        MazeGraph g{GraphInfo{true, binaryTreeFunc, {}}};

        GridRankerMap ranker;
        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x)
                ranker[{x, y}] = boost::add_vertex(g);

        // Now add all east and south edges.
        for (auto y = 0; y < height; ++y) {
            for (auto x = 0; x < width; ++x) {
                // Check if we can add east.
                if (x < width - 1) {
                    const vertex v1 = ranker[{x, y}];
                    const vertex v2 = ranker[{x + 1, y}];
                    const EdgeInfo ei { v1, types::Direction::EAST, v2, types::Direction::WEST };
                    boost::add_edge(v1, v2, ei, g);
                }
                // Check if we can add south.
                if (y < height - 1) {
                    const vertex v1 = ranker[{x, y}];
                    const vertex v2 = ranker[{x, y + 1}];
                    const EdgeInfo ei { v1, types::Direction::SOUTH, v2, types::Direction::NORTH };
                    boost::add_edge(v1, v2, ei, g);
                }
            }
        }

        // If we are x-looped, add the additional edges.
        if (xorientation != types::AxialOrientation::DISCONNECTED) {
            bool flip = xorientation == types::AxialOrientation::REVERSE_LOOPED;
            for (auto y = 0; y < height; ++y) {
                const vertex v1 = ranker[{0, y}];
                const vertex v2 = ranker[{width - 1, flip ? (height - y - 1) : y}];
                const EdgeInfo ei { v1, types::Direction::WEST, v2, types::Direction::EAST };
                boost::add_edge(v1, v2, ei, g);
            }
        }

        // If we are y-looped, add the additional edges.
        if (yorientation != types::AxialOrientation::DISCONNECTED) {
            bool flip = yorientation == types::AxialOrientation::REVERSE_LOOPED;
            for (auto x = 0; x < width; ++x) {
                const vertex v1 = ranker[{x, 0}];
                const vertex v2 = ranker[{flip ? (width - x - 1) : x, height - 1}];
                const EdgeInfo ei { v1, types::Direction::NORTH, v2, types::Direction::SOUTH };
                boost::add_edge(v1, v2, ei, g);
            }
        }
        g.m_property.get()->m_value.gridRankerMap = ranker;
        return g;
    }

    MazeGraph GraphUtils::createInitialMaze(const MazeGraph &tmplt) noexcept {
        MazeGraph out;
        for (auto[vIter, vEnd] = boost::vertices(tmplt); vIter != vEnd; ++vIter)
            boost::add_vertex(out);
        out.m_property.get()->m_value.gridRankerMap = tmplt.m_property.get()->m_value.gridRankerMap;
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