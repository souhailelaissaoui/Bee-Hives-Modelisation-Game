#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include "CFSM.hpp"
#include "Utility/Utility.hpp"

CFSM::CFSM(States states)
    : state_(0)
    , states_(states)
{
}

State CFSM::getState() const
{
    return states_[state_];
}
void CFSM::nextState()
{
    state_ += 1;
    if (state_ == states_.size()) {
        state_ = 0;
    }
    onEnterState(states_[state_]);
}
void CFSM::onState(State state, sf::Time dt) {}

void CFSM::action(sf::Time dt)
{
    onState(states_[state_], dt);
}

void CFSM::onEnterState(State state) {}
CFSM::~CFSM() {};
