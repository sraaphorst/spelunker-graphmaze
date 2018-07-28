/**
 * MazeGraph.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include "MazeGraph.h"

namespace spelunker::graphmaze {
    const std::size_t VertexInfoPropertyTag::num = (std::size_t)&VertexInfoPropertyTag::num;
    const std::size_t EdgeInfoPropertyTag::num = (std::size_t)&EdgeInfoPropertyTag::num;
    const std::size_t GraphInfoPropertyTag::num = (std::size_t)&GraphInfoPropertyTag::num;
}