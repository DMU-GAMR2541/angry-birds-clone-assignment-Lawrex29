#pragma once
#include "DynamicObject.h"

class Pig : public DynamicObject
{

public:
    //Sets pig to be dynamic object with position rotation, mass and health
    Pig(const sf::Vector2f& position = {}, float rotation = 0.f, float mass = 1.f, int initialHealth = 100);
    ~Pig() override = default;

    //Gets object type, returns pig if not overidden by child classes
    std::string_view getType() const noexcept override { return "Pig"; }

    //Functions for pig to take damage when hit by bird, to check health and check if destroyed (when health is less than or equal to 0)
    void takeDamage(int damage);
    int getHealth() const noexcept;
    bool isDestroyed() const noexcept;

    // resets pig to be used again
    void reset(const sf::Vector2f& position, int health = 100);

    // Is pig in use by pool
    bool isActive() const noexcept;
    void setActive(bool active) noexcept;

    //What happens when a bird collides with pig or pig collides with a block
    void onCollision(GameObject& other) override;

    //Sets pigs health and if it has been destroyed
private:
    int m_health;
    bool m_isDestroyed;
    bool m_isActive{ false }; 
};

