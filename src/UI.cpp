#include "UI.h"
#include <sstream>

UI::UI(){
    // Bar positioned at bottom and is greu
    m_barBackground.setSize(sf::Vector2f(500.0f, 30.0f));
    m_barBackground.setPosition(100.0f, 500.0f);
    m_barBackground.setFillColor(sf::Color(50, 50, 50)); //grey
    m_barBackground.setOutlineThickness(3.0f);
    m_barBackground.setOutlineColor(sf::Color::White);

    // starys at 0
    m_barFill.setSize(sf::Vector2f(0.0f, 30.0f));
    m_barFill.setPosition(100.0f, 500.0f);
    m_barFill.setFillColor(sf::Color(50, 200, 80));
}

//font set up
void UI::setFont(const sf::Font& font){
    m_font = &font;
    m_percentText.setFont(font);
    m_percentText.setCharacterSize(22);
    m_percentText.setFillColor(sf::Color::White);
    m_percentText.setPosition(350.0f, 540.0f);

    m_logText.setFont(font);
    m_logText.setCharacterSize(19);
    m_logText.setFillColor(sf::Color(200, 200, 200));
    m_logText.setPosition(0.0f, 420.0f);
}

void UI::setProgress(float percent)
{
    if (percent < 0.0f)   percent = 0.0f;
    if (percent > 100.0f) percent = 100.0f;
    m_progress = percent;

    // Bar progresses as percentage increase
    m_barFill.setSize(sf::Vector2f(600.0f * (m_progress / 100.0f), 30.0f));
    if (m_font)
    {
        m_percentText.setString(std::to_string(static_cast<int>(m_progress)) + "%");
    }
}

float UI::getProgress() const noexcept
{
    return m_progress;
}

// STL associative container label lookub
void UI::setLabel(const std::string& key, const std::string& value)
{
    m_labels[key] = value;
}
std::string UI::getLabel(const std::string& key) const
{
    auto it = m_labels.find(key);
    return (it != m_labels.end()) ? it->second : "";
}

// STL
void UI::pushLog(const std::string& message)
{
    m_logs.push_back(message);

    // Limits dmessagesa stored
    if (m_logs.size() > 3)
        m_logs.erase(m_logs.begin());

    rebuildLogText();
}

const std::vector<std::string>& UI::getLogs() const noexcept
{
    return m_logs;
}

void UI::rebuildLogText(){
    if (!m_font)
        return;

    std::ostringstream oss;
    for (const auto& msg : m_logs)
        oss << msg << "\n";

    m_logText.setString(oss.str());
}
void UI::draw(sf::RenderTarget& target) const{
    target.draw(m_barBackground);
    target.draw(m_barFill);

    if (m_font)
    {
        target.draw(m_percentText);
        target.draw(m_logText);
    }
}