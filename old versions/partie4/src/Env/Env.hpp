#include <Utility/Vec2d.hpp>
#include "World.hpp"
#include "Flower.hpp"
#include "Hive.hpp"
#include "FlowerGenerator.hpp"


#ifndef Env_H
#define Env_H

class Env :  public Drawable, public Updatable // ne peut pas heriter de Drawable et Updatable car se sont des classes abtraites, et Env est utilisé parfois sans passer par des pointeurs.
{
private:
    World world_; // terrain
    FlowerGenerator generator_; // permet la generation de fleur automatique
    std::vector<Flower*> Flowers_; // collections de pointeurs sur des fleurs
    std::vector<Hive*> Hives_; // collections de pointeurs sur des ruches


public :
    Bee* getBeeAt(Vec2d p);
    Env(const Env &) = delete; // supprimer le constructeur de copie afin d'eviter une copie d'element trop volumineux
    Env(); // constructeur
    ~Env(); //destructeur
    void update(sf::Time dt); // met a jour notre environnement: maj du genetor_, et des fleurs.
    void drawOn(sf::RenderTarget& target) const override; // methode de dessin principale, va dessiner les fleurs et le monde
    void reset(); // supprime l'environnement actuel et en genere un nouveau
    void loadWorldFromFile(); // charge le world_ a partir d'un fichier .map
    void saveWorldToFile(); // sauvegarde le world_ dans un fichier .map
    bool addFlowerAt(Vec2d p); // ajoute une fleur a la position p si cela est possible, retourne si cela est possible ou pas
    bool addHiveAt(Vec2d p); // ajoute une ruche a la position p si cela est possible, retourne si cela est possible ou pas
    void drawFlowerZone(sf::RenderTarget& target, Vec2d const& position); // dessine momentanément le cercle rouge ou vert pour indiquer si il est possible ou non de dessiner une fleur dans la position souhaitée
    double gethumidity(Vec2d p) const; // retourne l'humidité de la cellule correspondante a la position p dans le wold_ de l'environnement.
    Hive* getCollidingHive(const Collider& body);
    Flower* getCollidingFlower(const Collider& body);
    bool isFlyable(const Vec2d& p); // verifie si la cellule dans laquelle se trouve la position graphique passée en argument nest pas une cellule de Roche.
    void drawHiveableZone(sf::RenderTarget& target, Vec2d const& position);


};

#endif
