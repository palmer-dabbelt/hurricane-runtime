#include <emulator/array.h++>

class tile: public hurricane::tile {
private:

public:
    tile(size_t x_posn, size_t y_posn)
        : hurricane::tile(x_posn, y_posn)
        {
        }

public:
    int main(void)
        {
            return x_posn() * 100 + y_posn();
        }
};

int main(int argc __attribute__((unused)),
         char **argv __attribute__((unused))
    )
{
    auto array = std::make_shared<hurricane::array>(
        8, 8,
        [](size_t x_posn, size_t y_posn)
        {
            return std::make_shared<tile>(x_posn, y_posn);
        });

    for (const auto& pair: array->run()) {
        auto posn_x = pair.first.first;
        auto posn_y = pair.first.second;
        auto ret = pair.second;

        if (ret != (int)(posn_y + posn_x * 100)) {
            fprintf(stderr, "Got %d for (%d, %d)\n",
                    ret, (int)posn_x, (int)posn_y);
            return 1;
        }
    }

    return 0;
}
