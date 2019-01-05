/*
 * prjsv 2015
 * 2014
 * Marco Antognini
 */

#include <Application.hpp>
#include <Env/Env.hpp>
#include <Env/Hive.hpp>

#include <cassert>

/*!
 * Test Hive creation in toric world + Generic Bee random movement
 *
 * This test has the ability to manually create Hive with `H` under
 * the cursor position (after testing whether the position is "hiveable"
 * or not). Additionally, generic bees can added to the last created
 * hive, if any, using `B`.
 *
 * NOTE: it is expected of this test to fail in step5 when scout
 *       and worker bees are introduced.
 */
class HiveTest : public Application
{
public:
    using Application::Application;

    // Override a few methods
    virtual void onEvent(sf::Event event, sf::RenderWindow& window) override final;
    virtual void onDraw(sf::RenderTarget& target) override final;

private:
    bool  mShowHiveableZone = false;   ///< Enable/disable hiveable zone visualisation
    Hive* mLastHive         = nullptr; ///< Keep track of the last hive that was created
};

IMPLEMENT_MAIN(HiveTest);

void HiveTest::onEvent(sf::Event event, sf::RenderWindow&)
{
    if (event.type == sf::Event::KeyPressed) {
        switch (event.key.code) {
        default:
            break;

        case sf::Keyboard::B:
            if (mLastHive != nullptr) {
                mLastHive->addBee();
            }
            break;

        case sf::Keyboard::H:
            mShowHiveableZone = true;
            break;
        }
    } else if (event.type == sf::Event::KeyReleased) {
        switch (event.key.code) {
        default:
            break;

        case sf::Keyboard::H: {
            // Add hive: try to create a new one and disable "hiveable" zone
            mShowHiveableZone = false;
            auto position = getCursorPositionInView();
            if (getEnv().addHiveAt(position)) {
                auto dummyRadius = 1;
                mLastHive = getEnv().getCollidingHive(Collider(position, dummyRadius));
                assert(mLastHive != nullptr);
                std::cout << "New hive created\n";
            } else {
                std::cout << "Couldn't create new hive\n";
            }
            break;
        }
        }
    }
}

void HiveTest::onDraw(sf::RenderTarget& target)
{
    // Render visualisation for hiveable
    if (mShowHiveableZone) {
        auto pos = getCursorPositionInView();
        getEnv().drawHiveableZone(target, pos);
    }
}
