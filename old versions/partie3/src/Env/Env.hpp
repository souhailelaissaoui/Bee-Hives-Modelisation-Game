#include <Utility/Vec2d.hpp>
#include "World.hpp"
#include "Flower.hpp"
#include "FlowerGenerator.hpp"


#ifndef Env_H
#define Env_H

class Env // ne peut pas heriter de Drawable et Updatable car se sont des classes abtraites, et Env est utilisé parfois sans passer par des pointeurs.
{
private:
    World world_; // terrain
    FlowerGenerator generator_; // permet la generation de fleur automatique
public :
    Env(const Env &) = delete; // supprimer le constructeur de copie afin d'eviter une copie d'element trop volumineux
    Env(); // constructeur
    ~Env(); //destructeur
    void update(sf::Time dt); // met a jour notre environnement: maj du genetor_, et des fleurs.
    void drawOn(sf::RenderTarget& target); // methode de dessin principale, va dessiner les fleurs et le monde
    void reset(); // supprime l'environnement actuel et en genere un nouveau
    void loadWorldFromFile(); // charge le world_ a partir d'un fichier .map
    void saveWorldToFile(); // sauvegarde le world_ dans un fichier .map
    std::vector<Flower*> Flowers_; // collections de pointeurs sur des fleurs
    bool addFlowerAt(Vec2d p); // ajoute une fleur a la position p si cela est possible, retourne si cela est possible ou pas
    void drawFlowerZone(sf::RenderTarget& target, Vec2d const& position); // dessine momentanément le cercle rouge ou vert pour indiquer si il est possible ou non de dessiner une fleur dans la position souhaitée
    double gethumidity(Vec2d p); // retourne l'humidité de la cellule correspondante a la position p dans le wold_ de l'environnement.
};

#endif
