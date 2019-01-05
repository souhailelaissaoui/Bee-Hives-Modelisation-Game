#include "World.hpp"
#include <iostream>
#include <Application.hpp>
#include <vector>
#include <fstream>
#include "Random/Random.hpp"


void World::reloadConfig()
{
    nbcells_ = getAppConfig()["simulation"]["world"]["cells"].toInt();
    sizecell_ = getAppConfig()["simulation"]["world"]["size"].toDouble() / nbcells_;
    nbwater_ = getAppConfig()["simulation"]["world"]["seeds"]["water"].toInt();
    nbgrass_ = getAppConfig()["simulation"]["world"]["seeds"]["grass"].toInt();
    Seed Seeds[nbwater_+nbgrass_];
    Seeds_ = Seeds;


    size_t taille(nbcells_ * nbcells_);
    cells_ = std::vector<Kind>( taille, Kind::Roche );
}

void World::drawOn(sf::RenderTarget& target)
{
    sf::Sprite cache(renderingCache_.getTexture());
    target.draw(cache);
}

void World::reloadCacheStructure()
{
    std::vector<sf::Vertex> vertexes(generateVertexes(getAppConfig()["simulation"]["world"]["textures"], nbcells_, sizecell_));
    waterVertexes_ = vertexes;
    rockVertexes_ = vertexes;
    grassVertexes_ = vertexes;
    renderingCache_.clear();
    renderingCache_.create(nbcells_ * sizecell_, nbcells_ * sizecell_);
}

void World::updateCache()
{

    sf::RenderStates rs;
    for (size_t x(0); x< nbcells_; x++) {

        for (size_t y(0); y< nbcells_; y++) {
            std::vector <size_t> i(indexesForCellVertexes(x, y, nbcells_ ));
            size_t num(nbcells_*x+y);
            if (cells_[num] == Kind::Herbe) {
                waterVertexes_[i[0]].color.a =0 ;
                waterVertexes_[i[1]].color.a = 0 ;
                waterVertexes_[i[2]].color.a=0;
                waterVertexes_[i[3]].color.a=0;
                rockVertexes_[i[0]].color.a =0 ;
                rockVertexes_[i[1]].color.a = 0 ;
                rockVertexes_[i[2]].color.a=0;
                rockVertexes_[i[3]].color.a=0;
                grassVertexes_[i[0]].color.a =255 ;
                grassVertexes_[i[1]].color.a = 255 ;
                grassVertexes_[i[2]].color.a=255;
                grassVertexes_[i[3]].color.a=255;
            }

            if (cells_[num] == Kind::Eau) {
                waterVertexes_[i[0]].color.a =255 ;
                waterVertexes_[i[1]].color.a = 255 ;
                waterVertexes_[i[2]].color.a=255;
                waterVertexes_[i[3]].color.a=255;
                rockVertexes_[i[0]].color.a =0 ;
                rockVertexes_[i[1]].color.a = 0 ;
                rockVertexes_[i[2]].color.a=0;
                rockVertexes_[i[3]].color.a=0;
                grassVertexes_[i[0]].color.a =0 ;
                grassVertexes_[i[1]].color.a = 0 ;
                grassVertexes_[i[2]].color.a=0;
                grassVertexes_[i[3]].color.a=0;
            }

            if (cells_[num] == Kind::Roche) {
                waterVertexes_[i[0]].color.a =0 ;
                waterVertexes_[i[1]].color.a = 0 ;
                waterVertexes_[i[2]].color.a=0;
                waterVertexes_[i[3]].color.a=0;
                rockVertexes_[i[0]].color.a =255 ;
                rockVertexes_[i[1]].color.a = 255 ;
                rockVertexes_[i[2]].color.a=255;
                rockVertexes_[i[3]].color.a=255;
                grassVertexes_[i[0]].color.a =0 ;
                grassVertexes_[i[1]].color.a = 0 ;
                grassVertexes_[i[2]].color.a=0;
                grassVertexes_[i[3]].color.a=0;
            }


        }
    }

    auto textures = getAppConfig()["simulation"]["world"]["textures"];
    rs.texture = &getAppTexture(textures["rock"].toString()); // ici pour la texture liée à la roche
    renderingCache_.draw(rockVertexes_.data(), rockVertexes_.size(), sf::Quads, rs);
    rs.texture = &getAppTexture(textures["water"].toString()); // ici pour la texture liée à la
    renderingCache_.draw(waterVertexes_.data(), waterVertexes_.size(), sf::Quads, rs);
    rs.texture = &getAppTexture(textures["grass"].toString()); // ici pour la texture liée à la
    renderingCache_.draw(grassVertexes_.data(), grassVertexes_.size(), sf::Quads, rs);
    renderingCache_.display();
}

void World::reset(bool reg)
{
    reloadConfig();
    reloadCacheStructure();
    for (size_t i(0); i < nbwater_; i++) {

        sf::Vector2i coord(uniform(0, nbcells_-1),uniform(0, nbcells_-1));
        Seed temp = {coord, Kind::Eau};
        Seeds_[i] = temp;
    }
    for (size_t i(nbwater_); i < nbgrass_+nbwater_; i++) {

        sf::Vector2i coord(uniform(0, nbcells_-1),uniform(0, nbcells_-1));
        Seed temp = {coord, Kind::Herbe};
        Seeds_[i] = temp;
    }
    for (size_t i(0); i < Seeds_.size(); i++) {
        size_t num(nbcells_*(Seeds_[i]).coord.x+(Seeds_[i]).coord.y);
        if (	cells_[num] != Kind::Eau) {
            cells_[num] = (Seeds_[i]).nat;
        }
    }
    if (reg) {
        steps(getAppConfig()["simulation"]["world"]["generation"]["steps"].toInt());
        smooths(getAppConfig()["simulation"]["world"]["generation"]["smoothness"]["level"].toInt());
    }
    updateCache();
}
void World::loadFromFile()
{


    std::ifstream entree(getApp().getResPath()+"world5.map");
    unsigned int nbcells;


    if (entree.fail()) {
        throw std::runtime_error("Impossible de lire le fichier map");
    }
    float size;
    entree >> nbcells >> size;
    nbcells_ = nbcells;
    sizecell_= size;
    cells_.clear();
    std::vector <Kind> cells;
    short int var;
    for (int i(0); i<=(nbcells_*nbcells_); i++) {
        entree >> var;
        cells.push_back(static_cast<Kind>(var));
    }



    entree.close();
    cells_ = cells;
    reloadCacheStructure();
    updateCache();
}
void World::step()
{
    for (size_t i(0); i < Seeds_.size(); i++) {

        if ((Seeds_[i].nat == Kind::Herbe) or (bernoulli(getAppConfig()["simulation"]["world"]["seeds"]["water teleport probability"].toDouble()) == 0)) {

            //	double x((Seeds_[i]).coord.x+uniform(-1, 1));
//	double y((Seeds_[i]).coord.y+uniform(-1, 1));
            int x;
            int y;
            int b(uniform(1,4));
            switch(b) {
            case 1 :
                if ((Seeds_[i]).coord.x==nbcells_-1) {
                    break;
                };
                x= (Seeds_[i]).coord.x+1;
                y=  (Seeds_[i]).coord.y;
                break;
            case 2 :
                if ((Seeds_[i]).coord.x==0) {
                    break;
                };
                x= (Seeds_[i]).coord.x-1;
                y=  (Seeds_[i]).coord.y;
                break;
            case 3 :
                if ((Seeds_[i]).coord.y==nbcells_-1) {
                    break;
                };
                x= (Seeds_[i]).coord.x;
                y=  (Seeds_[i]).coord.y+1;
                break;
            case 4 :
                if ((Seeds_[i]).coord.y==0) {
                    break;
                };
                x= (Seeds_[i]).coord.x;
                y=  (Seeds_[i]).coord.y-1;
                break;
            }
            size_t num(nbcells_*x+y);
            cells_[num] = (Seeds_[i]).nat;
            sf::Vector2i coord(x,y);
            Seeds_[i].coord=coord;
        } else {


            sf::Vector2i coord(uniform(0, nbcells_-1),uniform(0, nbcells_-1));
            size_t num(nbcells_*coord.x+coord.y);
            Seed temp = {coord, Kind::Eau};
            Seeds_[i] = temp;
            cells_[num] = (Seeds_[i]).nat;
        }
    }
}
void World::steps(int nb, bool update)
{
    for (int p(0); p< nb ; p++) {
        step();
    }
    if (update) updateCache();
}


void World::smooth()
{
    double taux(getAppConfig()["simulation"]["world"]["generation"]["smoothness"]["water neighbourhood ratio"].toDouble());
    double taux2(getAppConfig() ["simulation"]["world"]["generation"]["smoothness"]["grass neighbourhood ratio"].toDouble());
    double t;
    auto copy_de_cells_ = cells_;
    for (size_t x(0); x < nbcells_-1 ; x++ ) {
        for (size_t y(0); y < nbcells_-1 ; y++ ) {
            size_t num(nbcells_*x+y);
            if ((copy_de_cells_[num] == Kind::Roche) or (copy_de_cells_[num] == Kind::Herbe)) {
                double nbvoiseau(0);
                sf::Vector2i temp1(x+1,y);
                sf::Vector2i temp2(x,y+1);
                sf::Vector2i temp3(x-1,y);
                sf::Vector2i temp4(x,y-1);
                if ( cells_[temp1.x*nbcells_+temp1.y] == Kind::Eau ) nbvoiseau += 1;
                if ( cells_[temp2.x*nbcells_+temp2.y] == Kind::Eau ) nbvoiseau += 1;
                if ( cells_[temp3.x*nbcells_+temp3.y] == Kind::Eau ) nbvoiseau += 1;
                if ( cells_[temp4.x*nbcells_+temp4.y] == Kind::Eau ) nbvoiseau += 1;
                /*for (int i(0); i<3; i++) {						for (int i(0); i<; i++) {							if ( cells_[temp.x*nbcells_+temp.y] == Kind::Eau ) nbvoiseau += 1;temp.y -= 1;}temp.x -= 1;temp.y = y+1;	} */
                t = nbvoiseau/4;
                if (t>taux) {
                    copy_de_cells_[num] = Kind::Eau ;
                };
            }

            if ((cells_[num] == Kind::Roche) and (copy_de_cells_[num] != Kind::Eau)) {
                double nbvoisherbe(0);
                sf::Vector2i temp1(x+1,y);
                sf::Vector2i temp2(x,y+1);
                sf::Vector2i temp3(x-1,y);
                sf::Vector2i temp4(x,y-1);
                if ( cells_[temp1.x*nbcells_+temp1.y] == Kind::Herbe ) nbvoisherbe += 1;
                if ( cells_[temp2.x*nbcells_+temp2.y] == Kind::Herbe ) nbvoisherbe += 1;
                if ( cells_[temp3.x*nbcells_+temp3.y] == Kind::Herbe ) nbvoisherbe += 1;
                if ( cells_[temp4.x*nbcells_+temp4.y] == Kind::Herbe ) nbvoisherbe += 1;
                t = nbvoisherbe/4;
                if (t>taux2) {
                    copy_de_cells_[num] = Kind::Herbe;
                };
            }
        }
    }


    std::swap(cells_, copy_de_cells_); // quand le lissage est fini on copie copie_de_cells_ dans cell_ (le swap est une optimisation).


}
void World::smooths(int nb, bool update)
{
    for (int p(0); p< nb ; p++) {
        smooth();
    }
    if (update) {
        updateCache() ;
    };
}

void World::saveToFile()
{
    //std::ifstream entree(getApp().getResPath()+getAppConfig()["simulation"]["world"]["file"].toString());
    std::ofstream sortie(getApp().getResPath()+"world6.map");
    if (sortie.fail()) {
        throw std::runtime_error("Impossible d'ecrire le fichier map");
    }
    sortie << nbcells_ << std::endl << sizecell_ << std::endl;
    for (size_t i(0); i < cells_.size() ; i++) {
        sortie << static_cast<std::underlying_type<Kind>::type> (cells_[i]) << ' '  << std::flush;
        std::cout << "Éqp enregietres" << std::endl ;
    }
    sortie.close();
}
