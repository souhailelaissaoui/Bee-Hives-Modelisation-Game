#include <SFML/Graphics.hpp>
#include <iostream>
#include <Application.hpp>
#include <vector>
#include "Random/Random.hpp"
#include "Bee.hpp"
#include "Env.hpp"
#include <math.h>
#include "Utility/Utility.hpp"

Bee::Bee(Hive* home, Vec2d center, double radius,double energy, double speed):
    Collider(center, radius),
    home_(home), energy_(energy), speed_(speed*Vec2d::fromRandomAngle()) {}

j::Value const& Bee::getConfig() const
{
    return getAppConfig()["simulation"]["bees"]["generic"];
}

void Bee::drawOn(sf::RenderTarget& target) const
{
    auto const& texture = getAppTexture((getConfig()["texture"]).toString());
    auto BeeSprite = buildSprite(getPosition(), getRadius(), texture);
    if (speed_.angle() >= M_PI/2  or speed_.angle() <= -M_PI/2 ) {
        BeeSprite.scale(1, -1);
    }
    BeeSprite.rotate(speed_.angle()/DEG_TO_RAD);
    target.draw(BeeSprite);
}

void Bee::update(sf::Time dt)
{
    double proba_rot ( getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble() ) ;
    double alpha_max ( getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());
    if (bernoulli(proba_rot) ) {
        speed_.rotate(uniform(-alpha_max, alpha_max));
    }
    Vec2d position_possible ( speed_ * dt.asSeconds() );

    if (getAppEnv().isFlyable(getPosition() + position_possible)) {
        move(position_possible);
    } else {
        double beta;
        if (bernoulli(0.5)) {
            beta = M_PI/4;
        } else {
            beta = -M_PI/4;
        }
        speed_.rotate(beta);
    }
    energy_ -= 0.1 * dt.asSeconds();
    if (energy_ <=0) {
        energy_ = 0;
    }
};

bool Bee::isDead()
{
    if (energy_==0) {
        return true;
    } else {
        return false;
    }
}




