/**
 * Direction.h
 *
 * By Sebastian Raaphorst, 2018.
 */

#pragma once

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/version.hpp>

#include <string>
#include <vector>

#include <typeclasses/Show.h>

namespace spelunker::types {
    /**
     * The list of possible directions we consider.
     * The directions are primarily used in binary tree, sidewinder, and Eller's maze.
     */
    enum class Direction {
        // These are for grid and spherical mazes.
        NORTH,
        EAST,
        SOUTH,
        WEST,
        NORTHEAST,
        SOUTHEAST,
        SOUTHWEST,
        NORTHWEST,
        UP,
        DOWN,
        // These are for circular mazes
        CLOCKWISE,
        COUNTERCLOCKWISE,
        OUT,
        IN,

    };

    /// Function to serialize a Direction.
    template <typename Archive>
    void serialize(Archive &ar, Direction &d, const unsigned int version) {
        ar & d;
    }

    /**
     * Given a direction, return its opposite.
     * @param d the direction
     * @return the opposite direction
     */
    Direction flip(Direction d);

    /// A list of all Directions for iteration.
    const std::vector<Direction> directions();

    /// Get a shorthand representation of a direction.
    std::string directionShortName(Direction d);

    /// Get a string (all in lowercase) of a direction.
    std::string directionName(Direction d);

    /// Get as an offset for array manipulation.
    inline unsigned int dirIdx(const Direction &d) {
        return static_cast<unsigned int>(d);
    }
}

BOOST_CLASS_VERSION(spelunker::types::Direction, 1)


namespace spelunker::typeclasses {
    template<>
    struct Show<types::Direction> {
        static std::string show(const types::Direction d) {
            return types::directionName(d);
        }
        static constexpr bool is_instance = true;
        using type = types::Direction;
    };
}
