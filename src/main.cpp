#include <iostream>
#include "ball.h"
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>

using namespace std;

int main()
{
    // * create window
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 4;

    sf::RenderWindow *window = new sf::RenderWindow(
        sf::VideoMode({640u, 480u}),
        "pongping",
        sf::State::Windowed,
        settings);

    // * start clock
    sf::Clock clock;

    // * visual constants and vals
    const float PADDING_X = 20.0f;
    const float PADDING_Y = 20.0f;
    const float PONG_HEIGHT = 100.0f;
    const float PONG_WIDTH = 10.0f;
    const float PONG_XDIFF = PADDING_X + PONG_WIDTH;

    const float BUTTON_WIDTH = 100.0f;
    const float BUTTON_HEIGHT = 100.0f;

    const int X_JITTER = 100;
    const float INITIAL_XVEL = 300.0f;
    const float INITIAL_YVEL = 100.0f;
    const float INITAL_FVEL2 = INITIAL_XVEL * INITIAL_XVEL + INITIAL_YVEL * INITIAL_YVEL;

    float pongl_y = window->getSize().y / 2.0f;
    float pongr_y = window->getSize().y / 2.0f;

    bool gameOver = false;
    bool start = false;
    bool winner = false; // default winner is left for false

    // * ball
    Ball ball(5.0f, window->getSize().x / 2.0f, window->getSize().y / 2.0f, 0.0f, 0.0f, sf::Color::White);

    // * pongs
    sf::RectangleShape pongr({PONG_WIDTH, PONG_HEIGHT});
    pongr.setOrigin(pongr.getLocalBounds().getCenter());
    pongr.setPosition({window->getSize().x - PADDING_X, pongr_y});
    pongr.setFillColor(sf::Color::White);

    sf::RectangleShape pongl({PONG_WIDTH, PONG_HEIGHT});
    pongl.setOrigin(pongl.getLocalBounds().getCenter());
    pongl.setPosition({PADDING_X, pongl_y});
    pongl.setFillColor(sf::Color::White);

    // * text
    sf::Font font("assets/fonts/Courier New Bold.ttf");
    sf::Text gameoverText(font, "!!Game Over!!", 75);
    sf::Text winnerText(font, "Player 1 Wins", 50);

    // * buton
    sf::RectangleShape button({100.0f, 100.0f});
    button.setOrigin(button.getLocalBounds().getCenter());
    button.setPosition({window->getSize().x / 2.0f, window->getSize().y / 2.0f});
    button.setFillColor(sf::Color::White);

    gameoverText.setOrigin(gameoverText.getLocalBounds().getCenter());
    winnerText.setOrigin(winnerText.getLocalBounds().getCenter());

    // * sounds
    sf::SoundBuffer pong_sound_buffer("assets/sounds/pong.mp3");
    sf::Sound pong_sound(pong_sound_buffer);

    while (window->isOpen())
    {
        window->setFramerateLimit(60u); // * limiting to 60fps

        // * clock!!
        float deltaTime = clock.restart().asSeconds();
        float fps = 1.0f / deltaTime;

        // * text
        gameoverText.setPosition({window->getSize().x / 2.0f, window->getSize().y / 2.0f - 90.0f});
        winnerText.setPosition({window->getSize().x / 2.0f, window->getSize().y / 2.0f + 90.0f});

        while (const std::optional event = window->pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window->close();
            }
            if (event->is<sf::Event::Resized>())
            {
                // * udpating view
                sf::View view(sf::FloatRect({0.f, 0.f}, sf::Vector2f(window->getSize())));
                window->setView(view);
            }

            if (auto *press = event->getIf<sf::Event::MouseButtonReleased>())
            {
                if (press->button == sf::Mouse::Button::Left && button.getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
                {
                    start = true;
                    std::cout << "game over and start : " << gameOver << start << "\n";
                }
            }
        }

        // * movement
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::W))
        {
            pongl_y -= 5.0f;
            pongl_y = pongl_y < PADDING_Y + PONG_HEIGHT / 2.0f ? PADDING_Y + PONG_HEIGHT / 2.0f : pongl_y;
            pongl.setPosition({PADDING_Y, pongl_y});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::S))
        {
            pongl_y += 5.0f;
            pongl_y = pongl_y > window->getSize().y - PADDING_Y - PONG_HEIGHT / 2.0f ? window->getSize().y - PADDING_Y - PONG_HEIGHT / 2.0f : pongl_y;
            pongl.setPosition({PADDING_Y, pongl_y});
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up))
        {
            pongr_y -= 5.0f;
            pongr_y = pongr_y < PADDING_Y + PONG_HEIGHT / 2.0f ? PADDING_Y + PONG_HEIGHT / 2.0f : pongr_y;
            pongr.setPosition({window->getSize().x - PADDING_Y, pongr_y});
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down))
        {
            pongr_y += 5.0f;
            pongr_y = pongr_y > window->getSize().y - PADDING_Y - PONG_HEIGHT / 2.0f ? window->getSize().y - PADDING_Y - PONG_HEIGHT / 2.0f : pongr_y;
            pongr.setPosition({window->getSize().x - PADDING_Y, pongr_y});
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space) && ball.xvel == 0.0f && ball.yvel == 0.0f)
        {
            ball.setVelocity(INITIAL_XVEL, INITIAL_YVEL);
        }

        // * updates

        if (!gameOver && start)
        {
            ball.update(deltaTime);
        }

        // * collision detection
        if (start && ball.xpos > window->getSize().x - PONG_XDIFF && (ball.ypos < pongr_y + PONG_HEIGHT / 2.0f) && (ball.ypos > pongr_y - PONG_HEIGHT / 2.0f))
        {
            ball.collide(-1.0f, 0.0f, window->getSize().x - PONG_XDIFF, ball.ypos);
            pong_sound.play();
            int newx = ball.xvel + ::rand() % X_JITTER - X_JITTER / 2;
            int newy = std::sqrt(INITAL_FVEL2 - newx * newx);

            // std::cout << "velocities sum of squares : " << newx * newx + newy * newy << "\n";
            ball.setVelocity(newx, newy);
        }
        else if (ball.xpos > window->getSize().x - PONG_XDIFF)
        {
            if (!gameOver)
            {
                gameOver = true;
                start = false;
            }
            winner = false;
        }

        if (start && ball.xpos < PONG_XDIFF && (ball.ypos < pongl_y + PONG_HEIGHT / 2.0f) && (ball.ypos > pongl_y - PONG_HEIGHT / 2.0f))
        {
            ball.collide(-1.0f, 0.0f, PONG_XDIFF, ball.ypos);
            pong_sound.play();
            int newx = ball.xvel + ::rand() % X_JITTER - X_JITTER / 2;
            int newy = std::sqrt(INITAL_FVEL2 - newx * newx);
            ball.setVelocity(newx, newy);
        }
        else if (ball.xpos < PONG_XDIFF)
        {
            if (!gameOver)
            {
                gameOver = true;
                start = false;
            }
            winner = true;
        }

        if (ball.ypos > window->getSize().y - PADDING_Y)
        {
            ball.collide(0.0f, 1.0f, ball.xpos, window->getSize().y - PADDING_Y);
        }
        if (ball.ypos < PADDING_Y)
        {
            ball.collide(0.0f, 1.0f, ball.xpos, PADDING_Y);
        }

        // * button collision
        if (button.getGlobalBounds().contains(window->mapPixelToCoords(sf::Mouse::getPosition(*window))))
        {
            button.setFillColor(sf::Color::Blue);
            if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
            {
                button.setFillColor(sf::Color::Yellow);
            }
        }
        else
        {
            button.setFillColor(sf::Color::White);
        }

        // * drawing a background colour
        window->clear(sf::Color(13, 19, 28)); // * colour window buffer

        // * drawing
        if (!gameOver && start)
        {
            window->draw(ball.getBall());
            window->draw(pongr);
            window->draw(pongl);
        }
        else if (gameOver && !start)
        {
            if (winner)
            {
                winnerText.setString("Player 2 Wins");
            }
            window->draw(gameoverText);
            window->draw(winnerText);
            window->draw(button);
        }
        else if (!gameOver && !start)
        {
            window->draw(button);
        }
        else if (gameOver && start)
        {
            pongl_y = window->getSize().y / 2.0f;
            pongr_y = window->getSize().y / 2.0f;
            pongl.setPosition({PADDING_X, pongl_y});
            pongr.setPosition({window->getSize().x - PADDING_X, pongr_y});
            ball.setPosition(window->getSize().x / 2.0f, window->getSize().y / 2.0f);
            ball.setVelocity(0.0f, 0.0f);
            gameOver = false;
        }

        window->display();
    }

    delete window;
    return 0;
}