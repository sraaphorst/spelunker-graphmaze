/**
 * OrthogonalTemplates.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <algorithm>
#include <tuple>

#include <boost/graph/adjacency_list.hpp>

#include <types/AxialOrientation.h>
#include <types/Direction.h>

#include "MazeGraph.h"
#include "OrthogonalTemplates.h"

namespace spelunker::graphmaze {

    std::tuple<MazeGraph, int, int, GridRankerMap> OrthogonalTemplates::makeOrthogonalFromMaskGraph(std::istream &str) {
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

        MazeGraph g;

        GridRankerMap ranker;
        for (auto y = 0; y < cells.size(); ++y) {
            const auto row = cells[y];
            for (auto x = 0; x < row.size(); ++x) {
                if (!row[x]) continue;

                const auto v = boost::add_vertex(g);
                ranker[{x, y}] = v;

                // Create any WEST and NORTH edges.
                if (x > 0 && row[x - 1]) {
                    const auto vw = ranker[{x - 1, y}];
                    EdgeInfo ei{v, types::Direction::WEST, vw, types::Direction::EAST};
                    boost::add_edge(v, vw, ei, g);
                }

                if (y > 0 && cells[y - 1][x]) {
                    const auto vn = ranker[{x, y - 1}];
                    EdgeInfo ei{v, types::Direction::NORTH, vn, types::Direction::SOUTH};
                    boost::add_edge(v, vn, ei, g);
                }
            }
        }

        return g;
    }

    std::pair<MazeGraph, GridRankerMap> OrthogonalTemplates::makeOrthogonalGraph(int width, int height,
                                                                                 types::AxialOrientation xorientation,
                                                                                 types::AxialOrientation yorientation) {
        MazeGraph g;

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
                    const EdgeInfo ei{v1, types::Direction::EAST, v2, types::Direction::WEST};
                    boost::add_edge(v1, v2, ei, g);
                }
                // Check if we can add south.
                if (y < height - 1) {
                    const vertex v1 = ranker[{x, y}];
                    const vertex v2 = ranker[{x, y + 1}];
                    const EdgeInfo ei{v1, types::Direction::SOUTH, v2, types::Direction::NORTH};
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
                const EdgeInfo ei{v1, types::Direction::WEST, v2, types::Direction::EAST};
                boost::add_edge(v1, v2, ei, g);
            }
        }

        // If we are y-looped, add the additional edges.
        if (yorientation != types::AxialOrientation::DISCONNECTED) {
            bool flip = yorientation == types::AxialOrientation::REVERSE_LOOPED;
            for (auto x = 0; x < width; ++x) {
                const vertex v1 = ranker[{x, 0}];
                const vertex v2 = ranker[{flip ? (width - x - 1) : x, height - 1}];
                const EdgeInfo ei{v1, types::Direction::NORTH, v2, types::Direction::SOUTH};
                boost::add_edge(v1, v2, ei, g);
            }
        }

        return {g, ranker};
    }
}