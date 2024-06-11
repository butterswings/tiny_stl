#include <iostream>

#include <swing/meta/function_traits.hpp>
#include <swing/utility.hpp>

class clazz
{
public:
  void test(int, ...) noexcept(false);
};

void ff(int, const int*, ...) noexcept;

int main(int argc, char** argv, char** env)
{
  using namespace swing::meta;
  using namespace swing;
  using type = decltype(&clazz::test);
  using traits = function_traits<type>;

  using return_type   = traits::return_type;
  using params_type   = traits::params_type;
  using class_wrapper = traits::class_type;
  using pointer       = traits::pointer;
  using function_type = traits::function_type;

  std::cout << "const: \t\t"  << is_const_specified<type>::value         << std::endl
            << "volatile: \t" << is_volatile_specified<type>::value      << std::endl
            << "lvalue: \t"   << is_lvalue_specified<type>::value        << std::endl
            << "rvalue: \t"   << is_rvalue_specified<type>::value        << std::endl
            << "non_ref: \t"  << is_non_reference_specified<type>::value << std::endl
            << "noexcept: \t" << is_noexcept_specified<type>::value      << std::endl
            << "mem_fn: \t"   << is_member_function_pointer<type>::value;
  return 0;
}
