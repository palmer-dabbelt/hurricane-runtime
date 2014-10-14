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

#ifndef LIBHURRICANE_EMULATOR__QUEUE_HXX
#define LIBHURRICANE_EMULATOR__QUEUE_HXX

#include <memory>

namespace hurricane {
    class queue;
    typedef std::shared_ptr<queue> queue_ptr;
}

namespace hurricane {
    class queue {
    public:
        static const size_t default_queue_size = 8;

    private:

    public:
        /* Creates a single queue (implemented as a circular memory
         * buffer) that contains the given element count. */
        queue(size_t element_count = default_queue_size);
    };
}

#endif
