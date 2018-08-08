/**
 * RoundTemplates.cpp
 *
 * By Sebastian Raaphorst, 2018.
 */

// For M_PI.
#define _USE_MATH_DEFINES
#include <cmath>
#include <stdexcept>
#include <vector>

#include "RoundTemplates.h"

namespace spelunker::graphmaze {

    std::vector<int> RoundTemplates::calculateRingSizes(const int radius) {
        if (radius <= 0)
            throw std::invalid_argument("Radius must be positive.");

        const auto rowHeight = 1.0 / radius;
        std::vector<int> sizes { 1 };

        for (auto row = 1; row < radius; ++row) {
            // Determine the number of cells we want in this row. Their size should be as close as possible to the
            // number of cells in the previous row: we branch if it is not.
            const auto innerRadius = row * rowHeight;
            const auto innerCircumference = 2 * M_PI * innerRadius;
            const auto estimatedCellWidth = innerCircumference / sizes.back();
            const auto ratio = static_cast<int>(std::round(estimatedCellWidth / rowHeight));
            const auto cols = sizes.back() * ratio;
            sizes.emplace_back(cols);
        }

        return sizes;
    }
}