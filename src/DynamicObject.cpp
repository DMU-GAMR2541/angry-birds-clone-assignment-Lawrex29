#include "DynamicObject.h"

const float SCALE = 30.0f;

//Constructor to set position, rotation and mass
DynamicObject::DynamicObject(const sf::Vector2f& position, float rotation, float mass)
    : GameObject(position, rotation, false) //false to set dynamic object
{
    // Create a circle tahts yellow as default unless overriden by pig or bird
    auto circleShape = std::make_unique<sf::CircleShape>(10.0f);
    circleShape->setOrigin(10.0f, 10.0f);
    circleShape->setFillColor(sf::Color::Yellow);
    setShape(std::move(circleShape));
}

// uses parent GameObjects update and draw functions
void DynamicObject::update(float dt)
{
    GameObject::update(dt);
}

void DynamicObject::draw(sf::RenderTarget& target) const
{
    GameObject::draw(target);
}