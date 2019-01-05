#ifndef CFSM_H
#define CFSM_H
#include <Utility/Utility.hpp>
#include <vector>
typedef Uid State;
typedef std::vector<State> States; //ensemble d'etats

class CFSM
{
private:
    size_t state_;
    States states_;

public:
    CFSM(States states);
    State getState() const; //permet de consulter l'état courant
    void nextState(); // permet de passer à l'état suivant dans la liste de façon circulaire
    virtual void onState(State state, sf::Time dt); //indique ce que l'automate doit faire
    virtual void onEnterState(State state); //indique ce que doit faire l'automate dans son état courant
    void action(sf::Time dt);
    ~CFSM();
};

#endif
