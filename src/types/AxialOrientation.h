/**
 * Orientation.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

namespace spelunker::types {
    /**
     * This determines the orientation of an axis for various structures, particularly grids.
     * For example, a grid with one axis having an orientation of LOOPED results in a cylinder.
     * @see{GraphUtils} for more information.
     */
    enum class AxialOrientation {
        REVERSE_LOOPED = -1,
        DISCONNECTED = 0,
        LOOPED = 1,
    };
}