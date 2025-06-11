#include "flyingcat.hpp"
#include <GTest/gtest.h>

TEST(GameTest, calculateRotationAssertions) {
  auto game = std::make_shared<FlyingCat>();
  ASSERT_EQ(game->calculateRotation(0, 1), 90);
  ASSERT_EQ(game->calculateRotation(1, 0), 0);
}

TEST(GameTest, calculateRandMoveAssertions) {
  auto game = std::make_shared<FlyingCat>();
  for (int i = 0; i < 100; i++) {
    ASSERT_NE(game->calculateRandMove(1), sf::Vector2f({0, 0}));
  }
}

TEST(GameTest, changeFishAssertions) {
  auto game = std::make_shared<FlyingCat>();
  for (int i = -30; i < 30; i += rand() % 4) {
    int oldFishCount = game->fishCount;
    game->changeFish(i);
    ASSERT_EQ(oldFishCount + i, game->fishCount);
  }
}

TEST(GameTest, changeScoreAssertions) {
  auto game = std::make_shared<FlyingCat>();
  for (int i = -30; i < 30; i += rand() % 4) {
    int oldScore = game->score;
    game->changeScore(i);
    ASSERT_EQ(oldScore + i, game->score);
  }
}

TEST(GameTest, emitParticlesAssertions) {
  auto game = std::make_shared<FlyingCat>();
  for (int i = 0; i < 60; i += rand() % 4) {
    int oldParticlesCount = game->particles.size();
    game->emitParticles(i, "crumbs", 10, 5, 0, 0, 0, sf::Vector2f({0, 0}));
    ASSERT_EQ(oldParticlesCount + i, game->particles.size());
  }
}