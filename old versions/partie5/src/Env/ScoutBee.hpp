#ifndef ScoutBee_H
#define ScoutBee_H
#include <Env/Bee.hpp>

class ScoutBee : public Bee
{

private:
    int nbShares_ = 0; //nombre de fois que l'abeille a partage sa position

public:
    ScoutBee(Hive* home, Vec2d center);

    j::Value const& getConfig() const;

    void onState(State state, sf::Time dt);
    void onEnterState(State state);
    void drawOn(sf::RenderTarget& target) const override final; //dessine les abeille eclaireuses

    static State const IN_HIVE; //etat dans la ruche
    static State const SEARCHING; // etat recherche des fleurs
    static State const RETURNING; //etat retour a la ruche

    void interact(Bee* other); //interaction avec une autre abeille
    void interactWith(ScoutBee* scouting) {
        /*ne fait rien */
    }
    void interactWith(WorkerBee* working);
};
#endif
