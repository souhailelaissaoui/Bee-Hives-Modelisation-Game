#ifndef Hive_H
#define Hive_H
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>

#include <Env/Collider.hpp>
class Bee; // prédéclaration
class Hive : public Collider, public Drawable, public Updatable
{
private:
    std::vector<Bee*> Bees_; // collections de pointeurs sur des abeilles
    double nectar_;
public:
    void addBee();
    void update(sf::Time dt);
    void drawOn(sf::RenderTarget& targetWindow) const;
    void dropPollen(double qte);
    double takeNectar(double qte);
    Hive(Vec2d cent, double r);
    Hive(const Hive &) = delete; // supprimer le constructeur de copie afin d'eviter une copie d'element trop volumineux
    Hive& operator=(Hive const&) = delete;
    ~Hive();

};

#endif

