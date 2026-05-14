#include "SlingShot.h"
#include <iostream>

const float SCALE = 30.0f;
constexpr float SLINGSHOT_WIDTH = 15.0f;
constexpr float SLINGSHOT_HEIGHT = 60.0f;

Slingshot::Slingshot(const sf::Vector2f& position, float rotation)
    : StaticObject(position, rotation)
{
    // Create slingshot, tan coloured rectangle
    auto slingshotShape = std::make_unique<sf::RectangleShape>(sf::Vector2f(SLINGSHOT_WIDTH, SLINGSHOT_HEIGHT));
    slingshotShape->setOrigin(SLINGSHOT_WIDTH / 2.0f, SLINGSHOT_HEIGHT / 2.0f);
    slingshotShape->setFillColor(sf::Color(210, 180, 140)); // Tan color for slingshot
    setShape(std::move(slingshotShape));
}

// Loads a bird
void Slingshot::loadBird(std::string_view birdType)
{
    m_loadedBirdType = birdType;
    m_tension = 10.0f;

    std::cout << "Bird loaded: " << birdType << " | Tension reset to: " << m_tension << std::endl;
}

// Increases tension up to max
bool Slingshot::pullBack(int amount)
{
    // Validate amount
    if (amount < 0)
    {
        std::cout << "Invalid pull amount: " << amount << std::endl;
        return false;
    }

    float newTension = m_tension + static_cast<float>(amount);

    if (newTension > MAX_TENSION)
    {
        m_tension = static_cast<float>(MAX_TENSION);
        std::cout << "Max tension reached: " << m_tension << std::endl;
        return false;
    }

    m_tension = newTension;
    std::cout << "Slingshot pulled back. Current tension: " << m_tension << std::endl;
    return true;
}

// fires slingshot, resets tension
void Slingshot::release()
{
    std::cout << "Slingshot released! Bird fired with tension: " << m_tension
        << " | Bird type: " << m_loadedBirdType << std::endl;

    m_tension = 10.0f;
}

// Get functions
int Slingshot::getTension() const noexcept
{
    return static_cast<int>(m_tension);
}

std::string_view Slingshot::getLoadedBirdType() const noexcept
{
    return m_loadedBirdType;
}

// Uses parent for draw and update
void Slingshot::update(float dt)
{
    StaticObject::update(dt);
}

void Slingshot::draw(sf::RenderTarget& target) const
{
    StaticObject::draw(target);
}