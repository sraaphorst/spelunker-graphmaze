/**
 * GridTemplates.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <istream>
#include <string>
#include <tuple>
#include <vector>

#include "GridTemplates.h"
#include "MazeGraph.h"

namespace spelunker::graphmaze {

    MazeGraph GridTemplates::makeGrid(const int width, const int height) {
        return makeGrid(width, height, types::AxialOrientation::DISCONNECTED, types::AxialOrientation::DISCONNECTED);
    }

    MazeGraph GridTemplates::makeCylinder(const int width, const int height) {
        return makeGrid(width, height, types::AxialOrientation::LOOPED, types::AxialOrientation::DISCONNECTED);
    }

    MazeGraph GridTemplates::makeTorus(const int width, const int height) {
        return makeGrid(width, height, types::AxialOrientation::LOOPED, types::AxialOrientation::LOOPED);
    }

    MazeGraph GridTemplates::makeMobiusStrip(const int width, const int height) {
        return makeGrid(width, height, types::AxialOrientation::REVERSE_LOOPED, types::AxialOrientation::DISCONNECTED);
    }

    MazeGraph GridTemplates::makeKleinBottle(const int width, const int height) {
        return makeGrid(width, height, types::AxialOrientation::REVERSE_LOOPED, types::AxialOrientation::LOOPED);
    }

    MazeGraph GridTemplates::makeProjectivePlane(const int width, const int height) {
        return makeGrid(width, height, types::AxialOrientation::REVERSE_LOOPED, types::AxialOrientation::REVERSE_LOOPED);
    }

    MazeGraph GridTemplates::makeGridFromMask(std::istream &str) {
        auto [g, width, height, ranker] = makeOrthogonalFromMaskGraph(str);
        boost::set_property(g, GraphInfoPropertyTag(), makeGraphInfo(width, height, ranker));
    }

    MazeGraph GridTemplates::makeGrid(const int width, const int height,
                                      const types::AxialOrientation xorientation,
                                      const types::AxialOrientation yorientation) {
        auto [g, ranker] = makeOrthogonalGraph(width, height, xorientation, yorientation);
        boost::set_property(g, GraphInfoPropertyTag(), makeGraphInfo(width, height, ranker));
    }

    GraphInfo makeGraphInfo(int width, int height, GridRankerMap &grm) {
        GraphInfo gi;
        gi.width = width;
        gi.height = height;
        gi.type = types::TessellationType::ORTHOGONAL;
        gi.binaryTreeCandidates = [](int) {
            return std::deque<types::Direction>{types::Direction::EAST, types::Direction::SOUTH};
        };
        gi.gridRankerMaps = {grm};
        return gi;
    }
}