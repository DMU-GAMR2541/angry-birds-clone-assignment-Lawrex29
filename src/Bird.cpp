#include "Bird.h"
#include "Pig.h"
#include "Block.h"

Bird::Bird(const sf::Vector2f& position, float rotation, float mass)
    : DynamicObject(position, rotation, mass)
{
    setBirdType(BirdType::Red);
}

void Bird::setBirdType(BirdType type)
{
    m_birdType = type;

    switch (type)
    {
        //Red is standard bird (Speed is increased to show pig getting destroyed, otherwise would be 1)
    case BirdType::Red:
        m_damageMultiplier = 1.0f;
        m_mass = 1.0f;
        m_speedMultiplier = 2.8f;
        if (m_shape)
            static_cast<sf::CircleShape*>(m_shape.get())->setFillColor(sf::Color(200, 30, 30));
        break;

        //Fast bird but deals half damage
    case BirdType::Blue:
        m_damageMultiplier = 0.5f;
        m_mass = 0.5f;
        m_speedMultiplier = 1.4f;
        if (m_shape)
            static_cast<sf::CircleShape*>(m_shape.get())->setFillColor(sf::Color(30, 100, 200));
        break;

		//Slow bird but deals double damage
    case BirdType::Yellow:
        m_damageMultiplier = 2.0f;
        m_mass = 2.0f;
        m_speedMultiplier = 0.7f;
        if (m_shape)
            static_cast<sf::CircleShape*>(m_shape.get())->setFillColor(sf::Color(230, 200, 20));
        break;
    }

    if (m_body)
    {
        b2MassData massData;
        m_body->GetMassData(&massData);
        massData.mass = m_mass;
        m_body->SetMassData(&massData);
    }
}

Bird::BirdType Bird::getBirdType() const noexcept
{
    return m_birdType;
}

float Bird::getSpeedMultiplier() const noexcept
{
    return m_speedMultiplier;
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