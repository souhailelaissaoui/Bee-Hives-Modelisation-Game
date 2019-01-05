#include <Utility/Vertex.hpp>
#include <SFML/Graphics.hpp>

#ifndef World_H
#define World_H
enum class Kind : short
{
    Herbe, Eau, Roche
};
struct Seed {
    sf::Vector2i coord;
    Kind nat;
};
class World
{
private:
    std::vector <Kind> cells_;
    int nbcells_; //nb cell pa rligne
    float sizecell_;// taille graphique de chaque cellule
    std::vector <sf::Vertex> grassVertexes_;
    std::vector <sf::Vertex> rockVertexes_;
    std::vector <sf::Vertex> waterVertexes_;
    sf::RenderTexture renderingCache_;
    bool regenerate = true;

    int nbwater_;
    int nbgrass_;
    Seed Seeds_[nbwater_+nbgrass_];
public:
    void reloadConfig ();
    void drawOn(sf::RenderTarget& target);
    void reloadCacheStructure ();
    void updateCache () ;
    void reset (bool reg);
    void loadFromFile();
    void step();
    void steps(int nb, bool update=false);
    void smooth();
    void smooths(int nb, bool update=false);
    void saveToFile();

};
#endif
