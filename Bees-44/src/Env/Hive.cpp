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
#include <Env/WorkerBee.hpp>
#include <Env/ScoutBee.hpp>

WorkerBee* Hive::addWorker()
{
    double size(uniform(0.0, getRadius()));
    WorkerBee* nouvelle(new WorkerBee(this, getPosition() + size * Vec2d::fromRandomAngle()));
    Bees_.push_back(nouvelle);
    return nouvelle;
}

ScoutBee* Hive::addScout()
{
    double size(uniform(0.0, getRadius() * 0.9));
    ScoutBee* nouvelle(new ScoutBee(this, getPosition() + size * Vec2d::fromRandomAngle()));
    Bees_.push_back(nouvelle);
    return nouvelle;
}

void Hive::update(sf::Time dt)
{

    std::vector<Bee*> inHive;
    std::vector<Bee*> temp(Bees_);
    for (size_t i(0); i < temp.size(); i++) {
        (temp[i])->update(dt);
        if (temp[i]->isDead()) {
            delete temp[i];
            Bees_[i] = nullptr;
        } else {
            if (isColliding(*Bees_[i])) {
                inHive.push_back(Bees_[i]);
            }
        }
    }
    Bees_.erase(std::remove(Bees_.begin(), Bees_.end(), nullptr), Bees_.end());

    for (size_t i(0); i < inHive.size(); i++) {
        for (size_t u(0); u < inHive.size(); u++) {
            inHive[i]->interact(inHive[u]);
        }
    }

    double proba = getAppConfig()["simulation"]["hive"]["reproduction"]["scout probability"].toDouble();
    if ((nectar_ > getAppConfig()["simulation"]["hive"]["reproduction"]["nectar threshold"].toDouble()) and (Bees_.size() < getAppConfig()["simulation"]["hive"]["reproduction"]["max bees"].toDouble())) {
        if (bernoulli(proba)) {
            addWorker();
        } else {
            addScout();
        }
    }
}

void Hive::drawOn(sf::RenderTarget& targetWindow) const
{
    auto const& texture = getAppTexture(getAppConfig()["simulation"]["hive"]["texture"].toString());
    auto HiveSprite = buildSprite(getPosition(), getRadius()* 2.5, texture);
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
        return qte;
    } else {
        nectar_ -= qte;
        return qte;
    }
}
Hive::Hive(Vec2d cent, double r)
    : Collider(cent, r)
    , nectar_(getAppConfig()["simulation"]["hive"]["initial"]["nectar"].toDouble()) {};
Hive::~Hive()
{
    for (size_t i(0); i < Bees_.size(); i++) {
        delete Bees_[i];
    }
}

bool Hive::isDead() const
{
    return (nectar_ == 0);
}

