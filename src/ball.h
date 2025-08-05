#ifndef BALL_H
#define BALL_H
#include <SFML/Graphics.hpp>

class Ball
{
private:
    sf::CircleShape ball;

public:
    float radius;
    float xpos;
    float ypos;
    float xvel;
    float yvel;

    bool isCollidedX = false;
    bool isCollidedY = false;
    sf::Color colour;

    Ball(float radius, float xpos, float ypos, float xvel, float yvel, sf::Color colour);

    void move(float deltaX, float deltaY);
    void setPosition(float x, float y);

    void setVelocity(float xvel, float yvel);
    void changeVelocity(float deltaXvel, float deltaYvel);

    void setColour(unsigned int r, unsigned int g, unsigned int b);
    void changeColour(int deltaR, int deltaG, int deltaB);

    sf::CircleShape getBall();

    void update(float deltaTime);

    void collide(float xvec, float yvec,
                 float xpos, float ypos);
};

#endif