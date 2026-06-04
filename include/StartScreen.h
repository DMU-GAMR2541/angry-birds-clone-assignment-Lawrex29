#pragma once
#include <SFML/Graphics.hpp>
#include <atomic>
#include <future>   //async features
#include <mutex>
#include <thread>
#include "UI.h"

class StartScreen
{
public:
    explicit StartScreen(sf::RenderWindow& window);
    ~StartScreen();
    //on space stqrts game
    bool run();

private:
    // thread 1 will load the sprite data
    void spriteLoadThread();

    // Thread 2 will sim physics
    std::future<void> launchPhysicsLoad();
    // update loading bar
    void updateProgress();

    // Load state
    std::atomic<float> m_spriteProgress{ 0.0f };   
    std::atomic<float> m_physicsProgress{ 0.0f }; 
    std::atomic<bool>  m_loadingDone{ false };
    std::mutex      m_logMutex;                 
    std::thread     m_spriteThread;
    std::future<void> m_physicsFuture;
    //Draw ui
    void drawBackground();
    void drawStaticImages();   
    void drawTitle();
    void drawUI();

    sf::RenderWindow& m_window;
    sf::Font          m_font;
    bool              m_fontLoaded = false;

    UI m_ui;

    // Title
    sf::Text m_titleText;

    sf::CircleShape    m_birdShape;   
    sf::CircleShape    m_pigShape;     

    // Background
    sf::RectangleShape m_sky;
    sf::RectangleShape m_ground;
};