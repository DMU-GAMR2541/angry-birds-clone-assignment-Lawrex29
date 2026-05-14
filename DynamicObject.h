#pragma once
#include "GameObject.h"

class DynamicObject : public GameObject
{
public:
    // Sets object to be dynamic with position, rotation and mass
    DynamicObject(const sf::Vector2f& position = {}, float rotation = 0.f, float mass = 1.f);
    ~DynamicObject() override = default;

    // Gets object type, returns dynamic object if not overridden by child classes
    std::string_view getType() const noexcept override { return "DynamicObject"; }

    // Physics simulated in update and printed in draw.
    void update(float dt) override;
    void draw(sf::RenderTarget& target) const override;

    // Used for collision resolution between pigs, birds and blocks.
    void setMass(float mass);
    float getMass() const noexcept;
};