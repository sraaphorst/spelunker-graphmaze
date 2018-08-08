/**
 * SphericalTemplates.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/graph/adjacency_list.hpp>

#include "MazeGraph.h"
#include "SphericalTemplates.h"

namespace spelunker::graphmaze {

    /**
     * We could base this on makeCircular by joining two circular halves together, but:
     * 1. We want NSEW directions instead of IN / OUT / CLOCKWISE / COUNTERCLOCKWISE directions;
     * 2. We must handle the equator differently; and
     * 3. We want vertex numbers to be strictly increasing from north pole to south pole.
     * Thus, we recode this to achieve these specifications, despite the repetitiveness.
     */
    MazeGraph SphericalTemplates::makeSpherical(int diameter) {
        MazeGraph g;

        // Calculate the ring sizes. If radius is odd, then we want to include the equator in this calculation.
        const auto northernRows = diameter / 2 + diameter % 2;
        const auto ringSizes = calculateRingSizes(northernRows);

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

        GraphInfo gi;
        gi.width = ringSizes.back();
        gi.height = diameter;
        gi.type = types::TessellationType::SPHERICAL;
        gi.binaryTreeCandidates = [](int) {
            return std::deque<types::Direction>{types::Direction::EAST, types::Direction::SOUTH};
        };
        gi.gridRankerMaps = {ranker};
        boost::set_property(g, GraphInfoPropertyTag(), gi);
        return g;
    }
}