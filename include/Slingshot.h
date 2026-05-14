#pragma once
#include "StaticObject.h"
#include <string_view>

class Slingshot : public StaticObject
{
public:
    // Sets slingshot to be static object with position and rotation
    Slingshot(const sf::Vector2f& position = {}, float rotation = 0.f);
    ~Slingshot() override = default;

    // Gets object type, returns slingshot if not overidden
    std::string_view getType() const noexcept override { return "Slingshot"; }

    // Functions to load a bird, pull back the slingshot and release it, to get the current tension and loaded bird type.
    void loadBird(std::string_view birdType);
    bool pullBack(int amount);
    void release();

    int getTension() const noexcept;
    std::string_view getLoadedBirdType() const noexcept;

    //
    void update(float dt) override;
    void draw(sf::RenderTarget& target) const override;

    //Intitial values for slingshot
private:
    float m_tension{ 10.0f };
    std::string_view m_loadedBirdType{ "Red" };
    static constexpr int MAX_TENSION = 100;
};