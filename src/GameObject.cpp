#include "GameObject.h"
#include <cmath>

// Constants for converting between box2d and sfml
const float SCALE = 30.0f;
const float PI = 3.1415927f;

// constructor that sets objects rotatioin and position
GameObject::GameObject(const sf::Vector2f& position, float rotation, bool isStatic)
    : m_isStatic(isStatic), m_body(nullptr)
{
    m_transform.setPosition(position);
    m_transform.setRotation(rotation);
}

// updates box2d and sfml to sync with each other
void GameObject::update(float dt)
{
    if (m_body)
    {
        syncTransformFromBody();
    }
}

//Renders the object using SFML
void GameObject::draw(sf::RenderTarget& target) const
{
    if (m_shape)
    {
        sf::RenderStates states;
        states.transform = m_transform.getTransform();
        target.draw(*m_shape, states);
    }
}

// Collision, should be overidden by child classes
void GameObject::onCollision(GameObject& other)
{
}

// Physics using Box2d
void GameObject::setBody(b2Body* body) noexcept
{
    m_body = body;
    if (m_body)
    {
        syncBodyFromTransform();

        //Sets physics properties
        for (b2Fixture* fixture = m_body->GetFixtureList(); fixture; fixture = fixture->GetNext())
        {
            fixture->SetRestitution(m_restitution);
            fixture->SetFriction(m_friction);
        }
    }
}

//Returens box2d body type
b2Body* GameObject::getBody() const noexcept
{
    return m_body;
}

//Applies force if dynamic object
void GameObject::applyForce(const sf::Vector2f& force)
{
    if (m_body && m_body->GetType() == b2_dynamicBody)
    {
        //Covert between smfl and box2d
        b2Vec2 b2Force(force.x / SCALE, force.y / SCALE);
        m_body->ApplyForceToCenter(b2Force, true);
    }
}

void GameObject::applyImpulse(const sf::Vector2f& impulse)
{
    if (m_body && m_body->GetType() == b2_dynamicBody)
    {
        // Covert between SFMl and box2d
        b2Vec2 b2Impulse(impulse.x / SCALE, impulse.y / SCALE);
        m_body->ApplyLinearImpulse(b2Impulse, m_body->GetWorldCenter(), true);
    }
}

// Converts angle and position between box2d and sfml
void GameObject::syncTransformFromBody()
{
    if (!m_body)
        return;

    // Get Box2D position and convert to SFML 
    b2Vec2 b2Pos = m_body->GetPosition();
    sf::Vector2f sfmlPos(b2Pos.x * SCALE, b2Pos.y * SCALE);
    m_transform.setPosition(sfmlPos);

    // Get Box2D rotation and convert to SFML
    float b2Angle = m_body->GetAngle();
    float sfmlAngle = b2Angle * (180.0f / PI);
    m_transform.setRotation(sfmlAngle);
}

void GameObject::syncBodyFromTransform() //Converts Sfml properties to Box2D
{
    if (!m_body)
        return;


    sf::Vector2f sfmlPos = m_transform.getPosition();
    b2Vec2 b2Pos(sfmlPos.x / SCALE, sfmlPos.y / SCALE);


    float sfmlAngle = m_transform.getRotation();
    float b2Angle = sfmlAngle * (PI / 180.0f);


    m_body->SetTransform(b2Pos, b2Angle);
}

// Set and get functions
bool GameObject::isStatic() const noexcept
{
    return m_isStatic;
}

const sf::Vector2f& GameObject::getPosition() const noexcept
{
    return m_transform.getPosition();
}

void GameObject::setPosition(const sf::Vector2f& pos)
{
    m_transform.setPosition(pos);

    if (m_body)
    {
        syncBodyFromTransform();
    }
}

float GameObject::getRotation() const noexcept
{
    return m_transform.getRotation();
}

void GameObject::setRotation(float angle)
{
    m_transform.setRotation(angle);

    if (m_body)
    {
        syncBodyFromTransform();
    }
}

sf::FloatRect GameObject::getGlobalBounds() const
{
    if (m_shape)
    {
        // Transform the local bounds by the current transform matrix
        return m_transform.getTransform().transformRect(m_shape->getLocalBounds());
    }

    // If no shape returns point at the objects position
    return sf::FloatRect(m_transform.getPosition(), sf::Vector2f(0.0f, 0.0f));
}

//protected set and get functions
void GameObject::setShape(std::unique_ptr<sf::Shape> shape)
{
    m_shape = std::move(shape);
}

const sf::Shape* GameObject::shape() const noexcept
{
    return m_shape.get();
}