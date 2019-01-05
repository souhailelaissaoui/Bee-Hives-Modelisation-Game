#include <SFML/Graphics.hpp>
#include <iostream>
#include <Application.hpp>
#include <vector>
#include "Random/Random.hpp"
#include "WorkerBee.hpp"
#include "Env.hpp"
#include <math.h>
#include "Utility/Utility.hpp"

State const WorkerBee::IN_HIVE  = createUid();
State const WorkerBee::GOING_TO_FLOWER = createUid();
State const WorkerBee::COLLECTING  = createUid();
State const WorkerBee::RETURNING  = createUid();

WorkerBee::WorkerBee(Hive* home, Vec2d center):
    Bee(home,center, getConfig()["size"].toDouble()
        , getConfig()["energy"]["initial"].toDouble()
        , getConfig()["speed"].toDouble()
        , {IN_HIVE, GOING_TO_FLOWER,COLLECTING,RETURNING}) {}

j::Value const& WorkerBee::getConfig() const
{
    return getAppConfig()["simulation"]["bees"]["worker"];
}

void WorkerBee::onState(State state, sf::Time dt)
{
    if (state == WorkerBee::IN_HIVE) {
        if ( nectar_ > 0 ) {
            double rate = getConfig()["transfer rate"].toDouble();
            double transferred = rate * dt.asSeconds();
            if (nectar_ < transferred) {
                transferred = nectar_;
                nectar_ = 0;
            } else {
                nectar_ -= transferred;
            }
            getHome()->dropPollen(transferred);
        } else {
            if ( getEnergie() < getConfig()["energy"]["to leave hive"].toDouble() ) {
                double rate = getConfig()["energy"]["consumption rates"]["eating"].toDouble();
                double consumed = getHome()->takeNectar(dt.asSeconds() * rate);
                addEnergie(consumed);
            } else {
                if (getMemory() != nullptr) {
                    nextState();
                }
            }
        }
    }
    if (state == WorkerBee::GOING_TO_FLOWER) {
        if (isPointInside(*getCible())) {
            nextState();
        }
    }
    if (state == WorkerBee::COLLECTING) {
        Flower* fleur = getAppEnv().getCollidingFlower(*this);
        if (fleur == nullptr) {
            nextState();
        } else {
            if (nectar_ < getConfig()["max pollen capacity"].toDouble() ) {
                double rate = getConfig()["harvest rate"].toDouble();
                nectar_ += fleur->takePollen(rate * dt.asSeconds());
            } else {
                nextState();
            }
        }
    }
    if (state == WorkerBee::RETURNING) {
        if ( getHome()->isColliderInside(*this) ) {
            nextState();
        }
    }
}

void WorkerBee::onEnterState(State state)
{
    if (state == WorkerBee::IN_HIVE) {
        setMovingState(Moving_state::repos);
    }
    if (state == WorkerBee::GOING_TO_FLOWER) {
        setCible(getMemory());
        setMovingState(Moving_state::cible);
    }
    if (state == WorkerBee::COLLECTING) {
        setMovingState(Moving_state::repos);
    }
    if (state == WorkerBee::RETURNING) {
        setCible(new Vec2d(getHome()->getPosition()) );
        setMovingState(Moving_state::cible);
        setMemory(nullptr);
    }
}

void WorkerBee::learnFlowerLocation( Vec2d flowerPosition)
{
    setMemory(new Vec2d(flowerPosition));
}

void WorkerBee::drawOn(sf::RenderTarget& target) const
{
    Bee::drawOn(target);
    if (isDebugOn() ) {
        Vec2d pos = getPosition();
        pos.y +=10;
        auto const text = buildText("Worker: energy "+to_nice_string(getEnergie()), pos, getAppFont(), 10, sf::Color::White);
        target.draw(text);
        std::string base;
        std::string next;
        if (this->getState() == IN_HIVE) {
            base= "in_hive_";
            if ( nectar_ > 0 ) {
                next = "pollen";
            } else {
                if ( getEnergie() < getConfig()["energy"]["to leave hive"].toDouble() ) {
                    next = "eat";
                } else {
                    if (getMemory() ==nullptr) {
                        next = "no_flower";
                    }
                }
            }
            if ( (getMemory() !=nullptr) and (nectar_ == 0) and ( getEnergie() < getConfig()["energy"]["to leave hive"].toDouble() ) ) {
                next = "???";
            }
        }
        if (this->getState() == GOING_TO_FLOWER) {
            base = "to_flower";
        }
        if (this->getState() == COLLECTING) {
            base = "collecting_pollen";
        }
        if (this->getState() == RETURNING) {
            base = "back_to_hive";
        }
        Vec2d pos2 = getPosition();
        pos2.y +=getRadius();
        auto const text2 = buildText(base+next, pos2, getAppFont(), 10, sf::Color::White);
        target.draw(text2);
    }
}

void WorkerBee::interact(Bee* other)
{
    other->interactWith(this);
}
