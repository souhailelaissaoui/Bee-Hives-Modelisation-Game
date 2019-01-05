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
    void onState(State state, sf::Time dt);
    void onEnterState(State state);
    void learnFlowerLocation(Vec2d flowerPosition);
    void drawOn(sf::RenderTarget& target) const override;

    void interact(Bee* other);
    void interactWith(ScoutBee* scouting) {}
    void interactWith(WorkerBee* working)  {
        /* ne fait rien */
    }

    static State const IN_HIVE;
    static State const GOING_TO_FLOWER;
    static State const COLLECTING;
    static State const RETURNING;
};

#endif
