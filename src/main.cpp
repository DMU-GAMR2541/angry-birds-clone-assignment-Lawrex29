#include <thread>
#include <mutex>
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "Bird.h"
#include "SlingShot.h"
#include "Pig.h"
#include "Block.h"
#include "PigPool.h"
#include "StartScreen.h"
#include "UI.h"
#include <atomic>
#include <chrono>
#include <vector>
#include <unordered_map>

//Listener so objects will call oncollision when colliding
class ContactListener : public b2ContactListener
{
    void BeginContact(b2Contact* contact) override
    {
        uintptr_t dataA = contact->GetFixtureA()->GetBody()->GetUserData().pointer;
        uintptr_t dataB = contact->GetFixtureB()->GetBody()->GetUserData().pointer;

        if (dataA && dataB)
        {
            GameObject* objA = reinterpret_cast<GameObject*>(dataA);
            GameObject* objB = reinterpret_cast<GameObject*>(dataB);

            objA->onCollision(*objB);
            objB->onCollision(*objA);
        }
    }
};

// one thread for physics
void physicthread(b2World* world, Bird* bird, PigPool* pigPool, std::atomic<bool>& run, std::mutex& physics)
{
    const float PHYSICS_TIMESTEP = 1.0f / 60.0f;
    auto previousTime = std::chrono::high_resolution_clock::now();

    while (run) {
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - previousTime;

        if (deltaTime.count() >= PHYSICS_TIMESTEP) {
            {
                std::lock_guard<std::mutex> lock(physics);
                world->Step(PHYSICS_TIMESTEP, 8, 3);
            }

            {
                std::lock_guard<std::mutex> lock(physics);
                bird->update(PHYSICS_TIMESTEP);
                pigPool->updateActive(PHYSICS_TIMESTEP);
                // Return killed pigs to pool
                pigPool->releaseDestroyed();
            }

            previousTime = currentTime;
        }
        else {
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
    }
}

int main() {
    // --- 1. WINDOW SETUP ---
    sf::RenderWindow window(sf::VideoMode(800, 600), "Annoyed_Flocks");
    window.setFramerateLimit(60);

    // --- 2. START SCREEN (threaded loading + progress bar) ---
    {
        StartScreen startScreen(window);
        if (!startScreen.run())
            return 0; // Window closed on start screen
    }

    // Stl contianer
    std::vector<std::string> birdHistory;

    // display colllours using an associative containder
    std::unordered_map<std::string, sf::Color> birdColours = {
        { "Red",    sf::Color(200, 30,  30)  },
        { "Blue",   sf::Color(30,  100, 200) },
        { "Yellow", sf::Color(230, 200, 20)  }
    };

    //Show UI
    UI gameUI;

    //Conversion for Box2d and SFML
    const float SCALE = 30.0f;
    const float PI = 3.1415927f;

    //setup world.
    b2Vec2 b2_gravity(0.0f, 9.8f); // Earth-like gravity
    b2World world(b2_gravity);

    ContactListener contactListener;
    world.SetContactListener(&contactListener);

    //Creating objects
    Slingshot slingshot(sf::Vector2f(150.0f, 470.0f), 0.0f);
    Bird bird(sf::Vector2f(150.0f, 450.0f), 0.0f);
    Block block(sf::Vector2f(600.0f, 557.0f), 0.0f, Block::BlockType::Wood);

    // PigPool manager
    PigPool pigPool(world, 5, 100);

    // Sets the starting pigs on screen
    pigPool.acquire(sf::Vector2f(680.0f, 555.0f));
    pigPool.acquire(sf::Vector2f(720.0f, 555.0f));

    //Setup ground for the circle to move / bounce on. 

    b2BodyDef b2_groundBodyDef;
    b2_groundBodyDef.position.Set(400.0f / SCALE, 590.0f / SCALE);
    b2Body* b2_groundBody = world.CreateBody(&b2_groundBodyDef);

    //Box2d for ground
    b2PolygonShape b2_groundBox;
    b2_groundBox.SetAsBox(400.0f / SCALE, 10.0f / SCALE);
    b2_groundBody->CreateFixture(&b2_groundBox, 0.0f);

    //Set up the ground visualisation.
    sf::RectangleShape sf_groundVisual(sf::Vector2f(800.0f, 20.0f));
    sf_groundVisual.setOrigin(400.0f, 10.0f);
    sf_groundVisual.setFillColor(sf::Color(34, 139, 34)); // Forest Green

    //Static Box2d for slingshot
    b2BodyDef b2_slingshotDef;
    b2_slingshotDef.type = b2_staticBody;
    b2_slingshotDef.position.Set(150.0f / SCALE, 470.0f / SCALE);
    b2Body* b2_slingshotBody = world.CreateBody(&b2_slingshotDef);
    b2PolygonShape b2_slingshotBox;
    b2_slingshotBox.SetAsBox(7.5f / SCALE, 30.0f / SCALE);
    b2_slingshotBody->CreateFixture(&b2_slingshotBox, 0.0f);

    slingshot.setBody(b2_slingshotBody);

    //box2d for block
    b2BodyDef b2_blockDef;
    b2_blockDef.type = b2_staticBody;
    b2_blockDef.position.Set(600.0f / SCALE, 557.0f / SCALE);
    b2Body* b2_blockBody = world.CreateBody(&b2_blockDef);


    b2PolygonShape b2_blockBox;
    b2_blockBox.SetAsBox(20.0f / SCALE, 20.0f / SCALE);

    b2FixtureDef b2_blockFixture;
    b2_blockFixture.shape = &b2_blockBox;
    b2_blockFixture.friction = 0.5f;
    b2_blockBody->CreateFixture(&b2_blockFixture);

    block.setBody(b2_blockBody);

    //Box2d for bird
    b2BodyDef b2_birdDef;
    b2_birdDef.type = b2_dynamicBody;
    b2_birdDef.position.Set(150.0f / SCALE, 450.0f / SCALE);
    b2Body* b2_birdBody = world.CreateBody(&b2_birdDef);

    b2CircleShape b2_birdCircle;
    b2_birdCircle.m_radius = 10.0f / SCALE;

    b2FixtureDef b2_birdFixture;
    b2_birdFixture.shape = &b2_birdCircle;
    b2_birdFixture.density = 1.0f;
    b2_birdFixture.restitution = 0.3f; // Bounciness
    b2_birdFixture.friction = 0.4f;
    b2_birdBody->CreateFixture(&b2_birdFixture);

    bird.setBody(b2_birdBody);
    bird.setBirdType(Bird::BirdType::Red);

    //Crete threads
    std::atomic<bool> physicsRunning(true);
    std::mutex physicsMutex;

    // Calls thread for physics
    std::thread physics(physicthread, &world, &bird, &pigPool,
        std::ref(physicsRunning), std::ref(physicsMutex));

    // Thread to render objects
    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // Space launches, use R, B, Y to change bird type. THis effects their damage and colour
            if (event.type == sf::Event::KeyPressed) {
                //Red is standard bird
                if (event.key.code == sf::Keyboard::R) {
                    slingshot.loadBird("Red");
                    bird.setBirdType(Bird::BirdType::Red);
                }
                //Blue is fast and weak
                if (event.key.code == sf::Keyboard::B) {
                    slingshot.loadBird("Blue");
                    bird.setBirdType(Bird::BirdType::Blue);
                }
                //Yellow is slow and strong
                if (event.key.code == sf::Keyboard::Y) {
                    slingshot.loadBird("Yellow");
                    bird.setBirdType(Bird::BirdType::Yellow);
                }
                //Fires the bird from slingshot, provides console commands to show pigs remaining health
                if (event.key.code == sf::Keyboard::Space) {
                    // physics locked while rendering updated bird
                    std::lock_guard<std::mutex> lock(physicsMutex);

                    b2_birdBody->SetTransform(b2Vec2(150.0f / SCALE, 450.0f / SCALE), 0);
                    b2_birdBody->SetLinearVelocity(b2Vec2(0, 0));
                    b2_birdBody->SetAngularVelocity(0);

                    bird.setBirdType(bird.getBirdType());

                    slingshot.pullBack(50);
                    float tension = static_cast<float>(slingshot.getTension());
                    slingshot.release();

                    //Impulse
                    float speed = bird.getSpeedMultiplier();
                    float impulseX = tension * 0.06f * speed;
                    float impulseY = -tension * 0.035f * speed;
                    b2_birdBody->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), b2_birdBody->GetWorldCenter(), true);

                    std::cout << "Firing!!!!" << std::endl;

                    // STL container ton record bird history
                    std::string typeName = (bird.getBirdType() == Bird::BirdType::Red) ? "Red"
                        : (bird.getBirdType() == Bird::BirdType::Blue) ? "Blue"
                        : "Yellow";
                    birdHistory.push_back(typeName);
                    std::cout << "Birds fired this session: " << birdHistory.size() << std::endl;
                    auto colourIt = birdColours.find(typeName);
                    if (colourIt != birdColours.end())
                        std::cout << "Bird colour confirmed in map: " << typeName << std::endl;

                    // Update game UI label
                    gameUI.setLabel("lastBird", typeName);
                }
            }
        }

        // rendering thread
        {
            std::lock_guard<std::mutex> lock(physicsMutex);
            //Static doesn't move so is only in render
            sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE,
                b2_groundBody->GetPosition().y * SCALE);
        }

        window.clear(sf::Color(135, 206, 235)); // Sky Blue

        window.draw(sf_groundVisual);
        slingshot.draw(window);

        // Threads do not render while being drawn
        {
            std::lock_guard<std::mutex> lock(physicsMutex);
            bird.draw(window);
            pigPool.drawActive(window);
        }

        if (!block.isDestroyed())
            block.draw(window);

        window.display();
    }
    // pauses and joins physics thread to the main thread
    physicsRunning = false;
    physics.join();

    return 0;
}