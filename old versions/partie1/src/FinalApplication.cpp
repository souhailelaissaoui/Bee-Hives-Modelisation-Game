/*
 * prjsv 2016
 * 2013, 2014, 2016
 * Marco Antognini
 */

#include <Config.hpp>
#include <FinalApplication.hpp>

#include <cassert>

IMPLEMENT_MAIN(FinalApplication);

void FinalApplication::onRun()
{
    // Setup stats
    // TODO step5 uncomment me
    //addGraph(s::GENERAL, { s::FLOWERS, s::HIVES, s::SCOUTS, s::WORKERS, }, 0, 300);
}

void FinalApplication::onEvent(sf::Event, sf::RenderWindow&)
{
    // TODO add more stuff here
}

void FinalApplication::onDraw(sf::RenderTarget&)
{
    // TODO add more stuff here
}
