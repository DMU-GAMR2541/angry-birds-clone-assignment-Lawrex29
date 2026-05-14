#include "StaticObject.h"

const float SCALE = 30.0f;

// Constructor to set position and rotation, and create a default shape for static objects
StaticObject::StaticObject(const sf::Vector2f& position, float rotation)
    : GameObject(position, rotation, true)//true to set static object
{
    // Create a brown rectangle shape unless overidden by slingshot or block
    auto rectShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40.0f, 40.0f));
    rectShape->setOrigin(20.0f, 20.0f);
    rectShape->setFillColor(sf::Color(139, 69, 19)); // Brown using rgb (Sfml doesnt have brown?)
    setShape(std::move(rectShape));
}

// Uses parent GameObjects update and draw functions, used for syncing purposes as is static
void StaticObject::update(float dt)
{
    GameObject::update(dt);
}

void StaticObject::draw(sf::RenderTarget& target) const
{
    GameObject::draw(target);
}