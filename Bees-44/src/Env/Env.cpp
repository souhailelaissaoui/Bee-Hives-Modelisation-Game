#include "Env.hpp"
#include <iostream>
#include <cassert>
#include <Application.hpp>
#include <vector>
#include <Utility/Utility.hpp>
#include "Random/Random.hpp"

Bee* Env::getBeeAt(Vec2d p) const
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
        if (Hives_[i]->isDead()) {
            delete Hives_[i];
            Hives_[i] = nullptr;
        }
    }
    Hives_.erase(std::remove(Hives_.begin(), Hives_.end(), nullptr), Hives_.end());

    for (size_t i(0); i < Bears_.size(); i++) {
        Bears_[i]->update(dt);
        if (Bears_[i]->isDead()) {
            delete Bears_[i];
            Bears_[i] = nullptr;
        }
    }
    Bears_.erase(std::remove(Bears_.begin(), Bears_.end(), nullptr), Bears_.end());
}

void Env::drawOn(sf::RenderTarget& target) const
{
    world_.drawOn(target);
    for (size_t i(0); i < Flowers_.size(); i++) {
        Flowers_[i]->drawOn(target);
    }
    for (size_t i(0); i < Bears_.size(); i++) {
        Bears_[i]->drawOn(target);
    }
    for (size_t i(0); i < Hives_.size(); i++) {
        Hives_[i]->drawOn(target);
    }
    if (isDebugOn()) {
        auto const text = buildText(to_nice_string(world_.gethumidity(getApp().getCursorPositionInView())), getApp().getCursorPositionInView(), getAppFont(), 30, sf::Color::Red);
        target.draw(text);
    }
    if (creating_state != 0) {
        world_.drawCreatingZone(target);
        auto text(buildText("Creating mode : Water", getApp().getCursorPositionInView(), getAppFont(), 30, sf::Color::Red));

        if (creating_state == 2) {
            text = buildText("Creating mode : Grass", getApp().getCursorPositionInView(), getAppFont(), 30, sf::Color::Red);
        }

        target.draw(text);
    }
}

void Env::reset()
{
    for (size_t i(0); i < Bears_.size(); i++) {
        delete Bears_[i];
    }

    for (size_t i(0); i < Flowers_.size(); i++) {
        delete Flowers_[i];
    }
    for (size_t i(0); i < Hives_.size(); i++) {
        delete Hives_[i];
    }
    Flowers_.clear();
    Hives_.clear();
    world_.reset(true);
    generator_.reset();
    Bears_.clear();
}

Env::~Env()
{
    for (size_t i(0); i < Flowers_.size(); i++) {
        delete Flowers_[i];
    }
    for (size_t i(0); i < Bears_.size(); i++) {
        delete Bears_[i];
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

    for (size_t i(0); i < Bears_.size(); i++) {
        delete Bears_[i];
    }
    for (size_t i(0); i < Flowers_.size(); i++) {
        delete Flowers_[i];
    }
    for (size_t i(0); i < Hives_.size(); i++) {
        delete Hives_[i];
    }
    world_.loadFromFile();
    Flowers_.clear();
    Hives_.clear();
    generator_.reset();
    Bears_.clear();
}
void Env::saveWorldToFile()
{
    world_.saveToFile();
}

bool Env::addFlowerAt(Vec2d p)
{
    bool ans = false;
    if (getAppConfig()["simulation"]["env"]["max flowers"].toInt() >= Flowers_.size()) {
        Flower* flower(new Flower(getAppConfig()["simulation"]["env"]["initial"]["flower"]["size"]["manual"].toDouble(), p, uniform(getAppConfig()["simulation"]["env"]["initial"]["flower"]["nectar"]["min"].toInt(), getAppConfig()["simulation"]["env"]["initial"]["flower"]["nectar"]["max"].toInt())));

        auto try1 = getCollidingHive(*flower);
        if ((try1 == nullptr) and (world_.isGrowable(p))) {
            ans = true;

            Flowers_.push_back(flower);
        }
    } else {
        ans = true;
    }
    return ans;
}

bool Env::addBear()
{
    Vec2d p;
    do {

        p.x = (uniform(0, static_cast<int>(getApp().getWorldSize().x)));
        p.y = (uniform(0, static_cast<int>(getApp().getWorldSize().y)));

    } while (isFlyable(p) == false);
    Bears_.push_back(new Bear(p));
    std::cout << "New Bear Created" << std::endl;
}

Hive* Env::getCollidingHive(const Collider& body) const
{
    for (size_t i(0); i < Hives_.size(); i++) {
        if (Hives_[i]->isColliding(body)) {
            return Hives_[i];
        }
    }
    return nullptr;
}

Flower* Env::getCollidingFlower(const Collider& body) const
{
    for (size_t i(0); i < Flowers_.size(); i++) {
        if (Flowers_[i]->isColliding(body)) {
            return Flowers_[i];
        }
    }
    return nullptr;
}

void Env::drawFlowerZone(sf::RenderTarget& target, Vec2d const& position) const
{

    if (world_.isGrowable(position)) {
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
    double size(getAppConfig()["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble() * getAppConfig()["simulation"]["env"]["initial"]["hive"]["hiveable factor"].toDouble());

    if (world_.isHiveable(p, size)) {
        Collider ruche(p, size);
        auto try1 = getCollidingFlower(ruche);
        auto try2 = getCollidingHive(ruche);
        if (try1 == nullptr and try2 == nullptr) {
            Hives_.push_back(new Hive(p, getAppConfig()["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble()));

            return true;
        }
    }
    return false;
}

bool Env::isFlyable(const Vec2d& p)
{
    return world_.isFlyable(p);    // verifie si la cellule dans laquelle se trouve la position graphique passée en argument nest pas une cellule de Roche.
}

void Env::drawHiveableZone(sf::RenderTarget& target, Vec2d const& position) const
{

    double size(getAppConfig()["simulation"]["env"]["initial"]["hive"]["size"]["manual"].toDouble() * getAppConfig()["simulation"]["env"]["initial"]["hive"]["hiveable factor"].toDouble());

    Vec2d topLeft(position.x - size, position.y - size);
    Vec2d bottomRight(position.x + size, position.y + size);
    //bool swapped_y(topLeft.y > bottomRight.y);
    //bool swapped_x(topLeft.x > bottomRight.x);

    Collider ruche(position, size);
    auto try1 = getCollidingFlower(ruche);
    auto try2 = getCollidingHive(ruche);

    if (world_.isHiveable(position, size)) {

        if (try1 == nullptr and try2 == nullptr) {

            sf::RectangleShape shape(buildRectangle(topLeft, bottomRight, sf::Color::Green, 5.0));
            target.draw(shape);
        } else {
            sf::RectangleShape shape(buildRectangle(topLeft, bottomRight, sf::Color::Blue, 5.0));
            target.draw(shape);
        }
    } else {
        sf::RectangleShape shape(buildRectangle(topLeft, bottomRight, sf::Color::Red, 5.0));
        target.draw(shape);
    }
}

std::vector<Flower*> Env::getFlowers() const
{
    return Flowers_;
}
void Env::NextCreateState()
{
    if (creating_state != 0) {
        creating_state += 1;
        if (creating_state == 3) {
            creating_state = 0;
            world_.smooths(100, true);
            getApp().togglePause();
        }
    }
}
void Env::CreateWorld()
{
    if (creating_state == 1) {
        world_.AddWater(getApp().getCursorPositionInView());
    }
    if (creating_state == 2) {
        world_.AddGrass(getApp().getCursorPositionInView());
    }
    if (creating_state == 0) {
        for (size_t i(0); i < Flowers_.size(); i++) {
            delete Flowers_[i];
        }
        for (size_t i(0); i < Hives_.size(); i++) {
            delete Hives_[i];
        }
        Flowers_.clear();
        Hives_.clear();
        Bears_.clear();
        world_.reloadConfig();
        world_.reloadCacheStructure();
        world_.updateCache();
        creating_state = 1;
        getApp().togglePause();
    }
}

void Env::EditCreatingSize(bool mode)
{
    world_.EditCreatingSize(mode);
}
