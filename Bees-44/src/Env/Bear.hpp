#ifndef Bear_H
#define Bear_H
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>
#include <Env/CFSM.hpp>
#include <Env/Bee.hpp>

class Hive;

class Bear : public Collider, public Updatable, public Drawable, public CFSM
{
private:
    double energy_;
    Vec2d speed_;
    Moving_state MovingState_ = Moving_state::aleatoire;
    Vec2d* cible_;
    Vec2d* memory_ = nullptr;
    sf::Time avoidanceClock_;
    sf::Time HungryClock_;
public:
    void onState(State state, sf::Time dt);
    void onEnterState(State state);
    Bear(Vec2d center);
    void drawOn(sf::RenderTarget& target) const override;
    void update(sf::Time dt) override;
    void move(sf::Time dt);
    void targetmove(sf::Time dt);
    void randomMove(sf::Time dt);
    // setters
    void setMovingState(Moving_state etat);
    void setCible(Vec2d* cible);
    // getters
    double getEnergie() const;
    Vec2d* getCible() const;
    j::Value const& getConfig() const;
    bool isDead() const;

    Vec2d* getMemory() const;
    static State const GOING_TO_HIVE;
    static State const COLLECTING;
    static State const WALKING;
    ~Bear();
};

#endif
