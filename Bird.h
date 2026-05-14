#pragma once
#include "DynamicObject.h"

class Bird : public DynamicObject
{
    // Sets bird to be dynamic object with position, rotation and mass
public:
    Bird(const sf::Vector2f& position = {}, float rotation = 0.f, float mass = 1.f);
    ~Bird() override = default;

    // Gets object type, returns bird if not overidden
    std::string_view getType() const noexcept override { return "Bird"; }

    // What happens when a bird collides with pig or block
    void onCollision(GameObject& other) override;

    //Sets a multiplier for how much damage the bird does to pigs and blocks
private:
    float m_damageMultiplier{ 1.0f };
};