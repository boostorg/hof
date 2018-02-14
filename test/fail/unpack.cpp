#include <boost/hof/unpack.hpp>

struct foo
{};

namespace boost { namespace hof {

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
    boost::hof::unpack(boost::hof::always(1))(foo{});
}