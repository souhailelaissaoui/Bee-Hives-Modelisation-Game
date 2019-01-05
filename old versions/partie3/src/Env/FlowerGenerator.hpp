#ifndef FlowerGenerator_H
#define FlowerGenerator_H

class FlowerGenerator
{
private:
    sf::Time time_; // temps ecoule
public:
    FlowerGenerator(); // constructeur
    void update(sf::Time dt); //met a jour le generateur => genere de nouvelles fleurs
    void reset(); // remet le temps a 0
    ~FlowerGenerator(); // destructeur
};

#endif

