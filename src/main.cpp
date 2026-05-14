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

    //Needed for commit message about force push
    //Box2D works in meters. SFML works in pixels.
    const float SCALE = 30.0f;

    //Can set a definition for PI.
    const float PI = 3.1415927;

    //setup world.
    b2Vec2 b2_gravity(0.0f, 9.8f); // Earth-like gravity
    b2World world(b2_gravity);

    //Creating objects
    Slingshot slingshot(sf::Vector2f(150.0f, 470.0f), 0.0f);
    Bird bird(sf::Vector2f(150.0f, 450.0f), 0.0f);
    Pig pig(sf::Vector2f(680.0f, 555.0f), 0.0f, 1.0f, 100);
    Block block(sf::Vector2f(600.0f, 557.0f), 0.0f, Block::BlockType::Wood);

    //Setup ground for the circle to move / bounce on.
    //Needs to have a body definition and a body. We use a raw pointer for the b2Body as Box2d does the management itself.
    //A body can be defined as having a position, velocity, and mass. 

    b2BodyDef b2_groundBodyDef;
    b2_groundBodyDef.position.Set(400.0f / SCALE, 590.0f / SCALE);
    b2Body* b2_groundBody = world.CreateBody(&b2_groundBodyDef);

    //Define a fixture shape that relates to the collision for the ground.
    b2PolygonShape b2_groundBox;
    b2_groundBox.SetAsBox(400.0f / SCALE, 10.0f / SCALE);
    b2_groundBody->CreateFixture(&b2_groundBox, 0.0f);

    //Set up the ground visualisation.
    sf::RectangleShape sf_groundVisual(sf::Vector2f(800.0f, 20.0f));
    sf_groundVisual.setOrigin(400.0f, 10.0f);
    sf_groundVisual.setFillColor(sf::Color(34, 139, 34)); // Forest Green

    //Sets static body for slingshot
    b2BodyDef b2_slingshotDef;
    b2_slingshotDef.type = b2_staticBody;
    b2_slingshotDef.position.Set(150.0f / SCALE, 470.0f / SCALE);
    b2Body* b2_slingshotBody = world.CreateBody(&b2_slingshotDef);

    
    b2PolygonShape b2_slingshotBox;
    b2_slingshotBox.SetAsBox(7.5f / SCALE, 30.0f / SCALE); // Matches 15x60 rectangle in SlingShot.cpp
    b2_slingshotBody->CreateFixture(&b2_slingshotBox, 0.0f);

    slingshot.setBody(b2_slingshotBody);

    //Box2d body for block
    b2BodyDef b2_blockDef;
    b2_blockDef.type = b2_staticBody;
    b2_blockDef.position.Set(600.0f / SCALE, 557.0f / SCALE);
    b2Body* b2_blockBody = world.CreateBody(&b2_blockDef);

    //Gives Box2d body to block
    b2PolygonShape b2_blockBox;
    b2_blockBox.SetAsBox(20.0f / SCALE, 20.0f / SCALE); 

    b2FixtureDef b2_blockFixture;
    b2_blockFixture.shape = &b2_blockBox;
    b2_blockFixture.friction = 0.5f;
    b2_blockBody->CreateFixture(&b2_blockFixture);

    block.setBody(b2_blockBody);

    //Gives pig Box2d body to simulate physics
    b2BodyDef b2_pigDef;
    b2_pigDef.type = b2_dynamicBody;
    b2_pigDef.position.Set(680.0f / SCALE, 555.0f / SCALE);
    b2Body* b2_pigBody = world.CreateBody(&b2_pigDef);

    b2CircleShape b2_pigCircle;
    b2_pigCircle.m_radius = 12.0f / SCALE; 

    b2FixtureDef b2_pigFixture;
    b2_pigFixture.shape = &b2_pigCircle;
    b2_pigFixture.density = 1.2f;
    b2_pigFixture.restitution = 0.1f;
    b2_pigFixture.friction = 0.6f;
    b2_pigBody->CreateFixture(&b2_pigFixture);

    pig.setBody(b2_pigBody);

    //Gives bird Box2d body so physics can apply when fired
    b2BodyDef b2_birdDef;
    b2_birdDef.type = b2_dynamicBody;
    b2_birdDef.position.Set(150.0f / SCALE, 450.0f / SCALE);
    b2Body* b2_birdBody = world.CreateBody(&b2_birdDef);

    b2CircleShape b2_birdCircle;
    b2_birdCircle.m_radius = 10.0f / SCALE; 

    b2FixtureDef b2_birdFixture;
    b2_birdFixture.shape = &b2_birdCircle;
    b2_birdFixture.density = 1.0f;
    b2_birdFixture.restitution = 0.3f; // How much it will bounce
    b2_birdFixture.friction = 0.4f;
    b2_birdBody->CreateFixture(&b2_birdFixture);

    bird.setBody(b2_birdBody);

    // --- 7. MAIN LOOP ---
    while (window.isOpen()) {
        sf::Event event;

        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            // INPUT HANDLING: Press SPACE to launch
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
                    // Reset position of the bird so that it can be fired again from its original position.
                    b2_birdBody->SetTransform(b2Vec2(150.0f / SCALE, 450.0f / SCALE), 0);
                    b2_birdBody->SetLinearVelocity(b2Vec2(0, 0));
                    b2_birdBody->SetAngularVelocity(0);

                    //Slingshot fires
                    slingshot.pullBack(38); // Pull back the slingshot with a tension of 50
                    float tension = static_cast<float>(slingshot.getTension());
                    slingshot.release();

                    // Apply impulse (X-axis, Y-axis) Negative Y is UP in Box2D because gravity is positive.
                    float impulseX = tension * 0.06f;
                    float impulseY = -tension * 0.06f;
                    b2_birdBody->ApplyLinearImpulse(b2Vec2(impulseX, impulseY), b2_birdBody->GetWorldCenter(), true);

                    std::cout << "Firing!!!!" << std::endl;
                }
            }
        }

        // Update Physics
        world.Step(1.0f / 60.0f, 8, 3);

        //All of the visuals needs to be synced with the physics.
        bird.update(1.0f / 60.0f);
        pig.update(1.0f / 60.0f);

        //Static objects usually don't move, but we set the position once.
        sf_groundVisual.setPosition(b2_groundBody->GetPosition().x * SCALE, b2_groundBody->GetPosition().y * SCALE);

        //Render all of the content at each frame. Remember you need to clear the screen each iteration or artefacts remain.
        window.clear(sf::Color(135, 206, 235)); // Sky Blue

        window.draw(sf_groundVisual);
        slingshot.draw(window);
        bird.draw(window);
        pig.draw(window);
        block.draw(window);

        window.display();
    }

    return 0;
}