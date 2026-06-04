#pragma once
#include <SFML/Graphics.hpp>
#include <string>
//STL container
#include <unordered_map>  
#include <vector>         

class UI
{
public:
    UI();

    // Progress bar functions
    void setProgress(float percent);          // 0.0 - 100.0
    float getProgress() const noexcept;


    //Functions to display
    void draw(sf::RenderTarget& target) const;
    void setFont(const sf::Font& font);
    // Status labelsand log msgs
    void setLabel(const std::string& key, const std::string& value);
    std::string getLabel(const std::string& key) const;
    void pushLog(const std::string& message);
    const std::vector<std::string>& getLogs() const noexcept;

private:
    //Useing sfml to display
    float m_progress = 0.0f;
    sf::RectangleShape m_barBackground;
    sf::RectangleShape m_barFill;
    sf::Text           m_percentText;
    sf::Text           m_logText;
    std::unordered_map<std::string, std::string> m_labels;

    // STL sequence container
    std::vector<std::string> m_logs;
    const sf::Font* m_font = nullptr;
    void rebuildLogText();
};