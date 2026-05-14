#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <iostream>
#include "Bird.h" 
#include "SlingShot.h"
#include "Pig.h"
#include "Block.h"

int main() {
    // --- 1. WINDOW SETUP ---
    sf::RenderWindow window(sf::VideoMode(800, 600), "Annoyed_Flocks");
    window.setFramerateLimit(60);

    //Box2D works in meters. SFML works in pixels.
    const float SCALE = 30.0f;

    const float PI = 3.1415927f;

    //setup world.
    b2Vec2 b2_gravity(0.0f, 9.8f); // Earth-like gravity
    b2World world(b2_gravity);

    //Creating objects
    Slingshot slingshot(sf::Vector2f(150.0f, 500.0f), 100.0f);
    Bird bird(sf::Vector2f(100.0f, 500.0f), 0.0f);
    Pig pig(sf::Vector2f(700.0f, 500.0f), 0.0f);
    Block block(sf::Vector2f(550.0f, 500.0f), 0.0f, Block::BlockType::Wood);

    // --- BIRD BOX2D BODY SETUP ---
    // The bird needs a Box2D body so physics (gravity, impulses) can act on it.
    // Without this, m_body is null and applyForce/update do nothing.
    b2BodyDef b2_birdDef;
    b2_birdDef.type = b2_dynamicBody;
    b2_birdDef.position.Set(100.0f / SCALE, 500.0f / SCALE);
    b2Body* b2_birdBody = world.CreateBody(&b2_birdDef);

    b2CircleShape b2_birdShape;
    b2_birdShape.m_radius = 10.0f / SCALE; // Matches the 10.0f radius in DynamicObject

    b2FixtureDef b2_birdFixture;
    b2_birdFixture.shape = &b2_birdShape;
    b2_birdFixture.density = 1.0f;
    b2_birdFixture.restitution = 0.3f;
    b2_birdFixture.friction = 0.5f;
    b2_birdBody->CreateFixture(&b2_birdFixture);

    // Attach the Box2D body to the bird GameObject so update/applyForce work
    bird.setBody(b2_birdBody);

    //Setup ground
    b2BodyDef b2_groundBodyDef;
    b2_groundBodyDef.position.Set(400.0f / SCALE, 590.0f / SCALE);
    b2Body* b2_groundBody = world.CreateBody(&b2_groundBodyDef);

    b2PolygonShape b2_groundBox;
    b2_groundBox.SetAsBox(400.0f / SCALE, 10.0f / SCALE);
    b2_groundBody->CreateFixture(&b2_groundBox, 0.0f);

    sf::RectangleShape sf_groundVisual(sf::Vector2f(800.0f, 20.0f));
    sf_groundVisual.setOrigin(400.0f, 10.0f);
    sf_groundVisual.setFillColor(sf::Color(34, 139, 34)); // Forest Green

    //Setting up a wall
    b2BodyDef b2_wallDef;
    b2_wallDef.position.Set(750.0f / SCALE, 500.0f / SCALE);
    b2Body* b2_wallBody = world.CreateBody(&b2_wallDef);

    b2PolygonShape b2_wallBox;
    b2_wallBox.SetAsBox(10.0f / SCALE, 80.0f / SCALE);
    b2_wallBody->CreateFixture(&b2_wallBox, 0.0f);

    sf::RectangleShape sf_wallVisual(sf::Vector2f(20.0f, 160.0f));
    sf_wallVisual.setOrigin(10.0f, 80.0f);
    sf_wallVisual.setFillColor(sf::Color::Red);

    //Dynamic plank
    b2BodyDef b2_plankDef;
    b2_plankDef.type = b2_dynamicBody;
    b2_plankDef.position.Set(550.0f / SCALE, 450.0f / SCALE);
    b2Body* b2_plankBody = world.CreateBody(&b2_plankDef);

    b2PolygonShape b2_plankBox;
    b2_plankBox.SetAsBox(10.0f / SCALE, 60.0f / SCALE);

    b2FixtureDef b2_plankFixture;
    b2_plankFixture.shape = &b2_plankBox;
    b2_plankFixture.density = 1.5f;
    b2_plankFixture.friction = 0.3f;
    b2_plankBody->CreateFixture(&b2_plankFixture);

    sf::RectangleShape sf_plankVisual(sf::Vector2f(20.0f, 120.0f));
    sf_plankVisual.setOrigin(10.0f, 60.0f);
    sf_plankVisual.setFillColor(sf::Color(139, 69, 19)); // Brown

    //Legacy ball (kept from original)
    b2BodyDef b2_ballDef;
    b2_ballDef.type = b2_dynamicBody;
    b2_ballDef.position.Set(100.0f / SCALE, 400.0f / SCALE); // Moved up so it doesn't overlap bird
    b2Body* b2_ballBody = world.CreateBody(&b2_ballDef);

    b2CircleShape b2_circleShape;
    b2_circleShape.m_radius = 15.0f / SCALE;

    b2FixtureDef b2_ballFixture;
    b2_ballFixture.shape = &b2_circleShape;
    b2_ballFixture.density = 1.0f;
    b2_ballFixture.restitution = 0.5f;
    b2_ballBody->CreateFixture(&b2_ballFixture);

    sf::CircleShape sf_ballVisual(15.0f);
    sf_ballVisual.setOrigin(15.0f, 15.0f);
    sf_ballVisual.setFillColor(sf::Color::Yellow);

    // --- MAIN LOOP ---
    while (window.isOpen()) {

        // --- EVENT HANDLING (must come before update) ---
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::R) {
                    slingshot.loadBird("Red");
                }
                if (event.key.code == sf::Keyboard::B) {
                    slingshot.loadBird("Blue");
                }
                if (event.key.code == sf::Keyboard::Y) {
                    slingshot.loadBird("Yellow");
                }
                if (event.key.code == sf::Keyboard::Space) {
                    // Reset bird to launch position
                    b2_birdBody->SetTransform(b2Vec2(100.0f / SCALE, 500.0f / SCALE), 0);
                    b2_birdBody->SetLinearVelocity(b2Vec2(0, 0));
                    b2_birdBody->SetAngularVelocity(0);

                    // Pull back and release slingshot
                    slingshot.pullBack(50);
                    slingshot.release();

                    // Launch the bird using an impulse (matches how the legacy ball was launched)
                    b2_birdBody->ApplyLinearImpulse(b2Vec2(5.0f, -5.0f), b2_birdBody->GetWorldCenter(), true);

                    // Also reset and fire the legacy ball
                    b2_ballBody->SetTransform(b2Vec2(100.0f / SCALE, 400.0f / SCALE), 0);
                    b2_ballBody->SetLinearVelocity(b2Vec2(0, 0));
                    b2_ballBody->SetAngularVelocity(0);
                    b2_ballBody->ApplyLinearImpulse(b2Vec2(5.0f, -5.0f), b2_ballBody->GetWorldCenter(), true);

                    std::cout << "Firing!!!!" << std::endl;
                }
            }
        }

        // --- UPDATE PHYSICS ---
        world.Step(1.0f / 60.0f, 8, 3);

        // Sync bird with its Box2D body (makes it move on screen)
        bird.update(1.0f / 60.0f);

        // Sync legacy visuals manually
        sf_ballVisual.setPosition(b2_ballBody->GetPosition().x * SCALE, b2_ballBody->GetPosition().y * SCALE);
        sf_ballVisual.setRotation(b2_ballBody->GetAngle() * (180.0f / PI));

        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);
        sf_wallVisual.setPosition(b2_wallBody->GetPosition().x * SCALE, b2_wallBody->GetPosition().y * SCALE);

        sf_plankVisual.setPosition(b2_plankBody->GetPosition().x * SCALE, b2_plankBody->GetPosition().y * SCALE);
        sf_plankVisual.setRotation(b2_plankBody->GetAngle() * (180.0f / PI));

        // --- RENDER ---
        window.clear(sf::Color(135, 206, 235)); // Sky Blue

        window.draw(sf_groundVisual);
        window.draw(sf_wallVisual);
        window.draw(sf_plankVisual);
        window.draw(sf_ballVisual);
        slingshot.draw(window);
        bird.draw(window);      // Bird now moves because it has a Box2D body
        pig.draw(window);
        block.draw(window);

        window.display();
    }

    return 0;
}