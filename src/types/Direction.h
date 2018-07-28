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
     * The four possible directions in the mazes we consider.
     */
    enum class Direction {
        NORTH            =              0b1,
        EAST             =             0b10,
        SOUTH            =            0b100,
        WEST             =           0b1000,
        NORTHEAST        =          0b10000,
        SOUTHEAST        =         0b100000,
        SOUTHWEST        =        0b1000000,
        NORTHWEST        =       0b10000000,
        UP               =      0b100000000,
        DOWN             =     0b1000000000,
        // These are for circular mazes
        CLOCKWISE        =    0b10000000000,
        COUNTERCLOCKWISE =   0b100000000000,
        OUT              =  0b1000000000000,
        IN               = 0b10000000000000,
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
