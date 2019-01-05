#include "Env.hpp"
#include <iostream>
#include <cassert>
#include <Application.hpp>
#include <vector>
#include <Utility/Utility.hpp>
#include "Random/Random.hpp"

Bee* Env::getBeeAt(Vec2d p)
{
    return nullptr;
}

void Env::update(sf::Time dt)
{
    std::vector<Flower*> temp(Flowers_);
    for (size_t i(0); i < temp.size(); i++) {
        (temp[i])->update(dt);
        if (temp[i]->getPollen() == 0) {
            delete temp[i];
            Flowers_[i] = nullptr;
        }
    }
    Flowers_.erase(std::remove(Flowers_.begin(), Flowers_.end(), nullptr), Flowers_.end());
    generator_.update(dt);
    for (size_t i(0); i < Hives_.size(); i++) {
        (Hives_[i])->update(dt);
    }
}

void Env::drawOn(sf::RenderTarget& target) const
{
    world_.drawOn(target);
    for (size_t i(0); i < Flowers_.size(); i++) {
        Flowers_[i]->drawOn(target);
    }
    for (size_t i(0); i < Hives_.size(); i++) {
        Hives_[i]->drawOn(target);
    }
    if (isDebugOn()) {
        auto const text = buildText(to_nice_string(world_.gethumidity(getApp().getCursorPositionInView())), getApp().getCursorPositionInView(), getAppFont(), 30, sf::Color::Red);
        target.draw(text);
    }
}

void Env::reset()
{
    Flowers_.clear();
    Hives_.clear();
    world_.reset(true);
    generator_.reset();
}

Env::~Env()
{
    for (size_t i(0); i < Flowers_.size(); i++) {
        delete Flowers_[i];
    }
    for (size_t i(0); i < Hives_.size(); i++) {
        delete Hives_[i];
    }
}
Env::Env()
{
    try {
        loadWorldFromFile();
    } catch (std::runtime_error error) {
        reset();
    }
}
void Env::loadWorldFromFile()
{
    world_.loadFromFile();
}
void Env::saveWorldToFile()
{
    world_.saveToFile();
}

bool Env::addFlowerAt(Vec2d p)
{
    Flower* flower (new Flower (getAppConfig()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble(), p, uniform(getAppConfig()["simulation"]["env"]["initial"]["flower"]["nectar"]["min"].toInt(), getAppConfig()["simulation"]["env"]["initial"]["flower"]["nectar"]["max"].toInt())));

    auto try1 = getCollidingHive(*flower);
    if (try1 == nullptr) {
        if ( (world_.isGrowable(p)) and (getAppConfig()["simulation"]["env"]["max flowers"].toInt() > Flowers_.size() + 1)) {

            Flowers_.push_back(flower);

            return true;
        }
    }
    return false;
}


Hive* Env::getCollidingHive(const Collider& body)
{
    for (size_t i(0); i < Hives_.size(); i++) {
        if ( Hives_[i]->isColliding(body) ) {
            return Hives_[i];
        }

    }
    return nullptr;
}

Flower* Env::getCollidingFlower(const Collider& body)
{
    for (size_t i(0); i < Flowers_.size(); i++) {
        if ( Flowers_[i]->isColliding(body) ) {
            return Flowers_[i];
        }

    }
    return nullptr;
}


void Env::drawFlowerZone(sf::RenderTarget& target, Vec2d const& position)
{

    if (world_.isGrowable(position) ) {
        auto shape = buildAnnulus(position, getAppConfig()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble(), sf::Color::Green, 5.0);
        target.draw(shape);
    } else {
        auto shape = buildAnnulus(position, getAppConfig()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble(), sf::Color::Red, 5.0);
        target.draw(shape);
    }
}

double Env::gethumidity(Vec2d p) const
{
    return world_.gethumidity(p);
}
bool Env::addHiveAt(Vec2d p)
{
    double size(getAppConfig()["simulation"]["env"] ["initial"]["hive"]["size"]["manual"].toDouble() * getAppConfig()["simulation"]["env"] ["initial"]["hive"]["hiveable factor"].toDouble());
    Collider ruche(p, size);
    auto try1 = getCollidingFlower(ruche);
    auto try2 = getCollidingHive(ruche);
    if (try1 == nullptr and try2==nullptr) {

        Hive* hivee (new Hive (p, getAppConfig()["simulation"]["env"] ["initial"]["hive"]["size"]["manual"].toDouble()));

        Hives_.push_back(hivee);

        return true;
    }
    return false;
}

bool Env::isFlyable(const Vec2d& p)
{
    return world_.isFlyable(p);    // verifie si la cellule dans laquelle se trouve la position graphique passée en argument nest pas une cellule de Roche.
}

void Env::drawHiveableZone(sf::RenderTarget& target, Vec2d const& position)
{
    if (world_.isHiveable(position)) {
        sf::RectangleShape shape(buildRectangle( { position.x-, top }, { right, bottom }, sf::Color::Green, 5.0));
        target.draw(shape);
    } else {
        sf::RectangleShape shape(buildRectangle( { left, top }, { right, bottom }, sf::Color::Red, 5.0));
        target.draw(shape);
    }
}



