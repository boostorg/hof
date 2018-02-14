#include <boost/hof/always.hpp>
#include <memory>

int main() {
    auto f = boost::hof::always(std::unique_ptr<int>{new int(1)});
    auto i = f(1, 2, 3);
    (void)i;
}