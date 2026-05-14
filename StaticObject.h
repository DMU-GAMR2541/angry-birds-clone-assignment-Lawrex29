#pragma once
#include "GameObject.h"

class StaticObject : public GameObject
{
public:
    //Sets object to be static with only position and rotation
    StaticObject(const sf::Vector2f& position = {}, float rotation = 0.f);
    ~StaticObject() override = default;

    //gets object type, returns static object if not overidden by child classes
    std::string_view getType() const noexcept override { return "StaticObject"; }

    //No physics
    void update(float dt) override;
    void draw(sf::RenderTarget& target) const override;
};