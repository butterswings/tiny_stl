#define DBG_MACRO_NO_WARNING
#include <dbg.h>

#include <swing/string_literal.hpp>
#include <string>
#include <iostream>

int main(int argc, char** argv)
{
  using namespace swing::string_literal;
  using literal =
    concat<
      make_string_literal("make "), make_string_literal("C++ "),
      make_string_literal("great "), make_string_literal("again")
    >;
  std::cout << literal::size() << std::endl;
  std::cout << std::string(literal::value) << std::endl;
  using reversed = push_back<pop_front<reverse<literal>>, '\0'>;
  std::cout << reversed::value;

  return 0;
}

// TODO: iterator, optional, variant
// containers, concepts, debug module
