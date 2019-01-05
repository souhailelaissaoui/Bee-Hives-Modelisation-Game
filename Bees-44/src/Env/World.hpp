#include <Utility/Vertex.hpp>
#include <SFML/Graphics.hpp>
#include <Utility/Vec2d.hpp>
#include <Interface/Drawable.hpp>

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
class World : public Drawable
{
private:
    std::vector<Kind> cells_; // Le tableau de cellule qui modelise le monde.
    std::vector<double> cellsHumidity_; // Le tableau assignant a chaque cellule son niveau d'humidité.
    int humidityRange_; // constante calculée dans reloadConfig,
    int n_; // constante prise de la config
    int sig_; // constante prise de la config
    int nbcells_; //nombre de cellules par ligne
    float sizecell_; // taille graphique de chaquall -c "%f" -std=c++11e cellule
    std::vector<sf::Vertex> humidityVertexes_; // Les vertexes sont les points graphique qui vont modeliser nos cellules
    std::vector<sf::Vertex> grassVertexes_;
    std::vector<sf::Vertex> rockVertexes_;
    std::vector<sf::Vertex> waterVertexes_;
    sf::RenderTexture renderingCache_; //  ici on va dessiner nos textures pour ensuite l'afficher
    std::vector<Seed> Seeds_; // collection de graines qui va permettre de generer le terrain [?] array ?
    int nbwater_; // nombre de graine d'eau
    int nbgrass_; // nombre de graine d'herbe
    int creating_size = 3;
public:
    void humidify(double x1, double y1); // prend en argument la cellule d'eau qui vient d'apparaitre pour humidifier le voisinage
    void reloadConfig(); // recharge les parametres a partir du fichier de configuration
    void drawOn(sf::RenderTarget& target) const override; // dessine le monde dans le parametre target
    void reloadCacheStructure(); // initialise les vertextes et le parametre des textures ( renderingCache_)
    void updateCache(); // Définit la transparence des vertexes: une cellule d'eau aura son watervertex apparent mais les autres transparent
    void reset(bool reg = true); // détruit le monde actuel et en recree un nouveau a partir des parametres du fichier de configuration
    void loadFromFile(); // charge les niveaux d'humidité , le tableau de cellules et les parametres nbcells et sizecells depuis un fichier .map
    void step(); // fait se reproduire les graines une fois
    void steps(int nb, bool update = false); // appelle la fonction step nb fois, si update est true elle updateCache
    void smooth(); // permet de lisser le terrain en etant appelée plusieurs fois
    void smooths(int nb, bool update = false); // appelle smooth() plusieurs fois pour lisser le terrain
    void saveToFile() const; // sauvegarde le monde actuel dans un fichier .map
    bool isGrowable(const Vec2d& p) const; // verifie si la cellule dans laquelle se trouve la position graphique passée en argument est une cellule d'herbe.
    bool isFlyable(const Vec2d& p) const; // verifie si la cellule dans laquelle se trouve la position graphique passée en argument nest pas une cellule de Roche.
    double gethumidity(Vec2d p) const; // retourne la valeur d'humidité de la cellule dans laquelle se trouve la position graphique passée en argument.
    ~World(); // destructeur
    World(const World&) = delete; // supprimer le constructeur de copie afin d'eviter une copie d'element trop volumineux
    World();
    void clamping(Vec2d& center) const;
    bool isHiveable(Vec2d const& position, double radius) const;
    std::vector<std::size_t> indexesForRect(Vec2d const& topLeft, Vec2d const& bottomRight) const;
    static j::Value const& getConfig();
    bool isInsideWorld(int x, int y) const;
    bool isInsideWorld(Vec2d p) const;
    void AddWater(Vec2d pos);
    void EditCreatingSize(bool mode);
    void AddGrass(Vec2d pos);
    void drawCreatingZone(sf::RenderTarget& target)const ;
    void clampingCase(Vec2d& center) const;
};
#endif
