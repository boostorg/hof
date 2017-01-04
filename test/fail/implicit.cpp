#include <fit/implicit.hpp>

template<class T>
struct auto_caster
{
    template<class U>
    T operator()(U x)
    {
        return T(x);
    }
};


int main()
{
    fit::implicit<auto_caster> auto_cast = {};
    auto x = auto_cast(1.5);
    (void)x;
}
