#include "flyingcat.hpp"

int main() {
  std::srand(std::time(0));
  auto game = std::make_shared<FlyingCat>();
  game->run();
  return 0;
}