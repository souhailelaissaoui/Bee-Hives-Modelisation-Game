#include <SFML/Graphics.hpp>
#include <iostream>
#include <Application.hpp>
#include <vector>
#include "Random/Random.hpp"
#include "Bee.hpp"
#include "Env.hpp"
#include <math.h>
#include "Utility/Utility.hpp"

Bee::Bee(Hive* home, Vec2d center, double radius,double energy, double speed, States states):
    Collider(center, radius), CFSM(states) ,
    home_(home), energy_(energy), speed_(speed*Vec2d::fromRandomAngle()) {}



double  Bee::getEnergie() const
{
    return energy_;
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
    if (isDebugOn() ) {
        if (MovingState_ == Moving_state::aleatoire) {
            auto shape = buildAnnulus(getPosition(), getRadius(), sf::Color::Black, 5.0);
            target.draw(shape);
        }
        if (MovingState_ == Moving_state::cible) {
            auto shape = buildAnnulus(getPosition(), getRadius(), sf::Color::Blue, 3.0);
            target.draw(shape);
        }
    }

}

void Bee::update(sf::Time dt)
{
    this->action(dt);
    move(dt);

}
void Bee::randomMove(sf::Time dt)
{
    double proba_rot ( getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble() ) ;
    double alpha_max ( getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());
    if (bernoulli(proba_rot) ) {
        speed_.rotate(uniform(-alpha_max, alpha_max));
    }
    Vec2d position_possible ( speed_ * dt.asSeconds() );

    if (getAppEnv().isFlyable(getPosition() + position_possible)) {
        Collider::move(position_possible);
    } else {
        double beta;
        if (bernoulli(0.5)) {
            beta = M_PI/4;
        } else {
            beta = -M_PI/4;
        }
        speed_.rotate(beta);
    }

}

bool Bee::isDead() const
{
    if (energy_==0) {
        return true;
    } else {
        return false;
    }
}
void Bee::move(sf::Time dt)
{
    if (MovingState_ == Moving_state::cible) {
        targetmove(dt);
        double taux = this->getConfig()["energy"]["consumption rates"]["moving"].toDouble();
        energy_ -= taux * dt.asSeconds();
        if (energy_ <=0) {
            energy_ = 0;
        }
    }
    if (MovingState_ == Moving_state::aleatoire) {
        randomMove(dt);
        double taux = this->getConfig()["energy"]["consumption rates"]["moving"].toDouble();
        energy_ -= taux * dt.asSeconds();
        if (energy_ <=0) {
            energy_ = 0;
        }
    }

    if (MovingState_ == Moving_state::repos) {
        double taux = this->getConfig()["energy"]["consumption rates"]["idle"].toDouble();
        energy_ -= taux * dt.asSeconds();
        if (energy_ <=0) {
            energy_ = 0;
        }
    }
}

void Bee::targetmove(sf::Time dt)
{
    Vec2d position_possible;
    if (avoidanceClock_< sf::Time::Zero) {
        speed_ = directionTo(*cible_) * speed_.length() /distanceTo(*cible_);
    } else {
        avoidanceClock_ -= dt;
    }
    position_possible = speed_ * dt.asSeconds();
    if (getAppEnv().isFlyable(getPosition() + position_possible)) {
        Collider::move(position_possible);
    } else {
        avoidanceClock_ = sf::seconds(getConfig()["moving behaviour"]["target"]["avoidance delay"].toDouble());
        double beta;
        if (bernoulli(0.5)) {
            beta = M_PI/4;
        } else {
            beta = -M_PI/4;
        }
        speed_.rotate(beta);

    }
}

void Bee::addEnergie(double qt)
{
    energy_ += qt;
}

Vec2d* Bee::getMemory() const
{
    return MemorisedFlower_;
}
Vec2d* Bee::getCible() const
{
    return cible_;
}
Hive* Bee::getHome() const
{
    return home_;
}
void Bee::setMemory(Vec2d* fleur)
{
    MemorisedFlower_ = fleur;
}
void Bee::setMovingState(Moving_state etat)
{
    MovingState_ = etat;
}
void Bee::setCible(Vec2d* cible)
{
    cible_ = cible;
}
Bee::~Bee() {}
