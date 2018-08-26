/**
 * Direction.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

#include <string>
#include <vector>

#include "Direction.h"

namespace spelunker::types {
    Direction flip(const Direction d) {
        switch (d) {
            case Direction::NORTH:
                return Direction::SOUTH;
            case Direction::EAST:
                return Direction::WEST;
            case Direction::SOUTH:
                return Direction::NORTH;
            case Direction::WEST:
                return Direction::EAST;
            case Direction::NORTHEAST:
                return Direction::SOUTHWEST;
            case Direction::SOUTHEAST:
                return Direction::NORTHWEST;
            case Direction::SOUTHWEST:
                return Direction::NORTHEAST;
            case Direction ::NORTHWEST:
                return Direction::SOUTHEAST;
            case Direction::UP:
                return Direction::DOWN;
            case Direction::DOWN:
                return Direction::UP;
            case Direction::CLOCKWISE:
                return Direction::COUNTERCLOCKWISE;
            case Direction::COUNTERCLOCKWISE:
                return Direction::CLOCKWISE;
            case Direction::OUT:
                return Direction::IN;
            case Direction::IN:
                return Direction::OUT;
        }
    }

    const std::vector<Direction> directions() {
        return std::vector<Direction>{Direction::NORTH,
                                      Direction::EAST,
                                      Direction::SOUTH,
                                      Direction::WEST,
                                      Direction::NORTHEAST,
                                      Direction::SOUTHEAST,
                                      Direction::SOUTHWEST,
                                      Direction::NORTHWEST,
                                      Direction::UP,
                                      Direction::DOWN,
                                      Direction::CLOCKWISE,
                                      Direction::COUNTERCLOCKWISE,
                                      Direction::OUT,
                                      Direction::IN
        };
    }

    std::string directionShortName(Direction d) {
        switch (d) {
            case Direction::NORTH:
                return "N";
            case Direction::EAST:
                return "E";
            case Direction::SOUTH:
                return "S";
            case Direction::WEST:
                return "W";
            case Direction::NORTHEAST:
                return "NE";
            case Direction::SOUTHEAST:
                return "SE";
            case Direction::SOUTHWEST:
                return "SW";
            case Direction::NORTHWEST:
                return "NW";
            case Direction::UP:
                return "U";
            case Direction::DOWN:
                return "D";
            case Direction::CLOCKWISE:
                return "C";
            case Direction::COUNTERCLOCKWISE:
                return "CC";
            case Direction::OUT:
                return "OUT";
            case Direction::IN:
                return "IN";
        }
    }

    /// Get a string (all in lowercase) of a direction.
    std::string directionName(Direction d) {
        switch (d) {
            case Direction::NORTH:
                return "north";
            case Direction::EAST:
                return "east";
            case Direction::SOUTH:
                return "south";
            case Direction::WEST:
                return "west";
            case Direction::NORTHEAST:
                return "northeast";
            case Direction::SOUTHEAST:
                return "southeast";
            case Direction::SOUTHWEST:
                return "southwest";
            case Direction::NORTHWEST:
                return "northwest";
            case Direction::UP:
                return "up";
            case Direction::DOWN:
                return "down";
            case Direction::CLOCKWISE:
                return "clockwise";
            case Direction::COUNTERCLOCKWISE:
                return "counterclockwise";
            case Direction::OUT:
                return "out";
            case Direction::IN:
                return "in";
        }
    }
}