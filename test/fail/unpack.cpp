#include <boost/fit/unpack.hpp>

struct foo
{};

namespace boost { namespace fit {

template<>
struct unpack_sequence<foo>
{
    template<class F, class S>
    constexpr static int apply(F&&, S&&)
    {
        return 0;
    }
};
}

int main() {
    boost::fit::unpack(boost::fit::always(1))(foo{});
}