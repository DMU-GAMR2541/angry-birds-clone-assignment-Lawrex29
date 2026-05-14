#include "Block.h"
#include "Bird.h"
#include "Pig.h"
#include <iostream>

const float SCALE = 30.0f;

// Constructor using paretn to set position, rotation
Block::Block(const sf::Vector2f& position, float rotation, BlockType blockType)
    : StaticObject(position, rotation), m_blockType(blockType), m_health(0), m_isDestroyed(false) //Adds health, block type and is destroyed
{
    // Sets health of the different block types
    switch (blockType)
    {
    case BlockType::Wood:
        m_health = 30;
        setShape(createWoodBlock());
        break;
    case BlockType::Stone:
        m_health = 60;
        setShape(createStoneBlock());
        break;
    case BlockType::Ice:
        m_health = 15;
        setShape(createIceBlock());
        break;
    }
}

// Takes damage when hit by bird or pig
void Block::takeDamage(int damage)
{
    if (damage < 0)
        return;

    m_health -= damage;

    if (m_health <= 0)
    {
        m_health = 0;
        m_isDestroyed = true;

        std::cout << "Block destroyed! Health: " << m_health << std::endl;
    }
    else
    {
        std::cout << "Block takes damage: " << damage << " | Health remaining: " << m_health << std::endl;
    }
}

// Get functions
int Block::getHealth() const noexcept
{
    return m_health;
}

// Check if block has been destroyed
bool Block::isDestroyed() const noexcept
{
    return m_isDestroyed;
}

// Get the block type
Block::BlockType Block::getBlockType() const noexcept
{
    return m_blockType;
}

// Block takes damage on collision with other objects
void Block::onCollision(GameObject& other)
{
    if (other.getType() == "Bird")
    {
        Bird* bird = dynamic_cast<Bird*>(&other);
        if (bird && !isDestroyed())
        {
            takeDamage(10);
        }
    }
    else if (other.getType() == "Pig")
    {
        Pig* pig = dynamic_cast<Pig*>(&other);
        if (pig && !isDestroyed())
        {
            takeDamage(5); 
        }
    }
}

// Creates the block types with different appearances
std::unique_ptr<sf::Shape> Block::createWoodBlock()
{
    auto shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40.0f, 40.0f));
    shape->setOrigin(20.0f, 20.0f);
    shape->setFillColor(sf::Color(139, 69, 19)); // Brown = wood
    return shape;
}
std::unique_ptr<sf::Shape> Block::createStoneBlock()
{
    auto shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40.0f, 40.0f));
    shape->setOrigin(20.0f, 20.0f);
    shape->setFillColor(sf::Color(128, 128, 128)); // Gray = stone
    
	return shape;
}
std::unique_ptr<sf::Shape> Block::createIceBlock()
{
    auto shape = std::make_unique<sf::RectangleShape>(sf::Vector2f(40.0f, 40.0f));
    shape->setOrigin(20.0f, 20.0f);
    shape->setFillColor(sf::Color(173, 216, 230)); // blue = ice
	return shape;
}