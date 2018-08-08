/**
 * CircularTemplates.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/graph/adjacency_list.hpp>

#include <types/Direction.h>
#include <types/Tessellations.h>

#include "MazeGraph.h"
#include "CircularTemplates.h"

namespace spelunker::graphmaze {

    MazeGraph CircularTemplates::makeCircular(int radius) {
        MazeGraph g;

        // Calculate the ring sizes and create the binary tree function, which
        // always allows us to carve OUT and CLOCKWISE.
        const auto ringSizes = calculateRingSizes(radius);

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

        GraphInfo gi;
        gi.width = ringSizes.back();
        gi.height = radius;
        gi.type = types::TessellationType::CIRCULAR;
        gi.binaryTreeCandidates = [](int) {
            return std::deque<types::Direction>{types::Direction::CLOCKWISE, types::Direction::OUT};
        };
        gi.gridRankerMaps = {ranker};
        boost::set_property(g, GraphInfoPropertyTag(), gi);
        return g;
    }
}