#include <SFML/Graphics.hpp>
#include <iostream>
#include <Application.hpp>
#include <vector>
#include "Random/Random.hpp"
#include "Bear.hpp"
#include "Env.hpp"
#include <math.h>
#include "Utility/Utility.hpp"
State const Bear::GOING_TO_HIVE = createUid();
State const Bear::COLLECTING = createUid();
State const Bear::WALKING = createUid();
Bear::Bear(Vec2d center)
    : Collider(center, getConfig()["radius"].toDouble())
    , CFSM( { WALKING, GOING_TO_HIVE, COLLECTING })
, energy_(getConfig()["energy"].toDouble())
, speed_(getConfig()["speed"].toDouble() * Vec2d::fromRandomAngle())
{
}

double Bear::getEnergie() const
{
    return energy_;
}

void Bear::drawOn(sf::RenderTarget& target) const
{
    if (isDebugOn()) {
        Vec2d pos = getPosition();
        pos.y += 10;
        auto const text = buildText("Bear: energy " + to_nice_string(getEnergie()), pos, getAppFont(), 10, sf::Color::White);
        target.draw(text);
    }

    auto const& texture = getAppTexture((getConfig()["texture"]).toString());
    auto BearSprite = buildSprite(getPosition(), getRadius(), texture);

    target.draw(BearSprite);
    if (isDebugOn()) {
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

void Bear::update(sf::Time dt)
{
    this->action(dt);
    move(dt);
}
void Bear::randomMove(sf::Time dt)
{
    double proba_rot(getConfig()["moving behaviour"]["random"]["rotation probability"].toDouble());
    double alpha_max(getConfig()["moving behaviour"]["random"]["rotation angle max"].toDouble());
    if (bernoulli(proba_rot)) {
        speed_.rotate(uniform(-alpha_max, alpha_max));
    }
    Vec2d position_possible(speed_ * dt.asSeconds());

    if (getAppEnv().isFlyable(getPosition() + position_possible)) {
        Collider::move(position_possible);
    } else {
        double beta;
        if (bernoulli(0.5)) {
            beta = M_PI / 4;
        } else {
            beta = -M_PI / 4;
        }
        speed_.rotate(beta);
    }
}

bool Bear::isDead() const
{
    if (energy_ == 0) {
        return true;
    } else {
        return false;
    }
}
void Bear::move(sf::Time dt)
{
    if (MovingState_ == Moving_state::cible) {
        targetmove(dt);
        double taux = this->getConfig()["consumption rates"]["moving"].toDouble();
        energy_ -= taux * dt.asSeconds();
        if (energy_ <= 0) {
            energy_ = 0;
        }
    }
    if (MovingState_ == Moving_state::aleatoire) {
        randomMove(dt);
        double taux = this->getConfig()["consumption rates"]["moving"].toDouble();
        energy_ -= taux * dt.asSeconds();
        if (energy_ <= 0) {
            energy_ = 0;
        }
    }

    if (MovingState_ == Moving_state::repos) {
        double taux = this->getConfig()["consumption rates"]["idle"].toDouble();
        energy_ -= taux * dt.asSeconds();
        if (energy_ <= 0) {
            energy_ = 0;
        }
    }
}

void Bear::targetmove(sf::Time dt)
{
    Vec2d position_possible;
    if (avoidanceClock_ < sf::Time::Zero) {
        speed_ = directionTo(*cible_) * speed_.length() / distanceTo(*cible_);
    } else {
        avoidanceClock_ -= dt;
    }
    position_possible = speed_ * dt.asSeconds();
    if (getAppEnv().isFlyable(getPosition() + position_possible)) {
        Collider::move(position_possible);
    } else {
        avoidanceClock_ = sf::seconds(getConfig()["avoidance delay"].toDouble());
        double beta;
        if (bernoulli(0.5)) {
            beta = M_PI / 4;
        } else {
            beta = -M_PI / 4;
        }
        speed_.rotate(beta);
    }
}

Vec2d* Bear::getCible() const
{
    return cible_;
}

void Bear::setMovingState(Moving_state etat)
{
    MovingState_ = etat;
}
void Bear::setCible(Vec2d* cible)
{
    cible_ = cible;
}
Bear::~Bear()
{
}

j::Value const& Bear::getConfig() const
{
    return getAppConfig()["simulation"]["bear"];
}

void Bear::onState(State state, sf::Time dt)
{
    if (state == Bear::WALKING) {
        if ((energy_ < getConfig()["hungry"].toDouble()) and (memory_ != nullptr)) {
            nextState();
        }
        Hive* actuel(getAppEnv().getCollidingHive(*this));
        if ((memory_ == nullptr) and (actuel != nullptr)) {
            memory_ = new Vec2d(actuel->getPosition());
        }
    }
    if (state == Bear::GOING_TO_HIVE) {
        if (distanceTo(*getCible()) < getConfig()["distancecible"].toDouble()) {
            nextState();
        }
    }
    if (state == Bear::COLLECTING) {
        Hive* hive = getAppEnv().getCollidingHive(*this);
        if (hive == nullptr) {
            nextState();
            memory_ = nullptr;
        } else {

            if (energy_ < getConfig()["rassasie"].toDouble()) {

                double rate = getConfig()["consumption rates"]["eating"].toDouble();
                energy_ += hive->takeNectar(rate * dt.asSeconds());
            } else {
                nextState();
            }
        }
    }
}

void Bear::onEnterState(State state)
{
    if (state == Bear::WALKING) {
        setMovingState(Moving_state::aleatoire);
        setCible(nullptr);
    }
    if (state == Bear::GOING_TO_HIVE) {
        setMovingState(Moving_state::cible);
        setCible(memory_);
    }
    if (state == Bear::COLLECTING) {
        setCible(nullptr);
        setMovingState(Moving_state::repos);
    }
}

Vec2d* Bear::getMemory() const
{
    return memory_;
}
