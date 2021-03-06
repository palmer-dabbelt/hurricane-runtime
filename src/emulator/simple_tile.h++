/*
 * Copyright (C) 2014 Palmer Dabbelt
 *   <palmer.dabbelt@eecs.berkeley.edu>
 *
 * This file is part of hurricane-runtime.
 * 
 * hurricane-runtime is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * hurricane-runtime is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Affero General Public License for more details.
 * 
 * You should have received a copy of the GNU Affero General Public License
 * along with hurricane-runtime.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef LIBHURRICANE_EMULATOR__SIMPLE_TILE_HXX
#define LIBHURRICANE_EMULATOR__SIMPLE_TILE_HXX

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

        virtual int main(void)
            { return 0; }
    };
}

#endif
