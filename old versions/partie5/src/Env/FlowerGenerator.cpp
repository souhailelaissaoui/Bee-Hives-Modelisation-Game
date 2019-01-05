#include <SFML/Graphics.hpp>
#include <iostream>
#include <Application.hpp>
#include <vector>
#include "Random/Random.hpp"
#include "FlowerGenerator.hpp"
#include "Env.hpp"
#include <math.h>
#include "Utility/Utility.hpp"

FlowerGenerator::FlowerGenerator()
    : time_(sf::Time::Zero) {};
void FlowerGenerator::reset()
{
    time_ = sf::Time::Zero;
};

FlowerGenerator::~FlowerGenerator() {};

void FlowerGenerator::update(sf::Time dt)
{
    time_ += dt;
    if ((time_.asSeconds() >= getAppConfig()["simulation"]["flower generator"]["delay"].toDouble()) and (getAppConfig()["simulation"]["flower generator"]["active"] == true)) {
        reset();
        Vec2d p;
        do {
            p.x = (uniform(0, static_cast<int>(getApp().getWorldSize().x)));
            p.y = (uniform(0, static_cast<int>(getApp().getWorldSize().y)));
        } while (not getAppEnv().addFlowerAt(p));
    }
}
