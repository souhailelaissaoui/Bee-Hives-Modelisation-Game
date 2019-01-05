#ifndef Bee_H
#define Bee_H
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>
#include <Env/Collider.hpp>
#include <Env/Hive.hpp>

class Bee : public Collider, public Updatable, public Drawable
{
private:


    Hive* home_;
    double energy_;
    Vec2d speed_;

public:
    bool isDead();
    Bee(Hive* home, Vec2d center, double radius,double energy, double speed);
    virtual j::Value const& getConfig() const;
    void drawOn(sf::RenderTarget& target) const;
    void update(sf::Time dt);

};

#endif

