/**
 * Tessellations.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

namespace spelunker::types {
    /**
     * Tessellation type, used for graph rendering.
     */
    enum class TessellationType {
        GRID,
        CIRCULAR,
        SPHERICAL,
        DELTA,
        OMEGA,
        OCTAGONAL
    };

    /**
     * Octagonal tessellation, with diamonds at diagonals to the octagons.
     * See: https://cdn.printablepaper.net/samples/octagon-portrait-letter-1.png
     */
    enum class OctagonalTessellation {
        OCTAGON = 0,
        DIAMOND = 1,
    };
}