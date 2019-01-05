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
    Moving_state MovingState_ = Moving_state::repos; // etat de mouvement
    Vec2d* cible_;
    sf::Time avoidanceClock_; // Temps pendant lequel l'abeille ignore sa cible
public:
    Bee(Hive* home, Vec2d center, double radius, double energy, double speed, States states);
    virtual j::Value const& getConfig() const = 0;
    void drawOn(sf::RenderTarget& target) const override; // dessin
    void update(sf::Time dt) override; // mise a jour de l'abeille
    void move(sf::Time dt); // deplacement de l'abeille en fonction de son etat
    void targetmove(sf::Time dt); // deplacement cible
    void randomMove(sf::Time dt); // deplacement aleatoire
    void addEnergie(double qt); // ajout d'energie
    virtual void interact(Bee* other) = 0; //  permet l'interaction entre les abeilles grace a une methode polymorphique
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
    bool isDead() const; // verifie si l'abeille est morte

    ~Bee();
};

#endif
