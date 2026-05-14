#pragma once
#include <SFML/Graphics.hpp>
#include <box2d/box2d.h>
#include <memory>
#include <string_view>

class GameObject
{
public:
    // Sets object to be Game Object with position and rotation
    GameObject(const sf::Vector2f& position = {}, float rotation = 0.f, bool isStatic = false);
    virtual ~GameObject() = default;

    // Type of object, GameObject if not overridden by children
    virtual std::string_view getType() const noexcept { return "GameObject"; }

    // Syncs Box2D and SFML
    virtual void update(float dt);
    virtual void draw(sf::RenderTarget& target) const;

    // What happens when objects collide
    virtual void onCollision(GameObject& other);

    // Physics are achieved using Box2D
    void setBody(b2Body* body) noexcept;
    b2Body* getBody() const noexcept;

    void applyForce(const sf::Vector2f& force);
    void applyImpulse(const sf::Vector2f& impulse);

    // Get and set properties
    bool isStatic() const noexcept;
    const sf::Vector2f& getPosition() const noexcept;
    void setPosition(const sf::Vector2f& pos);
    float getRotation() const noexcept;
    void setRotation(float angle);

    sf::FloatRect getGlobalBounds() const;

protected:
    // Uses SFML shapes
    void setShape(std::unique_ptr<sf::Shape> shape);
    const sf::Shape* shape() const noexcept;

    // Syncs between SFML and Box2D 
    void syncTransformFromBody();
    void syncBodyFromTransform();

    bool m_isStatic;

    // Transform and visuals
    sf::Transformable m_transform;
    std::unique_ptr<sf::Shape> m_shape;

    // Box2D physics body (not owned; managed by b2World)
    b2Body* m_body{ nullptr };

    // Physics properties for Box2D 
    float m_restitution{ 0.2f };
    float m_friction{ 0.5f };
};