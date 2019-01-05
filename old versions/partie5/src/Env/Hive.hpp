#ifndef Hive_H
#define Hive_H
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>
#include <Env/Collider.hpp>
#include <Env/Bee.hpp>

class Hive : public Collider, public Drawable, public Updatable
{
private:
    std::vector<Bee*> Bees_; // collections de pointeurs sur des abeilles
    double nectar_;

public:
    WorkerBee* addWorker(); //ajout d'une abeille butineuse à une position aléatoire dans la ruche
    ScoutBee* addScout(); //ajout d'une abeille eclaireuse à une position aléatoire dans la ruche

    void update(sf::Time dt) override; //evolue les abeilles a chaque pas de temps
    void drawOn(sf::RenderTarget& targetWindow) const override; //dessine la ruche
    void dropPollen(double qte); // ajouter du nectar
    double takeNectar(double qte); //preleve une qte de nectar de la ruche

    Hive(Vec2d cent, double r);
    Hive(const Hive&) = delete; // supprimer le constructeur de copie afin d'eviter une copie d'element trop volumineux
    Hive& operator=(Hive const&) = delete;
    ~Hive();
};

#endif
