#include <SFML/Graphics.hpp>
#include <iostream>
#include <Application.hpp>
#include <vector>
#include "Random/Random.hpp"
#include "Hive.hpp"
#include "Env.hpp"
#include <math.h>
#include "Utility/Utility.hpp"
#include <Env/Bee.hpp>

void Hive::addBee()
{
    Bees_.push_back(new Bee(this, getPosition(), 10, 10, 10));
}

void Hive::update(sf::Time dt)
{
    for (size_t i(0); i < Bees_.size(); i++) {
        (Bees_[i])->update(dt);
    }

    std::vector<Bee*> temp(Bees_);
    for (size_t i(0); i < temp.size(); i++) {
        (temp[i])->update(dt);
        if (temp[i]->isDead()) {
            delete temp[i];
            Bees_[i] = nullptr;
        }
    }
    Bees_.erase(std::remove(Bees_.begin(), Bees_.end(), nullptr), Bees_.end());

};

void Hive::drawOn(sf::RenderTarget& targetWindow) const
{
    auto const& texture = getAppTexture(getAppConfig()["simulation"]["hive"]["texture"].toString());
    auto HiveSprite = buildSprite(getPosition(), getRadius() +2.5, texture);
    targetWindow.draw(HiveSprite);
    if (isDebugOn()) {
        auto const text = buildText(to_nice_string(nectar_), getPosition(), getAppFont(), 30, sf::Color::Yellow);
        targetWindow.draw(text);
    }
    for (size_t i(0); i < Bees_.size(); i++) {
        (Bees_[i])->drawOn(targetWindow);
    }

}
void Hive::dropPollen(double qte)
{
    nectar_ += qte;
}

double Hive::takeNectar(double qte)
{
    if (qte > nectar_) {
        qte = nectar_;
        nectar_ = 0;
        return qte ;
    } else {
        nectar_ -= qte;
        return qte;
    }
}
Hive::Hive(Vec2d cent, double r): Collider(cent,r), nectar_(getAppConfig()["simulation"]["hive"]["initial"]["nectar"].toDouble()) {};
Hive::~Hive()
{
    for (size_t i(0); i < Bees_.size(); i++) {
        delete Bees_[i];
    }
}




