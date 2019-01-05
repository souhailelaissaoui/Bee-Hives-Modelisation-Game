#ifndef Stats_H
#define Stats_H
#include <Interface/Updatable.hpp>
#include <Interface/Drawable.hpp>
#include <Stats/Graph.hpp>

typedef std::vector <std::unique_ptr<Graph>> Graphs;
typedef std::vector<std::string> Labels;

class Stats: public Drawable, public Updatable
{
private:
    Graphs graphs_; // temps ecoule
    Labels labels_;
    int actual_;
public:
    Stats(); // constructeur
    void update(sf::Time dt); // met a jour le generateur => genere de nouvelles fleurs
    void reset(); // remet le temps a 0
    ~Stats(); // destructeur
    void setActive(int num);
    void addGraph(int id, std::string label, Labels labels, int min, int max, Vec2d dimension) ;
    void drawOn(sf::RenderTarget& target) const;
};

#endif

