#include <fit/lazy.hpp>
#include <fit/placeholders.hpp>
#include <fit/flip.hpp>

int main() {
    auto i = (fit::flip(fit::_1 - fit::_2) * fit::_1)(3, 6);
    (void)i;
}
