/*
 * Copyright (C) 2014 Palmer Dabbelt
 *   <palmer.dabbelt@eecs.berkeley.edu>
 *
 * This file is part of dreamer-sim.
 * 
 * dreamer-sim is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * dreamer-sim is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with dreamer-sim.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBHURRICANE_EMULATOR__DIRECTION_HXX
#define LIBHURRICANE_EMULATOR__DIRECTION_HXX

#include <cstddef>
#include <vector>

namespace hurricane {
    /* The different network directions. */
    enum class direction {
        NORTH = 0,
        EAST  = 1,
        SOUTH = 2,
        WEST  = 3,
    };

    static const std::vector<enum direction>
    DIRECTIONS = { direction::NORTH,
                   direction::EAST,
                   direction::SOUTH,
                   direction::WEST };
}

#endif
