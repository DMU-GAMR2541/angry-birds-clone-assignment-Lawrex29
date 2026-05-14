#include "Pig.h"
#include "Bird.h"
#include "Block.h"
#include <iostream>

const float SCALE = 30.0f;

//Constructor to set position, rotation, mass and health
Pig::Pig(const sf::Vector2f& position, float rotation, float mass, int initialHealth)
    : DynamicObject(position, rotation, mass), m_health(initialHealth), m_isDestroyed(false) //Uses parent constructor to set position, rotation and mass. Helath and is destroyed specific to pig
{
    // Pig represented by pink circle
    auto pigShape = std::make_unique<sf::CircleShape>(12.0f);
    pigShape->setOrigin(12.0f, 12.0f);
    pigShape->setFillColor(sf::Color(255, 192, 203)); // RGB for pink
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

// Get and set functions
int Pig::getHealth() const noexcept
{
    return m_health;
}

bool Pig::isDestroyed() const noexcept
{
    return m_isDestroyed;
}

// Collision based on colliding object
void Pig::onCollision(GameObject& other)
{
    std::string_view otherType = other.getType();

    if (otherType == "Bird")
    {
        // If bird hits pig, pig will take damage based on birds velocity plus a base 20
        Bird* bird = dynamic_cast<Bird*>(&other);
        if (bird)
        {
            // Damage calculation for birds velocity
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
        // Block collides with pig takes lesser damage
        Block* block = dynamic_cast<Block*>(&other);
        if (block && !block->isDestroyed())
        {
            takeDamage(5);
        }
    }
}