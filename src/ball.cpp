#include <iostream>
#include <SFML/Graphics.hpp>
#include "ball.h"

Ball::Ball(float radius, float xpos, float ypos, float xvel, float yvel, sf::Color colour)
    : ball(radius)
{
    this->radius = radius;
    this->xpos = xpos;
    this->ypos = ypos;
    this->xvel = xvel;
    this->yvel = yvel;
    this->colour = colour;

    this->ball.setFillColor(colour);
    this->ball.setPosition({xpos, ypos});
    this->ball.setOrigin({radius, radius});
    this->ball.setRotation(sf::degrees(0.0f));
}

void Ball::move(float deltaX, float deltaY)
{
    this->xpos += deltaX;
    this->ypos += deltaY;

    this->ball.setPosition({xpos, ypos});
}

void Ball::setPosition(float x, float y)
{
    this->xpos = x;
    this->ypos = y;

    this->ball.setPosition({xpos, ypos});
}

void Ball::setVelocity(float xvel, float yvel)
{
    this->xvel = xvel;
    this->yvel = yvel;
}

void Ball::changeVelocity(float deltaXvel, float deltaYvel)
{
    this->xvel += deltaXvel;
    this->yvel += deltaYvel;
}

void Ball::setColour(unsigned int r, unsigned int g, unsigned int b)
{
    this->colour = sf::Color(r, g, b);
    this->ball.setFillColor(colour);
}

void Ball::changeColour(int deltaR, int deltaG, int deltaB)
{
    this->colour.r = std::clamp(this->colour.r + deltaR, 0, 255);
    this->colour.g = std::clamp(this->colour.g + deltaG, 0, 255);
    this->colour.b = std::clamp(this->colour.b + deltaB, 0, 255);
    this->ball.setFillColor(colour);
}

sf::CircleShape Ball::getBall()
{
    return this->ball;
}
void Ball::update(float deltaTime)
{
    this->xpos += this->xvel * deltaTime;
    this->ypos += this->yvel * deltaTime;

    this->ball.setPosition({xpos, ypos});
}

// * xvec and yvec are normals
// * these are completely elastic collisions
void Ball::collide(float xvec, float yvec,
                   float xpos, float ypos)
{
    this->yvel = yvec == 0 ? this->yvel : -1 * this->yvel;
    this->yvel = (yvel == 0.0f && this->yvel == 0.0f) || std::abs(this->yvel) < 15.0f ? 0.0f : this->yvel; // for rounding

    this->xvel = xvec == 0 ? this->xvel : -1 * this->xvel;
    this->xvel = (xvel == 0.0f && this->xvel == 0.0f) || std::abs(this->xvel) < 15.0f ? 0.0f : this->xvel; // for rounding

    this->setVelocity(this->xvel, this->yvel);
    std::cout << "new xvel : " << this->xvel << "\n";
    std::cout << "new yvel : " << this->yvel << "\n";

    this->setPosition(xpos, ypos);
}