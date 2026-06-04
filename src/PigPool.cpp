#include "PigPool.h"
#include <iostream>

const float PIG_POOL_SCALE = 30.0f;

PigPool::PigPool(b2World& world, int poolSize, int pigHealth)
    : m_world(world), m_pigHealth(pigHealth)
{
    // Pigs created off scren
    for (int i = 0; i < poolSize; ++i){
        sf::Vector2f parkPosition(-1000.0f, -1000.0f);
        auto pig = std::make_unique<Pig>(parkPosition, 0.0f, 1.0f, pigHealth);
        createBody(*pig, parkPosition);
        pig->setActive(false);


        m_pool.push_back(std::move(pig));
    }

    std::cout << "PigPool created with " << poolSize << " pigs." << std::endl;
}

// Pigs accessed by first one that is inactive
Pig* PigPool::acquire(const sf::Vector2f& position)
{
    for (auto& pig : m_pool)
    {
        if (!pig->isActive())
        {
            pig->reset(position, m_pigHealth);
            return pig.get();
        }
    }

    std::cout << "PigPool exhausted — no pigs available." << std::endl;
    return nullptr;
}

// sets pig to be inactive and position id offscreen
void PigPool::release(Pig* pig)
{
    if (!pig)
        return;

    pig->setActive(false);
    if (pig->getBody())
    {
        pig->getBody()->SetTransform(
            b2Vec2(-1000.0f / PIG_POOL_SCALE, -1000.0f / PIG_POOL_SCALE), 0.0f);
        pig->getBody()->SetLinearVelocity(b2Vec2(0, 0));
        pig->getBody()->SetAngularVelocity(0);
        pig->getBody()->SetAwake(false); 
    }

    std::cout << "Pig returned to pool." << std::endl;
}

// On pig death calles function to set it inactive and off screen
void PigPool::releaseDestroyed()
{
    for (auto& pig : m_pool)
    {
        if (pig->isActive() && pig->isDestroyed())
        {
            release(pig.get());
        }
    }
}


//Returns the pool of pigs
const std::vector<std::unique_ptr<Pig>>& PigPool::getAll() const noexcept
{
    return m_pool;
}
//Draw and update functions for pig
void PigPool::drawActive(sf::RenderTarget& target) const{
    for (const auto& pig : m_pool)
    {
        if (pig->isActive())
            pig->draw(target);
    }
}
void PigPool::updateActive(float dt){
    for (auto& pig : m_pool)
    {
        if (pig->isActive())
            pig->update(dt);
    }
}

// Box2d body fpr pig
void PigPool::createBody(Pig& pig, const sf::Vector2f& position)
{
    b2BodyDef bodyDef;
    bodyDef.type = b2_dynamicBody;
    bodyDef.position.Set(position.x / PIG_POOL_SCALE, position.y / PIG_POOL_SCALE);
    b2Body* body = m_world.CreateBody(&bodyDef);
    b2CircleShape circle;
    circle.m_radius = 12.0f / PIG_POOL_SCALE;

    b2FixtureDef fixtureDef;
    fixtureDef.shape = &circle;
    fixtureDef.restitution = 0.1f;
    fixtureDef.friction = 0.6f;
    fixtureDef.density = 1.2f;
    body->CreateFixture(&fixtureDef);

    pig.setBody(body);
}