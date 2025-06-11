#include "flyingcat.hpp"
#include <bits/stdc++.h>
#include <cmath>

Buff::Buff(std::string typ, std::shared_ptr<sf::Sprite> sprt) {
  type = typ;
  sprite = sprt;
};

Particle::Particle(std::string typ, int lftime, int vlX, int vlY, int vlRand,
                   std::shared_ptr<sf::Sprite> sprt) {
  type = typ;
  lifetime = lftime;
  velX = vlX;
  velY = vlY;
  velRand = vlRand;
  sprite = sprt;
};

float FlyingCat::calculateRotation(float x, float y) {
  return (180.f / M_PI) * atan(y / x);
}

sf::Vector2f FlyingCat::calculateRandMove(int r) {
  int rX = rand() % (r * 2 + 1) - r;
  if (rX == 0) {
    rX = (rand() % 2) * 2 - 1;
  }
  int rY = rand() % (r * 2 + 1) - r;
  if (rY == 0) {
    rY = (rand() % 2) * 2 - 1;
  }
  return sf::Vector2f({rX, rY});
}

FlyingCat::FlyingCat() {
  window = std::make_shared<sf::RenderWindow>(sf::VideoMode({1600, 900}),
                                              "Flying Cat");
  window->setFramerateLimit(60);

  velY = accY = 0.f;
  speed = 9.f;
  mouselock = false;
  backgroundOffset = 0;
  fishCount = 5;
  pillowCooldown = 0;
  fishSpawnCounter = pillowSpawnCounter = buffsSpawnCounter =
      catParticlesCounter = 0;
  cloudParalysis = 0;
  baitEffect = 0;

  backgroundTexture.loadFromFile("./resources/img/background.png");
  backgroundTexture.setRepeated(true);
  catTexture.loadFromFile("./resources/img/cat.png");
  fishTexture.loadFromFile("./resources/img/fish.png");
  pillowTexture.loadFromFile("./resources/img/pillow.png");
  catParticleTexture.loadFromFile("./resources/img/cat_particle.png");
  sparkParticleTexture.loadFromFile("./resources/img/spark_particle.png");
  crumbsParticleTexture.loadFromFile("./resources/img/crumbs_particle.png");
  featherParticleTexture.loadFromFile("./resources/img/feather_particle.png");

  cloudTexture.loadFromFile("./resources/img/cloud.png");
  bonusTexture.loadFromFile("./resources/img/bonus.png");
  baitTexture.loadFromFile("./resources/img/bait.png");
  mineTexture.loadFromFile("./resources/img/mine.png");
  presentTexture.loadFromFile("./resources/img/present.png");
  starTexture.loadFromFile("./resources/img/star.png");
  spiderTexture.loadFromFile("./resources/img/spider.png");

  biteSoundBuffer =
      std::make_shared<sf::SoundBuffer>("./resources/audio/bite.mp3");
  biteSound = std::make_shared<sf::Sound>(*biteSoundBuffer);

  bigBiteSoundBuffer =
      std::make_shared<sf::SoundBuffer>("./resources/audio/big_bite.mp3");
  bigBiteSound = std::make_shared<sf::Sound>(*bigBiteSoundBuffer);

  bawongSoundBuffer =
      std::make_shared<sf::SoundBuffer>("./resources/audio/bawong.mp3");
  bawongSound = std::make_shared<sf::Sound>(*bawongSoundBuffer);

  jumpSoundBuffer =
      std::make_shared<sf::SoundBuffer>("./resources/audio/jump.mp3");
  jumpSound = std::make_shared<sf::Sound>(*jumpSoundBuffer);

  baitSoundBuffer =
      std::make_shared<sf::SoundBuffer>("./resources/audio/bait.mp3");
  baitSound = std::make_shared<sf::Sound>(*baitSoundBuffer);

  explosionSoundBuffer =
      std::make_shared<sf::SoundBuffer>("./resources/audio/explosion.mp3");
  explosionSound = std::make_shared<sf::Sound>(*explosionSoundBuffer);

  lightningSoundBuffer =
      std::make_shared<sf::SoundBuffer>("./resources/audio/lightning.mp3");
  lightningSound = std::make_shared<sf::Sound>(*lightningSoundBuffer);

  presentSoundBuffer =
      std::make_shared<sf::SoundBuffer>("./resources/audio/present.mp3");
  presentSound = std::make_shared<sf::Sound>(*presentSoundBuffer);

  starSoundBuffer =
      std::make_shared<sf::SoundBuffer>("./resources/audio/star.mp3");
  starSound = std::make_shared<sf::Sound>(*starSoundBuffer);

  fallSoundBuffer =
      std::make_shared<sf::SoundBuffer>("./resources/audio/fall.mp3");
  fallSound = std::make_shared<sf::Sound>(*fallSoundBuffer);

  spiderSoundBuffer =
      std::make_shared<sf::SoundBuffer>("./resources/audio/spider.mp3");
  spiderSound = std::make_shared<sf::Sound>(*spiderSoundBuffer);

  musicSoundBuffer =
      std::make_shared<sf::SoundBuffer>("./resources/audio/soundtrack.mp3");
  musicSound = std::make_shared<sf::Sound>(*musicSoundBuffer);
  musicSound->setVolume(25);
  musicSound->setLooping(true);

  background = std::make_shared<sf::Sprite>(backgroundTexture);
  background->setTextureRect({{0, 0}, {1600, 900}});
  cat = std::make_shared<sf::Sprite>(catTexture);
  cat->setOrigin({0.5f, 0.5f});
  cat->setScale({0.5f, 0.5f});
  cat->setPosition({300.f, 200.f});

  std::vector<sf::Sprite> fishes;
  std::vector<sf::Sprite> pillows;
  std::vector<Buff> buffs;
  std::vector<Particle> particles;

  gameover = started = false;
  score = maxScore = lastScore = 0;

  font.openFromFile("./resources/fonts/Pacifico-Regular.ttf");

  gameoverText = std::make_shared<sf::Text>(font);
  gameoverText->setCharacterSize(50);
  gameoverText->setOutlineThickness(3);
  gameoverText->setString(L"НАЖИТЕ ПРОБЕЛ ЧТОБЫ НАЧАТЬ ЗАНОВО");
  sf::Vector2f gameoverTextCenter = gameoverText->getGlobalBounds().size / 2.f;
  sf::Vector2f gameoverTextLocalBounds = gameoverTextLocalBounds =
      gameoverTextCenter + gameoverText->getLocalBounds().position;
  gameoverText->setOrigin(gameoverTextLocalBounds);
  gameoverText->setPosition({800, 450});

  startText = std::make_shared<sf::Text>(font);
  startText->setCharacterSize(50);
  startText->setOutlineThickness(3);
  startText->setString(L"НАЖИТЕ ПРОБЕЛ ЧТОБЫ НАЧАТЬ");
  sf::Vector2f startTextCenter = startText->getGlobalBounds().size / 2.f;
  sf::Vector2f startTextLocalBounds = startTextLocalBounds =
      startTextCenter + startText->getLocalBounds().position;
  startText->setOrigin(startTextLocalBounds);
  startText->setPosition({800, 450});

  scoreText = std::make_shared<sf::Text>(font);
  scoreText->setString(L"Счёт: " + std::to_wstring(score));
  scoreText->setPosition({10.f, 10.f});
  scoreText->setCharacterSize(50);
  scoreText->setOutlineThickness(3);

  fishText = std::make_shared<sf::Text>(font);
  fishText->setString(L"Рыбы: " + std::to_wstring(fishCount));
  fishText->setPosition({10.f, 100.f});
  fishText->setCharacterSize(50);
  fishText->setOutlineThickness(3);
}

void FlyingCat::changeScore(int delta) {
  score += delta;
  scoreText->setString(L"Счёт: " + std::to_wstring(score));
}

void FlyingCat::changeFish(int delta) {
  fishCount += delta;
  fishText->setString(L"Рыбы: " + std::to_wstring(fishCount));
};

void FlyingCat::events() {
  while (const std::optional event = window->pollEvent()) {
    if (event->is<sf::Event::Closed>()) {
      window->close();
    } else if (const auto *keyPressed = event->getIf<sf::Event::KeyPressed>()) {
      if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
        window->close();
    }
    if (!gameover && started) {
      if (event->is<sf::Event::MouseButtonPressed>() && !mouselock &&
          !cloudParalysis && fishCount > 0) {
        jump();
        jumpSound->play();
        changeFish(-1);
        mouselock = true;
      }
      if (event->is<sf::Event::MouseButtonReleased>()) {
        mouselock = false;
      }
    }
  }

  if (!started && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)) {
    started = true;
    musicSound->play();
  }

  if (gameover && sf::Keyboard::isKeyPressed(sf::Keyboard::Scan::Space)) {
    restart();
  }
}

void FlyingCat::draw() {

  window->clear(sf::Color::Black);
  window->draw(*background);

  for (auto &fish : fishes) {
    window->draw(fish);
  }

  for (auto &pillow : pillows) {
    window->draw(pillow);
  }

  for (auto &buff : buffs) {
    window->draw(*buff.sprite);
  }

  window->draw(*cat);

  for (auto &particle : particles) {
    window->draw(*particle.sprite);
  }

  if (gameover) {
    window->draw(*gameoverText);
  }

  if (!started) {
    window->draw(*startText);
  }
  if (!gameover && started) {
    window->draw(*scoreText);
    window->draw(*fishText);
  }

  window->display();
}

void FlyingCat::run() {
  while (window->isOpen()) {
    events();
    game();
    draw();
  }
}

void FlyingCat::jump() {
  velY = 0.f;
  accY = -2.f;
}

void FlyingCat::backAnim() {
  if (backgroundOffset < 1800) {
    backgroundOffset += speed / 2;
  } else {
    backgroundOffset = 0;
  }
  background->setTextureRect({{backgroundOffset, 0}, {1600, 900}});
}

void FlyingCat::emitParticles(int amount, std::string type, int lifetime,
                              int velFromCenter, int velX, int velY,
                              int velRand, sf::Vector2f position) {
  std::vector<Particle> emited_particles;
  for (int i = 0; i < amount; i++) {
    std::shared_ptr<sf::Sprite> particleSprite;
    particleSprite = std::make_shared<sf::Sprite>(catParticleTexture);
    if (type == "spark") {
      particleSprite->setTexture(sparkParticleTexture);
    } else if (type == "crumbs") {
      particleSprite->setTexture(crumbsParticleTexture);
    } else if (type == "feather") {
      particleSprite->setTexture(featherParticleTexture);
    }

    sf::Vector2f moveDir = calculateRandMove(10).normalized();
    Particle particle(type, lifetime, moveDir.x * velFromCenter + velX,
                      moveDir.y * velFromCenter + velY, velRand,
                      particleSprite);
    particle.sprite->setPosition(position);
    particles.push_back(particle);
  }
}

void FlyingCat::particleSystem() {
  if (catParticlesCounter >= 3) {
    catParticlesCounter = 0;

    std::shared_ptr<sf::Sprite> particleSprite;
    particleSprite = std::make_shared<sf::Sprite>(catParticleTexture);
    Particle particle("cat", 30, 0, 0, 5, particleSprite);
    particle.sprite->setPosition(cat->getPosition());
    particles.push_back(particle);
  }

  for (int i = 0; i < particles.size(); i++) {
    particles[i].lifetime -= 1;

    if (particles[i].type == "cat") {
      particles[i].sprite->move(calculateRandMove(particles[i].velRand));
    }

    particles[i].sprite->move({-speed + particles[i].velX, particles[i].velY});

    if (particles[i].sprite->getPosition().x < -200 ||
        particles[i].lifetime <= 0) {
      particles.erase(particles.begin() + i);
    }
  }

  catParticlesCounter += 1;
}

void FlyingCat::fishSystem() {

  if (fishSpawnCounter >= 700 || (fishSpawnCounter >= 300 && baitEffect > 0)) {
    sf::Sprite fish(fishTexture);
    bool isColliding = true;
    while (isColliding) {
      fish.setPosition({2000 + (200 - rand() % 401), 50 + rand() % 801});

      isColliding = false;
      for (int i = 0; i < buffs.size(); i++) {
        if (buffs[i].sprite->getGlobalBounds().findIntersection(
                fish.getGlobalBounds())) {
          isColliding = true;
        };
      }
      for (int i = 0; i < pillows.size(); i++) {
        if (pillows[i].getGlobalBounds().findIntersection(
                fish.getGlobalBounds())) {
          isColliding = true;
        };
      }
      for (int i = 0; i < fishes.size(); i++) {
        if (fishes[i].getGlobalBounds().findIntersection(
                fish.getGlobalBounds())) {
          isColliding = true;
        };
      }
    }
    fish.setRotation(sf::degrees(45));
    fishes.push_back(fish);
    fishSpawnCounter = 0;
  }
  for (std::size_t i{}; i < fishes.size(); ++i) {
    fishes[i].move({-speed, 0.f});
    if (fishes[i].getGlobalBounds().findIntersection(cat->getGlobalBounds())) {
      changeFish(1);
      biteSound->play();
      emitParticles(20, "crumbs", 10, 5, 0, 0, 0, fishes[i].getPosition());
      fishes.erase(fishes.begin() + i);
    }
  }
  for (std::size_t i{}; i < fishes.size(); ++i) {
    if (fishes[i].getPosition().x < -100) {
      fishes.erase(fishes.begin() + i);
    }
  }
  fishSpawnCounter += speed;
}

void FlyingCat::pillowsSystem() {
  if (pillowSpawnCounter >= 900) {
    sf::Sprite pillow(pillowTexture);
    bool isColliding = true;
    while (isColliding) {
      pillow.setPosition({2000 + (200 - rand() % 401), 50 + rand() % 800});

      isColliding = false;
      for (int i = 0; i < buffs.size(); i++) {
        if (buffs[i].sprite->getGlobalBounds().findIntersection(
                pillow.getGlobalBounds())) {
          isColliding = true;
        };
      }
      for (int i = 0; i < pillows.size(); i++) {
        if (pillows[i].getGlobalBounds().findIntersection(
                pillow.getGlobalBounds())) {
          isColliding = true;
        };
      }
      for (int i = 0; i < fishes.size(); i++) {
        if (fishes[i].getGlobalBounds().findIntersection(
                pillow.getGlobalBounds())) {
          isColliding = true;
        };
      }
    }
    pillows.push_back(pillow);
    pillowSpawnCounter = 0;
  }
  for (std::size_t i{}; i < pillows.size(); ++i) {
    pillows[i].move({-speed, 0.f});
    if (pillows[i].getGlobalBounds().findIntersection(cat->getGlobalBounds())) {
      if (pillowCooldown == 0) {
        pillowCooldown = 120;
        jump();
        emitParticles(20, "feather", 40, 10, 0, -15, 4,
                      pillows[i].getPosition());
        bawongSound->play();
      }
    }
  }
  if (pillowCooldown > 0) {
    pillowCooldown -= 1;
  }
  for (std::size_t i{}; i < pillows.size(); ++i) {
    if (pillows[i].getPosition().x < -200) {
      pillows.erase(pillows.begin() + i);
    }
  }
  pillowSpawnCounter += speed;
}

void FlyingCat::buffsSystem() {
  if (buffsSpawnCounter >= 900) {
    int buffRand = std::rand() % 7;
    std::shared_ptr<sf::Sprite> buffSprite;
    buffSprite = std::make_shared<sf::Sprite>(mineTexture);
    std::string buffType = "cloud";
    switch (buffRand) {
    case 0:
      if (cloudParalysis == 0) {
        buffSprite->setTexture(cloudTexture);
        buffType = "cloud";
      } else {
        buffSprite->setTexture(starTexture);
        buffType = "star";
      }
      break;
    case 1:
      buffSprite->setTexture(mineTexture);
      buffType = "mine";
      break;
    case 2:
      buffSprite->setTexture(bonusTexture);
      buffType = "bonus";
      break;
    case 3:
      if (baitEffect == 0) {
        buffSprite->setTexture(baitTexture);
        buffType = "bait";
      } else {
        buffSprite->setTexture(mineTexture);
        buffType = "mine";
      }
      break;
    case 4:
      buffSprite->setTexture(presentTexture);
      buffType = "present";
      break;
    case 5:
      buffSprite->setTexture(starTexture);
      buffType = "star";
      break;
    case 6:
      if (score > 0) {
        buffSprite->setTexture(spiderTexture);
        buffType = "spider";
      } else {
        buffSprite->setTexture(starTexture);
        buffType = "star";
      }

      break;
    }

    bool isColliding = true;
    while (isColliding) {
      buffSprite->setPosition(
          {2000.f + (200.f - rand() % 401), 50 + rand() % 800});

      isColliding = false;
      for (int i = 0; i < buffs.size(); i++) {
        if (buffs[i].sprite->getGlobalBounds().findIntersection(
                buffSprite->getGlobalBounds())) {
          isColliding = true;
        };
      }
      for (int i = 0; i < pillows.size(); i++) {
        if (pillows[i].getGlobalBounds().findIntersection(
                buffSprite->getGlobalBounds())) {
          isColliding = true;
        };
      }
      for (int i = 0; i < fishes.size(); i++) {
        if (fishes[i].getGlobalBounds().findIntersection(
                buffSprite->getGlobalBounds())) {
          isColliding = true;
        };
      }
    }

    Buff buff(buffType, buffSprite);
    buffs.push_back(buff);
    buffsSpawnCounter = 0;
  }
  for (std::size_t i{}; i < buffs.size(); ++i) {
    buffs[i].sprite->move({-speed, 0});
    if (buffs[i].sprite->getGlobalBounds().findIntersection(
            cat->getGlobalBounds())) {
      if (buffs[i].type == "cloud") {
        cloudParalysis = 120;
        lightningSound->play();
        emitParticles(20, "spark", 30, 10, 0, 0, 0,
                      buffs[i].sprite->getPosition());
      } else if (buffs[i].type == "mine") {
        dead();
        explosionSound->play();
      } else if (buffs[i].type == "bonus") {
        changeFish(3);
        emitParticles(20, "crumbs", 10, 5, 0, 0, 0,
                      buffs[i].sprite->getPosition());
        bigBiteSound->play();
      } else if (buffs[i].type == "bait") {
        baitEffect = 400;
        baitSound->play();
      } else if (buffs[i].type == "present") {
        changeScore(5);
        presentSound->play();
      } else if (buffs[i].type == "star") {
        changeScore(1);
        starSound->play();
      } else if (buffs[i].type == "spider") {
        changeScore(-1);
        spiderSound->play();
      }

      buffs.erase(buffs.begin() + i);
    }
  }
  for (std::size_t i{}; i < buffs.size(); ++i) {

    if (buffs[i].sprite->getPosition().x < -200) {
      buffs.erase(buffs.begin() + i);
    }
  }
  buffsSpawnCounter += speed;
  if (cloudParalysis > 0) {
    cloudParalysis -= 1;
  }
  if (baitEffect > 0) {
    baitEffect -= 1;
  }
}

void FlyingCat::game() {
  if (!gameover && started) {
    backAnim();
    catPhysics();
    fishSystem();
    pillowsSystem();
    buffsSystem();
    particleSystem();
    speed += 0.0005;
  }
}

void FlyingCat::dead() {
  maxScore = std::max(score, maxScore);
  gameover = true;
  gameoverText->setString(L"НАЖИТЕ ПРОБЕЛ ЧТОБЫ НАЧАТЬ ЗАНОВО\n СЧЁТ: " +
                          std::to_wstring(score) + L"\nРЕКОРД: " +
                          std::to_wstring(maxScore) + L"\nПРЕДЫДУЩИЙ СЧЁТ: " +
                          std::to_wstring(lastScore));
  lastScore = score;
  sf::Vector2f gameoverTextCenter = gameoverText->getGlobalBounds().size / 2.f;
  sf::Vector2f gameoverTextLocalBounds = gameoverTextLocalBounds =
      gameoverTextCenter + gameoverText->getLocalBounds().position;
  gameoverText->setOrigin(gameoverTextLocalBounds);
  gameoverText->setPosition({800, 450});
}

void FlyingCat::restart() {
  score = velY = accY = 0;
  scoreText->setString(L"Счёт: " + std::to_wstring(score));
  cat->setRotation(sf::degrees(0));
  cat->setPosition({300.f, 200.f});
  fishCount = 5;
  fishText->setString(L"Рыбы: " + std::to_wstring(fishCount));
  cloudParalysis = 0;
  baitEffect = 0;
  pillowCooldown = 0;
  mouselock = false;
  fishes.clear();
  pillows.clear();
  buffs.clear();
  particles.clear();
  speed = 9.f;
  gameover = false;
}

void FlyingCat::catPhysics() {
  cat->move({0, velY});
  cat->setRotation(sf::degrees(calculateRotation(speed, velY)));
  accY = std::min(0.f, accY + 0.2f);
  velY += accY;
  velY += 0.15f;
  if (cat->getPosition().y > 950) {
    dead();
    fallSound->play();
  }
}
