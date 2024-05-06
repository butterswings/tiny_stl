#define DBG_MACRO_NO_WARNING
#include <dbg.h>

#include <iostream>
#include <swing/string_literal.hpp>
#include <swing/debug/perfect_name.hpp>

int main(int argc, char** argv)
{
  using t = decltype(swing::debug::perfect_name<int>());
  std::cout << t::value;

  return 0;
}

// TODO: iterator, optional, variant
// containers, concepts, debug module
