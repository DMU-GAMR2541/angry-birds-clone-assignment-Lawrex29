#include "StartScreen.h"
#include <chrono>
#include <iostream>

StartScreen::StartScreen(sf::RenderWindow& window)
    : m_window(window)
{
    //Trues to use system font if available
    const char* fontPaths[] = {
        "arial.ttf",
        "C:/Windows/Fonts/arial.ttf",
        "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
        "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
        "/System/Library/Fonts/Helvetica.ttc",
        nullptr
    };

    for (int i = 0; fontPaths[i] != nullptr; ++i)
    {
        if (m_font.loadFromFile(fontPaths[i]))
        {
            m_fontLoaded = true;
            break;
        }
    }

    m_ui.setFont(m_font);

    // Title
    if (m_fontLoaded)
    {
        m_titleText.setFont(m_font);
        m_titleText.setString("Super duper angry birds with a keg");
        m_titleText.setCharacterSize(50);
        m_titleText.setFillColor(sf::Color(255, 80, 0));

        // Centre title based on screen wifth
        sf::FloatRect bounds = m_titleText.getLocalBounds();
        m_titleText.setOrigin(bounds.width / 2.0f, bounds.height / 2.0f);
        m_titleText.setPosition(400.0f, 160.0f);
    }

//Stl contianer
    m_ui.setLabel("sprites", "Waiting...");
    m_ui.setLabel("physics", "Waiting...");
}

StartScreen::~StartScreen(){
    // Joins sprite thread into main for game start
    if (m_spriteThread.joinable())
        m_spriteThread.join();

    if (m_physicsFuture.valid())
        m_physicsFuture.wait();
}


bool StartScreen::run()
{
    // Sprites loAD on their own thread
    m_spriteThread = std::thread(&StartScreen::spriteLoadThread, this);

    // physics using async
    m_physicsFuture = launchPhysicsLoad();
    bool readyToContinue = false;
    while (m_window.isOpen())
    {
        sf::Event event;
        while (m_window.pollEvent(event)){
            if (event.type == sf::Event::Closed){
                m_window.close();
                return false;
            }
            // Game can only be startyed once loading is compleated
            if (event.type == sf::Event::KeyPressed &&
                event.key.code == sf::Keyboard::Space &&
                m_loadingDone)
            {
                readyToContinue = true;
            }
        }

        if (readyToContinue)
            break;

        // progress bar update
        updateProgress();

        // render all ui elements
        m_window.clear();
        drawBackground();
        drawStaticImages();
        drawTitle();
        drawUI();

        // continue prompt displayed
        if (m_loadingDone && m_fontLoaded)
        {
            sf::Text prompt;
            prompt.setFont(m_font);
            prompt.setString("Press SPACE");
            prompt.setCharacterSize(24);
            prompt.setFillColor(sf::Color::White);
            sf::FloatRect b = prompt.getLocalBounds();
            prompt.setOrigin(b.width / 2.0f, b.height / 2.0f);
            prompt.setPosition(400.0f, 575.0f);
            m_window.draw(prompt);
        }
        m_window.display();
    }
    if (m_spriteThread.joinable())
        m_spriteThread.join();

    if (m_physicsFuture.valid())
        m_physicsFuture.wait();

    return readyToContinue;
}
//Thresd for laoding sprites
void StartScreen::spriteLoadThread()
{
    const int steps = 10;
    for (int i = 1; i <= steps; ++i)
    {
        // Simulate work with a timer
        std::this_thread::sleep_for(std::chrono::milliseconds(200));

        m_spriteProgress = static_cast<float>(i) / steps * 100.0f;

        {
            std::lock_guard<std::mutex> lock(m_logMutex);
            m_ui.pushLog("Sprites: loaded " + std::to_string(i * 10) + "%");
            m_ui.setLabel("sprites", std::to_string(static_cast<int>(m_spriteProgress.load())) + "%");
        }
    }

    std::cout << "[SpriteThread] Sprite loading complete." << std::endl;
}
//Seconf thread for physics

std::future<void> StartScreen::launchPhysicsLoad()
{
    return std::async(std::launch::async, [this]()
        {
            const int steps = 10;
            for (int i = 1; i <= steps; ++i)
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(280));

                m_physicsProgress = static_cast<float>(i) / steps * 100.0f;

                {
                    std::lock_guard<std::mutex> lock(m_logMutex);
                    m_ui.pushLog("Physics: loaded " + std::to_string(i * 10) + "%");
                    m_ui.setLabel("physics", std::to_string(static_cast<int>(m_physicsProgress.load())) + "%");
                }
            }

            std::cout << "[AsyncPhysics] Physics loading complete." << std::endl;
        });
}

//Updates progress
void StartScreen::updateProgress(){
    float combined = (m_spriteProgress.load() + m_physicsProgress.load()) / 2.0f;
    m_ui.setProgress(combined);

    if (combined >= 100.0f && !m_loadingDone)
        m_loadingDone = true;
}

//Draws all sprites and the background
void StartScreen::drawBackground()
{
    m_window.draw(m_sky);
    m_window.draw(m_ground);
}

void StartScreen::drawStaticImages()
{
    m_window.draw(m_birdShape);
    m_window.draw(m_pigShape);
}

void StartScreen::drawTitle()
{
    if (m_fontLoaded)
        m_window.draw(m_titleText);
}

void StartScreen::drawUI()
{
    m_ui.draw(m_window);
}