#ifndef WorkerBee_H
#define WorkerBee_H
#include <Env/Bee.hpp>

class WorkerBee : public Bee
{
private:
    double nectar_ = 0;

public:
    WorkerBee(Hive* home, Vec2d center);

    j::Value const& getConfig() const;

    void onState(State state, sf::Time dt);//indique ce que l'automate doit faire
    void onEnterState(State state);//réalise les actions à entreprendre lorsque l'on entre dans un état donné
    void learnFlowerLocation(Vec2d flowerPosition);
    void drawOn(sf::RenderTarget& target) const override final;//dessine les abeilles butineuses

    void interact(Bee* other);
    void interactWith(ScoutBee* scouting) {}
    void interactWith(WorkerBee* working)  {
        /* ne fait rien */
    }

    static State const IN_HIVE;//etat dans la ruche
    static State const GOING_TO_FLOWER;//etat deplacement vers la fleur
    static State const COLLECTING;//etat collecte de nectar
    static State const RETURNING; //etat retour a la ruche
    ~WorkerBee() {}

};

#endif
