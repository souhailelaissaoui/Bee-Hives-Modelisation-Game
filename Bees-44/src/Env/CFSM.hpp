#ifndef CFSM_H
#define CFSM_H
#include <Utility/Utility.hpp>
#include <vector>
typedef Uid State;
typedef std::vector<State> States;
class CFSM
{
private:
    size_t state_;
    States states_;
public:
    CFSM(States states);
    State getState() const;
    void nextState();
    virtual void onState(State state, sf::Time dt);
    void action(sf::Time dt);
    virtual void onEnterState(State state);
    ~CFSM();
};

#endif
