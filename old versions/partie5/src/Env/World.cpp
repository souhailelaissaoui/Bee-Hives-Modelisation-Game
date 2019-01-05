#include "World.hpp"
#include <iostream>
#include <Application.hpp>
#include <vector>
#include <fstream>
#include <sstream>
#include "Random/Random.hpp"
#include <math.h>

void World::reloadConfig()
{
    auto const& cfg = getConfig();
    nbcells_ = cfg["cells"].toInt();
    sizecell_ = cfg["size"].toDouble() / nbcells_;
    nbwater_ = cfg["seeds"]["water"].toInt();
    nbgrass_ = cfg["seeds"]["grass"].toInt();
    n_ = cfg["generation"]["humidity"]["initial level"].toInt();
    sig_ = cfg["generation"]["humidity"]["decay rate"].toInt();
    double seuil = cfg["generation"]["humidity"]["threshold"].toDouble();
    humidityRange_ = -log(seuil / n_) * sig_; //  pour eviter la boucle do while
    Seeds_ = std::vector<Seed>(nbwater_ + nbgrass_);
    size_t taille(nbcells_ * nbcells_);
    cellsHumidity_ = std::vector<double>(taille, 0);
    cells_ = std::vector<Kind>(taille, Kind::Roche);
}

void World::drawOn(sf::RenderTarget& target) const
{
    if (getConfig()["show humidity"] == true) {
        target.draw(humidityVertexes_.data(), humidityVertexes_.size(), sf::Quads);
    } else {
        sf::Sprite cache(renderingCache_.getTexture());
        target.draw(cache);
    }
}

void World::reloadCacheStructure()
{
    std::vector<sf::Vertex> vertexes(generateVertexes(getConfig()["textures"], nbcells_, sizecell_));
    humidityVertexes_ = vertexes;
    waterVertexes_ = vertexes;
    rockVertexes_ = vertexes;
    grassVertexes_ = vertexes;
    renderingCache_.clear();
    renderingCache_.create(nbcells_ * sizecell_, nbcells_ * sizecell_);
}

void World::updateCache()
{
    double min(99999);
    double max(cellsHumidity_[0]);
    for (size_t i(0); i < cellsHumidity_.size(); i++) {
        if (cellsHumidity_[i] > max) {
            max = cellsHumidity_[i];
        };
        if (cellsHumidity_[i] < min) {
            min = cellsHumidity_[i];
        }
    }
    sf::RenderStates rs;
    for (int x(0); x < nbcells_; x++) {
        for (int y(0); y < nbcells_; y++) {
            std::vector<size_t> i(indexesForCellVertexes(x, y, nbcells_));
            size_t num(nbcells_ * y + x);
            double niveau_bleu = ((cellsHumidity_[num] - min) / (max - min)) * 255;
            for (int u(0); u < 4; u++) {
                rockVertexes_[i[u]].color.a = 0;
                waterVertexes_[i[u]].color.a = 0;
                grassVertexes_[i[u]].color.a = 0;
                humidityVertexes_[i[u]].color = sf::Color(0, 0, niveau_bleu);
            }
            if (cells_[num] == Kind::Herbe) {
                for (int u(0); u < 4; u++) {
                    grassVertexes_[i[u]].color.a = 255;
                }
            }
            if (cells_[num] == Kind::Eau) {
                for (int u(0); u < 4; u++) {
                    waterVertexes_[i[u]].color.a = 255;
                }
            }
            if (cells_[num] == Kind::Roche) {
                for (int u(0); u < 4; u++) {
                    rockVertexes_[i[u]].color.a = 255;
                }
            }
        }
    }
    auto textures = getConfig()["textures"];
    rs.texture = &getAppTexture(textures["rock"].toString()); // ici pour la texture liée à la roche
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs);
    rs.texture = &getAppTexture(textures["water"].toString()); // ici pour la texture liée à l'eau
    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, rs);
    rs.texture = &getAppTexture(textures["grass"].toString()); // ici pour la texture liée à l'herbe
    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, rs);
    renderingCache_.display();
}

void World::reset(bool reg)
{
    reloadConfig();
    reloadCacheStructure();
    for (int i(0); i < nbwater_; i++) {

        sf::Vector2i coord(uniform(0, nbcells_ - 1), uniform(0, nbcells_ - 1));
        Seeds_[i] = { coord, Kind::Eau };
    } // remplit le tableau de graines avec des graines d'eau avec des positions aleatoire
    for (int i(nbwater_); i < nbgrass_ + nbwater_; i++) {

        sf::Vector2i coord(uniform(0, nbcells_ - 1), uniform(0, nbcells_ - 1));
        Seeds_[i] = { coord, Kind::Herbe };
    } // remplit le tableau de graines avec des graines d'herbe avec des positions aleatoire

    for (size_t i(0); i < Seeds_.size(); i++) {
        size_t num(nbcells_ * (Seeds_[i]).coord.y + (Seeds_[i]).coord.x);
        if (cells_[num] != Kind::Eau) {
            cells_[num] = (Seeds_[i]).nat;
        }
    } // Permet a chaque graine de transmettre son type a la cellule qu'elle occupe

    if (reg) {
        steps(getConfig()["generation"]["steps"].toInt(), false);
        smooths(getConfig()["generation"]["smoothness"]["level"].toInt(), false);
    }
    updateCache();
    std::cout << "World regenerated" << std::endl;
}
void World::loadFromFile()
{
    std::ifstream entree(getApp().getResPath() + getConfig()["file"].toString());
    unsigned int nbcells;

    if (entree.fail()) {
        throw std::runtime_error("Impossible de lire le fichier map");
    }
    float size;
    entree >> nbcells >> std::ws >> size >> std::ws;
    nbcells_ = nbcells;
    sizecell_ = size;
    cells_.clear();
    cellsHumidity_.clear();
    size_t taille(nbcells_ * nbcells_);
    std::vector<Kind> cells(taille, Kind::Roche);
    std::vector<double> cellsHumidity(taille);
    short int var;
    for (size_t i(0); i <= (taille); i++) {
        entree >> var >> std::ws;
        cells[i] = static_cast<Kind>(var);
    }
    std::string line;
    std::getline(entree, line);
    std::istringstream entree2(line);
    for (size_t i(0); i <= (taille); i++) {
        double varr;
        entree2 >> varr >> std::ws;
        cellsHumidity[i] = varr;
    }

    entree.close();
    cells_ = cells;
    cellsHumidity_ = cellsHumidity;

    reloadCacheStructure();
    updateCache();
}
void World::step()
{
    auto const& cfg = getConfig();
    for (size_t i(0); i < Seeds_.size(); i++) {

        if ((Seeds_[i].nat == Kind::Herbe) or (bernoulli(cfg["seeds"]["water teleport probability"].toDouble()) == 0)) { //deplace les graines dans une des cases voisines
            int proba(uniform(0, 3));
            int x = (Seeds_[i]).coord.x;
            int y = (Seeds_[i]).coord.y;
            switch (proba) {
            case 0:
                x += 1;
                break;
            case 1:
                x -= 1;
                break;
            case 2:
                y += 1;
                break;
            case 3:
                y -= 1;
                break;
            }
            /*
            int x = (Seeds_[i]).coord.x += uniform(-1, 1);
            int y = (Seeds_[i]).coord.y += uniform(-1, 1);*/
            if (isInsideWorld(x, y)) {
                {
                    size_t num(nbcells_ * y + x);
                    cells_[num] = (Seeds_[i]).nat;
                    if ((Seeds_[i]).nat == Kind::Eau) {
                        humidify(x, y);
                    }
                    (Seeds_[i]).coord.x = x;
                    (Seeds_[i]).coord.y = y;
                }
            }
        } else {
            sf::Vector2i coord(uniform(0, nbcells_ - 1), uniform(0, nbcells_ - 1));
            size_t num(nbcells_ * coord.y + coord.x);
            Seeds_[i].coord = coord;
            humidify(coord.x, coord.y);
            cells_[num] = (Seeds_[i]).nat;
        }
    }
}
void World::steps(int nb, bool update)
{
    for (int p(0); p < nb; p++) {
        step();
    }
    if (update) {
        updateCache();
    };
}

void World::smooth()
{
    double taux(getConfig()["generation"]["smoothness"]["water neighbourhood ratio"].toDouble());
    double taux2(getConfig()["generation"]["smoothness"]["grass neighbourhood ratio"].toDouble());
    double t;
    auto copy_de_cells_ = cells_;
    for (int x(0); x < nbcells_; x++) {
        for (int y(0); y < nbcells_; y++) {
            size_t num(nbcells_ * y + x);
            if (copy_de_cells_[num] != Kind::Eau) {

                double nbvoiseau(0);
                double voisin(0);

                Vec2d temp(x + 1, y + 1);
                for (int i(0); i < 3; i++) {
                    for (int u(0); u < 3; u++) {
                        if (isInsideWorld(temp)) {

                            if (cells_[temp.y * nbcells_ + temp.x] == Kind::Eau) {
                                nbvoiseau += 1;
                            }
                            voisin += 1;
                        }
                        temp.y -= 1;
                    }

                    temp.x -= 1;
                    temp.y = y + 1;
                }
                t = nbvoiseau / voisin;
                if (t > taux) {
                    copy_de_cells_[num] = Kind::Eau;
                };
            }

            if ((cells_[num] == Kind::Roche) and (copy_de_cells_[num] != Kind::Eau)) {
                double nbvoisherbe(0);
                double voisin(0);
                Vec2d temp(x + 1, y + 1);
                for (int i(0); i < 3; i++) {
                    for (int u(0); u < 3; u++) {
                        if (isInsideWorld(temp)) {
                            if (cells_[temp.y * nbcells_ + temp.x] == Kind::Herbe) {
                                nbvoisherbe += 1;
                            }
                            voisin += 1;
                        }
                        temp.y -= 1;
                    }

                    temp.x -= 1;
                    temp.y = y + 1;
                }
                t = nbvoisherbe / voisin;

                if (t > taux2) {
                    copy_de_cells_[num] = Kind::Herbe;
                };
            }
        }
    }

    std::swap(cells_, copy_de_cells_); // quand le lissage est fini on copie copie_de_cells_ dans cell_ (le swap est une optimisation).
}
void World::smooths(int nb, bool update)
{
    for (int p(0); p < nb; p++) {
        smooth();
    }
    if (update) {
        updateCache();
    };
}

void World::saveToFile() const
{
    std::ofstream sortie(getApp().getResPath() + getConfig()["file"].toString());
    if (sortie.fail()) {
        throw std::runtime_error("Impossible d'ecrire le fichier map");
    }
    sortie << nbcells_ << std::endl
           << sizecell_ << std::endl;
    for (size_t i(0); i < cells_.size(); i++) {
        sortie << static_cast<std::underlying_type<Kind>::type>(cells_[i]) << ' ' << std::flush;
        std::cout << "Monde enregistré" << std::endl;
    }
    sortie << std::endl;
    for (size_t i(0); i < cellsHumidity_.size(); i++) {
        sortie << cellsHumidity_[i] << " " << std::flush;
    }
    sortie.close();
}

void World::humidify(double x1, double y1)
{

    for (int x(x1 - humidityRange_); x < x1 + humidityRange_ + 1.0; x++) {
        for (int y(y1 - humidityRange_); y < y1 + humidityRange_ + 1.0; y++) {
            if (isInsideWorld(x, y)) {
                int num(nbcells_ * y + x);
                double add = (n_ * exp(-std::hypot(x1 - x, y1 - y) / sig_));
                cellsHumidity_[num] = cellsHumidity_[num] + add;
            }
        }
    }
}
bool World::isGrowable(const Vec2d& p) const
{
    int x(p.x / sizecell_);
    int y(p.y / sizecell_);
    return (cells_[x + y * nbcells_] == Kind::Herbe);
}

double World::gethumidity(Vec2d p) const
{
    int x(p.x / sizecell_);
    int y(p.y / sizecell_);
    double result;
    if (isInsideWorld(x, y)) {
        result = cellsHumidity_[x + y * nbcells_];
    } else {
        result = 0;
    }
    return result;
}
bool World::isFlyable(Vec2d const& p) const
{
    int x(p.x / sizecell_);
    int y(p.y / sizecell_);
    Vec2d test(x, y);
    clampingCase(test);
    return (cells_[test.x + test.y * nbcells_] != Kind::Roche);
}
World::~World() {};
World::World() {};

bool World::isHiveable(Vec2d const& position, double radius) const
{

    Vec2d topLeft(position.x - radius, position.y - radius);
    Vec2d bottomRight(position.x + radius, position.y + radius); //clamping(topLeft); clamping(bottomRight);
    int x1(topLeft.x / sizecell_);
    int y1(topLeft.y / sizecell_);
    int x2(bottomRight.x / sizecell_);
    int y2(bottomRight.y / sizecell_);
    Vec2d CellCoord_top(x1, y1);
    Vec2d CellCoord_bottom(x2, y2);
    std::vector<std::size_t> test = indexesForRect(CellCoord_top, CellCoord_bottom);
    for (size_t i(0); i < test.size(); i++) {
        if (cells_[test[i]] != Kind::Herbe) {
            return false;
        }
    }
    return true;
}

std::vector<std::size_t> World::indexesForRect(Vec2d const& topLeft, Vec2d const& bottomRight) const
{

    std::vector<std::size_t> result;

    for (int x(topLeft.x); x < bottomRight.x; x++) {
        for (int y(topLeft.y); y < bottomRight.y; y++) {
            Vec2d oula(x, y);
            clampingCase(oula);
            size_t num(oula.x + nbcells_ * oula.y);

            result.push_back(num);
        }
    }

    return result;
}

void World::clamping(Vec2d& center) const
{
    auto worldSize = getApp().getWorldSize();
    auto width = worldSize.x; // largeur
    auto height = worldSize.y; // hauteur

    while (center.x > width) {
        center.x -= width;
    }
    while (center.x < 0) {
        center.x += width;
    }
    while (center.y > height) {
        center.y -= height;
    }
    while (center.y < 0) {
        center.y += height;
    }
}
void World::clampingCase(Vec2d& center) const
{

    while (center.x > nbcells_) {
        center.x -= nbcells_;
    }
    while (center.x < 0) {
        center.x += nbcells_;
    }
    while (center.y > nbcells_) {
        center.y -= nbcells_;
    }
    while (center.y < 0) {
        center.y += nbcells_;
    }
}

j::Value const& World::getConfig()
{
    return getAppConfig()["simulation"]["world"];
}

bool World::isInsideWorld(int x, int y) const
{
    return (x < nbcells_ and y < nbcells_) and (x >= 0 and y >= 0);
}
bool World::isInsideWorld(Vec2d p) const
{
    return isInsideWorld(p.x, p.y);
}
