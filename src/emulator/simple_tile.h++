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

#ifndef LIBDRMET__SIMPLE_TILE_HXX
#define LIBDRMET__SIMPLE_TILE_HXX

#include "tile.h++"

namespace hurricane {
    /* This simple tile is really only useful for checking that
     * hurricane is capable of building the relevant templates fully.
     * It's probably not useful for anything because it doesn't
     * execute any code at all... */
    class simple_tile: public tile {
    public:
        simple_tile(size_t x_posn, size_t y_posn)
            : hurricane::tile(x_posn, y_posn)
            {
            }

        ~simple_tile(void) {}

        int main(size_t x_posn __attribute__((unused)),
                 size_t y_posn __attribute__((unused)))
            { return 0; }
    };
}

#endif
