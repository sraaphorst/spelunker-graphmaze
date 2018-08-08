/**
 * UpsilonTemplates.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <boost/graph/adjacency_list.hpp>

#include <types/AxialOrientation.h>
#include <types/Direction.h>

#include "MazeGraph.h"
#include "UpsilonTemplates.h"

namespace spelunker::graphmaze {

    MazeGraph UpsilonTemplates::makeUpsilonGrid(int width, int height) {
        return makeUpsilonGrid(width, height, types::AxialOrientation::DISCONNECTED,
                                 types::AxialOrientation::DISCONNECTED);
    }

    MazeGraph UpsilonTemplates::makeUpsilonCylinder(int width, int height) {
        return makeUpsilonGrid(width, height, types::AxialOrientation::LOOPED, types::AxialOrientation::DISCONNECTED);
    }

    MazeGraph UpsilonTemplates::makeUpsilonTorus(int width, int height) {
        return makeUpsilonGrid(width, height, types::AxialOrientation::LOOPED, types::AxialOrientation::LOOPED);
    }

    MazeGraph UpsilonTemplates::makeUpsilonGrid(int width, int height,
                                                types::AxialOrientation xorientation,
                                                types::AxialOrientation yorientation) {
        // We don't support reverse-looping.
        if (xorientation == types::AxialOrientation::REVERSE_LOOPED
            || yorientation == types::AxialOrientation::REVERSE_LOOPED)
            throw types::UnsupportedTemplateGeneration{};

        MazeGraph g;
        GridRankerMap octagonalRanker;
        GridRankerMap diamondRanker;

        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x) {
                VertexInfo vi{0};
                octagonalRanker[{x, y}] = boost::add_vertex(vi, g);
            }

        const auto diamondWidth = width - (xorientation == types::AxialOrientation::DISCONNECTED ? 1 : 0);
        const auto diamondHeight = height - (yorientation == types::AxialOrientation::DISCONNECTED ? 1 : 0);
        for (auto y = 0; y < diamondWidth; ++y)
            for (auto x = 0; x < diamondHeight; ++x) {
                VertexInfo vi{1};
                diamondRanker[{x, y}] = boost::add_vertex(vi, g);
            }

        // Add all the octagonal - octagonal edges.
        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x) {
                const auto v = octagonalRanker[{x, y}];

                // SOUTH
                if (y + 1 < height || yorientation == types::AxialOrientation::LOOPED) {
                    const auto vs = octagonalRanker[{x, y + 1 < height ? y + 1 : 0}];
                    EdgeInfo ei{v, types::Direction::SOUTH, vs, types::Direction::NORTH};
                    boost::add_edge(v, vs, ei, g);
                }

                // EAST
                if (x + 1 < width || xorientation == types::AxialOrientation::LOOPED) {
                    const auto ve = octagonalRanker[{x + 1 < width ? x + 1 : 0, y}];
                    EdgeInfo ei{v, types::Direction::EAST, ve, types::Direction::WEST};
                    boost::add_edge(v, ve, ei, g);
                }
            }

        // Add all the diamond - octagonal edges.
        // This is slightly more complicated, because:
        // 1. We can always add the edges; and
        // 2. The width and height of the diamonds change depending on the axial orientation, with an extra row on
        //    the east and the south if looped.
        for (auto y = 0; y < diamondHeight; ++y)
            for (auto x = 0; x < diamondWidth; ++x) {
                const auto v = diamondRanker[{x, y}];

                const auto xe = (x + 1 == diamondWidth && xorientation == types::AxialOrientation::LOOPED) ? 0 : x + 1;
                const auto ys = (y + 1 == diamondHeight && yorientation == types::AxialOrientation::LOOPED) ? 0 : y + 1;

                // NORTHWEST: never need to loop.
                const auto vnw = octagonalRanker[{x, y}];
                EdgeInfo einw{v, types::Direction::NORTHWEST, vnw, types::Direction::SOUTHEAST};
                boost::add_edge(v, vnw, einw, g);

                // NORTHEAST: need to loop if on east boundary.
                const auto vne = octagonalRanker[{xe, y}];
                EdgeInfo eine{v, types::Direction::NORTHEAST, vne, types::Direction::SOUTHWEST};
                boost::add_edge(v, vne, eine, g);

                // SOUTHWEST: need to loop if on south boundary.
                const auto vsw = octagonalRanker[{x, ys}];
                EdgeInfo eisw{v, types::Direction::SOUTHWEST, vsw, types::Direction::NORTHEAST};
                boost::add_edge(v, vsw, eisw, g);

                // SOUTHEAST: need to loop if on south and / or east boundary.
                const auto vse = octagonalRanker[{xe, ys}];
                EdgeInfo eise{v, types::Direction::SOUTHEAST, vse, types::Direction::NORTHWEST};
            }

        // Merge the two rankers.
        GridRankerMap ranker;
        for (auto y = 0; y < height; ++y)
            for (auto x = 0; x < width; ++x)
                ranker[{x, y}] = octagonalRanker[{x, y}];
        for (auto y = 0; y < diamondHeight; ++y)
            for (auto x = 0; x < diamondWidth; ++x)
                ranker[{x + width, y + height}] = diamondRanker[{x, y}];

        GraphInfo gi;
        gi.width = width;
        gi.height = height;
        gi.type = types::TessellationType::UPSILON;
        gi.binaryTreeCandidates = [](int type) {
            switch (type) {
                case 0:
                    return std::deque<types::Direction>{types::Direction::EAST, types::Direction::SOUTHEAST};
                case 1:
                    return std::deque<types::Direction>{types::Direction::SOUTHEAST};
                default:
                    return std::deque<types::Direction>{};
            }
        };
        gi.gridRankerMaps = {octagonalRanker, diamondRanker};
        boost::set_property(g, GraphInfoPropertyTag(), gi);
        return g;
    }
};