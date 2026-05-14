#include "Bird.h"
#include "Pig.h"
#include "Block.h"

//COnstructor using parent values
Bird::Bird(const sf::Vector2f& position, float rotation, float mass)
    : DynamicObject(position, rotation, mass)
{
}

void Bird::onCollision(GameObject& other)
{
    if (!m_body)
        return;

    // Uses Box2d to calculate damage based off total velocity
    const b2Vec2& vel = m_body->GetLinearVelocity();
    float speed = std::sqrt(vel.x * vel.x + vel.y * vel.y);

    if (other.getType() == "Pig")
    {
        Pig& pig = static_cast<Pig&>(other);
        int damage = static_cast<int>(speed * m_damageMultiplier);
        pig.takeDamage(damage);
    }
    else if (other.getType() == "Block")
    {
        Block& block = static_cast<Block&>(other);
        // Birds are less effective against blocks
        int damage = static_cast<int>(speed * m_damageMultiplier * 0.5f); //Block objects take half damage
        block.takeDamage(damage);
    }
}