#ifndef Bee_H
#define Bee_H
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>
#include <Env/Flower.hpp>
#include <Env/CFSM.hpp>

class ScoutBee;
class WorkerBee;
class Hive;
enum Moving_state { repos, aleatoire, cible };

class Bee : public Collider, public Updatable, public Drawable, public CFSM
{
private:
    Hive* home_;
    double energy_;
    Vec2d speed_;
    Vec2d* MemorisedFlower_ = nullptr;
    Moving_state MovingState_ = Moving_state::repos;
    Vec2d* cible_;
    sf::Time avoidanceClock_;
public:
    Bee(Hive* home, Vec2d center, double radius, double energy, double speed, States states);
    virtual j::Value const& getConfig() const = 0;
    void drawOn(sf::RenderTarget& target) const override;
    void update(sf::Time dt) override;
    void move(sf::Time dt);
    void targetmove(sf::Time dt);
    void randomMove(sf::Time dt);
    void addEnergie(double qt);
    virtual void interact(Bee* other) = 0;
    virtual void interactWith(ScoutBee* scouting) = 0;
    virtual void interactWith(WorkerBee* working) = 0;
    // setters
    void setMemory(Vec2d* fleur);
    void setMovingState(Moving_state etat);
    void setCible(Vec2d* cible);
    // getters
    Vec2d* getMemory() const;
    double getEnergie() const;
    Hive* getHome() const;
    Vec2d* getCible() const;
    bool isDead() const;
    void getAgressed(double energy);
    void Agress(Bee* other);

    ~Bee();
};

#endif
