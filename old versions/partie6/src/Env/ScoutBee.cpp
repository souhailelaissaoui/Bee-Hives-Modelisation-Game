#include <SFML/Graphics.hpp>
#include <iostream>
#include <Application.hpp>
#include <vector>
#include "Random/Random.hpp"
#include "ScoutBee.hpp"
#include "WorkerBee.hpp"
#include "Env.hpp"
#include <math.h>
#include "Utility/Utility.hpp"
#include <string>

State const ScoutBee::IN_HIVE = createUid();
State const ScoutBee::SEARCHING = createUid();
State const ScoutBee::RETURNING = createUid();

ScoutBee::ScoutBee(Hive* home, Vec2d center)
    : Bee(home, center,
          getAppConfig()["simulation"]["bees"]["scout"]["size"].toDouble(),
          getAppConfig()["simulation"]["bees"]["scout"]["energy"]["initial"].toDouble(),
          getAppConfig()["simulation"]["bees"]["scout"]["speed"].toDouble(), { IN_HIVE, SEARCHING, RETURNING })
{
}

j::Value const& ScoutBee::getConfig() const
{
    return getAppConfig()["simulation"]["bees"]["scout"];
}

void ScoutBee::onState(State state, sf::Time dt)
{

    if (state == ScoutBee::IN_HIVE) {
        if (getEnergie() < getConfig()["energy"]["to leave hive"].toDouble()) {
            double rate = getConfig()["energy"]["consumption rates"]["eating"].toDouble();
            double consumed = getHome()->takeNectar(dt.asSeconds() * rate);
            addEnergie(consumed);
        } else {
            if (getMemory() == nullptr) {
                nextState();
            }
        }
        if (nbShares >= getConfig()["sharing"]["max"].toDouble()) {
            setMemory(nullptr);
        }
    }
    if (state == ScoutBee::SEARCHING) {
        Collider test(getPosition(), getRadius() + getConfig()["visibility range"].toDouble());
        Flower* fleur = getAppEnv().getCollidingFlower(test);
        if (fleur != nullptr) {
            setMemory(new Vec2d (fleur->getPosition()));
            nextState();
        }
        if (getEnergie() < getConfig()["energy"]["to seek flowers"].toDouble()) {
            nextState();
        }
    }
    if (state == ScoutBee::RETURNING) {
        if (getHome()->isColliderInside(*this)) {
            nextState();
        }
    }
}

void ScoutBee::onEnterState(State state)
{
    if (state == ScoutBee::IN_HIVE) {
        setMovingState(Moving_state::repos);
        nbShares = 0;
    }
    if (state == ScoutBee::RETURNING) {
        setCible(new Vec2d(getHome()->getPosition()));
        setMovingState(Moving_state::cible);
    }
    if (state == ScoutBee::SEARCHING) {
        setMemory(nullptr);
        setMovingState(Moving_state::aleatoire);
    }
}

void ScoutBee::drawOn(sf::RenderTarget& target) const
{
    Bee::drawOn(target);
    if (isDebugOn()) {
        Vec2d pos = getPosition();
        pos.y += 10;
        auto const text = buildText("Scout: energy " + to_nice_string(getEnergie()), pos, getAppFont(), 10, sf::Color::White);
        target.draw(text);
        std::string base;
        std::string next;
        if (this->getState() == IN_HIVE) {
            base = "in_hive_";

            if (getEnergie() < getConfig()["energy"]["to leave hive"].toDouble()) {
                next = "eat";
            } else {
                if (getMemory() == nullptr) {
                    next = "leaving";
                } else {
                    next = "sharing[" + std::to_string(nbShares) + "]";
                }
            }
        }
        if (this->getState() == RETURNING) {
            base = "back_to_hive";
        }
        if (this->getState() == SEARCHING) {
            base = "seeking_flower";
        }

        Vec2d pos2 = getPosition();
        pos2.y += 18;
        auto const text2 = buildText(base + next, pos2, getAppFont(), 10, sf::Color::White);
        target.draw(text2);
    }
}

void ScoutBee::interact(Bee* other)
{
    other->interactWith(this);
}

void ScoutBee::interactWith(WorkerBee* working)
{
    if (nbShares <= getConfig()["sharing"]["max"].toDouble()) {
        if (getMemory() != nullptr) {
            working->setMemory(getMemory());
            nbShares++;
        }
    }
}
