/*
 * prjsv 2016
 * 2013, 2014, 2016
 * Marco Antognini
 */

#ifndef INFOSV_FINAL_APPLICATION_HPP
#define INFOSV_FINAL_APPLICATION_HPP

#include <Application.hpp>

class FinalApplication : public Application
{
public:
    using Application::Application;

    virtual void onRun() override final;
    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
    virtual void onDraw(sf::RenderTarget& target) override final;
};

#endif // INFOSV_FINAL_APPLICATION_HPP
