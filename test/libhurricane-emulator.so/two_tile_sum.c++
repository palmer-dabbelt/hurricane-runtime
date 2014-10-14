#include <emulator/array.h++>
#include <emulator/simple_tile.h++>

class send_tile: public hurricane::tile {
private:

public:
    send_tile(size_t x_posn, size_t y_posn)
        : hurricane::tile(x_posn, y_posn)
        {
        }

public:
    virtual int main(void)
        {
            if (!((x_posn() == 0) && (y_posn() == 0)))
                abort();

            size_t sum = 0;
            for (size_t i = 1; i < 100; ++i) {
                send(hurricane::direction::SOUTH, i);
                sum += i;
            }

            send(hurricane::direction::SOUTH, 0);
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
            if (!((x_posn() == 0) && (y_posn() == 1)))
                abort();

            size_t sum = 0;
            hurricane::word i;
            while ((i = recv(hurricane::direction::NORTH)) != 0) {
                sum += i;
            }
            return sum;
        }
};

int main(int argc __attribute__((unused)),
         char **argv __attribute__((unused))
    )
{
    auto array = std::make_shared<hurricane::array>(
        8, 8,
        [](size_t x_posn, size_t y_posn) -> hurricane::tile_ptr
        {
            if ((x_posn == 0) && (y_posn == 0))
                return std::make_shared<send_tile>(x_posn, y_posn);
            if ((x_posn == 0) && (y_posn == 1))
                return std::make_shared<recv_tile>(x_posn, y_posn);

            return std::make_shared<hurricane::simple_tile>(x_posn, y_posn);
        });

    auto rets = array->run();

    auto send_ret = rets[std::make_pair(0, 0)];
    auto recv_ret = rets[std::make_pair(0, 1)];

    if (send_ret != recv_ret) {
        fprintf(stderr, "Mismatched sums: %d %d\n",
                send_ret, recv_ret);
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
