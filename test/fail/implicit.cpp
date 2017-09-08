#include <boost/fit/implicit.hpp>

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
    boost::fit::implicit<auto_caster> auto_cast = {};
    auto x = auto_cast(1.5);
    (void)x;
// This is not possible in c++17 due to guaranteed copy elison
#if BOOST_FIT_HAS_STD_17 || (defined(__GNUC__) && !defined (__clang__) && __GNUC__ == 4 && __GNUC_MINOR__ < 7)
    static_assert(false, "Always fail");
#endif
}
