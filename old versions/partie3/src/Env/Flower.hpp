#ifndef Flower_H
#define Flower_H

#include "Collider.hpp"
#include <Interface/Drawable.hpp>
#include <Interface/Updatable.hpp>
class Flower : public Collider , public Drawable, public Updatable  // Une fleur est un Collider : heritage
{

private:
    double pollen_; // quantité de pollen de la fleur
    double indice_; // indice de la texture
public:
    Flower(double radius, Vec2d center, double pollen); // constructeur
    double takePollen(double pollen); // retourne la quantité de pollen qui a pu être prélevée
    void drawOn(sf::RenderTarget& target) const; // Permet de dessiner la fleur
    void update(sf::Time dt); // met a jour la quantité de pollen de la fleur
    double getPollen (); // un getteur pour la quantité de pollen
    ~Flower(); // destructeur
};

#endif
