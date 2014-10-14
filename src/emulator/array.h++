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

#ifndef LIBHURRICANE_EMULATOR__ARRAY_HXX
#define LIBHURRICANE_EMULATOR__ARRAY_HXX

#include "on_tile.h++"
#include "tile.h++"
#include <functional>
#include <map>
#include <memory>

namespace hurricane {
    /* Holds all the information about a DREAMER array and allows it
     * to be configured for execution. */
    class array {
    private:
        const size_t _width;
        const size_t _height;
        const std::map<std::pair<size_t, size_t>, tile_ptr> _tiles;

    public:
        /* Creates a new array, given the size of that array and a
         * function used to create new nodes within that array.  Note
         * that any memory initialized by "func" needs to be allocated
         * with an "on_tile" allocator if it's going to end up on the
         * target tile, otherwise it'll be somewhere on the host
         * system, which is probably not what you want if you're
         * looking for code with decent performance. */
        array(size_t width, size_t height,
              std::function<tile_ptr(size_t width,
                                     size_t height)> func
            );

    public:
        size_t width (void) const { return _width;  }
        size_t height(void) const { return _height; }

    public:
        /* This actually starts executing the program that was setup
         * to run on DREAMER, returning a map of the return values
         * from every core that returned.  This function will block
         * until the given DREAMER program has finished executing. */
        std::map<std::pair<size_t, size_t>, int> run(void)
            {
                fork();
                return join();
            }

        /* Asynchronus versions of the above function. */
        void fork(void);
        std::map<std::pair<size_t, size_t>, int> join(void);
    };
}

#endif
