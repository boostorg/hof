#include <fit/lazy.hpp>
#include <fit/placeholders.hpp>
#include <fit/rotate.hpp>

int main() {
    auto i = (fit::rotate(fit::_1 - fit::_2) * fit::_1)(3, 6);
    (void)i;
}
