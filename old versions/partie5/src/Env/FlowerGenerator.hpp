#ifndef FlowerGenerator_H
#define FlowerGenerator_H
#include <Interface/Updatable.hpp>
class FlowerGenerator : public Updatable
{
private:
    sf::Time time_; // temps ecoule
public:
    FlowerGenerator(); // constructeur
    void update(sf::Time dt) override; //met a jour le generateur => genere de nouvelles fleurs
    void reset(); // remet le temps a 0
    ~FlowerGenerator(); // destructeur
};

#endif

