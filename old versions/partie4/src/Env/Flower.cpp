#include <SFML/Graphics.hpp>
#include <iostream>
#include <Application.hpp>
#include <vector>
#include "Random/Random.hpp"
#include "Flower.hpp"
#include "Env.hpp"
#include <math.h>
#include "Utility/Utility.hpp"

Flower::Flower(double radius, Vec2d center, double pollen)
    : Collider(center, radius)
    , pollen_(pollen)
    , indice_(uniform(0, static_cast<int>((getAppConfig()["simulation"]["flower"]["textures"]).size() - 1)     )) {};
double Flower::takePollen(double pollen)
{
    if (pollen > pollen_) {
        pollen = pollen_;
        pollen_ = 0;
        return pollen;
    } else {
        pollen_ -= pollen;
        return pollen;
    };
}

void Flower::drawOn(sf::RenderTarget& target) const
{
    auto const& texture = getAppTexture((getAppConfig()["simulation"]["flower"]["textures"])[indice_].toString());
    auto flowerSprite = buildSprite(getPosition(), getRadius(), texture);
    target.draw(flowerSprite);
}

void Flower::update(sf::Time dt)
{
    double seuil = (getAppConfig()["simulation"]["flower"]["growth"]["threshold"].toDouble());
    double humidity = getAppEnv().gethumidity(getPosition());
    double add = dt.asSeconds() * std::log( humidity / seuil);
    if (add < 0) {
        takePollen(-add);
    } else {
        pollen_ = pollen_ + add;
    }
    double seuill = getAppConfig()["simulation"]["flower"]["growth"]["split"].toDouble();
    if (pollen_ >= seuill) {
        pollen_ = pollen_ / 2.0;


        double d = uniform(0.5 * getRadius(), 2.0 * getRadius());
        int tryy = 0;
        do {
            tryy += 1;
        } while ((getAppEnv().addFlowerAt(getPosition() + Vec2d::fromRandomAngle() * d) == false) and (tryy < 100));

    }
}
double Flower::getPollen () const
{
    return pollen_;
};
Flower::~Flower() {};
