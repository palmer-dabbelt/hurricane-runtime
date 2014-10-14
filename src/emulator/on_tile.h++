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

#ifndef LIBHURRICANE_EMULATOR__ON_TILE_HXX
#define LIBHURRICANE_EMULATOR__ON_TILE_HXX

#include <cstddef>
#include <cstdlib>

namespace hurricane {
    /* A special allocator that ensures that allocated memory lives on
     * a particular tile. */
    template <class to_alloc>
    class on_tile {
    public:
        typedef to_alloc value_type;

    private:
        const size_t _x_posn;
        const size_t _y_posn;

    public:
        /* Creates a new allocator that targets a particular tile,
         * given the position of that tile. */
        on_tile(size_t x_posn, size_t y_posn)
            : _x_posn(x_posn),
              _y_posn(y_posn)
            {
            }

        on_tile(const on_tile<to_alloc>& that)
            : _x_posn(that._x_posn),
              _y_posn(that._y_posn)
            {
            }

    public:
        value_type* allocate(std::size_t n)
            {
                return (value_type*)malloc(sizeof(value_type) * n);
            }

        void deallocate(value_type* ptr, std::size_t n __attribute__((unused)))
            {
                return free(ptr);
            }
        
    };
}

#endif
