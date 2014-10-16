#include "test_main.h++"
#include <emulator/array.h++>
#include <emulator/simple_tile.h++>

class send_gen: public hurricane::edge_input {
private:

public:
    send_gen(size_t x_posn, size_t y_posn)
        : hurricane::edge_input(x_posn, y_posn)
        {
        }

public:
    virtual int main(void)
        {
            if (!((x_posn() == 0) && (y_posn() == 0)))
                return 0;

            size_t sum = 0;
            for (size_t i = 1; i < 100; ++i) {
                send(hurricane::direction::EAST, i);
                sum += i;
            }

            send(hurricane::direction::EAST, 0);
            return sum;
        }
};

class forw_tile: public hurricane::tile {
private:

public:
    forw_tile(size_t x_posn, size_t y_posn)
        : hurricane::tile(x_posn, y_posn)
        {
        }

public:
    virtual int main(void)
        {
            if (!((x_posn() == 0) && (y_posn() == 0)))
                abort();

            hurricane::word packet;
            size_t sum = 0;
            do {
                packet = recv(hurricane::direction::WEST);
                send(hurricane::direction::EAST, packet);
                sum += packet;
            } while (packet != 0);

            return sum;
        }
};

class recv_tile: public hurricane::tile {
private:

public:
    recv_tile(size_t x_posn, size_t y_posn)
        : hurricane::tile(x_posn, y_posn)
        {
        }

public:
    virtual int main(void)
        {
            if (!((x_posn() == 1) && (y_posn() == 0)))
                abort();

            size_t sum = 0;
            hurricane::word i;
            while ((i = recv(hurricane::direction::WEST)) != 0) {
                sum += i;
            }
            return sum;
        }
};

int test_main(void)
{
    auto array = std::make_shared<hurricane::array>(
        8, 8,
        [](size_t x_posn, size_t y_posn) -> hurricane::tile_ptr
        {
            if ((x_posn == 0) && (y_posn == 0))
                return std::make_shared<forw_tile>(x_posn, y_posn);
            if ((x_posn == 1) && (y_posn == 0))
                return std::make_shared<recv_tile>(x_posn, y_posn);

            return std::make_shared<hurricane::simple_tile>(x_posn, y_posn);
        });

    array->set_edge_inputs(
        [&](size_t x_posn, size_t y_posn)
        {
            return std::make_shared<send_gen>(x_posn, y_posn);
        });

    auto rets = array->run();

    auto forw_ret = rets[std::make_pair(0, 0)];
    auto recv_ret = rets[std::make_pair(1, 0)];

    if (recv_ret != forw_ret) {
        fprintf(stderr, "Mismatched sums: %d %d\n",
                forw_ret, recv_ret);
        abort();
    }

    if (recv_ret != 4950) {
        fprintf(stderr, "Mismatched sums: %d %d\n",
                4950, recv_ret);
        abort();
    }

    for (const auto& pair: array->run()) {
        auto posn_x = pair.first.first;
        auto posn_y = pair.first.second;
        auto ret = pair.second;

        fprintf(stderr, "(%d, %d) -> %d\n",
                (int)posn_x,
                (int)posn_y,
                ret
            );
    }

    return 0;
}
