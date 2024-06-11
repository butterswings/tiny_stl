#include <swing/tuple.hpp>

#include <concepts>
#include <iostream>
#include <optional>
#include <ranges>
#include <thread>
#include <vector>

char buf[256];

int main(int argc, char** argv)
{
  std::vector<int> input{1, 2, 3, 4};
  auto result = input | std::views::reverse;
  for (const auto& e : result)
    std::cout << e << " ";

  return 0;
}

// TODO: iterator, optional, variant
// containers, concepts, debug module
