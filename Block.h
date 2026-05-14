#pragma once
#include "StaticObject.h"

class Block : public StaticObject
{
public:
    // Different types of blocks with different health values and friction/mass
    enum class BlockType
    {
        Wood,
        Stone,
        Ice
    };

    // Sets block to be static object with position, rotation and type of block
    Block(const sf::Vector2f& position = {}, float rotation = 0.f, BlockType blockType = BlockType::Wood);
    ~Block() override = default;

    // Gets object type, returns block if not overidden
    std::string_view getType() const noexcept override { return "Block"; }

    //Functions to take damage when hit by bird or pig, to check health and check if destroyed (when health is depleted)
    void takeDamage(int damage);
    int getHealth() const noexcept;
    bool isDestroyed() const noexcept;

    // Gets the type of block affecting mass and friction
    BlockType getBlockType() const noexcept;

    // What happens when a bird or pig collides with a block
    void onCollision(GameObject& other) override;

    // Sets blocks health and if it has been destroyed
private:
private:
    // Methods to create block types
    std::unique_ptr<sf::Shape> createWoodBlock();
    std::unique_ptr<sf::Shape> createStoneBlock();
    std::unique_ptr<sf::Shape> createIceBlock();

    BlockType m_blockType;
    int m_health;
    bool m_isDestroyed;
};