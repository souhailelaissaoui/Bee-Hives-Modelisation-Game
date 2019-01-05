#include <SFML/Graphics.hpp>
#include <iostream>
#include <Application.hpp>
#include <vector>
#include "Random/Random.hpp"
#include "Stats/Stats.hpp"
#include "Env/Env.hpp"
#include <math.h>
#include "Utility/Utility.hpp"

Stats::Stats()
{};

void  Stats::reset()
{

    for (size_t i(0); i< graphs_.size(); i++) {
        graphs_[i]->reset();
    }
};

Stats::~Stats() {};

void Stats::update(sf::Time dt)
{
}

void Stats::setActive(int num)
{
    if ((num >= 0) and (num <= graphs_.size())) {
        actual_ = num;
    }
}


void Stats::addGraph(int id, std::string label, Labels labels, int min, int max, Vec2d dimension)
{
    graphs_.push_back(std::unique_ptr<Graph>(new Graph(labels,dimension,min,max)));
    labels_.push_back(label);
    setActive(id);
}
void Stats::drawOn(sf::RenderTarget& target) const
{
    for (size_t i(0); i< graphs_.size(); i++) {
        graphs_[i]->drawOn(target);
    }
}
