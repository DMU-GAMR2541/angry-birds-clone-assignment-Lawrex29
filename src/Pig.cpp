#include "Pig.h"
#include "Bird.h"
#include "Block.h"
#include <iostream>

const float SCALE = 30.0f;

// Constructor to set position, rotation, mass and health
Pig::Pig(const sf::Vector2f& position, float rotation, float mass, int initialHealth)
    : DynamicObject(position, rotation, mass), m_health(initialHealth), m_isDestroyed(false)
{
    // Pig represented by pink circle
    auto pigShape = std::make_unique<sf::CircleShape>(12.0f);
    pigShape->setOrigin(12.0f, 12.0f);
    pigShape->setFillColor(sf::Color(255, 192, 203));
    setShape(std::move(pigShape));
}

// Destroyed if health is less than or equal to 0
void Pig::takeDamage(int damage)
{
    if (damage < 0)
        return;

    m_health -= damage;

    if (m_health <= 0)
    {
        m_health = 0;
        m_isDestroyed = true;
        std::cout << "Pig destroyed! Health: " << m_health << std::endl;
    }
    else
    {
        std::cout << "Pig takes damage: " << damage << " | Health remaining: " << m_health << std::endl;
    }
}

int Pig::getHealth() const noexcept
{
    return m_health;
}

bool Pig::isDestroyed() const noexcept
{
    return m_isDestroyed;
}

// Resets pig so pool can reuse it
void Pig::reset(const sf::Vector2f& position, int health)
{
    m_health = health;
    m_isDestroyed = false;
    m_isActive = true;
    setPosition(position);

    if (m_body)
    {
        m_body->SetLinearVelocity(b2Vec2(0.0f, 0.0f));
        m_body->SetAngularVelocity(0.0f);
        m_body->SetAwake(true);
    }
}

bool Pig::isActive() const noexcept
{
    return m_isActive;
}

void Pig::setActive(bool active) noexcept
{
    m_isActive = active;
}

// Collision based on colliding object
void Pig::onCollision(GameObject& other)
{
    std::string_view otherType = other.getType();

    if (otherType == "Bird")
    {
        Bird* bird = dynamic_cast<Bird*>(&other);
        if (bird)
        {
            b2Body* birdBody = bird->getBody();
            if (birdBody)
            {
                b2Vec2 velocity = birdBody->GetLinearVelocity();
                float speed = velocity.Length();
                int baseDamage = 20;
                int totalDamage = baseDamage + static_cast<int>(speed * 5.0f);
                takeDamage(totalDamage);
            }
            else
            {
                takeDamage(20);
            }
        }
    }
    else if (otherType == "Block")
    {
        Block* block = dynamic_cast<Block*>(&other);
        if (block && !block->isDestroyed())
        {
            takeDamage(5);
        }
    }
}