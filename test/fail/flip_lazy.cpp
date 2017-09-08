#include <boost/fit/lazy.hpp>
#include <boost/fit/placeholders.hpp>
#include <boost/fit/flip.hpp>

int main() {
    auto i = (boost::fit::flip(boost::fit::_1 - boost::fit::_2) * boost::fit::_1)(3, 6);
    (void)i;
}
