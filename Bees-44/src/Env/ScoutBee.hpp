#ifndef ScoutBee_H
#define ScoutBee_H
#include <Env/Bee.hpp>

class ScoutBee : public Bee
{
private:
    double nbShares = 0;
public:
    ScoutBee(Hive* home, Vec2d center);
    j::Value const& getConfig() const;
    void onState(State state, sf::Time dt);
    void onEnterState(State state);
    void drawOn(sf::RenderTarget& target) const override;
    static State const IN_HIVE;
    static State const SEARCHING;
    static State const RETURNING;
    void interact(Bee* other);
    void interactWith(ScoutBee* scouting) {
        /*ne fait rien */
    }
    void interactWith(WorkerBee* working);
};
#endif
