#include "ddk_iterable.h"

namespace ddk
{
namespace
{

const detail::__numbers_iterable<int> integers_container = detail::__numbers_iterable<int>();
const detail::__numbers_iterable<int> odd_integers_container = detail::__numbers_iterable<int>(make_function([](const int& i_number) { return 2 * i_number + 1; }));
const detail::__numbers_iterable<int> even_integers_container = detail::__numbers_iterable<int>(make_function([](const int& i_number) { return 2 * i_number; }));

const detail::__numbers_iterable<float> reals_container = detail::__numbers_iterable<float>();
const detail::__numbers_iterable<float> odd_reals_container = detail::__numbers_iterable<float>(make_function([](const float& i_number) { return 2 * i_number + 1; }));
const detail::__numbers_iterable<float> even_reals_container = detail::__numbers_iterable<float>(make_function([](const float& i_number) { return 2 * i_number; }));

}

const const_random_access_iterable<const int> integers = deduce_iterable(integers_container);
const const_random_access_iterable<const int> odd_integers = deduce_iterable(odd_integers_container);
const const_random_access_iterable<const int> even_integers = deduce_iterable(even_integers_container);

const const_random_access_iterable<const int> naturals = view::constrain(integers,[](const int& i_value){ return i_value > 0; });
const const_random_access_iterable<const int> odd_naturals = view::constrain(odd_integers,[](const int& i_value) { return i_value > 0; });
const const_random_access_iterable<const int> even_naturals = view::constrain(even_integers,[](const int& i_value) { return i_value > 0; });

const const_random_access_iterable<const float> reals = deduce_iterable(reals_container);
const const_random_access_iterable<const float> odd_reals = deduce_iterable(odd_reals_container);
const const_random_access_iterable<const float> even_reals = deduce_iterable(even_reals_container);

}
