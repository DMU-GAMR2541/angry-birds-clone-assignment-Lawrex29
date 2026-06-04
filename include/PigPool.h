#pragma once
#include<SFML/Graphics.hpp>
#include "Pig.h"
#include <vector>
#include <memory>
#include <box2d/box2d.h>

class PigPool
{
public:
    //pool, of 5 pigs
    PigPool(b2World& world, int poolSize = 5, int pigHealth = 100);
 
    // Returns pointer to an empty pig if one available
    Pig* acquire(const sf::Vector2f& position);
    void drawActive(sf::RenderTarget& target) const;
 
    //Pig is set to be inactive once destroyed
    void release(Pig* pig);
    void releaseDestroyed();
 
    
 
    // Pigs updated
    void updateActive(float dt);
    //Returns all pigs for collision checkks
    const std::vector<std::unique_ptr<Pig>>& getAll() const noexcept;
 
private:
    std::vector<std::unique_ptr<Pig>> m_pool;
    b2World& m_world;
    int m_pigHealth;
 
    // Pig has box2d body
    void createBody(Pig& pig, const sf::Vector2f& position);
};